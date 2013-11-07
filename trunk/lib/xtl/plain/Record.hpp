#ifndef XTL_PLAIN__RECORD_HPP__
#define XTL_PLAIN__RECORD_HPP__ 1

#include <string.h>

#include <algorithm>
#include <stdexcept>

#include "RecordPrototype.hpp"
#include "../io/InputStream.hpp"
#include "../io/OutputStream.hpp"
#include "../io/Serializable.hpp"

/*
	TODO:
#define PROTOTYPE(NAME) \
	static const char * PrototypeName() { return NAME; } \
	template <typename U, unsigned int I> struct A { enum { MaxIndex = A<U, I - 1>::MaxIndex }; }; \
	template <typename U> struct A<U, 0> { enum { MaxIndex = 0 }; };

#define FIELD(IDX, TYPE, NAME) \
	template <typename U> struct A<U, IDX> : public A<U, IDX - 1> \
	{ \
		enum { MaxIndex = IDX }; \
		A() : A<U, IDX - 1>() { printf("field %d: %s\n", IDX, #NAME); PrototypeBuilder().AddField_##TYPE(#NAME); } \
	}; \
	static unsigned int _field_index_##NAME() { static const unsigned int index = Prototype()->GetFieldIndex("" #NAME, sizeof(XTL::TYPE)); return index; } \
	static unsigned int _field_offset_##NAME() { static const unsigned int offset = Prototype()->GetField(_field_index_##NAME())->Offset(); return offset; } \
	XTL::TYPE NAME() const { return *reinterpret_cast<const XTL::TYPE *>(static_cast<const char *>(this->Data()) + _field_offset_##NAME()); } \
	XTL::TYPE & NAME() { return *reinterpret_cast<XTL::TYPE *>(static_cast<char *>(this->Data()) + _field_offset_##NAME()); } \
	void NAME(XTL::TYPE value) { *reinterpret_cast<XTL::TYPE *>(static_cast<char *>(this->Data()) + _field_offset_##NAME()) = value; }

#define XTL_PLAIN_FIELD_UINT_32 (IDX, NAME)  FIELD(IDX, XTL::UINT_32, NAME)
#define XTL_PLAIN_FIELD_INT_32  (IDX, NAME)  FIELD(IDX, XTL::INT_32,  NAME)
#define XTL_PLAIN_FIELD_UINT_64 (IDX, NAME)  FIELD(IDX, XTL::UINT_64, NAME)
#define XTL_PLAIN_FIELD_INT_64  (IDX, NAME)  FIELD(IDX, XTL::INT_64,  NAME)

template <typename T>
class RecordType : public XTL::PLAIN::RecordBase<T>
{
	public:

		static XTL::PLAIN::RecordPrototype::SharedConstPtr Prototype()
		{
			static XTL::PLAIN::RecordPrototype::SharedConstPtr prototype = RecordType<T>::BuildPrototype();
			return prototype;
		}

	public:

		static XTL::PLAIN::RecordPrototypeBuilder & PrototypeBuilder()
		{
			static XTL::PLAIN::RecordPrototypeBuilder prototypeBuilder(T::PrototypeName());
			return prototypeBuilder;
		}

		static XTL::PLAIN::RecordPrototype::SharedConstPtr BuildPrototype()
		{
			static typename T::template A<int, T::template A<int, 32>::MaxIndex> a;
			return PrototypeBuilder().Finish();
		}
};

class FriendPair : public RecordType<FriendPair>
{
	public:

		XTL_PLAIN_PROTOTYPE("FriendPair")
		XTL_PLAIN_FIELD( 1, UINT_32, user_id   )
		XTL_PLAIN_FIELD( 2, UINT_64, friend_id )
};

	...
	FriendPair::Rec rec;
	rec.user_id(1);
	rec.friend_id(2);
	printf("PrototypeSize=%u\n", FriendPair::Prototype()->Size());
	printf("sizeof(FriendPair)=%lu\n", sizeof(FriendPair));

	FriendPair::Ref ref(rec);
	ref.user_id(339052);

	printf("user_id=%u friend_id=%llu\n", rec.user_id(), rec.friend_id());
	...
*/

namespace XTL
{
namespace PLAIN
{
	class RecordConstRef
	{
		public:

			explicit RecordConstRef(const RecordPrototype * prototype)
				: prototype_(prototype),
				  data_(0)
			{
				;;
			}

			RecordConstRef(const RecordPrototype * prototype, const void * data)
				: prototype_(prototype),
				  data_(const_cast<void *>(data))
			{
				;;
			}

			const RecordPrototype * Prototype() const
			{
				return prototype_;
			}

			unsigned int Size() const
			{
				return prototype_->Size();
			}

			const void * Data() const
			{
				return data_;
			}

			bool IsNull() const
			{
				return data_ == 0;
			}

		protected:

