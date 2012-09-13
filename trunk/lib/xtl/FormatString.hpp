#ifndef XTL__FORMAT_STRING_HPP__
#define XTL__FORMAT_STRING_HPP__ 1

#include <memory>
#include <string>

namespace XTL
{
	const std::string StringPrintFormatted(const char * format, ...);


	template <typename T1>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1);

	template <typename T1,
	          typename T2>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2);

	template <typename T1,
	          typename T2,
	          typename T3>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6,
	                                      const T7 & t7);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7,
	          typename T8>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6,
	                                      const T7 & t7,
	                                      const T8 & t8);

	// Lazy-evaluating formatted string
	class FormattedString
	{
		public:

			explicit FormattedString(const char * format)
				: format_(format)
			{
				;;
			}

			virtual ~FormattedString() throw() { ;; }

			virtual const std::string ToString() const = 0;

			static inline std::auto_ptr<FormattedString> Create(const char * format);

			template <typename T1>
			static inline std::auto_ptr<FormattedString> Create(const char * format, const T1 & t1);

			template <typename T1, typename T2>
			static inline std::auto_ptr<FormattedString> Create(const char * format, const T1 & t1, const T2 & t2);

			template <typename T1, typename T2, typename T3>
			static inline std::auto_ptr<FormattedString> Create(const char * format, const T1 & t1, const T2 & t2, const T3 & t3);

			template <typename T1, typename T2, typename T3, typename T4>
			static inline std::auto_ptr<FormattedString> Create(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4);

		protected:

			const char * const format_;

		private:

			class P0;
			template <typename T1> class P1;
			template <typename T1, typename T2> class P2;
			template <typename T1, typename T2, typename T3> class P3;
			template <typename T1, typename T2, typename T3, typename T4> class P4;
	};

	class FormattedString::P0 : public FormattedString
	{
		public:

			explicit P0(const char * format)
				: FormattedString(format) { ;; }

			virtual ~P0() throw() { ;; }

			virtual const std::string ToString() const { return format_; }
	};

	template <typename T1>
	class FormattedString::P1 : public FormattedString
	{
		public:

			P1(const char * format, const T1 & t1)
				: FormattedString(format), t1_(t1) { ;; }

			virtual ~P1() throw() { ;; }

			virtual const std::string ToString() const { return XTL::FormatString(format_, t1_); }

		private:

			const T1 & t1_;
	};

	template <typename T1, typename T2>
	class FormattedString::P2 : public FormattedString
	{
		public:

			P2(const char * format, const T1 & t1, const T2 & t2)
				: FormattedString(format), t1_(t1), t2_(t2) { ;; }

			virtual ~P2() throw() { ;; }

			virtual const std::string ToString() const { return XTL::FormatString(format_, t1_, t2_); }

		private:

			const T1 & t1_;
			const T2 & t2_;
	};

	template <typename T1, typename T2, typename T3>
	class FormattedString::P3 : public FormattedString
	{
		public:

			P3(const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
				: FormattedString(format), t1_(t1), t2_(t2), t3_(t3) { ;; }

			virtual ~P3() throw() { ;; }

			virtual const std::string ToString() const { return XTL::FormatString(format_, t1_, t2_, t3_); }

		private:

			const T1 & t1_;
			const T2 & t2_;
			const T3 & t3_;
	};

	template <typename T1, typename T2, typename T3, typename T4>
	class FormattedString::P4 : public FormattedString
	{
		public:

			P4(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
				: FormattedString(format), t1_(t1), t2_(t2), t3_(t3), t4_(t4) { ;; }

			virtual ~P4() throw() { ;; }

			virtual const std::string ToString() const { return XTL::FormatString(format_, t1_, t2_, t3_, t4_); }

		private:

			const T1 & t1_;
			const T2 & t2_;
			const T3 & t3_;
			const T4 & t4_;
	};

	std::auto_ptr<FormattedString> FormattedString::Create(const char * format)
	{
		return std::auto_ptr<FormattedString>(new FormattedString::P0(format));
	}

	template <typename T1>
	std::auto_ptr<FormattedString> FormattedString::Create(const char * format, const T1 & t1)
	{
		return std::auto_ptr<FormattedString>(new FormattedString::P1<T1>(format, t1));
	}

	template <typename T1, typename T2>
	std::auto_ptr<FormattedString> FormattedString::Create(const char * format, const T1 & t1, const T2 & t2)
	{
		return std::auto_ptr<FormattedString>(new FormattedString::P2<T1, T2>(format, t1, t2));
	}

	template <typename T1, typename T2, typename T3>
	std::auto_ptr<FormattedString> FormattedString::Create(const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		return std::auto_ptr<FormattedString>(new FormattedString::P3<T1, T2, T3>(format, t1, t2, t3));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	std::auto_ptr<FormattedString> FormattedString::Create(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		return std::auto_ptr<FormattedString>(new FormattedString::P4<T1, T2, T3, T4>(format, t1, t2, t3, t4));
	}
}

#include "FormatString.inc"

#endif

