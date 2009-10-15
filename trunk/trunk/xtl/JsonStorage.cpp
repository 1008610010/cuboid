#include "JsonStorage.hpp"

namespace XTL
{
	JsonStorage::JsonStorage(const std::string & filePath)
		: file_(filePath),
		  lock_(filePath + ".lock")
	{
		;;
	}

	bool JsonStorage::Create(bool waitLock)
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

	bool JsonStorage::Open(bool waitLock)
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
		catch (const FileLock::Error & e)
		{
			throw;
		}
		catch (const JsonFile::Error & e)
		{
			lock_.Unlock();
			throw;
		}
		catch (const JsonParser::Error & e)
		{
			lock_.Unlock();
			throw;
		}
	}

	bool JsonStorage::Flush()
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

	void JsonStorage::Close()
	{
		if (lock_.Locked())
		{
			file_.Clear();
			lock_.Unlock();
		}
	}
}

