#ifndef _XTL__PROGRAM_ARGUMENTS_HPP__
#define _XTL__PROGRAM_ARGUMENTS_HPP__ 1

#include <string.h>
#include <string>
#include <list>
#include <stdexcept>

namespace XTL
{
	class ArgumentBase
	{
		public:

			ArgumentBase(const unsigned long long & flag)
				: flag_(flag) { ;; }

			virtual ~ArgumentBase() { ;; }

			virtual bool NeedValue() const = 0;

			virtual void Set(const void * v) = 0;

		protected:

			unsigned long long flag_;
	};

	struct ArgumentDesc
	{
		const char   * flag1;
		const char   * flag2;
		const char   * desc;
		ArgumentBase * arg;
		void         * params;
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
		};

		class ArgumentScalar : public Argument
		{
			public:

				virtual void Set(const void * v)
				{
					if (Argument::Exists())
					{
						throw std::runtime_error("Doubled program option");
					}

					DoSet(v);

					Argument::SetFlag();
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

/*
	struct ArgumentModeValueFlags
	{
		const char * const flag1,
		const char * const flag2
	};

	class ArgumentMode : public Argument
	{
		public:

			ArgumentMode()
				: Argument(), value_(-1) { ;; }

			virtual ~ArgumentMode()
			{
				for (std::list<ArgumentModeValue *>::iterator itr = values_.begin();
				     itr != values_.end();
				     ++itr)
				{
					delete *itr;
				}
			}

			virtual bool NeedValue()
			{
				return false;
			}

			virtual void Set(const char * s)
			{
				if (Exists())
				{
					throw std::runtime_error("Doubled program option");
				}

				value_ = reinterpret_cast<const ArgumentModeValue *>(s)->Value();

				ArgumentsPool::Instance().SetFlag(flag_);
			}

		protected:

			void Init(ArgumentModeValueFlags & valuesFlags)
			{
				long long int i = 0;
				for (ArgumentModeValueFlags * flags = valuesFlags;
				     flags.flag1 != 0 || flags.flag2 != 0;
				     ++flags, ++i)
				{
					ArgumentModeValue * modeValue = new ArgumentModeValue(*this, i);
					values_.push_back(modeValue);
					
				}
			}

			std::list<ArgumentModeValue *> values_;
			long long value_;
	};

	class ArgumentModeValue : public Argument
	{
		public:

			ArgumentModeValue(ArgumentMode & parent, const long long int & value)
				: Argument(), parent_(parent), value_(value) { ;; }

			virtual bool NeedValue()
			{
				return false;
			}

			virtual void Set(const char * s)
			{
				parent_.Set(reinterpret_cast<const char *>(this));
			}

			const long long & Value() const { return value_; }

		protected:

			ArgumentMode  & parent_;
			long long int   value_;
	};
*/
	};

	class ArgumentsPool
	{
		public:

			enum
			{
				REQUIRED = 0x0001,
				PASSWORD = 0x0002
			};

			unsigned long long NextFlag()
			{
				return flag_ = (flag_ > 0 ? (flag_ << 1) : 1);
			}

			void SetFlag(const unsigned long long & flag)
			{
				set_ |= flag;
			}

			void AddDesc(const ArgumentDesc & desc)
			{
				// TODO: check for such flag1 or flag2 is already exists
				descList_.push_back(desc);
			}

			bool Contains(const unsigned long long & flag)
			{
				return (set_ & flag) != 0;
			}

			ArgumentDesc * FindByFlag1(const char * flag1)
			{
				for (std::list<ArgumentDesc>::iterator itr = descList_.begin();
				     itr != descList_.end();
				     ++itr)
				{
					if (itr->flag1 != 0 && ::strcmp(flag1, itr->flag1) == 0)
					{
						return &(*itr);
					}
				}
				return 0;
			}

