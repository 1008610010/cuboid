#include <cstdio>
#include <cstdlib>

#include <deque>
#include <list>

#include <xtl/utils/Chronometer.hpp>
#include <xtl/Exception.hpp>
#include <xtl/linux/UnixError.hpp>
#include <xtl/linux/fs/File.hpp>
#include <xtl/linux/fs/FileStats.hpp>
#include <xtl/tp/SimpleTemplate.hpp>
#include <xtl/io/OutputStream.hpp>
#include <xtl/io/FileOutputStream.hpp>
#include <xtl/Variant2.hpp>

namespace XTL
{
/*
	class StringTemplateDebugger
	{
		public:

			StringTemplateDebugger(const char * source)
			{
				ParseSimpleTemplate(source, *this);
			}

			void OnText(const char * ptr, unsigned int size)
			{
				fprintf(stdout, "TEXT  -> \"%s\"\n", std::string(ptr, size).c_str());
			}

			void OnParameter(unsigned int paramIndex)
			{
				fprintf(stdout, "PARAM -> %u\n", paramIndex);
			}
	};

	template <class StringOutputter>
	void WriteStringTemplate(StringOutputter & outputter, const char * source, const StringTemplateValuesArray & values)
	{
		StringTemplateOutputter<StringOutputter>(outputter, source, values);
	}

	template <class StringOutputter, typename T0>
	void WriteStringTemplate(StringOutputter & outputter, const char * source, const T0 & t0)
	{
		StringTemplateValuesArray values(1);
		values.Set(0, t0);
		WriteStringTemplate(outputter, source, values);
	}

	template <class StringOutputter, typename T0, typename T1>
	void WriteStringTemplate(StringOutputter & outputter, const char * source, const T0 & t0, const T1 & t1)
	{
		StringTemplateValuesArray values(2);
		values.Set(0, t0);
		values.Set(1, t1);
		WriteStringTemplate(outputter, source, values);
	}

	class StringOutputter_String
	{
		public:

			StringOutputter_String()
				: buffer_()
			{
				;;
			}

			void Write(const char * ptr, unsigned int size)
			{
				buffer_.insert(buffer_.end(), ptr, ptr + size);
			}

			const std::string GetResult() const
			{
				return std::string(buffer_.begin(), buffer_.end());
			}

		private:

			std::deque<char> buffer_;
	};

	template <typename T0>
	const std::string String(const char * source, const T0 & t0)
	{
		StringOutputter_String outputter;
		WriteStringTemplate(outputter, source, t0);
		return outputter.GetResult();
	}

	template <typename T0, typename T1>
	const std::string String(const char * source, const T0 & t0, const T1 & t1)
	{
		StringOutputter_String outputter;
		WriteStringTemplate(outputter, source, t0, t1);
		return outputter.GetResult();
	}

	class StringOutputter_File
	{
		public:

			StringOutputter_File(FILE * file)
				: file_(file)
			{
				;;
			}

			void Write(const char * ptr, unsigned int size)
			{
				::fwrite(ptr, 1, size, file_);
			}

		private:

			FILE * file_;
	};

	StringOutputter_File stdoutOutputter(stdout);

	StringOutputter_File stderrOutputter(stderr);

	class StringTemplateParameter
	{
		public:

			template <typename StringOutputter>
			void Write(StringOutputter & outputter)
			{
				if (value_ == 0)
				{
					// TODO: throw exception here
					return;
				}
				outputter.Write(value_->Chars(), value_->Size());
			}
	};

	class StringTemplate
	{
		public:

			explicit StringTemplate(const char * source)
				: nodes_(),
				  params_()
			{
				ParseSimpleTemplate(source, *this);
			}

			template <typename T>
			void Set(unsigned int paramIndex, const T & value)
			{
				StringTemplateParameter * param = params_.Get(paramIndex);
				if (param == 0)
				{
					// Do nothing. Skip parameter, if it is not present in template.
					// Template for example: "Hello, $0 $2!" - parameter with index 1 is absent.
					return;
				}
				param->SetValue(CreateTemplateValue(value));
			}

			template <class StringOutputter>
			void Evaluate(StringOutputter & outputter)
			{
				NodeOutputter<StringOutputter> visitor(outputter);
				const ListOfNodes::iterator end = nodes_.end();
				for (ListOfNodes::iterator itr = nodes_.begin(); itr != end; ++itr)
				{
					(*itr)->Visit(visitor);
				}
			}

			template <class StringOutputter, typename T0>
			void Evaluate(StringOutputter & outputter, const T0 & t0)
			{
				Set(0, t0);
				Evaluate(outputter);
			}

			template <class StringOutputter, typename T0, typename T1>
			void Evaluate(StringOutputter & outputter, const T0 & t0, const T1 & t1)
			{
				Set(0, t0);
				Set(1, t1);
				Evaluate(outputter);
			}

		private:

			class Node_Text : public Node
			{
					virtual void Visit(NodeVisitor & visitor) const
					{
						visitor.VisitText(*this);
					}

					template <typename StringOutputter>
					void Write(StringOutputter & outputter) const
					{
						outputter.Write(text_.data(), text_.size());
					}
			};

			class Node_Parameter : public Node
			{
				public:

					explicit Node_Parameter(StringTemplateParameter * param)
						: param_(param)
					{
						;;
					}

					template <typename StringOutputter>
					void Write(StringOutputter & outputter) const
					{
						param_->Write(outputter);
					}

				private:

					StringTemplateParameter * param_;
			};

			template <class StringOutputter>
			class NodeOutputter : public NodeVisitor
			{
				public:

					NodeOutputter(StringOutputter & outputter)
						: outputter_(outputter)
					{
						;;
					}

					virtual ~NodeOutputter() throw()
					{
						;;
					}

					virtual void VisitText(const Node_Text & node)
					{
						node.Write(outputter_);
					}

					virtual void VisitParameter(const Node_Parameter & node)
					{
						node.Write(outputter_);
					}

				private:

					StringOutputter & outputter_;
			};

			typedef std::list<Node *> ListOfNodes;

			ListOfNodes nodes_;
			StringTemplateParametersArray params_;
	};
*/
}

