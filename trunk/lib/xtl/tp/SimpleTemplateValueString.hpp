#ifndef XTL__SIMPLE_TEMPLATE_VALUE_STRING_HPP__
#define XTL__SIMPLE_TEMPLATE_VALUE_STRING_HPP__ 1

#include <string.h>
#include <string>

namespace XTL
{
	class SimpleTemplateValueString : public SimpleTemplateValue
	{
		public:

			SimpleTemplateValueString(const std::string & value)
				: value_(value) { ;; }

			SimpleTemplateValueString(const char * ptr, unsigned int size)
				: value_(ptr, size) { ;; }

			virtual ~SimpleTemplateValueString() throw() { ;; }

			virtual const char * Chars() const
			{
				return value_.data();
			}

			virtual unsigned int Size() const
			{
				return value_.size();
			}

		private:

			const std::string value_;
	};

	class SimpleTemplateValueChars : public SimpleTemplateValue
	{
		public:

			SimpleTemplateValueChars(const char * ptr, unsigned int size)
				: ptr_(ptr),
				  size_(size) { ;; }

			virtual ~SimpleTemplateValueChars() throw() { ;; }

			virtual const char * Chars() const
			{
				return ptr_;
			}

			virtual unsigned int Size() const
			{
				return size_;
			}

		private:

			const char * const ptr_;
			unsigned int       size_;
	};

	template <>
	struct SimpleTemplateValueCreator<std::string>
	{
		static SimpleTemplateValue * CreateCopy(const std::string & obj)
		{
			return new SimpleTemplateValueString(obj);
		}

		static SimpleTemplateValue * CreateReference(const std::string & obj)
		{
			return new SimpleTemplateValueChars(obj.data(), obj.size());
		}
	};

	template <>
	struct SimpleTemplateValueCreator<const char *>
	{
		static SimpleTemplateValue * CreateCopy(const char * obj)
		{
			return new SimpleTemplateValueString(obj, ::strlen(obj));
		}

		static SimpleTemplateValue * CreateReference(const char * obj)
		{
			return new SimpleTemplateValueChars(obj, ::strlen(obj));
		}
	};
}

#endif

