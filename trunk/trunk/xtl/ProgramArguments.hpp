#ifndef _XTL__PROGRAM_ARGUMENTS_HPP__
#define _XTL__PROGRAM_ARGUMENTS_HPP__ 1

#include <string.h>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>

namespace XTL
{
	class ProgramOptionsError
	{
		public:

			ProgramOptionsError(const std::string & what)
				: what_(what) { ;; }

			const char * What() const
			{
				return what_.c_str();
			}

		protected:

			std::string what_;
	};

	class OptionDesc;
	class OptionsPool;

	class Option
	{
		public:

			enum
			{
				OPTIONAL = 0x0000,
				REQUIRED = 0x0001,
				PASSWORD = 0x0002
			};

			Option(const unsigned long long & id)
				: id_(id) { ;; }

			virtual ~Option() { ;; }

			virtual bool NeedValue() const = 0;

			virtual OptionsPool * Set(const void * v) = 0;

			const unsigned long long & Id() const
			{
				return id_;
			}

			virtual const OptionDesc * Desc() const
			{
				throw ProgramOptionsError("Internal logic error: abstract method Desc()");
			}

		protected:

			unsigned long long id_;
	};

	struct OptionDesc
	{
		const char         * label1;
		const char         * label2;
		const char         * desc;
		unsigned long long   flags;

		const std::string AsString() const;
	};

	class OptionsPool
	{
		public:

			typedef std::list<std::pair<const OptionDesc *, Option *> > ListOfOptions;

			void AddOption(const OptionDesc * desc, Option * option);

			const unsigned long long NextId();

			void SetPresent(const unsigned long long & id);

			bool Contains(const unsigned long long & id);

			bool Parse(int argc, const char * argv[]);

		protected:

			OptionsPool();

			const ListOfOptions::iterator FindByLabel1(const char * label1);

			const ListOfOptions::iterator FindByLabel2(const char * label2);

			void CheckRequiredOptions();

			unsigned long long nextId_;
			unsigned long long set_;
			ListOfOptions      options_;
	};

	bool OptionStringToInteger(const char * s, long long & i);

	template <typename _PoolType>
	struct ProgramOptions
	{
		class PoolOption : public Option
		{
			public:

				typedef _PoolType PoolType;

				static PoolType & Pool()
				{
					return *(PoolType::Instance());
				}

				PoolOption()
					: Option(Pool().NextId())
				{
					;;
				}

				bool Exists() const
				{
					return Pool().Contains(id_);
				}

				void SetPresent()
				{
					Pool().SetPresent(id_);
				}

				void AddToPool()
				{
					Pool().AddOption(Desc(), this);
				}
		};

		class OptionScalar : public PoolOption
		{
			public:

				virtual OptionsPool * Set(const void * v)
				{
					if (PoolOption::Exists())
					{
						throw ProgramOptionsError(std::string("Duplicate program option: ") + this->Desc()->AsString());
					}

					DoSet(v);

					PoolOption::SetPresent();
					return 0;
				}

				virtual void DoSet(const void * v) = 0;
		};

		class OptionFlag : public OptionScalar
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
		class TypedOption : public OptionScalar
		{
			public:

				TypedOption() : OptionScalar(), value_() { ;; }

				T & operator* ()              { return value_; }

				const T & operator* () const  { return value_; }

				virtual bool NeedValue() const
				{
					return true;
				}

			protected:

				T value_;
		};

		class OptionString : public TypedOption<std::string>
		{
			public:

				void DoSet(const void * v)
				{
					this->value_.assign(static_cast<const char *>(v));
				}
		};

		class OptionInteger : public TypedOption<long long>
		{
			public:

				virtual void DoSet(const void * v)
				{
					if (!OptionStringToInteger(static_cast<const char *>(v), this->value_))
					{
						throw ProgramOptionsError(
							std::string("Invalid option's integer value '") + static_cast<const char *>(v) + "'"
						);
					}
				}
		};

		template <typename DeriveClass>
		class ArgumentModes : public PoolOption
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

				virtual OptionsPool * Set(const void *)
				{
					return 0;
				}

				void Select(const OptionDesc * desc, int index)
				{
					if (PoolOption::Exists())
					{
						throw ProgramOptionsError(
							std::string("Ambiguous program mode (") +
							selectedDesc_->AsString() +
							" and " +
							desc->AsString() +
							")"
						);
					}

					PoolOption::SetPresent();

					selectedDesc_ = desc;
					selectedIndex_ = index;
				}

				const OptionDesc * Selected() const
				{
					return selectedDesc_;
				}

				const int SelectedIndex() const
				{
					return selectedIndex_;
				}

				static int NextModeIndex()
				{
					return Counter::NextIndex();
				}

			protected:

				ArgumentModes()
					: PoolOption(),
					  selectedDesc_(0),
					  selectedIndex_(-1)
				{
					;;
				}

				const OptionDesc * selectedDesc_;
				int selectedIndex_;
		};

		class OptionMode : public PoolOption
		{
			public:

				OptionMode(int index)
					: PoolOption(), index_(index), active_(false)
				{
					;;
				}

				virtual OptionsPool * Set(const void * v)
				{
					active_ = true;
					return 0;
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

				int  index_;
				bool active_;
		};
/*
		class OptionModeFlag : public OptionMode
		{
			public:

				OptionModeFlag(int index)
					: OptionMode(index)
				{
					;;
				}

				virtual bool NeedValue() const
				{
					return false;
				}

				virtual OptionsPool * Set(const void * v)
				{
					ParentType::Instance()->Select(Desc(), Index());
					active_ = true;
					return 0;
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

				int  index_;
				bool active_;
		};
*/
		template <typename DeriveClass>
		class ArgumentModeFlag : public PoolOption
		{
			public:

				class Pool : public OptionsPool
				{
					public:

						static Pool * Instance()
						{
							static Pool instance;
							return &instance;
						}
				};

				ArgumentModeFlag(int index)
					: PoolOption(), index_(index), active_(false)
				{
					;;
				}

				virtual bool NeedValue() const
				{
					return false;
				}

				virtual OptionsPool * Set(const void * v)
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

#define POOL(NAME) \
	class NAME : public OptionsPool \
	{ \
		public: \
			static NAME * Instance() \
			{ \
				static NAME instance; \
				return &instance; \
			} \
	};

#define METHOD_DESC(LABEL1, LABEL2, FLAGS, DESC) \
	virtual const OptionDesc * Desc() const \
	{ \
		static OptionDesc desc = { LABEL1, LABEL2, DESC, FLAGS }; \
		return &desc; \
	}

#define ARGUMENT_FLAG(NAME, LABEL1, LABEL2, FLAGS, DESC) \
		protected: \
			class __Option_##NAME : public ProgramOptions<PoolType>::OptionFlag \
			{ \
				public: \
					__Option_##NAME() : ProgramOptions<PoolType>::OptionFlag() \
					{ \
						AddToPool(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, FLAGS, DESC) \
			}; \
			__Option_##NAME __##NAME; \
		public: \
			bool NAME() { return __##NAME.Exists(); }

#define ARGUMENT_STRING(NAME, LABEL1, LABEL2, FLAGS, DESC) \
		protected: \
			class __Option_##NAME : public ProgramOptions<PoolType>::OptionString \
			{ \
				public: \
					__Option_##NAME() : ProgramOptions<PoolType>::OptionString() \
					{ \
						AddToPool(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, FLAGS, DESC) \
			}; \
			__Option_##NAME __##NAME; \
		public: \
			const std::string & NAME() const { return *__##NAME; } \
			bool Has##NAME() { return __##NAME.Exists(); }

#define ARGUMENT_INTEGER(NAME, LABEL1, LABEL2, FLAGS, DESC) \
		protected: \
			class __Option_##NAME : public ProgramOptions<PoolType>::OptionInteger \
			{ \
				public: \
					__Option_##NAME() : ProgramOptions<PoolType>::OptionInteger() \
					{ \
						AddToPool(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, FLAGS, DESC) \
			}; \
			__Option_##NAME __##NAME; \
		public: \
			const long long & NAME() const { return *__##NAME; } \
			bool Has##NAME() { return __##NAME.Exists(); }

#define ARGUMENT_MODES_BEGIN(NAME, FLAGS, DESC) \
		protected: \
			class __ArgumentModes_##NAME : public ProgramOptions<PoolType>::ArgumentModes<__ArgumentModes_##NAME> \
			{ \
				public: \
					__ArgumentModes_##NAME() : ProgramOptions<PoolType>::ArgumentModes<__ArgumentModes_##NAME>() \
					{ \
						PoolOption::AddToPool(); \
					} \
					virtual const OptionDesc * Desc() const \
					{ \
						static OptionDesc desc = { 0, 0, DESC, FLAGS }; \
						return &desc; \
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
			class __ArgumentMode_##NAME : public ProgramOptions<PoolType>::ArgumentModeFlag<__ArgumentMode_##NAME> \
			{ \
				typedef ProgramOptions<PoolType>::ArgumentModeFlag<__ArgumentMode_##NAME> Super; \
				public: \
					__ArgumentMode_##NAME() : Super(ParentType::NextModeIndex()) \
					{ \
						PoolOption::AddToPool(); \
					} \
					virtual const OptionDesc * Desc() const \
					{ \
						static OptionDesc desc = { LABEL1, LABEL2, DESC, FLAGS }; \
						return &desc; \
					} \
					virtual OptionsPool * Set(const void * v) \
					{ \
						ParentType::Instance()->Select(Desc(), Index()); \
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

	template <class T> class Singleton
	{
		public:

			static T & Instance()
			{
				static T instance;
				return instance;
			}

		protected:

			Singleton() { ;; }

		private:

			Singleton(const Singleton &);
			Singleton & operator= (const Singleton &);
	};

	class ProgramArguments : public Singleton<ProgramArguments>
	{
		protected:

			POOL(PoolType)

		public:

			bool Parse(int argc, const char * argv[])
			{
				return PoolType::Instance()->Parse(argc, argv);
			}

		protected:

			ARGUMENT_STRING(Config, "c", "config", REQUIRED | PASSWORD, "Config file name")
			ARGUMENT_INTEGER(Power, "p", "power",  REQUIRED, "Power of calculation")
			ARGUMENT_INTEGER(Size,  "s", "size",   OPTIONAL, "Size of array")
/*
			ARGUMENT_MODES_BEGIN(Period, OPTIONAL, "Period of calculation")

				ARGUMENT_MODE_FLAG_BEGIN(Daily, "d", "daily", OPTIONAL, "Daily")
					ARGUMENT_STRING(Config, "c", "config", REQUIRED, "Daily config file name")
				ARGUMENT_MODE_FLAG_END(Daily)

				ARGUMENT_MODE_FLAG_BEGIN(Monthly, "m", "monthly", OPTIONAL, "Monthly")
					ARGUMENT_STRING(Config, "c", "config", REQUIRED, "Monthly config file name")
				ARGUMENT_MODE_FLAG_END(Monthly)

			ARGUMENT_MODES_END(Period)
*/
	};
}

#endif
