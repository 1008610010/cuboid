#ifndef _XTL__PROGRAM_ARGUMENTS_HPP__
#define _XTL__PROGRAM_ARGUMENTS_HPP__ 1

#include <string.h>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>

namespace XTL
{
	class ArgumentsPool;

	class ArgumentBase
	{
		public:

			enum
			{
				OPTIONAL = 0x0000,
				REQUIRED = 0x0001,
				PASSWORD = 0x0002
			};

			ArgumentBase(const unsigned long long & flag)
				: flag_(flag) { ;; }

			virtual ~ArgumentBase() { ;; }

			virtual bool NeedValue() const = 0;

			virtual ArgumentsPool * Set(const void * v) = 0;

		protected:

			unsigned long long flag_;
	};

	struct ArgumentDesc
	{
		const char         * label1;
		const char         * label2;
		const char         * desc;
		unsigned long long   flags;
		ArgumentBase       * arg;
	};

	class ArgumentsPool
	{
		public:

			unsigned long long NextFlag()
			{
				return flag_ = (flag_ > 0 ? (flag_ << 1) : 1);
			}

			void SetFlag(const unsigned long long & flag)
			{
				set_ |= flag;
			}

			void AddDesc(const ArgumentDesc & desc, ArgumentBase * arg)
			{
				fprintf(stderr, "AddDesc(%s, %s)\n", desc.label1, desc.label2);
				// TODO: check for such label1 or label2 is already exists
				ArgumentDesc newDesc(desc);
				newDesc.arg = arg;
				descList_.push_back(newDesc);
			}

			bool Contains(const unsigned long long & flag)
			{
				return (set_ & flag) != 0;
			}

			ArgumentDesc * FindByLabel1(const char * label1)
			{
				for (std::list<ArgumentDesc>::iterator itr = descList_.begin();
				     itr != descList_.end();
				     ++itr)
				{
					if (itr->label1 != 0 && ::strcmp(label1, itr->label1) == 0)
					{
						return &(*itr);
					}
				}
				return 0;
			}

			ArgumentDesc * FindByLabel2(const char * label2)
			{
				for (std::list<ArgumentDesc>::iterator itr = descList_.begin();
				     itr != descList_.end();
				     ++itr)
				{
					if (itr->label2 != 0 && ::strcmp(label2, itr->label2) == 0)
					{
						return &(*itr);
					}
				}
				return 0;
			}

			bool Parse(int argc, const char * argv[]);

		protected:

			ArgumentsPool()
				: flag_(0),
				  set_(0),
				  descList_(0)
			{
				;;
			}

			unsigned long long flag_;
			unsigned long long set_;
			std::list<ArgumentDesc> descList_;
	};

	template <typename _PoolType>
	struct ArgumentTypes
	{
		class Argument : public ArgumentBase
		{
			public:

				typedef _PoolType PoolType;

				Argument()
					: ArgumentBase(PoolType::Instance()->NextFlag())
				{
					;;
				}

				bool Exists() const
				{
					return PoolType::Instance()->Contains(flag_);
				}

				void SetFlag()
				{
					PoolType::Instance()->SetFlag(flag_);
				}

				void AddDesc(const ArgumentDesc & desc, ArgumentBase * arg)
				{
					PoolType::Instance()->AddDesc(desc, arg);
				}
		};

		class ArgumentScalar : public Argument
		{
			public:

				virtual ArgumentsPool * Set(const void * v)
				{
					if (Argument::Exists())
					{
						throw std::runtime_error("Doubled program option");
					}

					DoSet(v);

					Argument::SetFlag();
					return 0;
				}

				virtual void DoSet(const void * v) = 0;
		};

		class ArgumentFlag : public ArgumentScalar
		{
			public:

				virtual bool NeedValue() const
				{
					return false;
				}

				virtual void DoSet(const void * s)
				{
					;;
				}
		};

		template <typename T>
		class TypedArgument : public ArgumentScalar
		{
			public:

				TypedArgument() : ArgumentScalar(), value_() { ;; }

				T & operator* ()
				{
					return value_;
				}

				const T & operator* () const
				{
					return value_;
				}

				virtual bool NeedValue() const
				{
					return true;
				}

			protected:

				T value_;
		};

		class ArgumentString : public TypedArgument<std::string>
		{
			public:

				void DoSet(const void * v)
				{
					(*(*this)).assign(static_cast<const char *>(v));
				}
		};

		class ArgumentInteger : public TypedArgument<long long>
		{
			typedef TypedArgument<long long> Super;

			public:

				virtual void DoSet(const void * v)
				{
					const char * p = static_cast<const void *>(v);

					bool neg = false;
					if (*p == '-')
					{
						neg = true;
						++p;
					}
					else if (*p == '+')
					{
						++p;
					}

					Super::value_ = 0;
					for (; *p != '\0'; ++p)
					{
						if (*p < '0' || *p > '9')
						{
							throw std::runtime_error("Invalid integer value");
						}
						Super::value_ = (Super::value_ << 3) + (Super::value_ << 1) + (*p - '0');
					}

					if (neg)
					{
						Super::value_ = -Super::value_;
					}
				}
		};

