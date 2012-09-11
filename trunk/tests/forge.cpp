#include <cstdio>
#include <cstdlib>

#include <deque>
#include <list>

#include <xtl/utils/Chronometer.hpp>
#include <xtl/Exception.hpp>
#include <xtl/linux/UnixError.hpp>
#include <xtl/linux/fs/File.hpp>
#include <xtl/linux/fs/FileStats.hpp>
#include <xtl/io/OutputStream.hpp>
#include <xtl/io/FileOutputStream.hpp>

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

#include <xtl/simple_template/SimpleTemplate.hpp>
#include <xtl/simple_template/SimpleTemplateItemVisitor.hpp>
#include <xtl/simple_template/SimpleTemplateValueArray.hpp>
#include <xtl/simple_template/SimpleTemplateValueString.hpp>
#include <xtl/simple_template/SimpleTemplateValueInteger.hpp>
#include <xtl/simple_template/SimpleTemplateValueChar.hpp>
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
#include <xtl/linux/net/SocketSelector.hpp>
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

#include <xtl/tp/TextCharSource.hpp>


class StringEscapeSequenceParser
{
	public:

		virtual ~StringEscapeSequenceParser() throw() { ;; }

		virtual char Parse(XTL::TextCharSource & charSource) = 0;
};


#include <xtl/utils/AutoPtrMap.hpp>

class EscapeSequenceParser
{
	public:

		class Subparser
		{
			public:

				virtual ~Subparser() throw() { ;; }

				virtual void Parse(XTL::TextCharSource & charSource, std::string & result) const = 0;
		};

		EscapeSequenceParser()
			: subparsers_()
		{
			;;
		}

		bool Parse(XTL::TextCharSource & charSource, std::string & result) const
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

		XTL::AutoPtrMap<char, Subparser> subparsers_;
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

				virtual void Parse(XTL::TextCharSource & charSource, std::string & result) const
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

