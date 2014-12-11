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

/*
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
*/

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
#include <map>

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

#include <xtl/ini/IniConfig.hpp>

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
		case XTL::Number::RATIONAL:         printf("rational %g\n", n.ToDouble()); break;
	}
}

/*
 * {$var}
 * {$outer.inner.inner2}
 * {{LOOP var_array AS LOOP_VAR}}...{{/LOOP}}
 * {{IF var}}...{{ELSEIF}}...{{ELSE}}...{{/IF}}
 * {{INCLUDE "Template/File/Path"}}
 */

#include "TemplateParser.hpp"
#include "TemplateTreeBuilder.hpp"

#include <xtl/linux/utils/TcpSocketServer.hpp>

class MyTcpServer : public XTL::TcpSocketServer
{
	class ClientHandler : public XTL::TcpSocketServer::ClientHandler
	{
		public:

			explicit ClientHandler(Client & client)
				: XTL::TcpSocketServer::ClientHandler(client)
			{
				;;
			}

			virtual ~ClientHandler() throw()
			{
				;;
			}

			virtual void OnClientConnected()
			{
				printf("OnClientConnected()\n");
			}

			virtual void OnClientDisconnected()
			{
				printf("OnClientDisconnected()\n");
			}

			virtual void OnReceiveError(const XTL::UnixError & e)
			{
				printf("OnReceiveError(%s)\n", e.What().c_str());
			}

			virtual void OnDataReceived(const void * buffer, unsigned int size)
			{
				printf("OnDataReceived(%u)\n", size);
			}
	};

	public:

		MyTcpServer()
			: XTL::TcpSocketServer()
		{
			;;
		}

		virtual ~MyTcpServer() throw() { ;; }

		virtual std::auto_ptr<XTL::TcpSocketServer::ClientHandler> CreateClientHandler(XTL::TcpSocketServer::Client & client)
		{
			return std::auto_ptr<XTL::TcpSocketServer::ClientHandler>(new ClientHandler(client));
		}

		virtual void OnTerminated()
		{
			printf("Server was terminated\n");
		}
};

std::auto_ptr<MyTcpServer> server;

#include <signal.h>

namespace XTL
{
	typedef void (*SignalHandlerProc) (int);

	void SetSignalHandler(int signum, SignalHandlerProc proc)
	{
		struct sigaction sa;
		::memset(&sa, 0, sizeof(sa));
		sa.sa_handler = proc;

		if (::sigaction(signum, &sa, 0) != 0)
		{
			throw UnixError();
		}
	}
}

void TerminateProgram(int)
{
	if (server.get() != 0)
	{
		server->OnTerminated();
	}
}

#include <xtl/linux/utils/TcpSocketClient.hpp>
#include <xtl/linux/Utils.hpp>
#include <xtl/linux/net/SocketAddressError.hpp>

namespace XTL
{
	class TcpClient
	{
		public:

			TcpClient(const std::string & host, unsigned int port, bool blocking)
				: clientSocket_(TcpClientSocket::Create(blocking)),
				  serverAddress_(host, port)
			{

			}

			bool Connect(unsigned int attemptsCount, double timeout)
			{
				if (clientSocket_.Connect(serverAddress_))
				{
					return true;
				}

				while (attemptsCount > 0)
				{
					Sleep(timeout);

					if (clientSocket_.Connect(serverAddress_))
					{
						return true;
					}

					--attemptsCount;
				}

				return false;
			}

			bool Send(const void * buffer, int size, unsigned int attemptsCount, double timeout)
			{
				const char * p = static_cast<const char *>(buffer);

				int wasSent = clientSocket_.Send(p, size);
				size -= wasSent;
				p += wasSent;

				if (size == 0)
				{
					return true;
				}

				while (attemptsCount > 0)
				{
					Sleep(timeout);

					wasSent = clientSocket_.Send(p, size);
					size -= wasSent;
					p += wasSent;

					if (size == 0)
					{
						return true;
					}

					--attemptsCount;
				}

				return false;
			}

		private:

			XTL::TcpClientSocket clientSocket_;
			XTL::SocketAddressInet serverAddress_;
	};
}

