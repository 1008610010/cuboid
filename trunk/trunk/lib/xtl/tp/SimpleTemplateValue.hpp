#ifndef XTL__SIMPLE_TEMPLATE_VALUE_HPP__
#define XTL__SIMPLE_TEMPLATE_VALUE_HPP__ 1

namespace XTL
{
	class SimpleTemplateValue
	{
		public:

			virtual ~SimpleTemplateValue() throw() { ;; }

			virtual const char * Chars() const = 0;

			virtual unsigned int Size() const = 0;
	};

	template <typename T>
	struct SimpleTemplateValueCreator
	{
		/**
		 * Создать объект SimpleTemplateValue, хранящий копию передаваемого объекта T.
		 */
		static SimpleTemplateValue * CreateCopy(const T & obj);

		/**
		 * Создать объект SimpleTemplateValue, хранящий только ссылку(ы) на данные объекта T.
		 * Нужно применять в случае, когда время жизни объекта SimpleTemplateValue
		 * заведомо меньше времени жизни объекта T.
		 */
		static SimpleTemplateValue * CreateReference(const T & obj);
	};

	template <typename T>
	SimpleTemplateValue * CreateSimpleTemplateValueCopy(T obj)
	{
		return SimpleTemplateValueCreator<T>::CreateCopy(obj);
	}

	template <typename T>
	SimpleTemplateValue * CreateSimpleTemplateValueReference(T obj)
	{
		return SimpleTemplateValueCreator<T>::CreateReference(obj);
	}
}

#endif

