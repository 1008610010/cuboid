#ifndef XTL_PLAIN__RECORD_HPP__
#define XTL_PLAIN__RECORD_HPP__ 1

#include <string.h>

#include <stdexcept>

#include "RecordPrototype.hpp"
#include "../io/InputStream.hpp"
#include "../io/OutputStream.hpp"
#include "../io/Serializable.hpp"

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

			// TODO: void Clear();

			void * Data()
			{
				return data_;
			}

			void * Data() const
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

			class Ref : public ConstRef, XTL::Serializable
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

					virtual void Read(XTL::InputStream & stream)
					{
						stream.Read(Data(), this->Size());
					}

					virtual void Write(XTL::OutputStream & stream) const
					{
						stream.Write(Data(), this->Size());
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

				private:

					Rec(const Rec &);
					Rec & operator= (const Rec &);

					PrototypeHolder prototypeHolder_;
			};

			bool IsNull() const
			{
				return data_ == 0;
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
	TYPE NAME() const { return *reinterpret_cast<const TYPE *>(static_cast<const char *>(Data()) + _field_offset_##NAME()); } \
	TYPE & NAME() { return *reinterpret_cast<TYPE *>(static_cast<char *>(Data()) + _field_offset_##NAME()); } \
	void NAME(TYPE value) { *reinterpret_cast<TYPE *>(static_cast<char *>(Data()) + _field_offset_##NAME()) = value; }

#endif

