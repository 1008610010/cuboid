#ifndef _XTL__PROGRAM_OPTIONS_HPP__
#define _XTL__PROGRAM_OPTIONS_HPP__ 1

#include "Singleton.hpp"
#include <string> // std::string
#include <list>   // std::list
#include <memory> // std::auto_ptr

/**
 * @namespace XTL Extended Template Library
 */
namespace XTL
{
	/**
	 * @class ProgramOptionsError
	 * @brief Класс - исключение при ошибках парсинга аргументов.
	 *
	 * Выбрасывается при обнаружении ошибок парсинга аргументов.
	 * Например, при повторном появлении одного и того же аргумента, или
	 * при неверном задании значения аргумента.
	 */
	class ProgramOptionsError
	{
		public:

			/**
			 * @brief Конструктор.
			 * @param what Текстовое представление детализирующее ошибку.
			 */
			ProgramOptionsError(const std::string & what)
				: what_(what) { ;; }

			/**
			 * @brief Получить строку, детализирующую возникшую ошибку.
			 * @return Строка, детализирующая возникшую ошибку.
			 */
			const char * What() const
			{
				return what_.c_str();
			}

		protected:

			const std::string what_; ///< Текстовое представление ошибки.
	};

	/**
	 * @class ProgramOptionsExit
	 * @brief Класс - исключение, означающее останов выполнения программы
	 * (не аварийное завершение) в процессе разбора агументов.
	 *
	 * Выбрасывается в момент, когда необходимо завершить выполнение программы
	 * в процессе парсинга аргументов. Необходимо использовать в
	 * аргументах-триггерах, по которым происходят некоторые действия
	 * с последующем остановом (к примеру вывод справки).
	 * В конструктор исключения передается код завершения программы, который
	 * необходимо вернуть операционной системе при выходе.
	 */
	class ProgramOptionsExit
	{
		public:

			/**
			 * @brief Конструктор.
			 * @param code Код завершения программы.
			 */
			ProgramOptionsExit(int code)
				: code_(code) { ;; }

			/**
			 * @brief Получить код завершения программы.
			 * @return Код завершения программы.
			 */
			int Code() const
			{
				return code_;
			}

		protected:

			int code_; /// Код завершения программы.
	};

	class OptionsPool;

	/**
	 * @struct OptionDesc
	 * @brief Описатель аргумента программы
	 */
	struct OptionDesc
	{
		const char         * label1; ///< Метка -label1 [value1].
		const char         * label2; ///< Метка --label2[=value2].
		const char         * value2; ///< Шаблон значания для label2.
		const char         * text;   ///< Текстовое описание аргумента.
		unsigned long long   flags;  ///< Флаги аргумента.

		/**
		 * @brief Получить текстовое представление аргумента.
		 * @return Текстовое представление аргумента.
		 */
		const std::string AsString() const;

		/**
		 * @brief Проверить является ли аргумент обязательным.
		 * @retval true Задание этого аргумента обязательно.
		 * @retval false В противном случае.
		 */
		bool Required() const;
	};

	class Option
	{
		public:

			enum Flags
			{
				OPTIONAL = 0x0000,
				REQUIRED = 0x0001,
				PASSWORD = 0x0002
			};

			enum PoolFlags
			{
				SKIP_CHECK    = 0x0001,
				MODE_SELECTOR = 0x0002,
				MODE_VALUE    = 0x0004
			};

			Option()
				: exists_(false)
			{
				;;
			}

			virtual ~Option()
			{
				;;
			}

			bool Exists() const
			{
				return exists_;
			}

			virtual OptionsPool * Set()
			{
				if (exists_)
				{
					throw ProgramOptionsError(std::string("Duplicate program option: '") + this->Desc()->AsString() + "'");
				}
				exists_ = true;
				return 0;
			}

			virtual OptionsPool * SubPool()
			{
				return 0;
			}

			virtual const OptionDesc * Desc() const
			{
				throw ProgramOptionsError("Internal logic error: abstract method Desc()");
			}

		protected:

			bool exists_;
	};

	class OptionValue
	{
		public:

			OptionValue() { ;; }

			virtual bool NeedValue() const = 0;