			const RecordPrototype * prototype_;
			void * data_;
	};

	class RecordRef : public RecordConstRef, XTL::Serializable
	{
		public:

			explicit RecordRef(const RecordPrototype * prototype)
				: RecordConstRef(prototype)
			{
				;;
			}

			RecordRef(const RecordPrototype * prototype, void * data)
				: RecordConstRef(prototype, data)
			{
				;;
			}

			void Clear()
			{
				::memset(Data(), '\0', this->Size());
			}

			void * Data() const
			{
				return data_;
			}

			void * Data()
			{
				return data_;
			}

			void Assign(const RecordConstRef & other)
			{
				if (Prototype() != other.Prototype())
				{
					throw std::runtime_error("RecordRef assignment of different prototype");
				}

				::memcpy(Data(), other.Data(), this->Size());
			}

			virtual void Read(XTL::InputStream & stream)
			{
				stream.Read(Data(), this->Size());
			}

			virtual void Write(XTL::OutputStream & stream) const
			{
				stream.Write(Data(), this->Size());
			}
	};

	class Record
	{
		public:

			explicit Record(RecordPrototype::SharedConstPtr prototype)
				: prototype_(prototype),
				  data_(new char[prototype->Size()])
			{
				::memset(data_, '\0', prototype->Size());
			}

			~Record() throw()
			{
				delete [] static_cast<char *>(data_);
			}

			operator RecordConstRef () const
			{
				return RecordConstRef(prototype_.Get(), data_);
			}

			operator RecordRef ()
			{
				return RecordRef(prototype_.Get(), data_);
			}

			void Clear()
			{
				::memset(data_, '\0', prototype_->Size());
			}

			void Assign(const RecordConstRef & other)
			{
				if (prototype_.Get() != other.Prototype())
				{
					throw std::runtime_error("RecordRef assignment of different prototype");
				}

				::memcpy(data_, other.Data(), prototype_->Size());
			}

		private:

			RecordPrototype::SharedConstPtr prototype_;
			void * data_;
	};

	template <typename RecordType>
	class RecordBase
	{
		public:

			typedef RecordType ThisType;

			class ConstRef : public RecordType
			{
				public:

					ConstRef()
						: RecordType()
					{
						;;
					}

					explicit ConstRef(void * data)
						: RecordType()
					{
						this->data_ = data;
					}

					explicit ConstRef(const void * data)
						: RecordType()
					{
						this->data_ = const_cast<void *>(data);
					}

					ConstRef(XTL::PLAIN::RecordConstRef ref)
						: RecordType()
					{
						CheckPrototype(ref.Prototype());
						this->data_ = const_cast<void *>(ref.Data());
					}

					ConstRef(XTL::PLAIN::RecordRef ref)
						: RecordType()
					{
						CheckPrototype(ref.Prototype());
						this->data_ = ref.Data();
					}

					const RecordType * operator-> () const
					{
						return this;
					}

					unsigned int Size() const
					{
						return Prototype()->Size();
					}

					void Write(XTL::OutputStream & stream) const
					{
						stream.Write(Data(), this->Size());
					}

				protected:

					static const RecordPrototype * Prototype()
					{
						return RecordType::Prototype().Get();
					}

					static void CheckPrototype(const RecordPrototype * prototype)
					{
						if (Prototype() != prototype)
						{
							throw std::runtime_error("Invalid prototype in assignment");
						}
					}
			};

			class Ref : public ConstRef
			{
				public:

					Ref()
						: ConstRef() { ;; }

					explicit Ref(void * data)
						: ConstRef(data) { ;; }

					Ref(XTL::PLAIN::RecordRef ref)
						: ConstRef(ref.Data())
					{
						;;
					}

					RecordType * operator-> ()
					{
						return this;
					}

					void Clear()
					{
						::memset(Data(), '\0', this->Size());
					}

					void Read(XTL::InputStream & stream)
					{
						stream.Read(Data(), this->Size());
					}

					operator RecordConstRef() const
					{
						return RecordConstRef(this->Prototype(), this->data_);
					}

					operator RecordRef()
					{
						return RecordRef(this->Prototype(), this->data_);
					}
			};

			class Rec : public Ref
			{
				public:

					/*
						Класс нужен для того, чтобы ограничить тип возвращаемого значения статическим методом RecordType::Prototype().
						Он обязан возвращать SharedConstPtr.
					*/
					class PrototypeHolder
					{
						public:

							explicit PrototypeHolder(RecordPrototype::SharedConstPtr prototype)
								: prototype_(prototype)
							{
								;;
							}

							const RecordPrototype::SharedConstPtr & operator->() const
							{
								return prototype_;
							}

							const RecordPrototype * Prototype() const
							{
								return prototype_.Get();
							}

						private:

							PrototypeHolder(const PrototypeHolder &);
							PrototypeHolder & operator= (const PrototypeHolder &);