		template <typename DeriveClass>
		class ArgumentModes : public Argument
		{
			public:

				static DeriveClass * Instance()
				{
					static DeriveClass instance;
					return &instance;
				}

				class Sentinel
				{
					public:

						Sentinel()
						{
							Instance();
						}

						static DeriveClass * Instance()
						{
							return ArgumentModes::Instance();
						}
				};

				class Counter
				{
					public:

						static int NextIndex()
						{
							return Instance().count_++;
						}

					protected:

						Counter() : count_(0) { ;; }

						static Counter & Instance()
						{
							static Counter instance;
							return instance;
						}

						int count_;
				};

				virtual bool NeedValue() const
				{
					return false;
				}

				virtual ArgumentsPool * Set(const void *)
				{
					return 0;
				}

				void Select(const ArgumentDesc * value, int index)
				{
					if (Argument::Exists())
					{
						throw std::runtime_error("Doubled program mode");
					}

					Argument::SetFlag();
					selected_ = value;
					selectedIndex_ = index;
				}

				const ArgumentDesc * Selected() const
				{
					return selected_;
				}

				static int NextModeIndex()
				{
					return Counter::NextIndex();
				}

				int SelectedIndex() const
				{
					return selectedIndex_;
				}

			protected:

				ArgumentModes()
					: Argument(),
					  selected_(0),
					  selectedIndex_(-1)
				{
					;;
				}

				const ArgumentDesc * selected_;
				int selectedIndex_;
		};

		template <typename DeriveClass>
		class ArgumentModeFlag : public Argument
		{
			public:

				class Pool : public ArgumentsPool
				{
					public:

						static Pool * Instance()
						{
							static Pool instance;
							return &instance;
						}
				};

				ArgumentModeFlag(int index)
					: Argument(), index_(index), active_(false)
				{
					;;
				}

				virtual bool NeedValue() const
				{
					return false;
				}

				virtual ArgumentsPool * Set(const void * v)
				{
					active_ = true;
					return Pool::Instance();
				}

				int Index() const
				{
					return index_;
				}

				bool Active() const
				{
					return active_;
				}

			protected:

				class ModeArguments
				{
					typedef Pool PoolType;
				};

				int  index_;
				bool active_;
		};
	};

	class ArgumentsMainPool : public ArgumentsPool
	{
		public:

			static ArgumentsMainPool * Instance()
			{
				static ArgumentsMainPool instance;
				return &instance;
			}

		protected:

