#include "LevelDB.h"

#include <string.h>

#include <stdexcept>

#include <leveldb/db.h>

#include "ThreadLocalVariable.hpp"

namespace
{
	class LevelDB
	{
		public:

			static LevelDB & Instance()
			{
				static LevelDB instance;
				return instance;
			}

			~LevelDB() throw()
			{
				Close();
			}

			bool IsOpened() const
			{
				return db_ != 0;
			}

			void Open(const std::string & dirPath)
			{
				if (IsOpened())
				{
					Close();
				}

				leveldb::Options options;
				options.create_if_missing = true;
				leveldb::Status status = leveldb::DB::Open(options, dirPath, &db_);

				if (!status.ok())
				{
					throw std::runtime_error(status.ToString());
					db_ = 0;
				}
			}

			void Close()
			{
				if (db_ != 0)
				{
					delete db_;
					db_ = 0;
				}
			}

			void Put(const char * keyData, size_t keySize, const char * valueData, size_t valueSize)
			{
				CheckOpened();

				leveldb::Status status = db_->Put(leveldb::WriteOptions(), leveldb::Slice(keyData, keySize), leveldb::Slice(valueData, valueSize));

				if (!status.ok())
				{
					throw std::runtime_error(status.ToString());
				}
			}

			bool Get(const char * keyData, size_t keySize, std::string & value)
			{
				CheckOpened();

				leveldb::Status status = db_->Get(leveldb::ReadOptions(), leveldb::Slice(keyData, keySize), &value);
				if (status.IsNotFound())
				{
					return false;
				}
				else if (status.ok())
				{
					return true;
				}
				else
				{
					throw std::runtime_error(status.ToString());
				}
			}

		private:

			LevelDB()
				: db_(0)
			{
				;;
			}

			LevelDB(const LevelDB &);
			LevelDB & operator= (const LevelDB &);

			void CheckOpened()
			{
				if (!IsOpened())
				{
					throw std::runtime_error("The database is not opened");
				}
			}

			leveldb::DB * db_;
	};

	XTL::ThreadLocalVariable<std::string> LAST_ERROR;

	void SetLastError(const std::string & value)
	{
		LAST_ERROR.Ref() = value;
	}
}

extern "C"
{
	const char * LevelDB_LastError()
	{
		return LAST_ERROR.Ref().c_str();
	}

	void LevelDB_SetLastError(const char * message)
	{
		SetLastError(message);
	}

	int LevelDB_Open(const char * dirPath)
	{
		try
		{
			LevelDB::Instance().Open(dirPath);
			return 0;
		}
		catch (const std::runtime_error & e)
		{
			SetLastError(e.what());
			return -1;
		}
	}

	void LevelDB_Close()
	{
		LevelDB::Instance().Close();
	}

	int LevelDB_Get(const char * keyData, size_t keySize, LevelDB_GetCallback callback, void * param)
	{
		try
		{
			std::string value;
			if (LevelDB::Instance().Get(keyData, keySize, value))
			{
				if (callback != 0)
				{
					callback(value.data(), value.size(), param);
				}

				return 0;
			}
			else
			{
				return 1;
			}
		}
		catch (const std::runtime_error & e)
		{
			SetLastError(e.what());
			return -1;
		}
	}

	int LevelDB_Put(const char * keyData, size_t keySize, const char * valueData, size_t valueSize)
	{
		try
		{
			LevelDB::Instance().Put(keyData, keySize, valueData, valueSize);
			return 0;
		}
		catch (const std::runtime_error & e)
		{
			SetLastError(e.what());
			return -1;
		}
	}
}
