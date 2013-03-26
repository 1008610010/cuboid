#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include <LevelDB.h>


typedef struct {
	ngx_str_t database_path;
} ngx_http_leveldb_main_conf_t;


static char * ngx_http_leveldb_set_loc_handler(ngx_conf_t * cf, ngx_command_t * cmd, void * conf);
static ngx_int_t ngx_http_leveldb_post_config(ngx_conf_t * cf);
static void * ngx_http_leveldb_create_main_conf(ngx_conf_t * cf);
static char * ngx_http_leveldb_init_main_conf(ngx_conf_t * cf, void * conf);
static ngx_int_t ngx_http_leveldb_init_worker(ngx_cycle_t * cycle);
static ngx_int_t ngx_http_leveldb_handler(ngx_http_request_t * r);
static void ngx_http_leveldb_command_put(ngx_http_request_t * r);


static ngx_command_t ngx_http_leveldb_commands[] = {
	{
		ngx_string("leveldb_path"),
		NGX_HTTP_MAIN_CONF | NGX_CONF_TAKE1,
		ngx_conf_set_str_slot,
		NGX_HTTP_MAIN_CONF_OFFSET,
		offsetof(ngx_http_leveldb_main_conf_t, database_path),
		NULL
	},
	{
		ngx_string("leveldb_handler"),
		NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
		ngx_http_leveldb_set_loc_handler,
		0,
		0,
		NULL
	},
	ngx_null_command
};

static ngx_http_module_t ngx_http_leveldb_module_ctx = {
	NULL,                              /* preconfiguration */
	ngx_http_leveldb_post_config,      /* postconfiguration */

	ngx_http_leveldb_create_main_conf, /* create main configuration */
	ngx_http_leveldb_init_main_conf,   /* init main configuration */

	NULL,                              /* create server configuration */
	NULL,                              /* merge server configuration */

	NULL,                              /* create location configuration */
	NULL                               /* merge location configuration */
};

ngx_module_t ngx_http_leveldb_module = {
	NGX_MODULE_V1,
	&ngx_http_leveldb_module_ctx,  /* module context */
	ngx_http_leveldb_commands,     /* module directives */
	NGX_HTTP_MODULE,               /* module type */
	NULL,                          /* init master */
	NULL,                          /* init module */
	ngx_http_leveldb_init_worker,  /* init process */
	NULL,                          /* init thread */
	NULL,                          /* exit thread */
	NULL,                          /* exit process */
	NULL,                          /* exit master */
	NGX_MODULE_V1_PADDING
};


static char * ngx_http_leveldb_set_loc_handler(ngx_conf_t * cf, ngx_command_t * cmd, void * conf)
{
	ngx_http_core_loc_conf_t * clcf;

	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_leveldb_handler;

	return NGX_CONF_OK;
}

static ngx_int_t ngx_http_leveldb_post_config(ngx_conf_t * cf)
{
	ngx_log_error(NGX_LOG_NOTICE, cf->log, 0, "ngx_http_leveldb_post_config");

	ngx_core_conf_t * ccf = (ngx_core_conf_t *) ngx_get_conf(cf->cycle->conf_ctx, ngx_core_module);

	if (ccf->worker_processes != 1) {
		ngx_log_error(NGX_LOG_EMERG, cf->log, 0, "Could not start when worker_processes = %z > 1", ccf->worker_processes);
		return NGX_ERROR;
	}

	ngx_http_leveldb_main_conf_t * gcf;

	gcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_leveldb_module);

	ngx_log_error(NGX_LOG_DEBUG, cf->log, 0, "database_path = %s", gcf->database_path.data);

	return NGX_OK;
}

static void ngx_http_leveldb_cleanup(void * data)
{
	ngx_http_leveldb_main_conf_t * conf = data;

	conf = conf;

	LevelDB_Close();
}

static void * ngx_http_leveldb_create_main_conf(ngx_conf_t * cf)
{
	ngx_http_leveldb_main_conf_t * gcf;

	ngx_log_error(NGX_LOG_DEBUG, cf->log, 0, "ngx_http_leveldb_create_main_conf");

	gcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_leveldb_main_conf_t));
	if (gcf == NULL) {
		return NULL;
	}

	ngx_str_null(&gcf->database_path);
	// gcf->enable = NGX_CONF_UNSET_UINT;

	ngx_pool_cleanup_t * cln;
	cln = ngx_pool_cleanup_add(cf->pool, 0);
	if (cln == NULL) {
		return NULL;
	}

	cln->handler = ngx_http_leveldb_cleanup;
	cln->data = gcf;

	return gcf;
}

