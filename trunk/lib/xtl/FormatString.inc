namespace XTL
{
	namespace INTERNAL
	{
		template <typename T>
		inline const T & PrepareFormat(const T & t)
		{
			return t;
		}

		inline const char * PrepareFormat(const std::string & s)
		{
			return s.c_str();
		}
	}

	template <typename T1>
	const std::string FormatString(const char * format,
	                               const T1 & t1)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1)
		);
	}

	template <typename T1,
	          typename T2>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3,
	                               const T4 & t4)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3),
			INTERNAL::PrepareFormat(t4)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3,
	                               const T4 & t4,
	                               const T5 & t5)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3),
			INTERNAL::PrepareFormat(t4),
			INTERNAL::PrepareFormat(t5)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3,
	                               const T4 & t4,
	                               const T5 & t5,
	                               const T6 & t6)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3),
			INTERNAL::PrepareFormat(t4),
			INTERNAL::PrepareFormat(t5),
			INTERNAL::PrepareFormat(t6)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3,
	                               const T4 & t4,
	                               const T5 & t5,
	                               const T6 & t6,
	                               const T7 & t7)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3),
			INTERNAL::PrepareFormat(t4),
			INTERNAL::PrepareFormat(t5),
			INTERNAL::PrepareFormat(t6),
			INTERNAL::PrepareFormat(t7)
		);
	}

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7,
	          typename T8>
	const std::string FormatString(const char * format,
	                               const T1 & t1,
	                               const T2 & t2,
	                               const T3 & t3,
	                               const T4 & t4,
	                               const T5 & t5,
	                               const T6 & t6,
	                               const T7 & t7,
	                               const T8 & t8)
	{
		return StringPrintFormatted(
			format,
			INTERNAL::PrepareFormat(t1),
			INTERNAL::PrepareFormat(t2),
			INTERNAL::PrepareFormat(t3),
			INTERNAL::PrepareFormat(t4),
			INTERNAL::PrepareFormat(t5),
			INTERNAL::PrepareFormat(t6),
			INTERNAL::PrepareFormat(t7),
			INTERNAL::PrepareFormat(t8)
		);
	}
}