			virtual void Set(const OptionDesc * desc, const char * s) = 0;

		protected:

			OptionValue(const OptionValue &);
			OptionValue & operator= (const OptionValue &);
	};

	// TODO: add value handler (in Set() method)
	template <typename T>
	class OptionValueScalar : public OptionValue
	{
		public:

			typedef T ValueType;

			virtual bool NeedValue() const
			{
				return true;
			}

			ValueType & operator* ()
			{
				return value_;
			}

			const ValueType & operator* () const
			{
				return value_;
			}

		protected:

			ValueType value_;
	};

	class OptionValueString : public OptionValueScalar<std::string>
	{
		public:

			virtual void Set(const OptionDesc * desc, const char * s);
	};

	class OptionValueInteger : public OptionValueScalar<long long int>
	{
		public:

			virtual void Set(const OptionDesc * desc, const char * s);
	};

	template <typename _Handler>
	class OptionValueHandler : public OptionValue
	{
		public:

			OptionValueHandler(_Handler handler, bool needValue)
				: handler_(handler) { ;; }

			virtual bool NeedValue() const 
			{
				return needValue_;
			}
			
			virtual void Set(const OptionDesc * desc, const char * s)
			{
				handler_(desc, s);
			}

		protected:

			_Handler handler_;
			bool     needValue_;
	};

	typedef void (*OptionHandler)(const OptionDesc * desc, const char * s);

	typedef OptionValueHandler<OptionHandler> OptionValueTrigger;

	class OptionsPool
	{
		public:

			struct OptionEntry
			{
				const OptionDesc * desc;
				Option           * option;
				OptionValue      * value;
				unsigned int       flags;

				OptionEntry(const OptionDesc * desc_, Option * option_, OptionValue * value_, unsigned int flags_)
					: desc(desc_), option(option_), value(value_), flags(flags_) { ;; }

				bool SkipCheck() const
				{
					return (flags & Option::SKIP_CHECK) != 0;
				}

				bool IsModeSelector() const
				{
					return (flags & Option::MODE_SELECTOR) != 0;
				}

				bool IsModeValue() const
				{
					return (flags & Option::MODE_VALUE) != 0;
				}
			};

			typedef std::list<OptionEntry> ListOfOptions;

			void RegisterOption(const OptionDesc * desc, Option * option, OptionValue * value, unsigned int flags);

			void Parse(int argc, const char * argv[], std::string & programName);

			template <typename Iterator>
			void Iterate(Iterator iterator, int depth)
			{
				ListOfOptions::const_iterator end = options_.end();
				ListOfOptions::const_iterator modeItr = end;

				for (ListOfOptions::const_iterator itr = options_.begin();
				     itr != end;
				     ++itr)
				{
//					fprintf(stderr, "[%s %d %d]\n", itr->desc->AsString().c_str(), itr->flags, itr->SkipCheck());
					if (itr->SkipCheck())
					{
						continue;
					}
					else if (itr->IsModeValue())
					{
						if (modeItr == end)
						{
							modeItr = itr;
						}
					}
					else if (itr->IsModeSelector())
					{
						iterator.OnModeSelector(itr->desc, itr->option, depth);

						if (modeItr != end)
						{
							for (; modeItr != itr; ++modeItr)
							{
								// TODO: may be add to OnModeValue params of parent mode option?
								if (modeItr->SkipCheck())
								{
									continue;
								}
								iterator.OnModeValue(modeItr->desc, modeItr->option, depth);
								OptionsPool * subpool = modeItr->option->SubPool();
								if (subpool != 0)
								{
									subpool->Iterate(iterator, depth + 1);
								}
							}
							modeItr = end;
						}
					}
					else if (modeItr != end)
					{
						throw ProgramOptionsError("Internal error. Invalid order of modes.");
					}
					else
					{
						iterator.OnOption(itr->desc, itr->option, depth);
					}
				}
			}

		protected:

			OptionsPool();

			OptionEntry * FindByLabel1(const char * label1);

			OptionEntry * FindByLabel2(const char * label2);

			void CheckRequiredOptions();

			ListOfOptions options_;
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

	template <typename _Pool>
	struct ProgramOptions
	{
		class OptionFlag : public Option
		{
			public:

				void Register(OptionValue * value, unsigned int flags)
				{
					_Pool::Instance()->RegisterOption(Desc(), this, value, flags);
				}
		};

		template <typename _DeriveClass>
		class OptionSingleton : public OptionFlag
		{
			public:

				static _DeriveClass * Instance()
				{
					static _DeriveClass instance;
					return &instance;
				}

				class Sentinel
				{
					public:

						Sentinel()
						{
							Instance();
						}

						static _DeriveClass * Instance()
						{
							return _DeriveClass::Instance();
						}
				};
		};

		template <typename _OptionValue>
		class OptionScalar : public OptionFlag
		{
			public:

				typedef typename _OptionValue::ValueType ValueType;

				OptionScalar()
					: OptionFlag(), optionValue_() { ;; }

				ValueType & operator* ()
				{
					return *optionValue_;
				}

				const ValueType & operator* () const
				{
					return *optionValue_;
				}

			protected:

				_OptionValue * Value()
				{
					return &optionValue_;
				}

				_OptionValue optionValue_;
		};

		class OptionTrigger : public OptionFlag
		{
			public:

				OptionTrigger(OptionHandler handler, bool needValue)
					: optionValue_(handler, needValue) { ;; }

			protected:

				OptionValueTrigger * Value()
				{
					return &optionValue_;
				}

				OptionValueTrigger optionValue_;
		};

		template <typename _DeriveClass>
		class OptionModeSelector : public OptionSingleton<_DeriveClass>
		{
			public:

				typedef _DeriveClass OptionModeSelector_;

				class Counter : public Singleton<Counter>
				{
					public:

						Counter()
							: Singleton<Counter>(), count_(0) { ;; }

						static int NextIndex()
						{
							return Counter::Instance().count_++;
						}

					protected:

						int count_;
				};

				virtual OptionsPool * Set()
				{
					throw ProgramOptionsError("Internal logic error: OptionModeSelector::Set()");
				}

				void Select(int index, const OptionDesc * desc)
				{
					if (this->Exists())
					{
						throw ProgramOptionsError(
							std::string("Ambiguous program mode ('") +
							selectedDesc_->AsString() +
							"' and '" +
							desc->AsString() +
							"' specified)"
						);
					}

					selectedIndex_ = index;
					selectedDesc_ = desc;

					Option::Set();
				}

				static int NextIndex()
				{
					return Counter::NextIndex();
				}

				const OptionDesc * Selected() const
				{
					return selectedDesc_;
				}

				const int SelectedIndex() const
				{
					return selectedIndex_;
				}

			protected:

				OptionModeSelector()
					: OptionSingleton<_DeriveClass>(),
					  selectedIndex_(-1),
					  selectedDesc_(0)
				{
					;;
				}

				int selectedIndex_;
				const OptionDesc * selectedDesc_;
		};

		template <typename _OptionModeSelector>
		class OptionModeFlag : public OptionFlag
		{
			public:

				OptionModeFlag()
					: OptionFlag(),
					  index_(_OptionModeSelector::NextIndex())
				{
					;;
				}

				virtual OptionsPool * Set()
				{
					Option::Set();
					_OptionModeSelector::Instance()->Select(index_, this->Desc());
					return this->SubPool();
				}

				int Index() const
				{
					return index_;
				}

			protected:

				int  index_;
		};

		template <typename _OptionModeSelector,
		          typename _OptionValue>
		class OptionModeScalar : public OptionModeFlag<_OptionModeSelector>
		{
			public:

				typedef typename _OptionValue::ValueType ValueType;

				ValueType & operator* ()
				{
					return *optionValue_;
				}

				const ValueType & operator* () const
				{
					return *optionValue_;
				}

			protected:

				_OptionValue * Value()
				{
					return &optionValue_;
				}

				_OptionValue optionValue_;
		};

		template <typename _DeriveClass,
		          typename _OptionModeSelector>
		class OptionModeFlagPool : public OptionModeFlag<_OptionModeSelector>
		{
			protected:

				POOL(SubPool_)

			public:

				virtual OptionsPool * SubPool()
				{
					return SubPool_::Instance();
				}

			protected:

				class OptionsBase
				{
					typedef SubPool_ Pool;
				};
		};

		template <typename _DeriveClass,
		          typename _OptionModeSelector,
		          typename _OptionValue>
		class OptionModeScalarPool : public OptionModeScalar<_OptionModeSelector, _OptionValue>
		{
			protected:

				POOL(SubPool_)

			public:

				virtual OptionsPool * SubPool()
				{
					return SubPool_::Instance();
				}

			protected:

				class OptionsBase
				{
					typedef SubPool_ Pool;
				};
		};

		template <typename _DeriveClass>
		class OptionGroup : public OptionSingleton<_DeriveClass>
		{
			protected:

				POOL(SubPool_)

			public:

				static void RegisterOption(Option * option, OptionValue * value)
				{
					_Pool::Instance()->RegisterOption(
						option->Desc(),
						option,
						value,
						Option::SKIP_CHECK
					);
				}

				virtual OptionsPool * SubPool()
				{
					return SubPool_::Instance();
				}

				virtual OptionsPool * Set()
				{
					throw ProgramOptionsError("Internal logic error: OptionGroup::Set()");
				}

				OptionsPool * Select(const OptionDesc * desc)
				{
					if (this->Exists())
					{
						return 0;
					}

					Option::Set();
					return SubPool_::Instance();
				}

			protected:

				typedef _DeriveClass OptionGroup_;

				OptionGroup()
					: OptionSingleton<_DeriveClass>() { ;; }
		};

		template <typename _DeriveClass,
		          typename _OptionModeSelector>
		class OptionModeGroup : public OptionGroup<_DeriveClass>
		{
			typedef OptionGroup<_DeriveClass> Super;

			public:

				OptionsPool * Select(const OptionDesc * desc)
				{
					if (this->Exists())
					{
						return 0;
					}

					Option::Set();
					_OptionModeSelector::Instance()->Select(index_, this->Desc());
					return Super::SubPool_::Instance();
				}

				int Index() const
				{
					return index_;
				}

			protected:

				OptionModeGroup()
					: OptionGroup<_DeriveClass>(),
					  index_(_OptionModeSelector::NextIndex())
				{
					;;
				}

			protected:

				int index_;
		};

		template <typename _OptionGroup>
		class OptionGroupFlag : public OptionFlag
		{
			public:

				virtual OptionsPool * Set()
				{
					Option::Set();
					return _OptionGroup::Instance()->Select(this->Desc());
				}

				void Register()
				{
					OptionFlag::Register(0, 0);
					_OptionGroup::RegisterOption(this, 0);
				}
		};

		template <typename _OptionGroup,
		          typename _OptionValue>
		class OptionGroupScalar : public OptionScalar<_OptionValue>
		{
			typedef OptionScalar<_OptionValue> Super;

			public:

				virtual OptionsPool * Set()
				{
					Option::Set();
					return _OptionGroup::Instance()->Select(this->Desc());
				}

				void Register()
				{
					Super::Register(this->Value(), 0);
					_OptionGroup::RegisterOption(this, this->Value());
				}
		};
	};
}


#define METHOD_DESC(LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	virtual const OptionDesc * Desc() const \
	{ \
		static OptionDesc desc = { LABEL1, LABEL2, VALUE2, TEXT, FLAGS }; \
		return &desc; \
	}


#define ARGUMENT_FLAG(ID, LABEL1, LABEL2, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionFlag \
			{ \
				typedef ProgramOptions<Pool>::OptionFlag Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, 0); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, TEXT) \
			}; \
			__Option_##ID __##ID; \
		public: \
			bool Is##ID() { return __##ID.Exists(); }


#define ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, VALUE_CLASS) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionScalar<VALUE_CLASS> \
			{ \
				typedef ProgramOptions<Pool>::OptionScalar<VALUE_CLASS> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(Value(), 0); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
			}; \
			__Option_##ID __##ID; \
		public: \
			bool Has##ID() { return __##ID.Exists(); } \
			const VALUE_CLASS::ValueType & Get##ID() const { return *__##ID; } \

#define ARGUMENT_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueString)

#define ARGUMENT_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueInteger)