				virtual void Parse(XTL::TextCharSource & charSource, std::string & result) const
				{
					charSource.Advance();
					throw std::runtime_error("UnicodeChar::Parse() is unimplemented");
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

		const std::string Parse(XTL::TextCharSource & charSource) const
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
					result.append(charSource.ReleaseString());
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

			result.append(charSource.ReleaseString());
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


/*
namespace XTL
{
class TextParser
{
	public:

		class Error : public Exception
		{
			public:

				Error(const TextCursor & cursor, const std::string & what)
					: Exception(),
					  cursor_(cursor),
					  what_(what)
				{
					;;
				}

				virtual ~Error() throw()
				{
					;;
				}

				virtual const std::string What() const
				{
					return what_;
				}

			private:

				const TextCursor  cursor_;
				const std::string what_;
		};

		static bool IsWhiteSpace(char c)
		{
			return HasClass(c, CHAR_SPACE | CHAR_TAB | CHAR_CARRIAGE_RETURN | CHAR_LINE_FEED);
		}

		static bool IsAlpha(char c)
		{
			return HasClass(c, CHAR_LETTER_UPPERCASE | CHAR_LETTER_LOWERCASE);
		}

		static bool IsDigit(char c)
		{
			return HasClass(c, CHAR_DIGIT);
		}

		static bool IsIdentifierHead(char c)
		{
			return HasClass(c, CHAR_LETTER_UPPERCASE | CHAR_LETTER_LOWERCASE | CHAR_UNDERSCORE);
		}

		static bool IsIdentifierTail(char c)
		{
			return HasClass(c, CHAR_LETTER_UPPERCASE | CHAR_LETTER_LOWERCASE | CHAR_UNDERSCORE | CHAR_DIGIT);
		}

		static int HexToInt(char c)
		{
			if (c >= 'A' && c <= 'F')
			{
				return 10 + (c - 'A');
			}
			else if (c >= 'a' && c <= 'f')
			{
				return 10 + (c - 'a');
			}
			else
			{
				return DecToInt(c);
			}
		}

		static int DecToInt(char c)
		{
			if (c >= '0' && c <= '9')
			{
				return c - '0';
			}

			return -1;
		}

		static const std::string ParseIdentifier(TextCharSource & charSource)
		{
			return "";
		}

		template <typename T>
		static void ParseDecInteger(TextCharSource & charSource, T & result)
		{
			int digit = DecToInt(charSource.GetChar());
			if (digit < 0)
			{
				throw TextParser::Error(charSource.GetCursor(), "ParseDecInteger requires decimal digit");
			}

			result = digit;
			charSource.Advance();

			while ((digit = DecToInt(charSource.GetChar())) >= 0)
			{
				result *= 10;
				result += digit;
				charSource.Advance();
			}
		}

		template <typename T>
		static void ParseHexInteger(TextCharSource & charSource, T & result)
		{
			int digit = HexToInt(charSource.GetChar());
			if (digit < 0)
			{
				throw TextParser::Error(charSource.GetCursor(), "ParseHexInteger requires hex digit");
			}

			result = digit;
			charSource.Advance();

			while ((digit = HexToInt(charSource.GetChar())) >= 0)
			{
				result <<= 4;
				result += digit;
				charSource.Advance();
			}
		}

	private:

		static bool HasClass(char c, XTL::UINT_64 charClass)
		{
			return BaseCharClassifier::Instance().HasClass(c, charClass);
		}
};
}
*/

#include <xtl/BitSet.hpp>

#include <xtl/tp/TextCharSource.hpp>

/*
namespace XTL
{
	class NumberParser : private TextParser
	{
		public:

			class Result
			{
				private:

					enum Type
					{
						INTEGER = 0,
						FLOAT   = 1
					};

					union Value
					{
						long long int i;
						double f;

						Value(long long int value)
							: i(value)
						{
							;;
						}

						Value(double value)
							: f(value)
						{
							;;
						}
					};

				public:

					explicit Result(long long int value)
						: type_(INTEGER),
						  value_(value)
					{
						;;
					}

					explicit Result(double value)
						: type_(FLOAT),
						  value_(value)
					{
						;;
					}

					bool IsInteger() const
					{
						return type_ == INTEGER;
					}

					bool IsFloat() const
					{
						return type_ == FLOAT;
					}

					void CastToFloat()
					{
						if (type_ != FLOAT)
						{
							type_ = FLOAT;
							double f = value_.i;
							value_.f = f;
						}
					}

					long long int AsInteger() const
					{
						return value_.i;
					}

					double AsFloat() const
					{
						return value_.f;
					}

					double & AsFloat()
					{
						return value_.f;
					}

					const std::string ToString() const
					{
						return IsInteger() ?
							FormatString("I: %lld", AsInteger()) :
							FormatString("F: %g", AsFloat());
					}

				private:

					Type type_;
					Value value_;
			};

			template <typename T>
			static const T ParseInteger(TextCharSource & charSource)
			{
				return NumberParser(charSource).ParseInteger<T>();
			}

			explicit NumberParser(TextCharSource & charSource)
				: TextParser(charSource)
			{
				;;
			}

			template <typename T>
			const T ParseInteger()
			{
				bool negative = false;

				if (GetChar() == '-')
				{
					negative = true;
					Advance();
				}

				if (!IsDigit(GetChar()))
				{
					throw std::runtime_error("");
				}

				T result = CharToInt(GetChar());
				Advance();

				for (char c = GetChar(); IsDigit(c); Advance(), c = GetChar())
				{
					result *= 10;
					result += CharToInt(c);
				}

				return negative ? -result : result;
			}

			const Result Parse()
			{
				Result result(ParseInteger<long long int>());

				if (GetChar() == '.')
				{
					Advance();

					double up = 0.0;
					double down = 1.0;

					for (char c = GetChar(); IsDigit(c); Advance(), c = GetChar())
					{
						up *= 10.0;
						up += CharToInt(c);
						down *= 10.0;
					}

					printf("%g %g\n", up, down);

					result.CastToFloat();
					result.AsFloat() += up / down;

					printf("%d\n", result.IsInteger());
					printf("%d\n", result.IsFloat());
				}

				if (GetChar() == 'E' || GetChar() == 'e')
				{
					Advance();

					if (GetChar() == '+')
					{
						Advance();
					}

					long long int exponent = ParseInteger<long long int>();

					result.CastToFloat();
					result.AsFloat() *= ::pow(10.0, exponent);
				}

				return result;
			}

		protected:

			static bool IsDigit(char c)
			{
				return CLASS_DIGIT.Contains(c);
			}

			static int CharToInt(char c)
			{
				return c - '0';
			}

			static const CharClass CLASS_DIGIT;
	};

	const CharClass NumberParser::CLASS_DIGIT(TextParser::CommonCharClassifier::Instance(), TextParser::CHAR_DIGIT);
}
*/
/*
namespace XTL
{
	class ConfigParser : private TextParser
	{
		public:

			void Parse()
			{
				SkipSpaces();

				while (NotAtEnd())
				{
					char c = GetChar();
					if (c == ';' || c == '#')
					{
						ParseComments();
					}
					else if (c == '[')
					{
						ParseSectionName();
					}
					else if (IsIdentifierHead(c))
					{
						ParseKeyValue();
					}
					else
					{
					}

					SkipSpaces();
				}
			}

			void SkipLinearSpaces()
			{
				SkipCharClass(CLASS_LINEAR_SPACE);
			}

			void SkipSpaces()
			{
				SkipCharClass(CLASS_SPACE);
			}

		private:

			void ParseComments()
			{
				Advance();
				if (WaitChar('\n'))
				{
					Advance();
				}
			}

			void ParseSectionName()
			{
				Advance();
				SkipLinearSpaces();

				const std::string section = ReadIdentifier();
				if (section.empty())
				{
					throw 1;
				}

				SkipLinearSpaces();
				if (ReadChar(']'))
				{
					ParseLineTail();
				}
				else
				{
					throw 2;
				}
			}

			void ParseLineTail()
			{
				SkipLinearSpaces();
				char c = GetChar();
				if (c == '#')
				{
					ParseComments();
				}
				else if (c == '\n')
				{
					Advance();
				}
				else
				{
					throw 3;
				}
			}

			void ParseKeyValue()
			{
				const std::string key = ReadIdentifier();
				SkipLinearSpaces();

				if (GetChar() == '=')
				{
					Advance();
					SkipLinearSpaces();
				}

				char c = GetChar();
				if (c == '"')
				{
				}
				else if (c == '\'')
				{
				}
				else if (IsNumberHead(c))
				{
				}
				else if (IsBarewordHead(c))
				{
				}
				else
				{
					throw 4;
				}
			}

			static bool IsIdentifierHead(char c)
			{
				return DEFAULT_IDENTIFIER_HEAD.Contains(c);
			}

			static bool IsNumberHead(char c)
			{
				return CLASS_NUMBER_HEAD.Contains(c);
			}

			static bool IsBarewordHead(char c)
			{
				return false;
			}

			static const CharClass CLASS_LINEAR_SPACE;
			static const CharClass CLASS_SPACE;
			static const CharClass CLASS_NUMBER_HEAD;
	};

	const CharClass ConfigParser::CLASS_LINEAR_SPACE(TextParser::CommonCharClassifier::Instance(),
		TextParser::CHAR_SPACE           |
		TextParser::CHAR_TAB             |
		TextParser::CHAR_CARRIAGE_RETURN
	);

	const CharClass ConfigParser::CLASS_SPACE(TextParser::CommonCharClassifier::Instance(),
		TextParser::CHAR_SPACE           |
		TextParser::CHAR_TAB             |
		TextParser::CHAR_CARRIAGE_RETURN |
		TextParser::CHAR_LINE_FEED
	);

	const CharClass ConfigParser::CLASS_NUMBER_HEAD(TextParser::CommonCharClassifier::Instance(),
		TextParser::CHAR_DIGIT |
		TextParser::CHAR_MINUS
	);
}
*/
#include <set>

class CharStateMachine
{
	public:

		CharStateMachine()
			: chars_(),
			  links_(),
			  maxState_(0)
		{
			;;
		}

		unsigned int GetState(unsigned int oldState, char c) const
		{
			if (oldState >= links_.size())
			{
				return 0;
			}

			std::map<char, unsigned int>::const_iterator itr = links_[oldState].find(c);

			return itr == links_[oldState].end() ? 0 : itr->second;
		}

		unsigned int MaxState() const
		{
			return maxState_;
		}

	protected:

		void Link(unsigned int oldState, char c, unsigned int newState)
		{
			if (newState == 0)
			{
				throw std::runtime_error("Attempt to link to zero state");
			}

			chars_.insert(c);

			if (oldState > maxState_)
			{
				maxState_ = oldState;
			}

			if (newState > maxState_)
			{
				maxState_ = newState;
			}

			if (links_.size() <= oldState)
			{
				links_.resize(oldState + 1);
			}

			links_[oldState][c] = newState;
		}

	private:

		std::set<char> chars_;
		std::vector<std::map<char, unsigned int> > links_;
		unsigned int maxState_;
};

class Matcher : public CharStateMachine
{
	public:

		explicit Matcher(const std::string & sample)
			: CharStateMachine()
		{
			for (unsigned int i = 0; i < sample.length(); ++i)
			{
				Link(i, sample[i], i + 1);
			}
		}
};

class Searcher
{
	public:

		class StateSet
		{
			public:



			private:

				std::set<unsigned int, std::greater<unsigned int> > states_;
		};

		Searcher(const Matcher & matcher)
		{
			if (matcher.MaxState() >= sizeof(unsigned int))
			{
				throw std::runtime_error("Too many states in Matcher");
			}

			std::set<unsigned int> states;
			std::deque<unsigned int> newStates;

			newStates.push_back(0);

			while (!newStates.empty())
			{
//				unsigned int stateSet = newStates.front();
				newStates.pop_front();

				std::map<char, StateSet> stateSetLinks;
/*
				foreach (unsigned int state : stateSet)
				{
					const std::map<char, unsigned int> * const stateLinks = matcher.StateLinks(state);

					if (stateLinks == 0)
					{
						continue;
					}

					for (std::map<char, unsigned int>::const_iterator itr = stateLinks->begin(); itr != stateLinks->end(); ++itr)
					{
						stateSetLinks[itr->first].AddState(itr->second);
					}
				}
*/
			}
		}
};

class StateSet
{
	public:

		StateSet()
			: states_(),
			  digest_(0)
		{
			;;
		}

		bool operator< (const StateSet & other) const
		{
			if (digest_ != 1)
			{
				if (other.digest_ != 1)
				{
					return digest_ < other.digest_;
				}
				else
				{
					return true;
				}
			}
			else
			{
				if (other.digest_ != 1)
				{
					return false;
				}
				else
				{
					return DeepCompare(other);
				}
			}
		}

		bool DeepCompare(const StateSet & other) const
		{
			const std::vector<unsigned int>::const_iterator end1 = states_.end();
			const std::vector<unsigned int>::const_iterator end2 = other.states_.end();
			std::vector<unsigned int>::const_iterator itr1 = states_.begin();
			std::vector<unsigned int>::const_iterator itr2 = other.states_.begin();

			while (itr1 != end1 && itr2 != end2)
			{
				if (*itr1 < *itr2)
				{
					return true;
				}
				else if (*itr1 > *itr2)
				{
					return false;
				}

				++itr1;
				++itr2;
			}

			if (itr1 != end1)
			{
				return false;
			}

			if (itr2 != end2)
			{
				return true;
			}

			return false;
		}

		void AddState(unsigned int state)
		{
			if (state == 0)
			{
				throw std::runtime_error("Attempt to add zero state to StateSet");
			}

			std::vector<unsigned int>::iterator itr = std::lower_bound(states_.begin(), states_.end(), state, std::greater<unsigned int>());

			if (itr != states_.end() && *itr == state)
			{
				return;
			}

			states_.insert(itr, state);

			if (digest_ != 1)
			{
				if (state < sizeof(digest_) * 8)
				{
					digest_ |= (1 << state);
				}
				else
				{
					digest_ = 1;
				}
			}
		}

		void DebugPrint() const
		{
			for (std::vector<unsigned int>::const_iterator itr = states_.begin(); itr != states_.end(); ++itr)
			{
				printf("%u ", *itr);
			}

			printf("\n");
		}

		const std::string ToString() const
		{
			std::string result;

			for (std::vector<unsigned int>::const_iterator itr = states_.begin(); itr != states_.end(); ++itr)
			{
				if (*itr == 0)
				{
					continue;
				}

				if (!result.empty())
				{
					result.append("-");
				}

				result.append(XTL::IntegerToString(*itr));
			}

			return result;
		}

	private:

		std::vector<unsigned int> states_;
		XTL::UINT_8 digest_;
};

void PushSorted(std::vector<unsigned int> & v, unsigned int i)
{
	std::vector<unsigned int>::iterator itr = std::lower_bound(v.begin(), v.end(), i, std::greater<unsigned int>());
	v.insert(itr, i);
}


#include <stack>
#include <xtl/tp/TextCursor.hpp>

class Token
{
};

class Lexema
{
};

class Operand : public Lexema
{
};

class Operator : public Lexema
{
};

class TokenHandler
{
	public:

		virtual ~TokenHandler() throw() { ;; }

		virtual void Execute(std::stack<const Lexema *> lexemaStack, const Token & token) = 0;
};

class SyntaxError
{
};


class ExpressionParser
{
	/*
		id
		+ E
		E + E
		( E )
		id ( E )
	*/
	static const unsigned int TOKEN_IDENTIFIER    = 0;
	static const unsigned int TOKEN_PLUS          = 1;
	static const unsigned int TOKEN_OPEN_BRACKET  = 2;
	static const unsigned int TOKEN_CLOSE_BRACKET = 3;

	static const unsigned int OPERAND_IDENTIFIER = 0;
	static const unsigned int OPERATOR_OPEN_BRACKET = 0;
	static const unsigned int OPERATOR_ADDITION = 0;
	static const unsigned int OPERATOR_POSITIVE = 0;
};

// RegularExpressionParser

class RegularExpressionTree
{
	public:
	private:

		class Node
		{
			public:

				virtual ~Node() throw() { ;; }
		};

		class NodeChar : public Node
		{
			public:

				virtual ~NodeChar() throw() { ;; }

		};
/*
		class CharClass

		class Concat : public Node
		{
		};

		class Repeat0 : public Node
		{
		};

		class Alternation: public Node
		{
			private:

				std::vector<Node *> nodes_;
		};
*/
};

/*
	E -> E +
	E -> E *
	E -> E | E
	E -> ( E )
	E -> { E }
*/

enum
{
	TOKEN_PLUS,
	TOKEN_STAR,
	TOKEN_LINE,
	TOKEN_BRACKET_ROUND_OPEN,
	TOKEN_BRACKET_ROUND_CLOSE,
	TOKEN_BRACKET_CURLY_OPEN,
	TOKEN_BRACKET_CURLY_CLOSE
};

enum
{
	LEXEMA_CHAR,
	LEXEMA_CHAR_CLASS,
	LEXEMA_CONCAT,
	LEXEMA_REPETITION,
	LEXEMA_ALTERNATION,
	LEXEMA_GROUP,
	LEXEMA_SUBMATCH
};

/*
	   c C * | ( ) { }
	@  P
	c
*/


void ParseRegularExpression(const char * re)
{
	while (*re != '\0')
	{
		switch (*re)
		{
		case '(':
			break;

		case ')':
			break;

		case '*':
			break;

		case '+':
			break;

		case '|':
			break;

		case '[':
			++re;
			{
				bool inverted = false;
				if (*re == '^')
				{
					inverted = true;
					++re;
				}
				while (*re != '\0' && *re != ']')
				{
					++re;
				}

				if (inverted)
				{
					;;
				}
			}
			if (*re == '\0')
			{
				throw std::runtime_error("");
			}
			break;

		case '\\':
			++re;
			if (*re == '\0')
			{
				throw std::runtime_error("");
			}
			else if (*re == '\\')
			{
				// Char('\')
			}
			else if (*re == 'd')
			{
				// CharClass(DIGIT)
			}
			else if (*re == 'w')
			{
				// CharClass(ALPHA_NUMERIC)
			}
			else if (*re == 's')
			{
				// CharClass(SPACE)
			}
			else if (*re == 'S')
			{
				// CharClass(NON_SPACE)
			}
			break;

		default:
			// Char(*re)
			break;
		}
		++re;
	}
}

/*
      TN    TNt   NTN  NTNtN NT     tNT    NtNTN
@     PUSH  PUSH  PUSH PUSH  REDUCE ERROR  ERROR
OPRND ERROR ERROR POP  POP   POP    POP    POP
NTN   PUSH  PUSH  ?    ?     ?      POP    POP
TN    PUSH  PUSH  ?    ?     ?      POP    POP
TNt   PUSH  PUSH  PUSH PUSH  REDUCE ??     ERROR
NTNtN PUSH  PUSH  PUSH PUSH  REDUCE ERROR  ??
NtNTN PUSH  PUSH  ?    ?     ?      POP    POP
NT, tNT - не могут быть в стеке, т.к. сворачиваются сразу при обработке

?? - REDUCE or ERROR
NT - PUSH_REDUCE

++x++ => (++x)++ || ++(x++)
!x+y  => !(x+y)  || (!x)+y
a?b:x+y => (a?b:x)+y || a?b:(x+y)
a?b:c?x:y => (a?b:c)?x:y | a?b:(c?x:y)
*/

/*
class Char : public Expression::Operand
{
	public:

		explicit Char(char c)
			: c_(c) { ;; }

		virtual ~Char() throw() { ;; }

	private:

		const char c_;
};
*/

enum
{
	CONCAT,
	ALTER,
	REPEAT,
	OPEN_R,
	CLOSE_R,
	OPEN_C,
	CLOSE_C
};

/*
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
|         | OPERAND | CONCAT     | REPEAT  |  ALTER  | OPEN_R  | CLOSE_R | OPEN_C  | CLOSE_C |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| empty   | push;   | push;      | xxx     | push;   | push;   | error;  | push;   | error;  |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| OPERAND | ---     | pop;       | pop;    | pop;    | xxx     | pop;    | xxx     | pop;    |
|         |         |            | push;   |         |         |         |         |         |
|         |         |            | reduce; |         |         |         |         |         |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| CONCAT  | push;   | pop;       | xxx     | pop;    | push;   | pop;    | push;   | pop;    |
|         |         | add_param; |         |         |         |         |         |         |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| ALTER   | push;   | push;      | xxx     | pop;    | push;   | pop;    | push;   | pop;    |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| OPEN_R  | push;   | push;      | xxx     | push;   | push;   | reduce; | push;   | error;  |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
| OPEN_C  | push;   | push;      | xxx     | push;   | push;   | error;  | push;   | reduce; |
+---------+---------+------------+---------+---------+---------+---------+---------+---------+
*/

#include <xtl/tp/Parser.hpp>
#include <xtl/tp/NumberLiteralParser.hpp>
#include <xtl/tp/StringLiteralParser.hpp>
#include <xtl/Variant.hpp>
#include <xtl/VariantScalar.hpp>
#include <xtl/VariantPtr.hpp>

namespace XTL
{
	class IniConfig
	{
		public:

			class SectionDoesNotExist
			{
				public:

					explicit SectionDoesNotExist(const std::string & sectionName)
						: sectionName_(sectionName)
					{
						;;
					}

					const std::string & SectionName() const
					{
						return sectionName_;
					}

				private:

					const std::string sectionName_;
			};

			class KeyDoesNotExist
			{
				public:

					explicit KeyDoesNotExist(const std::string & sectionName, const std::string & key)
						: sectionName_(sectionName),
						  key_(key)
					{
						;;
					}

					const std::string & SectionName() const
					{
						return sectionName_;
					}

					const std::string & Key() const
					{
						return key_;
					}

				private:

					const std::string sectionName_;
					const std::string key_;
			};

			class Section
			{
				public:

					explicit Section(const std::string & sectionName)
						: sectionName_(sectionName),
						  pairs_()
					{
						;;
					}

					bool Exists(const std::string & key)
					{
						return pairs_.count(key) > 0;
					}

					void Set(const std::string & key, const VariantPtr & value)
					{
						pairs_[key] = value;
					}

					VariantPtr Get(const std::string & key) const
					{
						std::map<std::string, VariantPtr>::const_iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							throw KeyDoesNotExist(sectionName_, key);
						}

						return itr->second;
					}

					const std::string GetString(const std::string & key, const std::string & defaultValue) const
					{
						std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							return defaultValue;
						}

						return itr->second.ToString();
					}

					const long long int GetLongLongInt(const std::string & key, const long long int & defaultValue) const
					{
						std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							return defaultValue;
						}

						return itr->second.ToLongLongInt();
					}

				private:

					const std::string sectionName_;
					mutable std::map<std::string, VariantPtr> pairs_;
			};