			ArgumentDesc * FindByFlag2(const char * flag2)
			{
				for (std::list<ArgumentDesc>::iterator itr = descList_.begin();
				     itr != descList_.end();
				     ++itr)
				{
					if (itr->flag2 != 0 && ::strcmp(flag2, itr->flag2) == 0)
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

#define ARGUMENT_STRING(NAME, FLAG1, FLAG2, DESC) \
		protected: \
			class __Argument##NAME : public ArgumentString \
			{ \
				public: \
					__Argument##NAME() : ArgumentString() \
					{ \
						PoolType::Instance()->AddDesc(Desc()); \
					} \
					ArgumentDesc & Desc() \
					{ \
						static ArgumentDesc desc = { FLAG1, FLAG2, DESC, this }; \
						return desc; \
					} \
			}; \
			__Argument##NAME _##NAME; \
		public: \
			const std::string & NAME() const { return *_##NAME; } \
			bool Has##NAME() { return _##NAME.Exists(); }

/*
#define ARGUMENT_FLAG(NAME, FLAG1, FLAG2, DESC) \
		protected: \
			class Argument##NAME : public ArgumentFlag \
			{ \
				public: \
					Argument##NAME() : ArgumentFlag() \
					{ \
						ArgumentsPool::Instance().AddDesc(Desc()); \
					} \
					ArgumentDesc & Desc() \
					{ \
						static ArgumentDesc desc = { FLAG1, FLAG2, DESC, this }; \
						return desc; \
					} \
			}; \
			Argument##NAME _##NAME; \
		public: \
			const bool NAME() const { return _##NAME.Exists(); } \
			bool Has##NAME() { return _##NAME.Exists(); }
*/

/*
	PROGRAM_ARGUMENTS_BEGIN
		ARGUMENT_STRING(Config,  "c", "config", "ConfigFileName", REQUIRED)
		ARGUMENT_FLAG  (Quickly, "q", "quickly", "Do it quickly", OPTIONAL)
		ARGUMENT_MODE_BEGIN(Mode, OPTIONAL)
			ARGUMENT_MODE_VALUE("d", "daily",   "Update daily info")
			ARGUMENT_MODE_VALUE("m", "monthly", "Update monthly info")
		ARGUMENT_MODE_END
	PROGRAM_ARGUMENTS_END
*/

	class ProgramArguments
	{
		public:

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

//			ARGUMENT_STRING(Config,  "c", "config", "Config file name")
//			ARGUMENT_FLAG  (Quickly, "q", "quick",  "Do it quickly")

		///////////////////////////////////////////////////////////////////////////

		protected:

			class __ArgumentConfig : public ArgumentString
			{
				public:

					__ArgumentConfig() : ArgumentString()
					{
						PoolType::Instance()->AddDesc(Desc());
					}

					ArgumentDesc & Desc()
					{
						static ArgumentDesc desc = { "c", "config", "Config file name", this };
						return desc;
					}
			};

			__ArgumentConfig _Config;

		public:

			const std::string & Config() const { return *_Config; }

			bool HasConfig() { return _Config.Exists(); }

		///////////////////////////////////////////////////////////////////////////

		protected:

			class ArgumentMoodValue : public Argument
			{
				public:

					ArgumentMoodValue() : Argument() { ;; }

				protected:
			};

			class __ArgumentsPoolMood : public ArgumentsPool
			{
				public:

					static __ArgumentsPoolMood * Instance()
					{
						static __ArgumentsPoolMood instance;
						return &instance;
					}

				protected:

					__ArgumentsPoolMood() : ArgumentsPool() { ;; }
			};

			class __ArgumentMood : public Argument
			{
				public:

					static __ArgumentMood * Instance()
					{
						static __ArgumentMood instance;
						return &instance;
					}

					virtual bool NeedValue() const
					{
						return false;
					}

					virtual void Set(const void *) { ;; }

					void Select(const ArgumentDesc * value)
					{
						if (Exists())
						{
							throw std::runtime_error("Doubled program mode");
						}

						PoolType::Instance()->SetFlag(flag_);
						selected_ = value;
					}

				protected:

					__ArgumentMood()
						: Argument(),
						  selected_(0)
					{
						PoolType::Instance()->AddDesc(Desc());
					}

					ArgumentDesc & Desc()
					{
						static ArgumentDesc desc = { 0, 0, "Program mode", 0, 0 };
						return desc;
					}

					const ArgumentDesc * selected_;

				protected:

					class __ArgumentModeDay : public Argument
					{
						public:

							__ArgumentModeDay()
								: index_(0)
							{
								PoolType::Instance()->AddDesc(Desc());
							}

							virtual bool NeedValue() const
							{
								return false;
							}

							virtual void Set(const void *)
							{
								__ArgumentMood::Instance()->Select(&(Desc()));
							}

						protected:

							ArgumentDesc & Desc()
							{
								static ArgumentDesc desc = { "d", "daily", "", this, 0 };
								return desc;
							}

							unsigned long long index_;
					};

					__ArgumentModeDay __ModeDay;

//					typedef ArgumentTypes<__ArgumentsPoolMood>::Argument        Argument;
//					typedef ArgumentTypes<__ArgumentsPoolMood>::ArgumentFlag    ArgumentFlag;
//					typedef ArgumentTypes<__ArgumentsPoolMood>::ArgumentString  ArgumentString;
//					typedef ArgumentTypes<__ArgumentsPoolMood>::ArgumentInteger ArgumentInteger;
			};

			class __ArgumentMoodSentinel
			{
				public:

					__ArgumentMoodSentinel()
					{
						__ArgumentMood::Instance();
					}
			};

			__ArgumentMoodSentinel _Mood;

/*
			class ArgumentMoode : public ArgumentMode
			{
				public:

					ArgumentMoode() : ArgumentMode()
					{
						ArgumentsPool::Instance().AddDesc(Desc());
					}

					ArgumentDesc & Desc()
					{
						static ArgumentDesc desc = { 0, 0, "Bla-bla-bla", this };
						return desc;
					}

				protected:

					ArgumentModeValue * values_;
			};
*/
			/*
				ARGUMENT_OPTION(Mode, 0, 0, "")
				ARGUMENT_OPTION_VALUE(Mode, "d", "day")
			*/

		protected:
/*
			class ArgumentPeriod : public ArgumentInteger
			{
				public:

					ArgumentPeriod() : ArgumentInteger()
					{
						ArgumentsPool::Instance().AddDesc(Desc());
					}

					ArgumentDesc & Desc()
					{
						static ArgumentDesc desc = { "p", "period", "Period of calculation", this };
						return desc;
					}
			};

			ArgumentPeriod _Period;

		public:

			const long long Period() const { return _Period.Exists(); }

			bool HasPeriod() { return _Period.Exists(); }
*/
	};
}

#endif