#define ARGUMENT_TRIGGER(ID, LABEL1, LABEL2, TEXT, FUNC) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionTrigger \
			{ \
				typedef ProgramOptions<Pool>::OptionTrigger Super; \
				public: \
					__Option_##ID() : Super(FUNC, false) \
					{ \
						Register(Value(), 0); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, TEXT) \
			}; \
			__Option_##ID __##ID;


#define ARGUMENT_MODES_BEGIN(ID, FLAGS, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeSelector<__Option_##ID> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeSelector<__Option_##ID> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, MODE_SELECTOR); \
					} \
					METHOD_DESC(0, 0, 0, FLAGS, TEXT)

#define ARGUMENT_MODES_END(ID) \
			}; \
		protected: \
			__Option_##ID::Sentinel __##ID; \
		public: \
			__Option_##ID & ID() { return *(__Option_##ID::Instance()); }


#define ARGUMENT_MODE_FLAG(ID, LABEL1, LABEL2, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeFlag<OptionModeSelector_> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeFlag<OptionModeSelector_> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, MODE_VALUE); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, TEXT) \
			}; \
		public: \
			__Option_##ID __##ID; \
			bool Is##ID() const { return __##ID.Exists(); }


#define ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, VALUE_CLASS) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeScalar<OptionModeSelector_, VALUE_CLASS> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeScalar<OptionModeSelector_, VALUE_CLASS> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(Value(), MODE_VALUE); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, (FLAGS) & ~REQUIRED, TEXT) \
			}; \
		public: \
			__Option_##ID __##ID; \
			bool Has##ID() const { return __##ID.Exists(); } \
			const VALUE_CLASS::ValueType & Get##ID() const { return *__##ID; }

#define ARGUMENT_MODE_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueString)

#define ARGUMENT_MODE_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_MODE_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueInteger)


#define ARGUMENT_MODE_FLAG_BEGIN(ID, LABEL1, LABEL2, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeFlagPool<__Option_##ID, OptionModeSelector_> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeFlagPool<__Option_##ID, OptionModeSelector_> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, MODE_VALUE); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, TEXT) \
					virtual OptionsPool * SubPool() \
					{ \
						if (options_.get() == 0) { options_.reset(new Options()); } \
						return Super::SubPool(); \
					} \
					class Options : public OptionsBase \
					{

#define ARGUMENT_MODE_FLAG_END(ID) \
					}; \
					const Options & GetOptions() const  { return *options_; } \
				protected: \
					std::auto_ptr<Options> options_; \
			}; \
		public: \
			__Option_##ID __##ID; \
			bool Is##ID() const { return __##ID.Exists(); } \
			const __Option_##ID::Options & ID() const { return __##ID.GetOptions(); }


#define ARGUMENT_MODE_SCALAR_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, VALUE_CLASS) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeScalarPool<__Option_##ID, OptionModeSelector_, VALUE_CLASS> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeScalarPool<__Option_##ID, OptionModeSelector_, VALUE_CLASS> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(Value(), MODE_VALUE); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, (FLAGS) & ~REQUIRED, TEXT) \
					virtual OptionsPool * SubPool() \
					{ \
						if (options_.get() == 0) { options_.reset(new Options()); } \
						return Super::SubPool(); \
					} \
					class Options : public OptionsBase \
					{

#define ARGUMENT_MODE_SCALAR_END(ID) \
					}; \
					const Options & GetOptions() const  { return *options_; } \
				protected: \
					std::auto_ptr<Options> options_; \
			}; \
		public: \
			__Option_##ID __##ID; \
			bool Has##ID() const { return __##ID.Exists(); } \
			const __Option_##ID::ValueType & Get##ID() const { return *__##ID; } \
			const __Option_##ID::Options & ID() const { return __##ID.GetOptions(); }


#define ARGUMENT_MODE_STRING_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_MODE_SCALAR_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueString)

#define ARGUMENT_MODE_STRING_END(ID) \
	ARGUMENT_MODE_SCALAR_END(ID)

#define ARGUMENT_MODE_INTEGER_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_MODE_SCALAR_BEGIN(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueInteger)

#define ARGUMENT_MODE_INTEGER_END(ID) \
	ARGUMENT_MODE_SCALAR_END(ID)