#include <xtl/TypeTraits.hpp>

#include <xtl/FormatString.hpp>

#include <xtl/tp/SimpleTemplateItemVisitor.hpp>

class HexOutputStream : public XTL::OutputStream
{
	public:

		HexOutputStream(unsigned int bytesPerLine)
			: bytesPerLine_(bytesPerLine),
			  bytesWritten_(0)
		{
			
		}

		virtual void Write(const void * buffer, unsigned int size)
		{
			const char * ptr = static_cast<const char *>(buffer);
			const char * end = ptr + size;
			for (; ptr != end; ++ptr)
			{
				++bytesWritten_;
				printf("%c%c%s", IntToHex((*ptr) >> 4), IntToHex(*ptr), bytesWritten_ % bytesPerLine_ == 0 ? "\n" : (bytesWritten_ % 4 == 0 ? "  " : " "));
			}
		}

		static char IntToHex(int i)
		{
			i &= 0x0f;
			if (i < 10)
			{
				return '0' + i;
			}
			else
			{
				return 'A' + (i - 10);
			}
		}

	private:

		const unsigned int bytesPerLine_;
		unsigned int bytesWritten_;
};

#include <xtl/tp/SimpleTemplateValueArray.hpp>
#include <xtl/tp/SimpleTemplateValueString.hpp>
#include <xtl/tp/SimpleTemplateValueInteger.hpp>
#include <xtl/tp/SimpleTemplateValueChar.hpp>
#include <xtl/StringUtils.hpp>

class ScreenOutputStream : public XTL::OutputStream
{
	public:

		virtual void Write(const void * buffer, unsigned int size)
		{
			fwrite(buffer, size, 1, stdout);
		}

		static ScreenOutputStream & Instance()
		{
			static ScreenOutputStream instance;
			return instance;
		}
};

template <typename T1>
void DebugPrint(const XTL::SimpleTemplate & tmpl, const T1 & t1)
{
	XTL::WriteTemplate(tmpl, ScreenOutputStream::Instance(), t1);
}

template <typename T1, typename T2>
void DebugPrint(const XTL::SimpleTemplate & tmpl, const T1 & t1, const T2 & t2)
{
	XTL::WriteTemplate(tmpl, ScreenOutputStream::Instance(), t1, t2);
}

#include <xtl/pgsql/Connection.hpp>
#include <xtl/pgsql/Cursor.hpp>
#include <xtl/pgsql/QueryResult.hpp>
#include <xtl/pgsql/Exception.hpp>
#include <xtl/BTree.hpp>

#include <algorithm>

class TestInteger
{
	public:

		TestInteger()
			: value_(0), initialized_(false) { ;; }
		
		TestInteger(int value)
			: value_(value), initialized_(true) { ;; }

		bool IsInitialized() const
		{
			return initialized_;
		}

		TestInteger & operator= (int value)
		{
			value_ = value;
			initialized_ = true;
			return *this;
		}

		operator int & ()
		{
			return value_;
		}

		operator const int & () const
		{
			return value_;
		}

	private:

		int  value_;
		bool initialized_;
};

struct FriendsPair
{
	unsigned int user_id;
	unsigned int friend_id;

	FriendsPair()
		: user_id(0), friend_id(0) { ;; }

	FriendsPair(unsigned int user_id_, unsigned int friend_id_)
		: user_id(user_id_), friend_id(friend_id_) { ;; }

