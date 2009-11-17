#ifndef _XTL__PROGRAM_OPTIONS_HPP__
#define _XTL__PROGRAM_OPTIONS_HPP__ 1

#include <string.h>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include "Singleton.hpp"

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

			enum Flags
			{
				OPTIONAL = 0x0000,
				REQUIRED = 0x0001,
				PASSWORD = 0x0002
			};

			Option(const unsigned long long & id)
				: id_(id) { ;; }

			virtual ~Option() { ;; }

			virtual bool IsMode() const            { return false; }

			virtual bool IsModeValue() const       { return false; }

			virtual OptionsPool * Subpool()        { return 0; }

			virtual OptionsPool * Set() = 0;

			const unsigned long long & Id() const  { return id_; }

			virtual const OptionDesc * Desc() const
			{
				throw ProgramOptionsError("Internal logic error: abstract method Desc()");
			}

		protected:

			friend class OptionsPool;

			unsigned long long id_;
	};

	struct OptionDesc
	{
		const char         * label1;
		const char         * label2;
		const char         * value2;
		const char         * desc;
		unsigned long long   flags;

		const std::string AsString() const;
	};

	class OptionValue
	{
		public:

			OptionValue() { ;; }

			virtual void Set(const char * s) = 0;

		protected:

			OptionValue(const OptionValue &);
			OptionValue & operator= (const OptionValue &);
	};

	template <typename T>
	class OptionValueScalar : public OptionValue
	{
		public:

			typedef T ValueType;

			ValueType & operator* ()              { return value_; }

			const ValueType & operator* () const  { return value_; }

		protected:

			ValueType value_;
	};

	class OptionValueString : public OptionValueScalar<std::string>
	{
		public:

			virtual void Set(const char * s);
	};

	class OptionValueInteger : public OptionValueScalar<long long int>
	{
		public:

			virtual void Set(const char * s);
	};

	class OptionsPool
	{
		public:

			struct OptionEntry
			{
				const OptionDesc * desc;
				Option           * option;
				OptionValue      * value;

				OptionEntry(const OptionDesc * desc_, Option * option_, OptionValue * value_)
					: desc(desc_), option(option_), value(value_) { ;; }
			};

			typedef std::list<OptionEntry> ListOfOptions;

			void AddOption(const OptionDesc * desc, Option * option, OptionValue * value);

			const unsigned long long NextId();

			void SetPresent(const unsigned long long & id);

			bool Contains(const unsigned long long & id);

			bool Parse(int argc, const char * argv[]);

			template <typename Iterator>
			void Iterate(Iterator iterator, int depth)
			{
				ListOfOptions::const_iterator end = options_.end();
				ListOfOptions::const_iterator modeItr = end;

				for (ListOfOptions::const_iterator itr = options_.begin();
				     itr != end;
				     ++itr)
				{
					if (itr->option->IsModeValue())
					{
						if (modeItr == end)
						{
							modeItr = itr;
						}
					}
					else
					{
						if (itr->option->IsMode())
						{
							iterator.OnMode(itr->desc, itr->option, depth);

							if (modeItr != end)
							{
								for (; modeItr != itr; ++modeItr)
								{
									// TODO: may be add to OnModeValue params of parent mode option?
									iterator.OnModeValue(modeItr->desc, modeItr->option, depth);
									OptionsPool * subpool = modeItr->option->Subpool();
									if (subpool != 0)
									{
										subpool->Iterate(iterator, depth + 1);
									}
								}
								modeItr = end;
							}
						}
						else
						{
							if (modeItr != end)
							{
								throw ProgramOptionsError("Internal error. Invalid order of modes.");
							}

							iterator.OnOption(itr->desc, itr->option, depth);
						}
					}
				}
			}

		protected:

			OptionsPool();

			OptionEntry * FindByLabel1(const char * label1);
			OptionEntry * FindByLabel2(const char * label2);

			void CheckRequiredOptions();

			unsigned long long nextId_;
			unsigned long long set_;
			ListOfOptions      options_;
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

	template <typename _PoolType>
	struct ProgramOptions
	{
		class OptionFlag : public Option
		{
			public:

				typedef _PoolType PoolType;

				static PoolType & Pool()
				{
					return *(PoolType::Instance());
				}

				OptionFlag() : Option(Pool().NextId()) { ;; }

				void AddToPool(OptionValue * value)
				{
					Pool().AddOption(Desc(), this, value);
				}

				bool Exists() const
				{
					return Pool().Contains(id_);
				}

				void SetPresent()
				{
					Pool().SetPresent(id_);
				}

				virtual OptionsPool * Set()
				{
					if (Exists())
					{
						throw ProgramOptionsError(std::string("Duplicate program option: '") + this->Desc()->AsString() + "'");
					}
					SetPresent();
					return 0;
				}
		};

		template <typename OptionValueType>
		class OptionScalar : public OptionFlag
		{
			public:

				typedef typename OptionValueType::ValueType ValueType;

				OptionScalar() : OptionFlag(), optionValue_() { ;; }

				ValueType & operator* ()              { return *optionValue_; }

				const ValueType & operator* () const  { return *optionValue_; }

			protected:

				OptionValueType * Value()             { return &optionValue_; }

				OptionValueType optionValue_;
		};

		template <typename DeriveClass>
		class OptionModes : public OptionFlag
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
							return OptionModes::Instance();
						}
				};

				class Counter : public Singleton<Counter>
				{
					public:

						Counter() : Singleton<Counter>(), count_(0) { ;; }

						static int NextIndex()
						{
							return Instance().count_++;
						}

					protected:

						int count_;
				};

				static int NextIndex()                   { return Counter::NextIndex(); }

				virtual bool IsMode() const              { return true; }

				virtual OptionsPool * Set()              { return 0; }

				const OptionDesc * Selected() const      { return selectedDesc_; }

				const int SelectedIndex() const          { return selectedIndex_; }

				void Select(const OptionDesc * desc, int index)
				{
					if (OptionFlag::Exists())
					{
						throw ProgramOptionsError(
							std::string("Ambiguous program mode ('") +
							selectedDesc_->AsString() +
							"' and '" +
							desc->AsString() +
							"' specified)"
						);
					}

					OptionFlag::SetPresent();

					selectedDesc_ = desc;
					selectedIndex_ = index;
				}

			protected:

				OptionModes()
					: OptionFlag(),
					  selectedDesc_(0),
					  selectedIndex_(-1)
				{
					;;
				}

				const OptionDesc * selectedDesc_;
				int selectedIndex_;
		};

		class OptionModeFlag : public OptionFlag
		{
			public:

				OptionModeFlag(int index)
					: OptionFlag(), index_(index), active_(false)
				{
					;;
				}

				virtual bool IsModeValue() const  { return true; }

				virtual OptionsPool * Set()
				{
					active_ = true;
					return 0;
				}

				int Index() const    { return index_; }

				bool Active() const  { return active_; }

			protected:

				int  index_;
				bool active_;
		};

		template <typename OptionValueType>
		class OptionModeScalar : public OptionModeFlag
		{
			public:

				typedef typename OptionValueType::ValueType ValueType;

				OptionModeScalar(int index) : OptionModeFlag(index), optionValue_() { ;; }

				ValueType & operator* ()              { return *optionValue_; }

				const ValueType & operator* () const  { return *optionValue_; }

			protected:

				OptionValueType * Value()             { return &optionValue_; }

				OptionValueType optionValue_;
		};

		template <typename DeriveClass>
		class OptionModeFlagWithPool : public OptionModeFlag
		{
			public:

				POOL(Pool)

				OptionModeFlagWithPool(int index) : OptionModeFlag(index) { ;; }

				virtual OptionsPool * Subpool() { return Pool::Instance(); }

			protected:

				class ArgumentsBase
				{
					typedef Pool PoolType;
				};
		};

		template <typename DeriveClass, typename OptionValueType>
		class OptionModeScalarWithPool : public OptionModeScalar<OptionValueType>
		{
			public:

				POOL(Pool)

				OptionModeScalarWithPool(int index) : OptionModeScalar<OptionValueType>(index) { ;; }

				virtual OptionsPool * Subpool() { return Pool::Instance(); }

			protected:

				class ArgumentsBase
				{
					typedef Pool PoolType;
				};
		};
	};