#define ARGUMENT_GROUP_BEGIN(ID, FLAGS, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionGroup<__Option_##ID> \
			{ \
				typedef ProgramOptions<Pool>::OptionGroup<__Option_##ID> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, 0); \
					} \
					METHOD_DESC(0, 0, 0, FLAGS, TEXT)

#define ARGUMENT_GROUP_END(ID) \
			}; \
		protected: \
			__Option_##ID::Sentinel __##ID; \
		public: \
			bool Is##ID() const { return __##ID.Instance()->Exists(); } \
			const __Option_##ID & ID() const { return *(__##ID.Instance()); }


#define ARGUMENT_GROUP_FLAG(ID, LABEL1, LABEL2, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<SubPool_>::OptionGroupFlag<OptionGroup_> \
			{ \
				typedef ProgramOptions<SubPool_>::OptionGroupFlag<OptionGroup_> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, 0, OPTIONAL, TEXT) \
			}; \
			__Option_##ID __##ID; \
		public: \
			bool Is##ID() const { return __##ID.Exists(); }


#define ARGUMENT_GROUP_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, VALUE_CLASS) \
		protected: \
			class __Option_##ID : public ProgramOptions<SubPool_>::OptionGroupScalar<OptionGroup_, VALUE_CLASS> \
			{ \
				typedef ProgramOptions<SubPool_>::OptionGroupScalar<OptionGroup_, VALUE_CLASS> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(); \
					} \
					METHOD_DESC(LABEL1, LABEL2, VALUE2, (FLAGS) | REQUIRED, TEXT) \
			}; \
			__Option_##ID __##ID; \
		public: \
			bool Has##ID() const { return __##ID.Exists(); } \
			const VALUE_CLASS::ValueType & Get##ID() const { return *__##ID; }

#define ARGUMENT_GROUP_STRING(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_GROUP_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueString)

#define ARGUMENT_GROUP_INTEGER(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT) \
	ARGUMENT_GROUP_SCALAR(ID, LABEL1, LABEL2, VALUE2, FLAGS, TEXT, OptionValueInteger)


#define ARGUMENT_MODE_GROUP_BEGIN(ID, TEXT) \
		protected: \
			class __Option_##ID : public ProgramOptions<Pool>::OptionModeGroup<__Option_##ID, OptionModeSelector_> \
			{ \
				typedef ProgramOptions<Pool>::OptionModeGroup<__Option_##ID, OptionModeSelector_> Super; \
				public: \
					__Option_##ID() : Super() \
					{ \
						Register(0, MODE_VALUE); \
					} \
					METHOD_DESC(0, 0, 0, OPTIONAL, TEXT)

#define ARGUMENT_MODE_GROUP_END(ID) \
			}; \
		protected: \
			__Option_##ID::Sentinel __##ID; \
		public: \
			bool Is##ID() const { return __##ID.Instance()->Exists(); } \
			const __Option_##ID & ID() const { return *(__##ID.Instance()); }


#define ARGUMENTS_BEGIN \
	namespace XTL \
	{ \
		class ProgramArguments : public Singleton<ProgramArguments> \
		{ \
			POOL(Pool) \
			protected: \
				std::string programName_; \
			public: \
				void Parse(int argc, const char * argv[]) \
				{ \
					Pool::Instance()->Parse(argc, argv, programName_); \
				} \
				const std::string ProgramName() const  { return programName_; } \
				template <typename Iterator> \
				static void Iterate(Iterator iterator) \
				{ \
					Pool::Instance()->Iterate(iterator, 0); \
				}

#define ARGUMENTS_END \
		}; \
	} \
	XTL::ProgramArguments * Args = 0;

#define ARGUMENTS_PARSE(ARGC, ARGV) \
	try \
	{ \
		Args = &(XTL::ProgramArguments::Instance()); \
		Args->Parse(ARGC, ARGV); \
	} \
	catch (const XTL::ProgramOptionsError & e) \
	{ \
		fprintf(stderr, "%s\n", e.What()); \
		return 1; \
	} \
	catch (const XTL::ProgramOptionsExit & e) \
	{ \
		return e.Code(); \
	}

#endif