static char * ngx_http_leveldb_init_main_conf(ngx_conf_t * cf, void * conf)
{
	ngx_http_leveldb_main_conf_t * gcf = conf;

	gcf = gcf;

	return NGX_CONF_OK;
}

static ngx_int_t ngx_http_leveldb_init_worker(ngx_cycle_t * cycle)
{
	ngx_http_leveldb_main_conf_t * gcf;

	ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "ngx_http_leveldb_init_worker");

	gcf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_leveldb_module);

	ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "database_path = %s", gcf->database_path.data);

	if (gcf->database_path.data != NULL)
	{
		if (LevelDB_Open((char *) gcf->database_path.data) < 0)
		{
			ngx_log_error(NGX_LOG_EMERG, cycle->log, 0, "Could not open leveldb");
			return NGX_ERROR;
		}
	}

	return NGX_OK;
}

/******************************** Fresh Meat ********************************/

// static ngx_str_t plain_text_type = ngx_string("plain/text");
// static ngx_str_t RESPONSE = ngx_string("FUUU!!!\n");

// TODO: May be remove MAX_VALUE_SIZE? Because restriction to request body size already exists.
static off_t MAX_VALUE_SIZE = 1 << 20;

static ngx_str_t URI_GET = ngx_string("/get/");
static ngx_str_t URI_PUT = ngx_string("/put/");


typedef struct {
	ngx_http_request_t * r;
	ngx_str_t value;
} ngx_http_leveldb_value_t;

static void ngx_http_leveldb_allocate_value(const char * data, size_t size, void * param)
{
	ngx_http_leveldb_value_t * value = param;
	ngx_http_request_t * r = value->r;

	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "ngx_http_leveldb_allocate_value size=%uz", size);

	value->value.data = ngx_palloc(r->pool, size);
	if (value->value.data == NULL)
	{
		ngx_log_error(NGX_LOG_CRIT, r->connection->log, 0, "Could not allocate buffer of size %uz", size);
		value->value.len = 0;
		return;
	}

	value->value.len = size;
	ngx_memcpy(value->value.data, data, size);
}

int ngx_http_leveldb_is_prefix(ngx_str_t * s, ngx_str_t * prefix)
{
	if (s->len < prefix->len)
	{
		return 0;
	}
	else if (prefix->len == 0)
	{
		return 1;
	}
	else
	{
		return memcmp(s->data, prefix->data, prefix->len) == 0;
	}
}

static ngx_int_t ngx_http_leveldb_handler(ngx_http_request_t * r)
{
	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "ngx_http_leveldb_handler");

	if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_POST)))
	{
		return NGX_HTTP_NOT_ALLOWED;
	}

	ngx_int_t rc;

	ngx_str_t response = ngx_string("Hello, world!\n");

	if (ngx_http_leveldb_is_prefix(&r->uri, &URI_GET))
	{
		rc = ngx_http_discard_request_body(r);
		if (rc != NGX_OK)
		{
			return rc;
		}

		ngx_str_t key;
		key.data = r->uri.data + URI_GET.len;
		key.len  = r->uri.len  - URI_GET.len;

		ngx_http_leveldb_value_t value = { r, ngx_string("DUMMY") };

		int result = LevelDB_Get((const char *) key.data, key.len, ngx_http_leveldb_allocate_value, &value);
		if (result < 1)
		{
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}
		else if (result == 1)
		{
			return NGX_HTTP_NO_CONTENT;
		}

		response = value.value;
	}
	else if (ngx_http_leveldb_is_prefix(&r->uri, &URI_PUT))
	{
		if (r->method != NGX_HTTP_POST)
		{
			return NGX_HTTP_NOT_ALLOWED;
		}
		else if (r->headers_in.content_length_n > MAX_VALUE_SIZE)
		{
			return NGX_HTTP_REQUEST_ENTITY_TOO_LARGE;
		}

		r->request_body_in_file_only = 0;
		r->request_body_in_single_buf = 1;

		rc = ngx_http_read_client_request_body(r, ngx_http_leveldb_command_put);
		// проверяем результат обработки, если не отработало, то возвращает код NGX_AGAIN,

		if (rc >= NGX_HTTP_SPECIAL_RESPONSE)
		{
			return rc;
		}

		return NGX_DONE;
	}
	else
	{
		return NGX_HTTP_NOT_FOUND;
	}

	// r->headers_out.last_modified_time = 23349600;

	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = response.len;
	ngx_str_set(&(r->headers_out.content_type), "application/octet-stream");

	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
	{
		return rc;
	}

	ngx_buf_t * b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL)
	{
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Could not allocated response buffer.");
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	b->pos = response.data;
	b->last = response.data + response.len;

	b->memory = 1;   /* Данные храняться в памяти только для чтения */
	                 /* (то есть фильтры должны скопировать эти данные перед обработкой, вместо того, чтобы изменять их) */
	b->last_buf = 1; /* Буферов в запросе больше не будет */

	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;

	return ngx_http_output_filter(r, &out);
}