	bool operator< (const FriendsPair & other) const
	{
		if (user_id < other.user_id)
		{
			return true;
		}
		else if (user_id > other.user_id)
		{
			return false;
		}
		else if (friend_id < other.friend_id)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator== (const FriendsPair & other) const
	{
		return user_id == other.user_id && friend_id == other.friend_id;
	}
};

// typedef XTL::BTree<int, int, std::less<int>, false, 256, 31> MyTree;
typedef int MyTreeItemType;
typedef int MyTreeKeyType;
typedef XTL::BTree<MyTreeItemType, MyTreeKeyType, std::less<MyTreeKeyType>, false, 2, 2> MyTree;

#include <stdexcept>
void TestTree(const MyTree & tree)
{
	if (tree.SelfTest(false, false))
	{
		fprintf(stdout, "SelfTest...Ok!\n");
	}
	else
	{
		fprintf(stdout, "SelfTest...FAILED!!!\n");
		throw std::runtime_error("Error");
	}
}


#include <xtl/linux/fs/FilePath.hpp>
#include <xtl/linux/fs/FilePathTokenizer.hpp>

#include <stdexcept>

namespace XTL
{
/*
	class FilePathSet
	{
		public:

			virtual ~FilePathSet() throw() { ;; }

			virtual unsigned int Size() const = 0;

			virtual const std::string GetPath(unsigned int index) const = 0;
	};

	class FileTransaction
	{
		public:

			enum Status
			{
				INACTIVE = 0,
				ACTIVE   = 1,
				CLOSED   = 2,
				DONE     = 3
			};

			FileTransaction(const FilePath & directoryPath, const std::string & fileName)
				: status_(INACTIVE),
				  filePath_(directoryPath + fileName), 
				  file_(TemporaryFilePath(directoryPath, fileName)),
				  bufferedFileStream_(new FileOutputStream(file_))
			{
				;;
			}

			UINT_64 Size() const
			{
				return bufferedFileStream_.GetWrittenBytes();
			}

			OutputStream & GetOutputStream()
			{
				return bufferedFileStream_;
			}

			void Begin()
			{
				if (status_ != INACTIVE && status_ != DONE)
				{
					throw ILLEGAL_OPERATION_ERROR("Transaction is already active");
				}

				// file_.Create(File::CREATE_NEW_READ_WRITE);
				file_.Create(File::RECREATE_READ_WRITE);
				status_ = ACTIVE;
			}

			void Close()
			{
				if (status_ != ACTIVE)
				{
					throw ILLEGAL_OPERATION_ERROR("Transaction is not active");
				}

				bufferedFileStream_.Flush();
				file_.Flush();
				file_.Close();
				status_ = CLOSED;
			}

			void Commit()
			{
				if (status_ != CLOSED)
				{
					throw ILLEGAL_OPERATION_ERROR("Transaction is not closed");
				}

				file_.Rename(filePath_);
				status_ = DONE;
			}

			void Rollback() throw()
			{
				if (status_ == INACTIVE)
				{
					return;
				}

				try
				{
					file_.Close();
					file_.Unlink();
				}
				catch (...)
				{
					// TODO: Лютый bullshit. Роллбэк не удался. Че делать?
				}

				status_ = INACTIVE;
			}

		private:

			static const std::string TemporaryFilePath(const FilePath & directoryPath, const std::string & fileName)
			{
				return directoryPath + (fileName + ".tmp");
			}

			int status_;
			std::string filePath_;
			File file_;
			BufferedOutputStream bufferedFileStream_;
	};

	class FileSetTransaction
	{
		public:

			FileSetTransaction(const FilePath & directoryPath, const FilePathSet & filePathSet)
				: files_(filePathSet.Size())
			{
				for (unsigned int i = 0; i < filePathSet.Size(); ++i)
				{
					files_.Set(i, new FileTransaction(directoryPath, filePathSet.GetPath(i)));
				}
			}

			OutputStream & GetOutputStream(unsigned int index)
			{
				return files_[index]->GetOutputStream();
			}

			UINT_64 Size(unsigned int index) const
			{
				return files_[index]->Size();
			}

			void Begin()
			{
				AutoRollerBack arb(*this);

				for (unsigned int i = 0; i < files_.Size(); ++i)
				{
					files_[i]->Begin();
				}

				arb.DoNotWorry();
			}

			void Commit()
			{
				AutoRollerBack arb(*this);

				for (unsigned int i = 0; i < files_.Size(); ++i)
				{
					files_[i]->Close();
				}

				for (unsigned int i = 0; i < files_.Size(); ++i)
				{
					files_[i]->Commit();
				}

				arb.DoNotWorry();
			}

			void Rollback() throw()
			{
				for (unsigned int i = 0; i < files_.Size(); ++i)
				{
					files_[i]->Rollback();
				}
			}

		private:

			class AutoRollerBack
			{
				public:

					AutoRollerBack(FileSetTransaction & fileSet)
						: fileSet_(&fileSet) { ;; }

					~AutoRollerBack() throw()
					{
						if (fileSet_ != 0)
						{
							fileSet_->Rollback();
						}
					}

					void DoNotWorry()
					{
						fileSet_ = 0;
					}

				private:

					FileSetTransaction * fileSet_;
			};

			FileSetTransaction(const FileSetTransaction &);
			FileSetTransaction & operator= (const FileSetTransaction &);

			AutoPtrVector<FileTransaction> files_;
	};
*/
}

#include <xtl/linux/fs/FileUtils.hpp>

namespace XTL
{
/*
	TARGET_DIR/file1
	TARGET_DIR/file2

	TARGET_DIR/file1.old  - hard link to TARGET_DIR/file1
	TARGET_DIR/file2.old

	TARGET_DIR/file1.new
	TARGET_DIR/file2.new

	WORK_DIR/NEW/1 -> TARGET_DIR/file1.new
	WORK_DIR/NEW/2 -> TARGET_DIR/file2.new
	WORK_DIR/OLD/1 -> TARGET_DIR/file1.old
	WORK_DIR/OLD/2 -> TARGET_DIR/file2.old
	WORK_DIR/tobe  -> WORK_DIR/NEW
	WORK_DIR/live  -> WORK_DIR/OLD

*/
/*

	bool CreateDirectory(const std::string & filePath)
	{
	}

	void CreatePath(const std::string & directoryPath)
	{
	}

	void CreateParentPath(const std::string & filePath)
	{
	}

	template <typename Iterator_>
	void CreatePath(const std::string & baseDirectory, Iterator_ begin, Iterator_ end)
	{
		if (begin == end)
		{
			return false;
		}

		std::string currentPath = baseDirectory;
		Iterator_ itr(begin);

		currentPath.append(*itr);
		bool result = CreateDirectory(currentPath);

		for (++itr; itr != end; ++itr)
		{
			currentPath.append("/").append(*itr);
			result = CreateDirectory(currentPath);
		}

		return result;
	}

	bool CreatePath(const std::string & filePath)
	{
		XTL::FilePath currentFilePath(XTL::FileUtils::GetCurrentDirectory());
		for (XTL::FilePath::Iterator itr(currentFilePath); !itr.AtEnd(); itr.Advance())
		{
			printf("%s\n", itr.CurrentPath().c_str());
		}
	}
*/
}

#include <xtl/utils/Date.hpp>

class TimePeriod
{
	public:

		template <typename T>
		struct Singleton
		{
			static const T & Instance()
			{
				static T instance;
				return instance;
			}
		};

		struct Type
		{
			virtual ~Type() throw() { ;; }

