#ifndef XTL__SINGLETON_HPP__
#define XTL__SINGLETON_HPP__ 1

namespace XTL
{
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
}

#endif