static ngx_int_t ngx_http_leveldb_read_request_body_from_file(ngx_http_request_t * r, ngx_file_t * file, ngx_str_t * body);

static ngx_int_t ngx_http_leveldb_read_request_body(ngx_http_request_t * r, ngx_str_t * body)
{
	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "ngx_http_leveldb_read_request_body (Content-Length: %uz)", r->headers_in.content_length_n);

	body->len = r->headers_in.content_length_n;

	if (r->request_body == NULL)
	{
		ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body is NULL");
		body->len = 0;
		body->data = NULL;
		return NGX_OK;
	}

	if (r->request_body->temp_file != NULL)
	{
		ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body is in file: %s", r->request_body->temp_file->file.name.data);

		if (ngx_http_leveldb_read_request_body_from_file(r, &r->request_body->temp_file->file, body) != NGX_OK)
		{
			return NGX_ERROR;
		}
	}
	else if (r->request_body->buf != NULL)
	{
		ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body is in buffer: %uz", r->request_body->buf->end - r->request_body->buf->start);

		body->data = r->request_body->buf->start;
	}
	else if (r->request_body->bufs != NULL)
	{
		ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body is in buffers chain");

		body->data = ngx_palloc(r->pool, body->len);
		if (body->data == NULL)
		{
			ngx_log_error(NGX_LOG_CRIT, r->connection->log, 0, "Could not allocate buffer of size %uz", body->len);
			return NGX_ERROR;
		}

		u_char * ptr = body->data;
		ngx_chain_t * cur = r->request_body->bufs;
		while (cur != NULL)
		{
			size_t chunk_size = cur->buf->last - cur->buf->pos;

			ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body chunk: %uz", (unsigned int) chunk_size);

			ngx_memcpy(ptr, cur->buf->pos, chunk_size);
			ptr += chunk_size;

			cur = cur->next;
		}

		// ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "request_body: %s", body->data);
	}
	else
	{
		ngx_log_error(NGX_LOG_CRIT, r->connection->log, 0, "Could not determine where request_body is");
		return NGX_ERROR;
	}

	return NGX_OK;
}

static ngx_int_t ngx_http_leveldb_read_request_body_from_file(ngx_http_request_t * r, ngx_file_t * file, ngx_str_t * body)
{
	body->data = ngx_palloc(r->pool, body->len);
	if (body->data == NULL)
	{
		ngx_log_error(NGX_LOG_CRIT, r->connection->log, 0, "Could not allocate buffer of size %uz", body->len);
		return NGX_ERROR;
	}

	ssize_t n = ngx_read_file(file, body->data, body->len, 0);
	if (n == NGX_ERROR)
	{
		ngx_log_error(NGX_LOG_CRIT, r->connection->log, ngx_errno, "Error in ngx_read_file()");
		return NGX_ERROR;
	}

	if ((size_t) n != body->len)
	{
		ngx_log_error(NGX_LOG_CRIT, r->connection->log, 0, "ngx_read_file() returned only %z bytes instead of %z", n, body->len);
		return NGX_ERROR;
	}

	return NGX_OK;
}

static void ngx_http_leveldb_command_put(ngx_http_request_t * r)
{
	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "ngx_http_leveldb_command_put");

	ngx_str_t request_body = { 0, NULL };
	if (ngx_http_leveldb_read_request_body(r, &request_body) != NGX_OK)
	{
		ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
		return;
	}

	if (request_body.data != NULL)
	{
		ngx_str_t key;
		key.data = r->uri.data + URI_PUT.len;
		key.len  = r->uri.len  - URI_PUT.len;

		LevelDB_Put((const char *) key.data, key.len, (const char *) request_body.data, request_body.len);
	}

	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = 0;
	r->header_only = 1;

	ngx_http_finalize_request(r, ngx_http_send_header(r));
}