#define METHOD_DESC(LABEL1, LABEL2, VALUE2, FLAGS, DESC) \
	virtual const OptionDesc * Desc() const \
	{ \
		static OptionDesc desc = { LABEL1, LABEL2, VALUE2, DESC, FLAGS }; \
		return &desc; \
	}


#define ARGUMENT_FLAG(ID, LABEL1, LABEL2, DESC) \
		protected: \
			class __Option_##ID : public ProgramOptions<PoolType>::OptionFlag \
			{ \
				public: \
					__Option_##ID() : ProgramOptions<PoolType>::OptionFlag() \
					{ \
						AddToPool(0); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, DESC) \
			}; \
			__Option_##ID __##ID; \
		public: \
			bool ID() { return __##ID.Exists(); }

#define ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, DESC, VALUE_CLASS) \
		protected: \
			class __Option_##ID : public ProgramOptions<PoolType>::OptionScalar<VALUE_CLASS> \
			{ \
				public: \
					__Option_##ID() : ProgramOptions<PoolType>::OptionScalar<VALUE_CLASS>() \
					{ \
						AddToPool(Value()); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, FLAGS, DESC) \
			}; \
			__Option_##ID __##ID; \
		public: \
			const VALUE_CLASS::ValueType & ID() const { return *__##ID; } \
			bool Has##ID() { return __##ID.Exists(); }

#define ARGUMENT_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, DESC) \
	ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, DESC, OptionValueString)

#define ARGUMENT_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, DESC) \
	ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, DESC, OptionValueInteger)