struct A
{
	virtual ~A() throw() { ;; }

	virtual void Do() = 0;
};

struct B : public A
{
	int i;

	B() : i(0) { ;; }

	virtual ~B() throw() { ;; }

	virtual void Do() { ++i; }
};

template <typename T> struct C : public A
{
	virtual ~C() throw() { ;; }

	virtual void Do() { t.Do(); }

	T t;
};

template <typename T> struct D : public A
{
	D() : a(new T()) { ;; }

	virtual ~D() throw() { ;; }

	virtual void Do() { a->Do(); }

	A * a;
};

class TemplateParamPool
{
	public:

		unsigned int AddLayer()
		{
			return layerIndex_;
		}

	private:

		unsigned int layerIndex_;
};

class CharSet
{
	public:



	private:

		std::set<char> chars_;
};

#include <xtl/linux/fs/FileUtils.hpp>

class CharStateMachine2
{
	public:

		CharStateMachine2()
			: links_()
		{
			;;
		}

		void AddString(const std::string & s)
		{
		}

/*
		(abc|abcde)(?:de)?
		abc
		abcde
		abcdede

		Match string "abcde":
			$1 == "abc" или $1 == "abcde" ?
*/

	private:

		std::map<int, std::map<char, int> > links_;
		std::set<int> finalStates_;
};

#include <xtl/utils/SimpleStringMatcher.hpp>

#include <xtl/BTree.hpp>

struct MyKey
{
	unsigned int i;

	MyKey()
		: i(0) { ;; }

	explicit MyKey(unsigned int i_)
		: i(i_) { ;; }

	bool operator< (const MyKey & other) const
	{
		return i < other.i;
	}
};

struct MyItem
{
	MyKey        key;
	unsigned int value;

	MyItem()
		: key(),
		  value()
	{
		;;
	}

	explicit MyItem(const MyKey & key_)
		: key(key_),
		  value()
	{
		;;
	}

	operator const MyKey & () const
	{
		return key;
	}

	MyItem & operator= (const MyKey & key_)
	{
		key = key_;
		return *this;
	}
};


// i    := 1..N
// F(i) := 1..N
unsigned int F1(unsigned int i, unsigned int N)
{
	return i;
}

unsigned int F2(unsigned int i, unsigned int N)
{
	return N + 1 - i;
}

unsigned int F3(unsigned int i, unsigned int N)
{
	return (i % 2 == 1 ? i / 2 : N - i / 2) + 1;
}

unsigned int F4(unsigned int i, unsigned int N)
{
	return (i % 2 == 1 ? (N - 1) / 2 - i / 2 : (N - 1) / 2 + i / 2) + 1;
}

typedef XTL::BTree<MyItem, MyKey, std::less<MyKey>, true, 4, 4> MyFuckingTree;

#include <xtl/PrintStream.hpp>
#include <xtl/VariantScalar.hpp>
#include <xtl/VariantArray.hpp>
#include <xtl/VariantStruct.hpp>

namespace XTL
{
	class VariantDumper : public Variant::Visitor
	{
		public:

			explicit VariantDumper(PrintStream & stream)
				: stream_(stream),
				  indent_(0)
			{
				;;
			}

			virtual ~VariantDumper() throw()
			{
				;;
			}

			virtual void Visit(const Variant::NullValue & v)
			{
				stream_.Print("null");
			}

			virtual void Visit(const Variant::Boolean & v)
			{
				stream_.Print(v.GetValue() ? "true" : "false");

			}

			virtual void Visit(const Variant::LongLongInt & v)
			{
				stream_.PrintF("%lld", v.GetValue());
			}

			virtual void Visit(const Variant::Double & v)
			{
				stream_.PrintF("%g", v.GetValue());
			}

			virtual void Visit(const Variant::String & v)
			{
				PrintString(stream_, v.GetValue());
			}

			virtual void Visit(const Variant::Array & v)
			{
				stream_.Print("[\n");

				std::auto_ptr<Variant::Array::ConstIterator> itr(v.GetConstIterator());

				if (itr->NotAtEnd())
				{
					++indent_;

					do
					{
						PrintIndent();

						itr->Current()->Visit(*this);
						itr->Advance();

						if (itr->NotAtEnd())
						{
							stream_.Print(",");
						}

						stream_.Print("\n");
					}
					while (itr->NotAtEnd());

					--indent_;
				}

				PrintIndent();
				stream_.Print("]");
			}

