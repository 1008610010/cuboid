#ifndef XTL__SHARED_PTR_HPP__
#define XTL__SHARED_PTR_HPP__ 1

namespace XTL
{
	/*
		TODO: create effective allocator for objects of type SharedPtrCounter
	*/
	class SharedPtrCounter
	{
		public:

			SharedPtrCounter()
				: refCount_(1)
			{
				;;
			}

			void AddRef()
			{
				++refCount_;
			}

			bool Release()
			{
				return --refCount_ == 0;
			}

			unsigned int RefCount() const
			{
				return refCount_;
			}

		private:

			SharedPtrCounter(const SharedPtrCounter &);
			SharedPtrCounter & operator= (const SharedPtrCounter &);

			unsigned int refCount_;
	};

	template <typename T>
	class SharedPtr
	{
		public:

			SharedPtr()
				: ptr_(0),
				  counter_(0)
			{
				;;
			}

			explicit SharedPtr(T * ptr)
				: ptr_(ptr),
				  counter_(ptr == 0 ? 0 : new SharedPtrCounter())
			{
				;;
			}

			SharedPtr(const SharedPtr & other)
				: ptr_(other.ptr_),
				  counter_(other.counter_)
			{
				AddRef();
			}

			template <typename U>
			SharedPtr(const SharedPtr<U> & other)
				: ptr_(other.ptr_),
				  counter_(other.counter_)
			{
				AddRef();
			}

			~SharedPtr() throw()
			{
				Release();
			}

			SharedPtr & operator= (const SharedPtr & other)
			{
				if (ptr_ != other.ptr_)
				{
					Release();
					ptr_ = other.ptr_;
					counter_ = other.counter_;
					AddRef();
				}

				return *this;
			}

			bool IsNull() const
			{
				return ptr_ == 0;
			}

			T * Get()
			{
				return ptr_;
			}

			T * Get() const
			{
				return ptr_;
			}

			T * operator-> ()
			{
				return ptr_;
			}

			T * operator-> () const
			{
				return ptr_;
			}

			T & operator* ()
			{
				return *ptr_;
			}

			T & operator* () const
			{
				return *ptr_;
			}

			unsigned int RefCount() const
			{
				return counter_->RefCount();
			}

			void Release()
			{
				if (counter_ != 0)
				{
					if (counter_->Release())
					{
						delete counter_;
						delete ptr_;
					}

					counter_ = 0;
					ptr_ = 0;
				}
			}

		private:

			template <typename U> friend class SharedPtr;

			void AddRef()
			{
				if (counter_ != 0)
				{
					counter_->AddRef();
				}
			}

			T * ptr_;
			SharedPtrCounter * counter_;
	};
}

#endif