			void Set(const std::string & sectionName, const std::string & key, VariantPtr value)
			{
				CreateSection(sectionName)->Set(key, value);
			}

			const Section & GetSection(const std::string & sectionName) const
			{
				Section * section = sections_[sectionName];

				if (section == 0)
				{
					throw SectionDoesNotExist(sectionName);
				}

				return *section;
			}

		protected:

			static const Section * EmptySection()
			{
				static const Section instance("__empty__");
				return &instance;
			}

			Section * CreateSection(const std::string & sectionName)
			{
				Section * section = sections_[sectionName];
				if (section == 0)
				{
					std::auto_ptr<Section> newSection(new Section(sectionName));
					section = newSection.get();
					sections_.Set(sectionName, newSection);
				}

				return section;
			}

		private:

			AutoPtrMap<std::string, Section> sections_;
	};

	class IniConfigParser : public Parser
	{
		public:

			class SingleQuotedStringParser : public XTL::StringLiteralParser
			{
				public:

					explicit SingleQuotedStringParser(XTL::CharSource & charSource)
						: StringLiteralParser(charSource, '\'', '\\')
					{
						;;
					}

					virtual ~SingleQuotedStringParser() throw() { ;; }

				protected:

					class ES : public StringLiteralParser::EscapeSequenceSet<ES>
					{
						public:

							ES()
								: XTL::StringLiteralParser::EscapeSequenceSet<ES>()
							{
								Add('\'', '\'');
								Add('\\', '\\');
							}
					};

					virtual void ParseEscapeSequence(XTL::CharBuffer & result)
					{
						ES::Instance().Parse(GetCharSource(), result);
					}
			};

			class DoubleQuotedStringParser : public XTL::StringLiteralParser
			{
				public:

					explicit DoubleQuotedStringParser(XTL::CharSource & charSource)
						: XTL::StringLiteralParser(charSource, '"', '\\')
					{
						;;
					}

					virtual ~DoubleQuotedStringParser() throw() { ;; }

				protected:

					class ES : public XTL::StringLiteralParser::EscapeSequenceSet<ES>
					{
						public:

							ES()
								: XTL::StringLiteralParser::EscapeSequenceSet<ES>()
							{
								Add('"', '"');
								Add('\\', '\\');
								Add('r', '\r');
								Add('n', '\n');
								Add('t', '\t');
							}
					};

					virtual void ParseEscapeSequence(XTL::CharBuffer & result)
					{
						ES::Instance().Parse(GetCharSource(), result);
					}
			};


			IniConfigParser(CharSource & charSource, IniConfig & config)
				: Parser(charSource),
				  config_(config),
				  currentSection_()
			{
				while (NotAtEnd())
				{
					ParseLine();
				}
			}

			void ParseLine()
			{
				SkipLinearSpaces();
				if (AtEnd())
				{
					return;
				}

				char c = GetChar();
				if (c == '[')
				{
					ReadSection();
				}
				else if (CharClass::IDENTIFIER_HEAD.Contains(c))
				{
					ReadKeyValue();
				}
				/*
				else if (c == '{')
				{
					ReadCommand();
				}
				*/

				SkipLinearSpaces();
				if (AtEnd())
				{
					return;
				}

				if (GetChar() == ';' || GetChar() == '#')
				{
					SkipComments();
				}

				SkipNewLine();
			}

		private:

			void SkipLinearSpaces()
			{
				while (NotAtEnd() && InClass(CharClass::LINEAR_SPACE))
				{
					Advance();
				}
			}

			void SkipNewLine()
			{
				if (AtEnd())
				{
					return;
				}

				if (GetChar() == '\r')
				{
					Advance();
					if (AtEnd())
					{
						return;
					}
				}

				if (GetChar() == '\n')
				{
					Advance();
				}
				else
				{
					ThrowError("New line expected");
				}
			}

			const std::string ReadIdentifier()
			{
				// Assert( CharClass.IDENTIFIER_HEAD.Contains(NeedChar()) )

				Mark();

				do
				{
					Advance();
				}
				while (NotAtEnd() && InClass(CharClass::IDENTIFIER_TAIL));

				return ReleaseString();
			}

			/*
			 * ... '[' \s* [A-Za-z_][A-Za-z0-9_]* \s* ']' ...
			 *   ---^                                     ^---
			 */
			void ReadSection()
			{
				// Assert( NeedChar() == '[' )

				try
				{
					Advance();
					SkipLinearSpaces();
					if (!CharClass::IDENTIFIER_HEAD.Contains(NeedChar()))
					{
						ThrowError("Section name was expected");
					}
					currentSection_ == ReadIdentifier();
					SkipLinearSpaces();
					if (NeedChar() != ']')
					{
						ThrowError("Closing square bracket was expected");
					}
					Advance();
				}
				catch (const EndOfFile &)
				{
					ThrowError("Unexpected end of file");
				}
			}

			void ReadKeyValue()
			{
				// Assert( CharClass.IDENTIFIER_HEAD.Contains(NeedChar()) )

				const std::string key = ReadIdentifier();

				bool wasSpace = false;
				if (NotAtEnd() && InClass(CharClass::LINEAR_SPACE))
				{
					wasSpace = true;
					Advance();
					SkipLinearSpaces();
				}

				try
				{
					char c = NeedChar();
					if (c == ':' || c == '=')
					{
						Advance();
						SkipLinearSpaces();
					}
					else if (!wasSpace)
					{
						ThrowError("Key-value divider was expected (':', '=' or SPACE)");
					}
				}
				catch (const EndOfFile &)
				{
					ThrowError("Value expected");
				}

				if (AtEnd())
				{
					ThrowError("Value expected");
				}

				config_.Set(currentSection_, key, ReadValue());
			}

			/*
			 * " ( [^\\"] | '\\' [rnt\\"]  )* "
			 * ' ( [^\\'] | '\\' [\\'] )* '
			 * -? [0-9]+ ( '.' [0-9]+ )? ( [eE] [+-] [0-9]+ )
			 * '0' ( ('x' [0-9A-Fa-f]+) | ('b' [0-1]+) | ([0-7]*) )
			 */
			VariantPtr ReadValue()
			{
				// Assert( NotAtEnd() && NotInClass(CharClass::LINEAR_SPACE) )

				char c = GetChar();

				if (c == '\'')
				{
					return VariantPtr(new Variant::String(SingleQuotedStringParser(GetCharSource()).Parse()));
				}
				else if (c == '"')
				{
					return VariantPtr(new Variant::String(DoubleQuotedStringParser(GetCharSource()).Parse()));
				}
				else if (CharClass::NUMBER_HEAD.Contains(c))
				{
					const Number n = FloatLiteralParser(GetCharSource()).Parse();

					if (n.IsFloat())
					{
						return VariantPtr(new Variant::Double(n.ToFloat()));
					}
					else
					{
						return VariantPtr(new Variant::LongLongInt(n.ToSignedInteger()));
					}
				}
				else
				{
					ThrowError("Value was expected");
				}
				/*
				else if (CharClass::IDENTIFIER_HEAD.Contains(c))
				{
				}
				*/
			}

			void SkipComments()
			{
				// Assert( NeedChar() == ';' || NeedChar() == '#' )

				do
				{
					Advance();
				}
				while (NotAtEnd() && GetChar() != '\n');
			}

			IniConfig   & config_;
			std::string   currentSection_;
	};
}

