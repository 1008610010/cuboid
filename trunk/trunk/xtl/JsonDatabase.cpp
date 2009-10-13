#include "JsonDatabase.hpp"

namespace XTL
{
	JsonDatabase::JsonDatabase(const std::string & filePath)
		: file_(filePath),
		  lock_(filePath + ".lock")
	{
		;;
	}

	bool JsonDatabase::Create(bool waitLock)
	{
		try
		{
			if (!lock_.Lock(waitLock))
			{
				throw Locked();
			}

			file_.Clear();
			return true;
		}
		catch (const FileLock::Error &)
		{
			throw;
		}
	}

	bool JsonDatabase::Open(bool waitLock)
	{
		try
		{
			if (!lock_.Lock(waitLock))
			{
				throw Locked();
			}

			if (!file_.Load())
			{
				lock_.Unlock();
				return false;
			}

			return true;
		}
		catch (const FileLock::Error &)
		{
			throw;
		}
		catch (const JsonFile::Error &)
		{
			lock_.Unlock();
			throw;
		}
		catch (const JsonParser::Error & e)
		{
			lock_.Unlock();
			throw Error(
				FormatString(
					"Database file corrupted (row %d, column %d): %s",
					e.Row() + 1,
					e.Column() + 1,
					e.What()
				)
			);
		}
	}

	bool JsonDatabase::Flush()
	{
		if (lock_.Locked())
		{
			file_.Save();
			return true;
		}
		else
		{
			return false;
		}
	}

	void JsonDatabase::Close()
	{
		if (lock_.Locked())
		{
			file_.Save();
			file_.Clear();
			lock_.Unlock();
		}
	}
}