			virtual void Visit(const Variant::Struct & v)
			{
				stream_.Print("{\n");

				std::auto_ptr<Variant::Struct::ConstIterator> itr(v.GetConstIterator());

				if (itr->NotAtEnd())
				{
					++indent_;

					do
					{
						PrintIndent();

						PrintString(stream_, itr->Key());
						stream_.Print(" : ");

						itr->Value()->Visit(*this);
						itr->Advance();

						if (itr->NotAtEnd())
						{
							stream_.Print(",");
						}

						stream_.Print("\n");
					}
					while (itr->NotAtEnd());

					--indent_;
				}

				PrintIndent();
				stream_.Print("}");
			}

		private:

			void PrintIndent()
			{
				XTL::CharRepeater<' '>::Print(stream_, 4 * indent_);
			}

			static void PrintString(PrintStream & stream, const std::string & s)
			{
				stream.Print("\"");

				unsigned int begin = 0;
				const unsigned int end = s.size();

				for (unsigned int i = 0; i < end; ++i)
				{
					if (s[i] == '\\')
					{
						if (begin < i)
						{
							stream.Print(s.substr(begin, i - begin));
						}
						stream.Print("\\\\");
						begin = i + 1;
					}
					else if (s[i] == '"')
					{
						if (begin < i)
						{
							stream.Print(s.substr(begin, i - begin));
						}
						stream.Print("\\\"");
						begin = i + 1;
					}
				}

				if (begin < end)
				{
					stream.Print(s.substr(begin, end - begin));
				}

				stream.Print("\"");
			}

			PrintStream  & stream_;
			unsigned int   indent_;
	};

	void Dump(PrintStream & stream, const Variant & value)
	{
		VariantDumper dumper(stream);

		value.Visit(dumper);
		stream.Print("\n");
	}
}

#include <xtl/json/JsonPrinter.hpp>

namespace XTL
{
	class VariantJsonifier : public Variant::Visitor
	{
		public:

			VariantJsonifier(PrintStream & stream, JsonPrinter::Spacer & spacer)
				: printer_(stream, spacer)
			{
				;;
			}

			virtual ~VariantJsonifier() throw()
			{
				;;
			}

			virtual void Visit(const Variant::NullValue & v)
			{
				printer_ << XTL::JSON::Null();
			}

			virtual void Visit(const Variant::Boolean & v)
			{
				printer_ << v.GetValue();
			}

			virtual void Visit(const Variant::LongLongInt & v)
			{
				printer_ << v.GetValue();
			}

			virtual void Visit(const Variant::Double & v)
			{
				printer_ << v.GetValue();
			}

			virtual void Visit(const Variant::String & v)
			{
				printer_ << v.GetValue();
			}

			virtual void Visit(const Variant::Array & v)
			{
				printer_ << XTL::JSON::Array();

				for (std::auto_ptr<Variant::Array::ConstIterator> itr(v.GetConstIterator()); itr->NotAtEnd(); itr->Advance())
				{
					itr->Current()->Visit(*this);
				}

				printer_ << XTL::JSON::End();
			}

			virtual void Visit(const Variant::Struct & v)
			{
				printer_ << XTL::JSON::Object();

				for (std::auto_ptr<Variant::Struct::ConstIterator> itr(v.GetConstIterator()); itr->NotAtEnd(); itr->Advance())
				{
					printer_ << itr->Key();
					itr->Value()->Visit(*this);
				}

				printer_ << XTL::JSON::End();
			}

		private:

			JsonPrinter printer_;
	};

	void ToJson(const XTL::Variant & value, PrintStream & printStream, JsonPrinter::Spacer & spacer)
	{
		XTL::VariantJsonifier vj(printStream, spacer);
		value.Visit(vj);
	}
}

void PrintBit(unsigned int bit)
{
	printf("%u\n", bit);
}

#include <xtl/Logger.hpp>
#include <xtl/utils/AutoPtrMapCache.hpp>