#include <xtl/CharBuffer.hpp>

namespace XTL
{
	class DictionaryStateMachine
	{
		public:

		private:
	};
}

void DebugPrint(const XTL::Number & n)
{
	switch (n.GetType())
	{
		case XTL::Number::SIGNED_INTEGER:   printf("signed %lld\n", static_cast<long long int>(n.ToSignedInteger())); break;
		case XTL::Number::UNSIGNED_INTEGER: printf("unsigned %llu\n", static_cast<unsigned long long int>(n.ToUnsignedInteger())); break;
		case XTL::Number::FLOAT:            printf("float %g\n", n.ToFloat()); break;
	}
}

/*
 * {$var}
 * {$outer.inner.inner2}
 * {{LOOP var_array AS LOOP_VAR}}...{{/LOOP}}
 * {{IF var}}...{{ELSEIF}}...{{ELSE}}...{{/IF}}
 * {{INCLUDE "Template/File/Path"}}
 */

namespace XTL
{
	class TemplateNode;

	class TemplateNodeList
	{
		public:

			explicit TemplateNodeList(TemplateNode * parentNode)
				: parentNode_(parentNode),
				  items_()
			{
				;;
			}

			TemplateNode * GetParentNode() const
			{
				return parentNode_;
			}

			bool IsEmpty() const
			{
				return items_.IsEmpty();
			}

