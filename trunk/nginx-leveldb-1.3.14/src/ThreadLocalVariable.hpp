#ifndef XTL__THREAD_LOCAL_VARIABLE__H__
#define XTL__THREAD_LOCAL_VARIABLE__H__ 1

#include <pthread.h>

namespace XTL
{
	template <typename T>
	class ThreadLocalVariable
	{
		private:

			static void Destroy(void * ptr)
			{
				delete static_cast<T *>(ptr);
				// printf("Thread variable was destroyed\n");
			}

		public:

			ThreadLocalVariable()
			{
				pthread_key_create(&key_, Destroy);
				// printf("Key was created\n");
			}

			const T & Ref() const
			{
				return const_cast<ThreadLocalVariable *>(this)->Ref();
			}

			T & Ref()
			{
				void * ptr = pthread_getspecific(key_);
				if (ptr == 0)
				{
					ptr = new T();
					pthread_setspecific(key_, ptr);
				}

				return *static_cast<T *>(ptr);
			}

		private:

			ThreadLocalVariable(const ThreadLocalVariable &);
			ThreadLocalVariable & operator= (const ThreadLocalVariable &);

			pthread_key_t key_;
	};
}

#endif