			virtual const std::string Stringify(const XTL::Date & date) const = 0;

			virtual const TimePeriod ParentPeriod(const XTL::Date & date) const = 0;

			struct Year;
			struct Month;
			struct Day;
		};

		struct Type::Year : public Type, Singleton<Year>
		{
			virtual const std::string Stringify(const XTL::Date & date) const
			{
				return XTL::FormatString("%04d", date.Year());
			}

			virtual const TimePeriod ParentPeriod(const XTL::Date & date) const
			{
				throw ILLEGAL_OPERATION_ERROR("Period 'year' has not parent");
			}
		};

		struct Type::Month : public Type, Singleton<Month>
		{
			virtual const std::string Stringify(const XTL::Date & date) const
			{
				return XTL::FormatString("%04d-%02d", date.Year(), date.Month());
			}

			virtual const TimePeriod ParentPeriod(const XTL::Date & date) const
			{
				return TimePeriod(date.Year());
			}
		};

		struct Type::Day : public Type, Singleton<Day>
		{
			virtual const std::string Stringify(const XTL::Date & date) const
			{
				return XTL::FormatString("%04d-%02d-%02d", date.Year(), date.Month(), date.Day());
			}

			virtual const TimePeriod ParentPeriod(const XTL::Date & date) const
			{
				return TimePeriod(date.Year(), date.Month());
			}
		};

		TimePeriod(int year)
			: type_(Type::Year::Instance()),
			  date_(year, 1, 1)
		{
			;;
		}

		TimePeriod(int year, int month)
			: type_(Type::Month::Instance()),
			  date_(year, month, 1)
		{
			;;
		}

		TimePeriod(int year, int month, int day)
			: type_(Type::Day::Instance()),
			  date_(year, month, day)
		{
			;;
		}

		const std::string ToString() const
		{
			return type_.Stringify(date_);
		}

		const TimePeriod ParentPeriod() const
		{
			return type_.ParentPeriod(date_);
		}

	private:

		const Type & type_;
		XTL::Date    date_;
};

/*
class JournalStatsTable
{
	public:

		class Type
		{
			public:

				int Value() const
				{
					return value_;
				}

			private:

				friend class JournalStatsTable;

				explicit Type(int value) : value_(value) { ;; }

				int value_;
		};

		static const Type WITHOUT_FRIENDSPAGE;
		static const Type WITH_FRIENDSPAGE;

		class PathSet : public XTL::FilePathSet
		{
			public:

				PathSet(const Type & type, const TimePeriod & period, bool withUniques)
					: period_(period),
					  withFriends_(type.Value()),
					  withUniques_(withUniques) { ;; }

				virtual ~PathSet() throw() { ;; }

				virtual unsigned int Size() const
				{
					return withUniques_ ? 3 : 2;
				}

				virtual const std::string GetPath(unsigned int index) const
				{
					return XTL::FormatString(FILE_PATH_TEMPLATES[index], withFriends_, period_.ToString().c_str());
				}

			private:

				static const char * const FILE_PATH_TEMPLATES[];

				const TimePeriod period_;
				const int        withFriends_;
				const bool       withUniques_;
		};
};

const JournalStatsTable::Type JournalStatsTable::WITHOUT_FRIENDSPAGE(0);
const JournalStatsTable::Type JournalStatsTable::WITH_FRIENDSPAGE(1);

const char * const JournalStatsTable::PathSet::FILE_PATH_TEMPLATES[] = {
	"journal_stats_%d-%s.idx",
	"journal_stats_%d-%s.dat",
	"journal_stats_%d-%s.uniq"
};
*/
/*
class LinearHashBucket
{
};

class LinearHash
{
	public:

		typedef std::deque<LinearHashBucket> LinearHashBucketArray;

		LinearHash()
			: buckets_(),
			  size_(0),
			  level_(0),
			  split_(0)
		{
			buckets_.push_back(new LinearHashBucket());
		}

		~LinearHash() throw()
		{
			const LinearHashBucketArray::iterator end = buckets_.end();
			for (LinearHashBucketArray::iterator itr = buckets_.begin; itr != end; ++itr)
			{
				delete (*itr);
			}
		}

		unsigned int Size() const
		{
			return size_;
		}

		bool Insert(const T & value)
		{
			unsigned int hashCode = HashFunction(value);
			unsigned int slot = HashLevel(hashCode, level_);
			if (slot < split_)
			{
				slot = HashLevel(hash, level_ + 1);
			}
		}

	private:

		unsigned int HashLevel(unsigned int hashCode, unsigned int level)
		{
			return hashCode & ((1 << level) - 1);
		}

		std::deque<LinearHashBucket *> buckets_;
		unsigned int size_;
		unsigned int level_;
		unsigned int split_;
};
*/

/*
class StructComparator
{
	public:

		StructComparator(const StructPrototype * prototype)
			: prototype_(prototype)
			  fields_(prototype->FieldsCount())
		{
			;;
		}

		int Compare(StructConstRef left, StructConstRef right)
		{
			// ASSERT: left->Prototype() == right->Prototype()

			for (unsigned int i = 0; i < prototype->FieldsCount(); ++i)
			{
				int result = prototype_->GetField(fields_[i]).Type().Compare(left, right);
				if (result != 0)
				{
					return result;
				}
			}

			return 0;
		}

	private:

		const StructPrototype * prototype_;
		std::vector<unsigned int> fields;
};
*/

#include <xtl/linux/net/SocketAddressInet.hpp>
#include <xtl/linux/net/TcpClientSocket.hpp>
#include <xtl/linux/net/TcpServerSocket.hpp>
#include <xtl/linux/net/TcpSocketSelector.hpp>
#include <math.h>

/*
namespace XTL
{
class TcpSocketSelector
{
	public:

		class SelectResult
		{
			public:

				SelectResult()
					: readSet_(),
					  writeSet_()
				{
					;;
				}

				bool IsReadable(int fd) const
				{
					return !IsDescInvalid(fd) && readSet_.Contains(fd);
				}

				bool IsWritable(int fd) const
				{
					return !IsDescInvalid(fd) && writeSet_.Contains(fd);
				}

			private:

				friend class TcpSocketSelector;


		};
};
}
*/

class AutoChronometer
{
	public:

		explicit AutoChronometer(const std::string & message)
			: chrono_(),
			  message_(message)
		{
			chrono_.Start();
		}

		~AutoChronometer() throw()
		{
			chrono_.Stop();
			printf("%s%0.3f\n", message_.c_str(), chrono_.Elapsed());
		}

	private:

		XTL::Chronometer  chrono_;
		const std::string message_;
};

#include <xtl/plain/FieldType.hpp>
#include <xtl/plain/Field.hpp>
#include <xtl/plain/Struct.hpp>
#include <xtl/plain/StructPrototype.hpp>
#include <xtl/plain/StructPrototypeBuilder.hpp>

struct S
{
	XTL::UINT_32 journal_id;
	XTL::UINT_64 data_offset;
	char         country[2];
};


template <typename T> struct Fuck
{
	static void Execute()
	{
		printf("Fuck!\n");
	}
};

template <> struct Fuck<XTL::UINT_32>
{
	static void Execute()
	{
		printf("It's UINT_32\n");
	}
};

template <typename T> void Execute(T)
{
	Fuck<T>::Execute();
}

#include <stddef.h>

template <typename T>
struct Alignment
{
	struct Dummy
	{
		char c;
		T    value;
	};

	static const unsigned int Value = offsetof(Dummy, value);
};

class Scalar
{
	public:


	private:

		union {
			XTL::INT_64 i_;
			double      f_;
		};

		const char * s_;
};

class CharSourceText
{
	public:

		static const char CHAR_NULL = '\0';

		virtual ~CharSourceText() throw()
		{
			;;
		}

		virtual bool NotAtEnd() const
		{
			return GetChar() != CHAR_NULL;
		}

		virtual char GetChar() const = 0;

		virtual void Advance() = 0;

		virtual void Mark() = 0;

		virtual void Unmark() = 0;

		virtual const std::string GetString() const = 0;

		bool AtEnd() const
		{
			return !NotAtEnd();
		}
};

class CharSourceTextCharPtr : public CharSourceText
{
	public:

		explicit CharSourceTextCharPtr(const char * ptr)
			: ptr_(ptr),
			  marked_(ptr)
		{
			;;
		}

		~CharSourceTextCharPtr() throw()
		{
			;;
		}

		virtual char GetChar() const
		{
			return *ptr_;
		}

		virtual void Advance()
		{
			++ptr_;
		}

		virtual void Mark()
		{
			marked_ = ptr_;
		}

		virtual void Unmark()
		{
			marked_ = 0;
		}

		virtual const std::string GetString() const
		{
			return std::string(marked_, ptr_ - marked_);
		}

	private:

		const char * ptr_;
		const char * marked_;
};

class IntegerParser
{
	public:

		IntegerParser(bool plusAllowed, bool minusAllowed)
			: plusAllowed_(plusAllowed),
			  minusAllowed_(minusAllowed)
		{
			;;
		}

		template <typename T>
		const T Parse(CharSourceText & charSource)
		{
			bool negative = false;

			char c = charSource.GetChar();
			if (c == '+')
			{
				if (IsPlusAllowed())
				{
					charSource.Advance();
				}
				else
				{
					throw std::runtime_error("IntegerParser - plus is not allowed");
				}
			}
			else if (c == '-')
			{
				if (IsMinusAllowed())
				{
					negative = true;
					charSource.Advance();
				}
				else
				{
					throw std::runtime_error("IntegerParser - minus is not allowed");
				}
			}

			if (!IsDigit(c))
			{
				throw std::runtime_error("IntegerParser - decimal digit expected");
			}

			T result = CharToDigit(c);

			charSource.Advance();
			c = charSource.GetChar();

			while (IsDigit(c))
			{
				result = 10 * result + CharToDigit(c);
				charSource.Advance();
				c = charSource.GetChar();
			}

			return negative ? -result : result;
		}

	private:

		bool IsPlusAllowed() const
		{
			return plusAllowed_;
		}

		bool IsMinusAllowed() const
		{
			return minusAllowed_;
		}

		bool IsDigit(char c) const
		{
			return c >= '0' && c <= '9';
		}

		int CharToDigit(char c) const
		{
			return c - '0';
		}

		const bool plusAllowed_;
		const bool minusAllowed_;
};

class StringEscapeSequenceParser
{
	public:

		virtual ~StringEscapeSequenceParser() throw() { ;; }

		virtual char Parse(CharSourceText & charSource) = 0;
};

#include <map>

template <typename KeyType_, typename ValueType_>
class AutoPtrMap
{
	public:

		typedef KeyType_   KeyType;
		typedef ValueType_ ValueType;

		AutoPtrMap()
			: map_()
		{
			;;
		}

		~AutoPtrMap() throw()
		{
			const typename std::map<KeyType, ValueType *>::iterator end = map_.end();
			for (typename std::map<KeyType, ValueType *>::iterator itr = map_.begin(); itr != end; ++itr)
			{
				delete itr->second;
			}
		}