							RecordPrototype::SharedConstPtr prototype_;
					};

					Rec()
						: Ref(),
						  prototypeHolder_(RecordType::Prototype())
					{
						this->data_ = new char[this->Size()];
						::memset(this->data_, '\0', this->Size());
					}

					Rec(XTL::InputStream & stream)
						: Ref(),
						  prototypeHolder_(RecordType::Prototype())
					{
						this->data_ = new char[this->Size()];

						try
						{
							this->Read(stream);
						}
						catch (...)
						{
							delete [] static_cast<char *>(this->data_);
							throw;
						}
					};

					~Rec() throw()
					{
						delete [] static_cast<char *>(this->data_);
					}

					const RecordPrototype * Prototype() const
					{
						return prototypeHolder_.Prototype();
					}

					operator RecordConstRef() const
					{
						return RecordConstRef(Prototype(), this->data_);
					}

					operator RecordRef()
					{
						return RecordRef(Prototype(), this->data_);
					}

					Rec & operator= (ConstRef ref)
					{
						if (this->data_ != ref->data_)
						{
							::memcpy(this->data_, ref->data_, this->Size());
						}

						return *this;
					}

					Rec & operator= (const Rec & other)
					{
						return operator=((ConstRef) other);
					}

					void Swap(Rec & other)
					{
						std::swap(this->data_, other.data_);
					}

				private:

					Rec(const Rec &);

					PrototypeHolder prototypeHolder_;
			};

			bool IsNull() const
			{
				return data_ == 0;
			}

			static RecordPrototype::SharedConstPtr Prototype()
			{
				static RecordPrototype::SharedConstPtr prototype = RecordType::BuildPrototype();
				return prototype;
			}

		protected:

			RecordBase()
				: data_(0)
			{
				;;
			}

			template <typename T> T & Field(unsigned int offset)
			{
				return *reinterpret_cast<T *>(static_cast<char *>(data_) + offset);
			}

			template <typename T> T Field(unsigned int offset) const
			{
				return *reinterpret_cast<const T *>(static_cast<const char *>(data_) + offset);
			}

			void * Data()
			{
				return data_;
			}

			const void * Data() const
			{
				return data_;
			}

			void * data_;
	};
}
}

#define DECLARE_FIELD(TYPE, NAME) \
	static unsigned int _field_index_##NAME() { static const unsigned int index = Prototype()->GetFieldIndex("" #NAME, sizeof(TYPE)); return index; } \
	static unsigned int _field_offset_##NAME() { static const unsigned int offset = Prototype()->GetField(_field_index_##NAME())->Offset(); return offset; } \
	TYPE NAME() const { return *reinterpret_cast<const TYPE *>(static_cast<const char *>(this->Data()) + _field_offset_##NAME()); } \
	TYPE & NAME() { return *reinterpret_cast<TYPE *>(static_cast<char *>(this->Data()) + _field_offset_##NAME()); } \
	void NAME(TYPE value) { *reinterpret_cast<TYPE *>(static_cast<char *>(this->Data()) + _field_offset_##NAME()) = value; }

#define DECLARE_FIELD_STRUCT(TYPE, NAME) \
	static unsigned int _field_index_##NAME() { static const unsigned int index = Prototype()->GetFieldIndex("" #NAME, TYPE::Prototype()->Size()); return index; } \
	static unsigned int _field_offset_##NAME() { static const unsigned int offset = Prototype()->GetField(_field_index_##NAME())->Offset(); return offset; } \
	typename TYPE::ConstRef NAME() const { return typename TYPE::ConstRef(static_cast<const char *>(this->Data()) + _field_offset_##NAME()); } \
	typename TYPE::Ref      NAME()       { return typename TYPE::Ref(static_cast<char *>(this->Data()) + _field_offset_##NAME()); } \

#define DECLARE_FIELD_STRUCT_TUPLE(TYPE, NAME, CAPACITY) \
	static unsigned int _field_index_##NAME() { static const unsigned int index = Prototype()->GetFieldIndex("" #NAME, TYPE::Prototype()->AlignedSize() * CAPACITY); return index; } \
	static unsigned int _field_offset_##NAME() { static const unsigned int offset = Prototype()->GetField(_field_index_##NAME())->Offset(); return offset; } \
	typename TYPE::ConstRef NAME(unsigned int index) const { \
		return typename TYPE::ConstRef( \
			static_cast<const char *>(this->Data()) + \
			_field_offset_##NAME() + \
			TYPE::Prototype()->AlignedSize() * index \
		); \
	} \
	typename TYPE::Ref NAME(unsigned int index) { \
		return typename TYPE::Ref( \
			static_cast<char *>(this->Data()) + \
			_field_offset_##NAME() + \
			TYPE::Prototype()->AlignedSize() * index \
		); \
	}

#endif