			TemplateNode * LastNode()
			{
				return items_.IsEmpty() ? 0 : items_.Back();
			}

			void Add(std::auto_ptr<TemplateNode> node)
			{
				items_.PushBack(node);
			}

			void DebugPrint(unsigned int indent) const;

		private:

			TemplateNode * const parentNode_;
			AutoPtrVector<TemplateNode> items_;
	};

	class TemplateNode
	{
		public:

			enum Type
			{
				TEXT     = 0,
				VARIABLE = 1,
				LOOP     = 2
			};

			TemplateNode(TemplateNodeList * parentList)
				: parentList_(parentList)
			{
				;;
			}

			virtual ~TemplateNode() throw() { ;; }

			TemplateNodeList * GetParentList() const
			{
				return parentList_;
			}

			virtual Type GetType() const = 0;

			virtual void DebugPrint(unsigned int indent) const = 0;

			class Text;
			class Variable;
			class Loop;

		protected:

			static void DebugPrintIndent(unsigned int indent)
			{
				for (; indent > 0; --indent)
				{
					printf("    ");
				}
			}

		private:

			TemplateNodeList * const parentList_;
	};

	void TemplateNodeList::DebugPrint(unsigned int indent) const
	{
		for (unsigned int i = 0; i < items_.Size(); ++i)
		{
			items_[i]->DebugPrint(indent);
		}
	}

