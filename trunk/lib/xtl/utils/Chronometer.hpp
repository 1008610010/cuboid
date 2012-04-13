#ifndef XTL__CHRONOMETER_HPP__
#define XTL__CHRONOMETER_HPP__ 1

namespace XTL
{
	class Chronometer
	{
		public:

			Chronometer();

			Chronometer & Start();

			Chronometer & Stop();

			double Elapsed() const;

		protected:

			double start_;
			double elapsed_;
	};
}

#endif