struct Obj
{
	const int i;

	explicit Obj(int i_)
		: i(i_)
	{
		printf("Construct %d\n", i);
	}

	~Obj() throw()
	{
		printf("Destruct %d\n", i);
	}

	struct Creator
	{
		const int i;

		explicit Creator(int i_)
			: i(i_)
		{
			;;
		}

		Obj * operator() () const
		{
			return new Obj(i);
		}
	};
};

int main(int argc, const char * argv[])
{
	{
		XTL::AutoPtrMapCache<std::string, Obj> cache(3);
		printf("---\n");
		cache["A"].SetIfNull(Obj::Creator(1));
		printf("---\n");
		cache["B"] = new Obj(2);
		printf("---\n");
		cache["C"] = new Obj(3);
		printf("---\n");
		cache["A"].SetIfNull(Obj::Creator(6));
		printf("---\n");
		cache["D"] = new Obj(4);
		printf("---\n");
		cache["B"];
		printf("---\n");
		cache["E"] = new Obj(5);
		printf("---\n");
		return 0;
	}

	{
		XTL::DefaultLogger().SetMinLogLevel(LOG_INFO);
		XTL::Log(LOG_ERROR, "Error");
		XTL::Log(LOG_WARN,  "Warn");
		XTL::Log(LOG_INFO,  "Info");
		XTL::Log(LOG_DEBUG, "Debug");
		XTL::Log(LOG_TRACE, "Trace");
		return 0;
	}

	{
		XTL::BitSet32 s;
		s.Add(4);
		s.Add(3);
		s.Add(0);
		s.Add(24);
		s.ForEach(PrintBit);

		for (XTL::BitSet32::const_iterator itr = s.begin(); itr != s.end(); ++itr)
		{
			printf(">>> %u\n", *itr);
		}

		return 0;
	}

	{
		XTL::VariantMap vm;
		vm.Set("abc", XTL::Variant::Null());
		vm.Set("xyz", XTL::Variant::True());
		vm.Set("and \"false\"", XTL::Variant::False());

		XTL::Dump(XTL::StdOut(), vm);
		return 0;

		XTL::JsonPrinter::CompactSpacer compactSpacer;
		XTL::JsonPrinter::FormattingSpacer formattingSpacer;

		XTL::ToJson(vm, XTL::StdOut(), formattingSpacer);

		return 0;

		XTL::JsonPrinter(XTL::StdOut(), formattingSpacer)
			<< XTL::JSON::Array()
				<< XTL::JSON::Null()
				<< true
				<< "Fuck !!!"
				<< 1
				<< 0.0
				<< 200000000000.0
				<< XTL::JSON::Null()
				<< XTL::JSON::Object()
					<< "null" << XTL::JSON::Null()
					<< "boolean0" << false
					<< "boolean1" << true
					<< "integer" << 1024
					<< "unsigned long long int" << 18446744073709551615llu
					<< "string" << "I said: \"It's ok!\""
					<< "string2" << "\\\\\\\\\\\r\n"
					<< "linear_array" << XTL::JSON::LinearArray()
						<< 1 << 2 << 3 << 4 << 5 << XTL::JSON::LinearArray()
								<< true << false
							<< XTL::JSON::End()
						<< XTL::JSON::End()
					<< XTL::JSON::End()
			<< XTL::JSON::End()
		<< XTL::JSON::Finish()
		;
	}

	return 0;

	XTL::FloatStringifier fs(-12345);
	XTL::IntegerStringifier<int> is(123);

	printf(">> (%s)\n", is.ToString().c_str());
	printf(">> (%s)\n", fs.ToString().c_str());

	return 0;

	{
		XTL::VariantDumper dumper(XTL::StdOut());

		XTL::VariantMap vmap;

		vmap.Set("x", XTL::VariantPtr(new XTL::Variant::LongLongInt(1024)));
		vmap.Set("y", XTL::VariantPtr(new XTL::Variant::Double(3.1415)));
		vmap.Set("First", XTL::Variant::Boolean::True());
		vmap.Set("Second", XTL::Variant::Boolean::False());

		//vmap.Visit(dumper);
		printf("\n");
		return 0;
	}

	MyFuckingTree bt;

	const unsigned int N = 1000;
	bool inserted = false;
	for (unsigned int i = 1; i <= N; ++i)
	{
		MyItem & item = bt.Insert(MyKey(F4(i, N)), inserted);
		item.value = i;
	}

	unsigned int i = 1;
	for (MyFuckingTree::ConstIterator itr = bt.Begin(); !itr.AtEnd(); itr.Advance())
	{
		if (i != (*itr).key.i)
		{
			fprintf(stderr, "Test Failed!\n");
			return 1;
		}
		++i;
	}

	fprintf(stderr, "Test ok!\n");

	printf("Tree size........%u\n", bt.Size());
	printf("Tree is empty....%s\n", bt.Empty() ? "YES" : "NO");
	printf("Leaves count.....%u\n", bt.LeavesCount());
	printf("Branches count...%u\n", bt.BranchesCount());

	return 0;

	{
		XTL::SimpleStringMatcher ssm("abc-(%d%d))");

		if (XTL::SimpleStringMatcher::Result result = ssm.Match("abc-23"))
		{
			printf("Matched '%s'!\n", result.GetSubmatch("abc-23", 0).c_str());
		}

		return 0;
	}

	{
		time_t ts = ::time(0);
		struct tm * t = ::gmtime(&ts);

		try
		{
			XTL::FileUtils::CreatePathForFile(XTL::FormatString("%04d/%02d/%02d/event_file", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour), 0700);
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "%s\n", e.What().c_str());
		}

		return 0;
	}

	/*
	{
		try
		{
			XTL::TcpClient client("microsoft.com", 80, false);

			bool r = client.Connect(10, 0.1);

			if (!r)
			{
				printf("NOT connected\n");
				return 1;
			}

			printf("connected\n");

			const std::string data(65536, 'x');// = "GET / HTTP/1.0\r\n\r\n";
			r = client.Send(data.data(), data.size(), 5, 0.1);

			printf("%s\n", r ? "Sent" : "Send failed!");
		}
		catch (XTL::SocketAddressError & e)
		{
			printf("Could not resolve address\n");
		}
		catch (const XTL::UnixError & e)
		{
			printf("(%u) %s\n", e.Code(), e.What().c_str());
		}

		return 0;
	}
	*/

	/*
	{
		// B    - 0.750  0.194
		// C<B> - 0.810  0.196
		// D<B> - 1.285  0.256

		std::auto_ptr<A> p(new D<B>());
		for (unsigned int i = 0; i < 100000000; ++i)
		{
			p->Do();
		}
		return 0;
	}
	*/

	/*
	{
		try
		{
			XTL::SetSignalHandler(SIGTERM, TerminateProgram);
			XTL::SetSignalHandler(SIGINT,  TerminateProgram);

			server.reset(new MyTcpServer());
			server->Listen(8086);
			server->Listen(8087);

			server->Run();
		}
		catch (const XTL::UnixError::Interrupted & e)
		{
			;;
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "UnixError: (%d) %s\n", e.Code(), e.What().c_str());
			return 1;
		}

		return 0;
	}
	*/

	{
		const std::string s = "what the fuck ? {{if z}} {{elseif y}} {{else}}{{/if}} {{loop a}} { { {{loop x}} 1 {{/loop}} 2 3{{/loop}}";
		XTL::CharSource::ConstCharPtr charSource(s.data(), s.size());

		try
		{
			XTL::TemplateTreeBuilder treeBuilder;
			XTL::TemplateParser parser(charSource, treeBuilder);
			parser.Parse();

			std::auto_ptr<XTL::TemplateNodeList> templateRootList = treeBuilder.Release();
			printf("=====================================================\n");
			templateRootList->DebugPrint(0);
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

		try
		{
			while (1)
			{
				char buffer[1024];

				unsigned int result = clientSocket.Receive(buffer, sizeof(buffer) - 1);

				if (result > 0)
				{
					buffer[result] = '\0';
					printf("%s", buffer);
					// printf("%d\n", size);
				}
			}
		}
		catch (XTL::ClientSocket::Disconnected & e)
		{
			;;
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