	class TemplateNode::Text : public TemplateNode
	{
		public:

			Text(TemplateNodeList * parentList, const std::string & text)
				: TemplateNode(parentList),
				  text_(text)
			{
				;;
			}

			virtual ~Text() throw()
			{
				;;
			}

			virtual Type GetType() const
			{
				return TEXT;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent);
				printf("TEXT\n");
			}

			void AppendText(const std::string & text)
			{
				text_.append(text);
			}

		private:

			std::string text_;
	};

	class TemplateNode::Variable : public TemplateNode
	{
		public:

			Variable(TemplateNodeList * parentList, const std::string & name)
				: TemplateNode(parentList),
				  name_(name)
			{
				;;
			}

			virtual ~Variable() throw() { ;; }

			virtual Type GetType() const
			{
				return VARIABLE;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent);
				printf("VARIABLE\n");
			}

		private:

			const std::string name_;
	};

	class TemplateNode::Loop : public TemplateNode
	{
		public:

			Loop(TemplateNodeList * parentList)
				: TemplateNode(parentList),
				  body_(this)
			{
				;;
			}

			TemplateNodeList * GetBody()
			{
				return &body_;
			}

			virtual Type GetType() const
			{
				return LOOP;
			}

			virtual void DebugPrint(unsigned int indent) const
			{
				DebugPrintIndent(indent);
				printf("LOOP\n");
				body_.DebugPrint(indent + 1);
			}

		private:

			TemplateNodeList body_;
	};

	class TemplateParser : protected Parser
	{
		public:

			TemplateParser(CharSource & charSource)
				: Parser(charSource),
				  rootList_(new TemplateNodeList(0)),
				  currentList_(rootList_.get())
			{
				;;
			}

			bool SkipChar(char c)
			{
				if (AtEnd() || GetChar() != c)
				{
					return false;
				}

				Advance();
				return true;
			}

			bool ReadString(const char * s)
			{
				for (; *s != '\0'; ++s)
				{
					if (AtEnd() || GetChar() != *s)
					{
						return false;
					}

					Advance();
				}

				return true;
			}

			void Parse()
			{
				Mark();

				while (NotAtEnd())
				{
					if (GetChar() != '{')
					{
						Advance();
						continue;
					}

					AddText(ReleaseString());

					Mark();
					Advance();

					if (!SkipChar('{'))
					{
						Advance();
						continue;
					}

					Unmark();

					SkipWhitespaces();

					if (GetChar() == '$') // {{$VAR}}
					{
						Advance();
						if (AtEnd() || NotInClass(CharClass::IDENTIFIER_HEAD))
						{
							ThrowError("Invalid syntax");
						}

						const std::string variableName = ReadIdentifier();

						if (!ReadString("}}"))
						{
							ThrowError("Invalid syntax");
						}

						Mark();
					}
					else
					{
						bool closing = false;
						if (SkipChar('/'))
						{
							closing = true;
						}

						std::string tag = NeedIdentifier();
						ToLowerCase(tag);

						if (tag == "if")
						{
							if (closing)
							{
								printf("{{ /if }}\n");
							}
							else
							{
								// ... {{if ...
								//       ---^
								SkipWhitespaces();

								const std::string variableName = NeedIdentifier();
								printf("{{ if %s }}\n", variableName.c_str());


							}
						}
						else if (tag == "loop")
						{
							if (closing)
							{
								printf("{{ /loop }}\n");

								TemplateNode * parentNode = currentList_->GetParentNode();
								if (parentNode == 0 || parentNode->GetType() != TemplateNode::LOOP)
								{
									ThrowError("Syntax error");
								}

								currentList_ = parentNode->GetParentList();
							}
							else
							{
								SkipWhitespaces();

								const std::string variableName = NeedIdentifier();
								printf("{{ loop %s }}\n", variableName.c_str());

								TemplateNode::Loop * newNode = new TemplateNode::Loop(currentList_);
								currentList_->Add(std::auto_ptr<TemplateNode>(newNode));
								currentList_ = newNode->GetBody();
							}
						}
						else if (tag == "else")
						{
							printf("{{ else }}\n");
						}
						else if (tag == "include")
						{
							printf("{{ include }}");
						}
						else
						{
							ThrowError("Invalid syntax");
						}

						SkipWhitespaces();

						if (!ReadString("}}"))
						{
							ThrowError("Invalid syntax");
						}

						Mark();
					}
				}

				AddText(ReleaseString());

				if (currentList_ != rootList_.get())
				{
					ThrowError("Invalid syntax");
				}

				printf("==================================================\n");
				rootList_->DebugPrint(0);
			}

		private:

			void AddText(const std::string & text)
			{
				if (text.size() > 0)
				{
					printf("TEXT: length=%u\n", text.size());

					TemplateNode * lastNode = currentList_->LastNode();
					if (lastNode == 0 || lastNode->GetType() != TemplateNode::TEXT)
					{
						std::auto_ptr<TemplateNode> newNode(new TemplateNode::Text(currentList_, text));
						currentList_->Add(newNode);
					}
					else
					{
						static_cast<TemplateNode::Text *>(lastNode)->AppendText(text);
					}

					/*
					std::auto_ptr<TemplateNode> newNode(new TemplateNode::Text(currentList_, text));
					currentList_->Add(newNode);
					*/
				}
			}

			void SkipWhitespaces()
			{
				while (NotAtEnd() && InClass(CharClass::WHITESPACE))
				{
					Advance();
				}
			}

			const std::string NeedIdentifier()
			{
				if (AtEnd() || NotInClass(CharClass::IDENTIFIER_HEAD))
				{
					ThrowError("Identifier was expected");
				}

				return ReadIdentifier();
			}

			std::auto_ptr<TemplateNodeList> rootList_;
			TemplateNodeList * currentList_;
	};
}