			ArgumentsMainPool() : ArgumentsPool() { ;; }
	};

#define POOL(NAME) \
	class NAME : public ArgumentsPool \
	{ \
		public: \
			static NAME * Instance() \
			{ \
				static NAME instance; \
				return &instance; \
			} \
	};

#define ARGUMENT_STRING(NAME, LABEL1, LABEL2, FLAGS, DESC) \
		protected: \
			class __Argument_##NAME : public ArgumentTypes<PoolType>::ArgumentString \
			{ \
				public: \
					__Argument_##NAME() : ArgumentTypes<PoolType>::ArgumentString() \
					{ \
						Argument::AddDesc(Desc(), this); \
					} \
					static ArgumentDesc & Desc() \
					{ \
						static ArgumentDesc desc = { LABEL1, LABEL2, DESC, FLAGS }; \
						return desc; \
					} \
			}; \
			__Argument_##NAME __##NAME; \
		public: \
			const std::string & NAME() const { return *__##NAME; } \
			bool Has##NAME() { return __##NAME.Exists(); }

#define ARGUMENT_MODES_BEGIN(NAME, FLAGS, DESC) \
		protected: \
			class __ArgumentModes_##NAME : public ArgumentTypes<PoolType>::ArgumentModes<__ArgumentModes_##NAME> \
			{ \
				public: \
					__ArgumentModes_##NAME() : ArgumentTypes<PoolType>::ArgumentModes<__ArgumentModes_##NAME>() \
					{ \
						Argument::AddDesc(Desc(), this); \
					} \
					static ArgumentDesc & Desc() \
					{ \
						static ArgumentDesc desc = { 0, 0, DESC, FLAGS }; \
						return desc; \
					} \
					typedef __ArgumentModes_##NAME ParentType; \


#define ARGUMENT_MODES_END(NAME) \
			}; \
		protected: \
			__ArgumentModes_##NAME::Sentinel __##NAME; \
		public: \
			__ArgumentModes_##NAME & NAME() \
			{ \
				return *(__ArgumentModes_##NAME::Instance()); \
			}

#define ARGUMENT_MODE_FLAG_BEGIN(NAME, LABEL1, LABEL2, FLAGS, DESC) \
		protected: \
			class __ArgumentMode_##NAME : public ArgumentTypes<PoolType>::ArgumentModeFlag<__ArgumentMode_##NAME> \
			{ \
				typedef ArgumentTypes<PoolType>::ArgumentModeFlag<__ArgumentMode_##NAME> Super; \
				public: \
					__ArgumentMode_##NAME() : Super(ParentType::NextModeIndex()) \
					{ \
						Argument::AddDesc(Desc(), this); \
					} \
					static ArgumentDesc & Desc() \
					{ \
						static ArgumentDesc desc = { LABEL1, LABEL2, DESC, FLAGS }; \
						return desc; \
					} \
					virtual ArgumentsPool * Set(const void * v) \
					{ \
						ParentType::Instance()->Select(&(Desc()), Index()); \
						args_.reset(new Arguments()); \
						return Super::Set(v); \
					} \
				protected: \
					class Arguments : public ModeArguments \
					{

#define ARGUMENT_MODE_FLAG_END(NAME) \
					}; \
					std::auto_ptr<Arguments> args_; \
			}; \
		public: \
			__ArgumentMode_##NAME __##NAME; \
			bool Is##NAME() const \
			{ \
				return __##NAME.Active(); \
			} \
			const __ArgumentMode_##NAME & NAME() \
			{ \
				return __##NAME; \
			} \

	class ProgramArguments
	{
		public:

			typedef ArgumentsMainPool PoolType;

			typedef ArgumentTypes<ArgumentsMainPool>::Argument        Argument;
			typedef ArgumentTypes<ArgumentsMainPool>::ArgumentFlag    ArgumentFlag;
			typedef ArgumentTypes<ArgumentsMainPool>::ArgumentString  ArgumentString;
			typedef ArgumentTypes<ArgumentsMainPool>::ArgumentInteger ArgumentInteger;

			static ProgramArguments & Instance()
			{
				static ProgramArguments args;
				return args;
			}

			bool Parse(int argc, const char * argv[])
			{
				return ArgumentsMainPool::Instance()->Parse(argc, argv);
			}

		protected:

			ProgramArguments() { ;; }
			ProgramArguments(const ProgramArguments &);
			ProgramArguments & operator= (const ProgramArguments &);

			ARGUMENT_STRING(Config, "c", "config", OPTIONAL, "Config file name")

			ARGUMENT_MODES_BEGIN(Period, OPTIONAL, "Period of calculation")

				ARGUMENT_MODE_FLAG_BEGIN(Daily, "d", "daily", OPTIONAL, "Daily")
					ARGUMENT_STRING(Config, "c", "config", REQUIRED, "Daily config file name")
				ARGUMENT_MODE_FLAG_END(Daily)

				ARGUMENT_MODE_FLAG_BEGIN(Monthly, "m", "monthly", OPTIONAL, "Monthly")
					ARGUMENT_STRING(Config, "c", "config", REQUIRED, "Monthly config file name")
				ARGUMENT_MODE_FLAG_END(Monthly)

			ARGUMENT_MODES_END(Period)

		///////////////////////////////////////////////////////////////////////////
/*
		protected:

			class __ArgumentModes_Period : public ArgumentTypes<PoolType>::ArgumentModes<__ArgumentModes_Period>
			{
				public:

					__ArgumentModes_Period()
						: ArgumentTypes<PoolType>::ArgumentModes<__ArgumentModes_Period>()
					{
						Argument::AddDesc(Desc(), this);
					}

					static ArgumentDesc & Desc()
					{
						static ArgumentDesc desc = { 0, 0, "Program mode" };
						return desc;
					}

					typedef __ArgumentModes_Period ParentType;

				protected:

					class __ArgumentMode_Daily : public ArgumentTypes<PoolType>::ArgumentModeFlag<__ArgumentMode_Daily>
					{
						typedef ArgumentTypes<PoolType>::ArgumentModeFlag<__ArgumentMode_Daily> Super;

						public:

							__ArgumentMode_Daily()
								: Super(ParentType::NextModeIndex())
							{
								Argument::AddDesc(Desc(), this);
							}

							static ArgumentDesc & Desc()
							{
								static ArgumentDesc desc = { "d", "daily", "" };
								return desc;
							}

							virtual ArgumentsPool * Set(const void * v)
							{
								ParentType::Instance()->Select(&(Desc()), Index());
								args_.reset(new Arguments());
								return Super::Set(v);
							}

						protected:

							class Arguments : public ModeArguments
							{
								ARGUMENT_STRING(Config, "c2", "config2", "Config2 file name")
							};

							std::auto_ptr<Arguments> args_;
					};

				public:

					__ArgumentMode_Daily __Daily;

					bool IsDaily() const
					{
						return __Daily.Active();
					}

					const __ArgumentMode_Daily & Daily()
					{
						return __Daily;
					}
			};

			__ArgumentModes_Period::Sentinel __Period;

		public:

			__ArgumentModes_Period & Period()
			{
				return *(__ArgumentModes_Period::Instance());
			}
*/
	};
}

#endif