#define ARGUMENT_MODES_BEGIN(ID, FLAGS, DESC) \
		protected: \
			class __OptionModes_##ID : public ProgramOptions<PoolType>::OptionModes<__OptionModes_##ID> \
			{ \
				public: \
					__OptionModes_##ID() : ProgramOptions<PoolType>::OptionModes<__OptionModes_##ID>() \
					{ \
						AddToPool(0); \
					} \
					METHOD_DESC(0, 0, 0, FLAGS, DESC) \
					typedef __OptionModes_##ID ParentType; \

#define ARGUMENT_MODES_END(ID) \
			}; \
		protected: \
			__OptionModes_##ID::Sentinel __##ID; \
		public: \
			__OptionModes_##ID & ID() \
			{ \
				return *(__OptionModes_##ID::Instance()); \
			}


#define ARGUMENT_MODE_FLAG(ID, LABEL1, LABEL2, DESC) \
		protected: \
			class __OptionMode_##ID : public ProgramOptions<PoolType>::OptionModeFlag \
			{ \
				typedef ProgramOptions<PoolType>::OptionModeFlag Super; \
				public: \
					__OptionMode_##ID() : Super(ParentType::NextIndex()) \
					{ \
						AddToPool(0); \
					} \
					virtual OptionsPool * Set() \
					{ \
						ParentType::Instance()->Select(Desc(), Index()); \
						return Super::Set(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, 0, DESC) \
			}; \
		public: \
			__OptionMode_##ID __##ID; \
			bool Is##ID() const { return __##ID.Active(); }

#define ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, DESC, VALUE_CLASS) \
		protected: \
			class __OptionMode_##ID : public ProgramOptions<PoolType>::OptionModeScalar<VALUE_CLASS> \
			{ \
				typedef ProgramOptions<PoolType>::OptionModeScalar<VALUE_CLASS> Super; \
				public: \
					_OptionMode_##ID() : Super(ParentType::NextIndex()) \
					{ \
						AddToPool(Value()); \
					} \
					virtual OptionsPool * Set() \
					{ \
						ParentType::Instance()->Select(Desc(), Index()); \
						return Super::Set(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, 0, DESC) \
			}; \
		public: \
			__OptionMode_##ID __##ID; \
			bool Is##ID() const {return __##ID.Active(); } \
			const VALUE_CLASS::ValueType & ID() const { return *__##ID; }

#define ARGUMENT_MODE_STRING(ID, LABEL1, LABEL2, VALUE2, DESC) \
	ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, DESC, OptionValueString)

#define ARGUMENT_MODE_INTEGER(ID, LABEL1, LABEL2, VALUE2, DESC) \
	ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, DESC, OptionValueInteger)


#define ARGUMENT_MODE_FLAG_BEGIN(ID, LABEL1, LABEL2, DESC) \
		protected: \
			class __OptionMode_##ID : public ProgramOptions<PoolType>::OptionModeFlagWithPool<__OptionMode_##ID> \
			{ \
				typedef ProgramOptions<PoolType>::OptionModeFlagWithPool<__OptionMode_##ID> Super; \
				public: \
					__OptionMode_##ID() : Super(ParentType::NextIndex()) \
					{ \
						AddToPool(0); \
					} \
					virtual OptionsPool * Subpool() \
					{ \
						if (args_.get() == 0) { args_.reset(new Arguments()); } \
						return Super::Subpool(); \
					} \
					virtual OptionsPool * Set() \
					{ \
						ParentType::Instance()->Select(Desc(), Index()); \
						Super::Set(); \
						return Subpool(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, 0, DESC) \
				protected: \
					class Arguments : public ArgumentsBase \
					{

#define ARGUMENT_MODE_FLAG_END(ID) \
					}; \
					std::auto_ptr<Arguments> args_; \
			}; \
		public: \
			__OptionMode_##ID __##ID; \
			bool Is##ID() const { return __##ID.Active(); }

#define ARGUMENT_MODE_SCALAR_BEGIN(ID, LABEL1, LABEL2, VALUE2, DESC, VALUE_CLASS) \
		protected: \
			class __OptionMode_##ID : public ProgramOptions<PoolType>::OptionModeScalarWithPool<__OptionMode_##ID, VALUE_CLASS> \
			{ \
				typedef ProgramOptions<PoolType>::OptionModeScalarWithPool<__OptionMode_##ID, VALUE_CLASS> Super; \
				public: \
					__OptionMode_##ID() : Super(ParentType::NextIndex()) \
					{ \
						AddToPool(Value()); \
					} \
					virtual OptionsPool * Subpool() \
					{ \
						if (args_.get() == 0) { args_.reset(new Arguments()); } \
						return Super::Subpool(); \
					} \
					virtual OptionsPool * Set() \
					{ \
						ParentType::Instance()->Select(Desc(), Index()); \
						Super::Set(); \
						return Subpool(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, 0, DESC) \
				protected: \
					class Arguments : public ArgumentsBase \
					{

#define ARGUMENT_MODE_SCALAR_END(ID) \
					}; \
					std::auto_ptr<Arguments> args_; \
			}; \
		public: \
			__OptionMode_##ID __##ID; \
			bool Is##ID() const { return __##ID.Active(); } \
			const __OptionMode_##ID::ValueType & ID() const { return *__##ID; }

#define ARGUMENT_MODE_STRING_BEGIN(ID, LABEL1, LABEL2, VALUE2, DESC) \
	ARGUMENT_MODE_SCALAR_BEGIN(ID, LABEL1, LABEL2, VALUE2, DESC, OptionValueString)

#define ARGUMENT_MODE_STRING_END(ID) \
	ARGUMENT_MODE_SCALAR_END(ID)

#define ARGUMENT_MODE_INTEGER_BEGIN(ID, LABEL1, LABEL2, VALUE2, DESC) \
	ARGUMENT_MODE_INTEGER_BEGIN(ID, LABEL1, LABEL2, VALUE2, DESC, OptionValueInteger)

#define ARGUMENT_MODE_INTEGER_END(ID) \
	ARGUMENT_MODE_INTEGER_END(ID)


	class ProgramArguments : public Singleton<ProgramArguments>
	{
		protected:

			POOL(PoolType)

		public:

			bool Parse(int argc, const char * argv[])
			{
				return PoolType::Instance()->Parse(argc, argv);
			}

			template <typename Iterator>
			static void Iterate(Iterator iterator)
			{
				PoolType::Instance()->Iterate(iterator, 0);
			}

		protected:

			ARGUMENT_STRING  (Config, "c", "config", "FILE", REQUIRED | PASSWORD, "Config file name")
			ARGUMENT_FLAG    (Turbo,  "t", "turbo", "Turbo calculation")
			ARGUMENT_INTEGER (Power,  "p", "power", "#", REQUIRED, "Power of calculation")
			ARGUMENT_INTEGER (Size,   "s", "size", "#",  OPTIONAL, "Size of array")

			ARGUMENT_MODES_BEGIN(Period, REQUIRED, "Period of calculation")
				ARGUMENT_MODE_FLAG(Daily, "d", "daily", "Daily")
//				ARGUMENT_MODE_FLAG(Monthly, "m", "monthly", PASSWORD, "Monthly")
			ARGUMENT_MODES_END(Period)

			ARGUMENT_MODES_BEGIN(Color, OPTIONAL, "Colour of the skin")
				ARGUMENT_MODE_STRING_BEGIN(BlackWhite, "bw", "black-and-white", "RGB", "Black and white schema")
					ARGUMENT_FLAG(Black, "b", "black", "Black color")
					ARGUMENT_FLAG(White, "w", "white", "White color")
//					ARGUMENT_MODES_BEGIN(Gray, OPTIONAL, "Gray color")
//						ARGUMENT_MODE_FLAG(LightGray, "lg", "lightgray", OPTIONAL, "Light gray colour")
//						ARGUMENT_MODE_FLAG(DarkGray, "dg", "darkgray", OPTIONAL, "Dark gray colour")
//					ARGUMENT_MODES_END(Gray)
				ARGUMENT_MODE_STRING_END(BlackWhite)
//				ARGUMENT_MODE_FLAG(Black, "b", "black", OPTIONAL, "Black color")
			ARGUMENT_MODES_END(Color)
			ARGUMENT_STRING(Log, 0, "log", "FILE", OPTIONAL, "Path to log file")
	};
}

#endif