int main(int argc, const char * argv[])
{
	{
		const std::string s = "what the fuck ? {{loop a}} { { {{loop x}} 1 {{/loop}} 2 {{/if}}3{{/loop}}";
		XTL::CharSource::ConstCharPtr charSource(s.data(), s.size());

		try
		{
			XTL::TemplateParser parser(charSource);
			parser.Parse();
		}
		catch (const XTL::Parser::Error & e)
		{
			fprintf(stderr, "%s\n", e.What());
		}

		return 0;
	}

	// const std::string s = "18446744073709551615";
	// const std::string s = "18446744073709551616";
	// const std::string s = "-9223372036854775808";
	// const std::string s = "-9223372036854775809";
	// const std::string s = "-0.0000000123e8";
	// const std::string s = "0b1010";
	// const std::string s = "0775";
	const std::string s = "0xffFF";
	XTL::CharSource::ConstCharPtr charSource(s.data(), s.size());

	XTL::FloatLiteralParser nlp(charSource);
	XTL::Number n = nlp.Parse();
	DebugPrint(n);
	return 0;

	/*
	XTL::IntegerBuilder ib;

	// const char * const s = "18446744073709551616";
	const char * const s = "9223372036854775808";
	ib.SetNegative();
	for (const char * p = s; *p != '\0'; ++p)
	{
		ib.AppendDecimal(*p - '0');
	}
	*/
	return 0;


	StateSet stateSet;

	srand(time(0));
	for (unsigned int i = 0; i < 20; ++i)
	{
		stateSet.AddState(rand() % 50 + 1);
		stateSet.DebugPrint();
	}

	printf("%s\n", stateSet.ToString().c_str());

	return 0;

/*
	printf("sof = %lu\n", sizeof(XTL::NumberParser::Result));

	XTL::TextCharSource::ConstCharPtr charSource("-12345678.9__abvd_12 + 1");

	XTL::NumberParser::Result r = XTL::NumberParser(charSource).Parse();
	printf("%s\n", r.ToString().c_str());
*/
/*
	XTL::TextParser prs(charSource);

	printf("%lld\n", llu);
	printf("[%s]\n", prs.ReadIdentifier().c_str());
*/
	return 0;


	XTL::BitSet<XTL::UINT_64> bs(true);

	// bs.Add(0);
	bs.Remove(0);
	// bs.Add(31);

	for (XTL::BitSet<XTL::UINT_64>::ConstIterator itr(bs); !itr.AtEnd(); itr.Advance())
	{
		printf("%d\n", *itr);
	}

	return 0;

	// XTL::TextCharSource::ConstCharPtr charSource("\"aaa\\t\\\"\\\"\\\\bbbbb\"");
/*
	JsonStringLiteralParser parser;

	printf("%s\n", parser.Parse(charSource).c_str());
*/

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

		XTL::SocketSelector socketSelector;
		socketSelector.Insert(clientSocket, true, true);

		if (clientSocket.Connect(address))
		{
			printf("Connected\n");
		}
		else
		{
			printf("Connect failed\n");
		}

		XTL::SocketSelector::SelectResult selectResult;

		while (1)
		{
			socketSelector.Select(selectResult, XTL::SocketSelector::Timeout(0, 100));
			printf("selected: %d\n", selectResult.SelectedCount());
			printf("readable=%d, writable=%d\n", selectResult.ReadableSet().Contains(clientSocket), selectResult.WritableSet().Contains(clientSocket));
			if (selectResult.SelectedCount() > 0)
			{
				break;
			}
		}

		while (1)
		{
			socketSelector.Select(selectResult, XTL::SocketSelector::Timeout(0, 100));
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