		AutoPtrMap & Set(const KeyType & key, std::auto_ptr<ValueType> value)
		{
			typename std::map<KeyType, ValueType *>::iterator itr = map_.find(key);

			if (itr != map_.end())
			{
				delete itr->second;
				itr->second = value.release();
			}
			else
			{
				map_[key] = value.release();
			}

			return *this;
		}

		ValueType * operator[] (const KeyType & key) const
		{
			typename std::map<KeyType, ValueType *>::const_iterator itr = map_.find(key);

			return itr == map_.end() ? 0 : itr->second;
		}

	private:

		AutoPtrMap(const AutoPtrMap &);
		AutoPtrMap & operator= (const AutoPtrMap &);

		std::map<KeyType, ValueType *> map_;
};

class EscapeSequenceParser
{
	public:

		class Subparser
		{
			public:

				virtual ~Subparser() throw() { ;; }

				virtual void Parse(CharSourceText & charSource, std::string & result) const = 0;
		};

		EscapeSequenceParser()
			: subparsers_()
		{
			;;
		}

		bool Parse(CharSourceText & charSource, std::string & result) const
		{
			Subparser * subparser = subparsers_[charSource.GetChar()];
			if (subparser != 0)
			{
				subparser->Parse(charSource, result);
				return true;
			}
			else
			{
				return false;
			}
		}

	protected:

		void AddSubparser(char key, std::auto_ptr<Subparser> subparser)
		{
			subparsers_.Set(key, subparser);
		}

	private:

		AutoPtrMap<char, Subparser> subparsers_;
};

class JsonEscapeSequenceParser : public EscapeSequenceParser
{
	public:

		JsonEscapeSequenceParser()
			: EscapeSequenceParser()
		{
			AddSubparser('\\', ParseEscapeChar('\\'));
			AddSubparser('/', ParseEscapeChar('/'));
			AddSubparser('"', ParseEscapeChar('"'));
			AddSubparser('t', ParseEscapeChar('\t'));
			AddSubparser('r', ParseEscapeChar('\r'));
			AddSubparser('n', ParseEscapeChar('\n'));
			AddSubparser('b', ParseEscapeChar('\b'));
			AddSubparser('f', ParseEscapeChar('\f'));
			// AddSubparser('u', ParseUnicodeChar());
		}

	protected:

		class EscapeChar : public Subparser
		{
			public:

				explicit EscapeChar(char ch)
					: Subparser(),
					  ch_(ch)
				{
					;;
				}

				virtual ~EscapeChar() throw()
				{
					;;
				}

				virtual void Parse(CharSourceText & charSource, std::string & result) const
				{
					charSource.Advance();
					result.append(1, ch_);
				}

			private:

				const char ch_;
		};

		class UnicodeChar : public Subparser
		{
			public:

				virtual ~UnicodeChar() throw()
				{
					;;
				}

				virtual void Parse(CharSourceText & charSource, std::string & result) const
				{
					charSource.Advance();

					// TODO: realize it!!!
				}

			private:
		};

		std::auto_ptr<Subparser> ParseEscapeChar(char ch) const
		{
			return std::auto_ptr<Subparser>(new EscapeChar(ch));
		}

		std::auto_ptr<Subparser> ParseUnicodeChar() const
		{
			return std::auto_ptr<Subparser>(new UnicodeChar());
		}
};

class StringLiteralParser
{
	public:

		StringLiteralParser(char boundingChar, char escapeSequenceChar)
			: boundingChar_(boundingChar),
			  escapeSequenceChar_(escapeSequenceChar)
		{
			;;
		}

		virtual ~StringLiteralParser() throw()
		{
			;;
		}

		const std::string Parse(CharSourceText & charSource) const
		{
			// ASSERT(charSource.GetChar() == boundingChar_)

			std::string result;

			charSource.Advance();
			charSource.Mark();

			char c;
			while ((c = charSource.GetChar()) != boundingChar_)
			{
				if (charSource.AtEnd())
				{
					throw std::runtime_error("Error");
				}
				else if (c == escapeSequenceChar_)
				{
					result.append(charSource.GetString());
					charSource.Unmark();

					charSource.Advance();

					if (GetEscapeSequenceParser().Parse(charSource, result))
					{
						charSource.Mark();
					}
					else
					{
						throw std::runtime_error(XTL::FormatString("Invalid escape sequence in string literal: %c%c", escapeSequenceChar_, charSource.GetChar()));
					}
				}
				else
				{
					charSource.Advance();
				}
			}

			result.append(charSource.GetString());
			charSource.Unmark();

			return result;
		}

	protected:

		virtual const EscapeSequenceParser & GetEscapeSequenceParser() const = 0;

	private:

		const char boundingChar_;
		const char escapeSequenceChar_;
};

class JsonStringLiteralParser : public StringLiteralParser
{
	public:

		JsonStringLiteralParser()
			: StringLiteralParser('"', '\\')
		{
			;;
		}

		virtual ~JsonStringLiteralParser() throw()
		{
			;;
		}

	protected:

		virtual const EscapeSequenceParser & GetEscapeSequenceParser() const
		{
			static JsonEscapeSequenceParser instance;

			return instance;
		}
};

int main(int argc, const char * argv[])
{
	CharSourceTextCharPtr charSource("\"aaa\\t\\\"\\\"\\\\bbbbb\"");

	JsonStringLiteralParser parser;

	printf("%s\n", parser.Parse(charSource).c_str());

//	printf("%lu\n", sizeof(Scalar));
	return 0;

	HexOutputStream hos(16);

	return 0;

	printf("INT_16 = %u\n", Alignment<XTL::INT_16>::Value);
	printf("INT_32 = %u\n", Alignment<XTL::INT_32>::Value);
	printf("INT_64 = %u\n", Alignment<XTL::INT_64>::Value);
	printf("float  = %u\n", Alignment<float>::Value);
	printf("double = %u\n", Alignment<double>::Value);
	printf("char * = %u\n", Alignment<char *>::Value);
	printf("void * = %u\n", Alignment<void *>::Value);
	printf("S      = %u\n", Alignment<S>::Value);

	printf("%lu\n", (unsigned long) offsetof(S, journal_id));
	printf("%lu\n", (unsigned long) offsetof(S, data_offset));
	printf("%lu\n", (unsigned long) offsetof(S, country));

	return 0;

	const int TEST_SIZE = 3000000;

	{
		AutoChronometer chrono("FileOutputStream...");
		XTL::FileOutputStream fos("test-data.1");

		for (int i = 0; i < TEST_SIZE; ++i)
		{
			fos.Write(&i, sizeof(i));
		}

		// fos.Close();
	}

	return 0;

	XTL::SocketAddressInet address("www.livejournal.ru", 80);

	printf("%s:%d\n", address.Host().c_str(), address.Port());

	XTL::TcpClientSocket clientSocket = XTL::TcpClientSocket::Create(false);

	try
	{
		printf("IsBlocking: %d\n", clientSocket.IsBlocking() ? 1 : 0);
		clientSocket.SetBlocking(true);
		printf("IsBlocking: %d\n", clientSocket.IsBlocking() ? 1 : 0);
		clientSocket.SetBlocking(false);
		printf("IsBlocking: %d\n", clientSocket.IsBlocking() ? 1 : 0);

		XTL::TcpSocketSelector socketSelector;
		socketSelector.Insert(clientSocket, true, true);

		if (clientSocket.Connect(address))
		{
			printf("Connected\n");
		}
		else
		{
			printf("Connect failed\n");
		}

		XTL::TcpSocketSelector::SelectResult selectResult;

		while (1)
		{
			socketSelector.Select(selectResult, XTL::TcpSocketSelector::Timeout(0, 100));
			printf("selected: %d\n", selectResult.SelectedCount());
			printf("readable=%d, writable=%d\n", selectResult.ReadableSet().Contains(clientSocket), selectResult.WritableSet().Contains(clientSocket));
			if (selectResult.SelectedCount() > 0)
			{
				break;
			}
		}

		while (1)
		{
			socketSelector.Select(selectResult, XTL::TcpSocketSelector::Timeout(0, 100));
			printf("selected: %d\n", selectResult.SelectedCount());
			printf("readable=%d, writable=%d\n", selectResult.IsReadable(clientSocket), selectResult.IsWritable(clientSocket));
			if (selectResult.SelectedCount() > 0)
			{
				break;
			}
		}


		return 0;


		const char * const request = "GET / HTTP/1.0\r\nHost: www.ya.ru\r\n\r\n";
		clientSocket.Send(request, strlen(request));
		printf("Request was sent\n");
		while (1)
		{
			char buffer[1024];
			int size = clientSocket.Receive(buffer, sizeof(buffer));
			if (size < 0)
			{
				break;
			}

			if (size != 0)
			{
				buffer[size] = '\0';
				printf("%s", buffer);
				// printf("%d\n", size);
			}
		}

		printf("\n");

		clientSocket.Close();

		/*
		XTL::TcpSocket serverSocket = XTL::TcpSocket::Create();
		XTL::SocketAddressInet serverAddress("127.0.0.1", 1133);
		serverSocket.Bind(serverAddress).Listen(5);
		getc(stdin);
		*/
	}
	catch (const XTL::UnixError & e)
	{
		printf("UnixError: %s\n", e.What().c_str());
	}

	return 0;
/*
	TimePeriod period(2012, 2, 29);
	JournalStatsTable::PathSet ps(JournalStatsTable::WITHOUT_FRIENDSPAGE, period, true);
	for (unsigned int i = 0; i < ps.Size(); ++i)
	{
		printf("%s\n", ps.GetPath(i).c_str());
	}

	XTL::FileSetTransaction fst(XTL::FilePath("."), ps);

	printf("Started\n");
	fst.Begin();
	printf("Transaction has begun\n");

	fst.Commit();

	return 0;
*/
	CharSourceTextCharPtr cs("-0001234567890");
	IntegerParser p(true, true);
	printf("%d\n", p.Parse<unsigned int>(cs));
	return 0;

	printf("Ok!\n");

	return 0;

	try
	{
		XTL::FileUtils::Rename("new", "old");
	}
	catch (const XTL::UnixError & e)
	{
		printf("%s\n", e.What().c_str());
	}

	return 0;

	XTL::FilePath currentFilePath(XTL::FileUtils::GetCurrentDirectory());
	for (XTL::FilePath::Iterator itr(currentFilePath); !itr.AtEnd(); itr.Advance())
	{
		printf("%s\n", itr.CurrentPath().c_str());
	}
	return 0;

	XTL::FileUtils::Rename("1", "2");
	return 0;


	MyTree btree;
	bool tmp;

	srand(time(0));

	int CC = 100000;
	for (int i = 0; i < CC; ++i)
	{
		btree.Insert(i, tmp);
	}

	TestTree(btree);

	while (!btree.Empty())
	{
		if (btree.Delete(rand() % CC))
		{
			TestTree(btree);
			printf("%u\n", btree.Size());
		}
	}

	for (int i = 0; i < CC; ++i)
	{
		btree.Delete(i);
		// btree.DebugPrint();
		// TestTree(btree);
	}

	TestTree(btree);
	return 0;

	XTL::Chronometer chrono;
	chrono.Start();
	for (unsigned int i = 0; i < 300; ++i)
	{
		//XTL::Chronometer chrono;
		//chrono.Start();
		for (unsigned int j = 0; j < 1000000; ++j)
		{
			int v = rand();
			v = v;
			btree.Insert(i * 1000000 + j, tmp);
			// btree.Insert(299999999 - (i * 1000000 + j), tmp);
			// btree.Insert(v, tmp);
			// btree.Insert(FriendsPair(i * 1000000 + j, v), tmp);
		}
		//chrono.Stop();
		//printf("%3u - %0.3f\n", i, chrono.Elapsed());
		//TestTree(btree);
	}
	chrono.Stop();

	printf("Time elapsed.....%0.3f\n", chrono.Elapsed());
	printf("Tree size........%u\n", btree.Size());
	printf("Leaves count.....%u\n", btree.LeavesCount());
	printf("Branches count...%u\n", btree.BranchesCount());
	TestTree(btree);
	getc(stdin);

/*
	for (unsigned int i = 0; i < COUNT; ++i)
	{
		int v = rand();
//		printf("%u - %d\n", i, v);
		btree.Insert(v, tmp);
//		btree.DebugPrint();
//		if (!btree.SelfTest(false, false))
//		{
//			return 0;
//		}
	}
*/
/*
	TestTree(btree);
	getc(stdin);
	return 0;

	for (unsigned int i = 0; i < 20; ++i)
	{
		btree.Insert(20 - i, tmp);
		btree.DebugPrint();
		TestTree(btree);
		fprintf(stdout, "---------------------------------------------------------------------\n");
	}

	return 0;

	printf("===================================================================\n");

	while (!btree.Empty())
	{
		btree.DebugPrint();
		btree.Delete(0);
		btree.DebugPrint();
		TestTree(btree);
		printf("-------------------------------------------------------------------\n");
	}
*/
	return 0;

	fprintf(stdout, "%u\n", btree.Size());
	fprintf(stdout, "[ ");
	for (MyTree::ConstIterator itr = btree.Begin(); !itr.AtEnd(); itr.Advance())
	{
		fprintf(stdout, "%d ", *itr);
	}
	fprintf(stdout, "]\n");

	if (btree.SelfTest())
	{
		fprintf(stdout, "SelfTest...Ok!\n");
	}
	else
	{
		fprintf(stdout, "SelfTest...FAILED!!!\n");
	}

	return 0;

	XTL::PGSQL::ConnectionConfig connectionConfig;
	connectionConfig.host     = "172.21.9.166";
	connectionConfig.port     = 54320;
	connectionConfig.database = "usersdb";
	connectionConfig.user     = "stats";
	connectionConfig.password = "Gl08alStat$";

	XTL::PGSQL::Connection dbc(connectionConfig);
	printf("%s\n", dbc.ConnectionString().c_str());

	dbc.Open();

	XTL::PGSQL::Cursor cursor(dbc);
	unsigned int total = 0;
	try {
		dbc.Execute("BEGIN;");
		// cursor.Open("all_users", "SELECT users.user_id, users.user_name FROM users LEFT JOIN users_stats ON (users.user_id = users_stats.user_id);");
		cursor.Open("all_users", "SELECT users.user_id, users.user_name FROM users;");
		XTL::PGSQL::QueryResult result;
		unsigned int fetchSize = 0;
		do {
			result = cursor.Fetch(10000);
			fetchSize = result.RowsCount();
			total += fetchSize;
			printf("%u\n", total);
		}
		while (fetchSize > 0);
	}
	catch (const XTL::PGSQL::QueryError & e) {
		printf("%s\n", e.What().c_str());
	}

	cursor.Close();

/*
	XTL::PGSQL::QueryResult result = dbh.Execute("SELECT user_id FROM users WHERE user_name = 'wildkin';");
	if (result.Success()) {
		printf("%s\n", result.GetString(0, 0).c_str());
	}
*/
	// printf("%s\n", XTL::FormatString("%0.0f", (double) 1.01e6).c_str());

	return 0;

	static const XTL::SimpleTemplate tmpl("Hello, $_! ($_)\n");
	DebugPrint(tmpl, "Leecha Hyboid", XTL::TypeTraits<short int>::MaxValue);
	DebugPrint(tmpl, "Dmitnin", XTL::TypeTraits<long long int>::MinValue);
	return 0;

	printf("%s\n", XTL::IntegerToString(XTL::TypeTraits<long long int>::MinValue).c_str());
	XTL::SimpleTemplateValueArray values(4);
	values.SetReference(1, "FUCK!");
	values.SetCopy(2, 123);
	values.SetCopy(3, 'Z');
	return 0;

	// XTL::SimpleTemplate tmpl("FUCK: $_$$");
	// printf("%s\n", GetTemplateSignature(tmpl).c_str());

	return 0;

//	printf("%s\n", XTL::String("$_ - $_ ($1, $0$1$1$1$$, $1)", "abc", '?').c_str());

	return 0;

	printf("%lld\n", XTL::TypeTraits<long long>::MinValue);

//	templateValues.Set(0, "abc");
//	templateValues.Set(1, "xyz");
/*
	StringTemplate tmpl("$_ : $_\n");
	tmpl.Evaluate(stdoutOutputter, "fuck", "you");
	tmpl.Evaluate(stdoutOutputter, "Kill", "Shurutov");
*/

//	StringTemplate tmpl("%1 - %2");
//	StringTemplateParser parser("fuck %_ $1$$$2$3 this");
//	StringTemplateDebugger("%_%2$_$$$$$$$$$$%_%0");
	return 0;

	try
	{
		if (::fopen("abc", "r") == 0)
		{
			throw XTL::UnixError();
		}
	}
	catch (const XTL::UnixError & e)
	{
		fprintf(stderr, "Error: %s\n", e.What().c_str());
	}

	return 0;
}
