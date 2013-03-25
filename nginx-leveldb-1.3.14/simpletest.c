#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LevelDB.h"

void PrintUsage(int argc, const char * argv[])
{
	printf("Usage: %s get KEY\n", argv[0]);
	printf("       %s put KEY VALUE\n", argv[0]);
	exit(0);
}

void PrintValue(const char * valueData, size_t valueSize, void * param)
{
	printf("%s\n", valueData);
}

int main(int argc, const char * argv[])
{
	printf("Ok!\n");

	if (LevelDB_Open("/tmp/nginx_leveldb") < 0)
	{
		printf("Could not open database\n");
		return 1;
	}

	if (argc < 2)
	{
		PrintUsage(argc, argv);
	}

	if (strcmp(argv[1], "put") == 0)
	{
		if (argc < 4)
		{
			PrintUsage(argc, argv);
		}

		LevelDB_Put(argv[2], strlen(argv[2]), argv[3], strlen(argv[3]));
	}
	else if (strcmp(argv[1], "get") == 0)
	{
		if (argc < 3)
		{
			PrintUsage(argc, argv);
		}

		int r = LevelDB_Get(argv[2], strlen(argv[2]), PrintValue, NULL);
		if (r == 1)
		{
			printf("Not found.\n");
		}
	}
	else
	{
		PrintUsage(argc, argv);
	}

/*
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "./proba", &db);

	if (!status.ok())
	{
		std::cerr << status.ToString() << std::endl;
		return 1;
	}

	status = db->Put(leveldb::WriteOptions(), "root", "qwerty");

	leveldb::Iterator * it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next())
	{
		std::cout << it->key().ToString() << ": "  << it->value().ToString() << std::endl;
	}
	// assert(it->status().ok());  // Check for any errors found during the scan
	delete it;

	delete db;
*/
}
