#ifndef _STXX__LINEAR_HASH_HPP__
#define _STXX__LINEAR_HASH_HPP__ 1

#include <string>
#include <sup/Types.h>
#include "RecordPool.hpp"

namespace STXX
{
	struct LinearHashHeader
	{
		UINT_64 size;
		UINT_64 level;
		UINT_64 split_slot;
		UINT_64 primary_count;
		UINT_64 overflow_count;
		UINT_64 primary_size;
		UINT_64 overflow_size;
		UCHAR_8 reserved[456];

		// WARNING: valgrind shows error on uninitialized value of field "reserved"
		LinearHashHeader()
			: size(0),
			  level(0),
			  split_slot(0),
			  primary_count(0),
			  overflow_count(0),
			  primary_size(0),
			  overflow_size(0)
		{
			;;
		}

		void Clear()
		{
			size = 0;
			level = 0;
			split_slot = 0;
			primary_count = 0;
			overflow_count = 0;
			primary_size = 0;
			overflow_size = 0;
		}

		void Advance()
		{
			++split_slot;
			if (split_slot == ((UINT_64) 1 << level))
			{
				split_slot = 0;
				++level;
			}
		}
	};

	template <typename _ItemType,
	          typename _KeyType,
	          UINT_32 CAPACITY>
	class LinearHashBucket
	{
		public:

			LinearHashBucket()
				: size_(0),
				  link_()
			{
				;;
			}

			UINT_64 Size() const
			{
				return size_;
			}

			void SetSize(UINT_64 size)
			{
				size_ = size;
			}

			bool Full() const
			{
				return size_ == CAPACITY;
			}

			_ItemType * InsertItem(const _KeyType & key)
			{
				new (slots_ + size_) _ItemType(key);
				return slots_ + (size_++);
			}

			void Add(const _ItemType & item)
			{
				if (slots_ + size_ == &item)
				{
					++size_;
				}
				else
				{
					slots_[size_++] = item;
				}
			}

			_ItemType & operator[] (UINT_32 index)
			{
				return slots_[index];
			}

			_ItemType * Search(const _KeyType & key)
			{
				for (UINT_32 i = 0; i < size_; ++i)
				{
					if ((const _KeyType &) slots_[i] == key)
					{
						return slots_ + i;
					}
				}
				return NULL;
			}

			const RecordPoolAddress & Link() const
			{
				return link_;
			}

			void SetLink(const RecordPoolAddress & link)
			{
				link_ = link;
			}

		private:

			UINT_64           size_;
			RecordPoolAddress link_;
			_ItemType         slots_[CAPACITY];
	};

	template <typename _SourceType,
	          typename _ItemType,
	          typename _KeyType,
	          typename _HashFunction,
	          UINT_32 PRIMARY_CAPACITY,
	          UINT_32 OVERFLOW_CAPACITY,
	          UINT_32 OVERFLOW_BITBLOCK_SIZE>
	class LinearHash
	{
		public:

			typedef LinearHashBucket<_ItemType, _KeyType, PRIMARY_CAPACITY>  PrimaryBucket;
			typedef LinearHashBucket<_ItemType, _KeyType, OVERFLOW_CAPACITY> OverflowBucket;
			typedef RecordPool<typename _SourceType::FileSource, OverflowBucket, OVERFLOW_BITBLOCK_SIZE> OverflowPool;

			typedef typename File::Record<LinearHashHeader>    HeaderRecord;
			typedef typename File::RecordPtr<LinearHashHeader> HeaderRecordPtr;
			typedef typename File::Record<PrimaryBucket>       PrimaryRecord;
			typedef typename File::RecordPtr<PrimaryBucket>    PrimaryRecordPtr;
			typedef typename OverflowPool::Record              OverflowRecord;
			typedef typename OverflowPool::RecordPtr           OverflowRecordPtr;

			LinearHash()
				: source_(NULL),
				  // TODO: Bitmap level is hardcoded!
				  poolOverflow_(6),
				  header_()
			{
				;;
			}

			typename _SourceType::FileSource & FilePrimary()
			{
				return source_->FilePrimary();
			}

			typename _SourceType::FileSource & FileOverflow()
			{
				return source_->FileOverflow();
			}

			bool Open(_SourceType * source)
			{
				source_ = source;
				if (poolOverflow_.Open(&(FileOverflow())))
				{
					if (FilePrimary().Size() > 0)
					{
						if (FilePrimary().Size() > sizeof(LinearHashHeader) &&
						    (FilePrimary().Size() - sizeof(LinearHashHeader)) % sizeof(PrimaryBucket) == 0 &&
						    FilePrimary().Load(0, header_))
						{
							return true;
						}
					}
					else
					{
						HeaderRecordPtr headerPtr = FilePrimary().template New<LinearHashHeader>();
						if (!headerPtr.IsNull())
						{
							headerPtr->Clear();
							PrimaryRecord primary;
							if (FilePrimary().New(primary))
							{
								headerPtr->primary_count++;
								header_.Assign(headerPtr);
								return true;
							}
						}
					}
					poolOverflow_.Close();
				}
				source_ = 0;
				return false;
			}

			bool Close()
			{
				return FilePrimary().Save(header_);
			}

			UINT_64 Size() const
			{
				return header_->size;
			}

			UINT_64 PrimaryCount() const
			{
				return header_->primary_count;
			}

			UINT_64 OverflowsCount() const
			{
				return header_->overflow_count;
			}

			UINT_64 PrimarySize() const
			{
				return header_->primary_size;
			}

			UINT_64 OverflowSize() const
			{
				return header_->overflow_size;
			}

			bool Get(const _KeyType & key, _ItemType & item)
			{
				// TODO: check if opened and not crushed (or not?)

				PrimaryRecord primaryRecord;

				UINT_64 keyHash = CalcHashFunc(key);
				UINT_64 slot = CalcHashSlot(keyHash);

				if (!LoadPrimary(slot, primaryRecord))
				{
					return false;
				}

				_ItemType * found = primaryRecord->Search(key);
				if (found != 0)
				{
					item = *found;
					return true;
				}

				RecordPoolAddress overflowAddress = primaryRecord->Link();
				while (overflowAddress != OverflowPool::NIL)
				{
					OverflowRecord overflowRecord;

					if (!LoadOverflow(overflowAddress, overflowRecord))
					{
						return false;
					}
					found = overflowRecord->Search(key);
					if (found != 0)
					{
						item = *found;
						return true;
					}
					overflowAddress = overflowRecord->Link();
				}

				return false;
			}

			template <typename ItemCallbackHandler>
			bool Insert(const _KeyType & key, bool & inserted, ItemCallbackHandler handler)
			{
				inserted = false;

				// TODO: check if opened and not crushed (or not?)

				PrimaryRecord primaryRecord;

				UINT_64 keyHash = CalcHashFunc(key);
				UINT_64 slot = CalcHashSlot(keyHash);

				if (!LoadPrimary(slot, primaryRecord))
				{
					return false;
				}

				// TODO: optimize key search with insert to overflow

				_ItemType * found = primaryRecord->Search(key);
				if (found != 0)
				{
					return handler(*found) ? SavePrimary(primaryRecord) : true;
				}

				OverflowRecord overflowRecord;
				RecordPoolAddress overflowAddress = primaryRecord->Link();

				while (overflowAddress != OverflowPool::NIL)
				{
					if (!LoadOverflow(overflowAddress, overflowRecord))
					{
						return false;
					}
					found = overflowRecord->Search(key);
					if (found != 0)
					{
						return handler(*found) ? SaveOverflow(overflowRecord) : true;
					}
					overflowAddress = overflowRecord->Link();
				}

				//if (primaryRecord->Full())
				// TODO: configure this
				if ((float) header_->primary_size / ((float) header_->primary_count * PRIMARY_CAPACITY) > 0.8)
				{
					if (!Split())
					{
						return false;
					}
					// WARNING: After splitting primaryRecord can be changed!!!
					// TODO: Think about when do not reload primaryRecord
					slot = CalcHashSlot(keyHash);
					if (!LoadPrimary(slot, primaryRecord))
					{
						return false;
					}
				}

				if (!primaryRecord->Full())
				{
					found = primaryRecord->InsertItem(key);
					inserted = true;
					header_->size++;
					header_->primary_size++;
					handler(*found);
					return SavePrimary(primaryRecord);
				}

				if (primaryRecord->Link() == OverflowPool::NIL)
				{
					OverflowRecordPtr newOverflowPtr = NewOverflow();
					if (newOverflowPtr.IsNull())
					{
						return false;
					}
					header_->overflow_count++;
					found = newOverflowPtr->InsertItem(key);
					inserted = true;
					header_->size++;
					header_->overflow_size++;
					handler(*found);
					primaryRecord->SetLink(newOverflowPtr.Address());
					return SavePrimary(primaryRecord);
				}
				else
				{
					// TODO: May be performance upgrade: when not filled overflow record
					//       is first in primaryRecord list. To do so, need rewrite
					//       Split() function and code below.
					OverflowRecord overflowRecord;
					RecordPoolAddress overflowAddress = primaryRecord->Link();

					while (overflowAddress != OverflowPool::NIL)
					{
						if (!LoadOverflow(overflowAddress, overflowRecord))
						{
							return false;
						}
						overflowAddress = overflowRecord->Link();
					}
					if (!overflowRecord->Full())
					{
						found = overflowRecord->InsertItem(key);
						inserted = true;
						header_->size++;
						header_->overflow_size++;
						handler(*found);
						return SaveOverflow(overflowRecord);
					}
					else
					{
						OverflowRecordPtr newOverflowPtr = NewOverflow();
						if (newOverflowPtr.IsNull())
						{
							return false;
						}
						header_->overflow_count++;
						found = newOverflowPtr->InsertItem(key);
						inserted = true;
						header_->size++;
						header_->overflow_size++;
						handler(*found);
						overflowRecord->SetLink(newOverflowPtr.Address());
						return SaveOverflow(overflowRecord);
						//newOverflowPtr->SetOverflow(primaryRecord->Overflow());
						//primaryRecord->SetOverflow(newOverflowPtr.Address());
						//return SetPrimaryRecord(primaryRecord);
					}
				}
			}

			bool Split()
			{
				//UINT_64 oldSize = GetRecordSize(header_->split_slot);
				//printf("SPLIT SIZE = %llu\n", oldSize);

				PrimaryRecordPtr oldRecord = GetPrimary(header_->split_slot);
				PrimaryRecordPtr newRecord = NewPrimary();
				if (oldRecord.IsNull() || newRecord.IsNull())
				{
					return false;
				}
				header_->primary_count++;
				UINT_32 oldRecordSize = oldRecord->Size();
				oldRecord->SetSize(0);
				for (UINT_32 i = 0; i < oldRecordSize; ++i)
				{
					UINT_64 slot = CalcNextHashSlot((const _KeyType &) (*oldRecord)[i]);
					if (slot == header_->split_slot)
					{
						oldRecord->Add((*oldRecord)[i]);
					}
					else
					{
						newRecord->Add((*oldRecord)[i]);
					}
				}

				OverflowRecordPtr oldOverflow;
				OverflowRecordPtr newOverflow;

				RecordPoolAddress overflowAddress = oldRecord->Link();

				while (overflowAddress != OverflowPool::NIL)
				{
					OverflowRecordPtr overflowPtr = GetOverflow(overflowAddress);
					if (overflowPtr.IsNull())
					{
						return false;
					}
					UINT_32 overflowSize = overflowPtr->Size();
					for (UINT_32 i = 0; i < overflowSize; ++i)
					{
						UINT_64 slot = CalcNextHashSlot((const _KeyType &) (*overflowPtr)[i]);
						if (slot == header_->split_slot)
						{
							if (!oldRecord->Full())
							{
								oldRecord->Add((*overflowPtr)[i]);
								header_->primary_size++;
								header_->overflow_size--;
							}
							else
							{
								if (oldOverflow.IsNull())
								{
									oldOverflow = GetOverflow(oldRecord->Link());
									if (oldOverflow.IsNull())
									{
										return false;
									}
									// We can do this, because oldOverflow <= overflowPtr
									// and overflowPtr->Size() is saved in overflowSize
									oldOverflow->SetSize(0);
								}
								else if (oldOverflow->Full())
								{
									oldOverflow = GetOverflow(oldOverflow->Link());
									if (oldOverflow.IsNull())
									{
										return false;
									}
									oldOverflow->SetSize(0);
								}

								oldOverflow->Add((*overflowPtr)[i]);
							}
						}
						else
						{
							if (!newRecord->Full())
							{
								newRecord->Add((*overflowPtr)[i]);
								header_->primary_size++;
								header_->overflow_size--;
							}
							else
							{
								if (newOverflow.IsNull())
								{
									newOverflow = NewOverflow();
									if (newOverflow.IsNull())
									{
										return false;
									}
									header_->overflow_count++;
									newRecord->SetLink(newOverflow.Address());
								}
								else
								{
									if (newOverflow->Full())
									{
										OverflowRecordPtr createdOverflow = NewOverflow();
										if (createdOverflow.IsNull())
										{
											return false;
										}
										header_->overflow_count++;
										newOverflow->SetLink(createdOverflow.Address());
										newOverflow = createdOverflow;
									}
								}

/*
								if (newOverflow.IsNull() || newOverflow->Full())
								{
									OverflowRecordPtr createdOverflow = poolOverflow_.New();
									if (createdOverflow.IsNull())
									{
										return false;
									}
									header_->overflow_count++;
									if (!newOverflow.IsNull())
									{
										newOverflow->SetOverflow(createdOverflow.Address());
									}
									newRecord->SetOverflow(createdOverflow.Address());
									newOverflow = createdOverflow;
								}
*/
								newOverflow->Add((*overflowPtr)[i]);
							}
 						}
					}
					overflowAddress = overflowPtr->Link();
				}

				if (oldRecord->Link() != OverflowPool::NIL)
				{
					if (oldOverflow.IsNull())
					{
						RecordPoolAddress overflowAddress = oldRecord->Link();
						oldRecord->SetLink(OverflowPool::NIL);
						while (overflowAddress != OverflowPool::NIL)
						{
							OverflowRecordPtr overflowPtr = GetOverflow(overflowAddress);
							if (overflowPtr.IsNull())
							{
								return false;
							}

							RecordPoolAddress nextOverflowAddress = overflowPtr->Link();

							if (!DeleteOverflow(overflowAddress))
							{
								return false;
							}
							header_->overflow_count--;
							overflowAddress = nextOverflowAddress;
						}
					}
					else
					{
						RecordPoolAddress overflowAddress = oldOverflow->Link();
						oldOverflow->SetLink(OverflowPool::NIL);
						while (overflowAddress != OverflowPool::NIL)
						{
							OverflowRecordPtr overflowPtr = GetOverflow(overflowAddress);
							if (overflowPtr.IsNull())
							{
								return false;
							}

							RecordPoolAddress nextOverflowAddress = overflowPtr->Link();
							if (!DeleteOverflow(overflowAddress))
							{
								return false;
							}
							header_->overflow_count--;
							overflowAddress = nextOverflowAddress;
						}
					}
				}

				//UINT_64 oldSlot = header_->split_slot;
				//UINT_64 newSlot = header_->split_slot + (1 << header_->level);

				header_->Advance();

				/*
				if (!TestRecord(oldSlot))
				{
					printf("SPLIT ERROR: OLD PRIMARY BUCKET\n");
					return false;
				}
				if (!TestRecord(newSlot))
				{
					printf("SPLIT ERROR: NEW PRIMARY BUCKET\n");
					return false;
				}
				UINT_64 s1 = GetRecordSize(oldSlot);
				UINT_64 s2 = GetRecordSize(newSlot);
				if (oldSize != s1 + s2)
				{
					printf("SPLIT ERROR: SIZE %llu != %llu + %llu\n", oldSize, s1, s2);
					return false;
				}
				*/
				return true;
			}

			bool SelfTest()
			{
				if (FilePrimary().Size() == 0)
				{
					return true;
				}

				if (FilePrimary().Size() != sizeof(LinearHashHeader) + header_->primary_count * sizeof(PrimaryBucket))
				{
					printf("Selftest error: Invalid primary file size\n");
					return false;
				}

				if (header_->primary_count != (1 << header_->level) + header_->split_slot)
				{
					fprintf(stdout, "Selftest error: header->primary_count\n");
					return false;
				}

				if (header_->size != header_->primary_size + header_->overflow_size)
				{
					fprintf(stdout, "Selftest error: invalid sum of primary and overflow size\n");
					return false;
				}

				UINT_64 size = 0;
				UINT_64 overflow_count = 0;

				printf("----------------------------------\n");
				printf("Items count = %llu\n", header_->size);
				printf("Level = %llu\n", header_->level);
				printf("Split slot = %llu\n", header_->split_slot);
				printf("Primary count = %llu\n", header_->primary_count);
				printf("Overflow count = %llu\n", header_->overflow_count);
				printf("Primary size = %llu\n", header_->primary_size);
				printf("Overflow size = %llu\n", header_->overflow_size);
				printf("Primary fill rate = %0.3f\n", (float) header_->primary_size / ((float) header_->primary_count * PRIMARY_CAPACITY));

				for (UINT_64 slot = 0; slot < header_->primary_count; ++slot)
				{
					PrimaryRecord primaryRecord;
					if (!LoadPrimary(slot, primaryRecord))
					{
						fprintf(stdout, "Selftest error: Unable to load primary bucket #%llu\n", slot);
						return false;
					}
					for (UINT_64 i = 0; i < primaryRecord->Size(); ++i)
					{
						_ItemType & item = (*primaryRecord)[i];
						if (CalcHashSlot(CalcHashFunc((const _KeyType &) item)) != slot)
						{
							fprintf(stdout, "Selftest error: Primary bucket slot = %llu, item slot = %llu\n", slot, CalcHashSlot(CalcHashFunc((const _KeyType &) item)));
							return false;
						}
					}
					size += primaryRecord->Size();

					RecordPoolAddress overflowAddress = primaryRecord->Link();
					if (overflowAddress != OverflowPool::NIL)
					{
						if (!primaryRecord->Full())
						{
							fprintf(stdout, "Selftest error: Primary bucket has overflows, but is not full\n");
							return false;
						}
						OverflowRecord overflowRecord;
						while (overflowAddress != OverflowPool::NIL)
						{
							if (!poolOverflow_.IsAllocated(overflowAddress))
							{
								fprintf(stdout, "Selftest error: FileRecordPool bitmap error\n");
								return false;
							}
							if (!poolOverflow_.Load(overflowAddress, overflowRecord))
							{
								fprintf(stdout, "Selftest error: Unable to load overflow bucket\n");
								return false;
							}
							++overflow_count;
							for (UINT_64 i = 0; i < overflowRecord->Size(); ++i)
							{
								_ItemType & item = (*overflowRecord)[i];
								if (CalcHashSlot(CalcHashFunc((const _KeyType &) item)) != slot)
								{
									fprintf(stdout, "Selftest error: Overflow bucket slot = %llu, item slot = %llu\n", slot, CalcHashSlot(CalcHashFunc((const _KeyType &) item)));
									return false;
								}
							}
							size += overflowRecord->Size();
							overflowAddress = overflowRecord->Link();
							if (overflowAddress != OverflowPool::NIL)
							{
								if (!overflowRecord->Full())
								{
									fprintf(stdout, "Selftest error: Overflow bucket has overflows, but is not full\n");
									return false;
								}
							}
						}
					}
				}
				if (size != header_->size)
				{
					fprintf(stdout, "Selftest error: size = %llu, header->size = %llu\n", size, header_->size);
					return false;
				}
				if (overflow_count != header_->overflow_count)
				{
					fprintf(stdout, "Selftest error: overflow_count = %llu, header->overflow_count = %llu\n", overflow_count, header_->overflow_count);
					return false;
				}

				return true;
			}

			UINT_64 GetRecordSize(UINT_64 slot)
			{
				UINT_64 size = 0;

				PrimaryRecord primaryRecord;
				if (!LoadPrimaryRecord(slot, primaryRecord))
				{
					return 0;
				}

				size += primaryRecord->Size();

				RecordPoolAddress overflowAddress = primaryRecord->Link();
				if (overflowAddress != OverflowPool::NIL)
				{
					OverflowRecord overflowRecord;
					while (overflowAddress != OverflowPool::NIL)
					{
						if (!LoadOverflow(overflowAddress, overflowRecord))
						{
							return 0;
						}
						size += overflowRecord->Size();
						overflowAddress = overflowRecord->Link();
					}
				}
				return size;
			}

			bool TestRecord2(UINT_64 slot)
			{
				PrimaryRecord primaryRecord;
				if (!LoadPrimaryRecord(slot, primaryRecord))
				{
					printf("TestRecord error: load primary record\n");
					return false;
				}

				for (UINT_32 i = 0; i < primaryRecord->Size(); ++i)
				{
					_ItemType & item = (*primaryRecord)[i];
					if (CalcHashSlot(CalcHashFunc((const _KeyType &) item)) != slot)
					{
						printf("TestRecord error: primary record slot error\n");
						return false;
					}
				}

				RecordPoolAddress overflowAddress = primaryRecord->Link();
				if (overflowAddress != OverflowPool::NIL)
				{
					if (!primaryRecord->Full())
					{
						printf("TestRecord error: primary bucket has overflows but is not full\n");
						return false;
					}
					OverflowRecord overflowRecord;
					while (overflowAddress != OverflowPool::NIL)
					{
						if (!storage_LoadOverflow(overflowAddress, overflowRecord))
						{
							printf("TestRecord error: unable to load overflow bucket\n");
							return false;
						}
						for (UINT_64 i = 0; i < overflowRecord->Size(); ++i)
						{
							_ItemType & item = (*overflowRecord)[i];
							if (CalcHashSlot(CalcHashFunc((const _KeyType &) item)) != slot)
							{
								printf("TestRecord error: overflow bucket slot\n");
								return false;
							}
						}
						overflowAddress = overflowRecord->Link();
						if (overflowAddress != OverflowPool::NIL && !overflowRecord->Full())
						{
							printf("TestRecord error: overflow bucket has overflows but is not full (%u %u size=%llu)\n",
								overflowAddress.chunk_index,
								overflowAddress.record_index,
								overflowRecord->Size());
							return false;
						}
					}
				}
				return true;
			}

			PrimaryRecordPtr NewPrimary()
			{
				// TODO: check created record address
				//       must be GetPrimaryAddress((1 << header_->level) + header_->split_slot)
				return FilePrimary().template New<PrimaryBucket>();
			}

			PrimaryRecordPtr GetPrimary(UINT_64 slot)
			{
				return FilePrimary().template Get<PrimaryBucket>(GetPrimaryAddress(slot));
			}

			bool LoadPrimary(UINT_64 slot, PrimaryRecord & record)
			{
				return FilePrimary().Load(GetPrimaryAddress(slot), record);
			}

			bool SavePrimary(const PrimaryRecord & record)
			{
				return FilePrimary().Save(record);
			}

			OverflowRecordPtr NewOverflow()
			{
				return poolOverflow_.New();
			}

			OverflowRecordPtr GetOverflow(const RecordPoolAddress & address)
			{
				return poolOverflow_.Get(address);
			}

			bool DeleteOverflow(const RecordPoolAddress & address)
			{
				return poolOverflow_.Delete(address);
			}

			bool LoadOverflow(const RecordPoolAddress & address, OverflowRecord & record)
			{
				return poolOverflow_.Load(address, record);
			}

			bool SaveOverflow(const OverflowRecord & record)
			{
				return poolOverflow_.Save(record);
			}

		private:

			UINT_64 CalcHashFunc(const _KeyType & key)
			{
				static _HashFunction hashFunction_;
				return hashFunction_(key);
			}

			UINT_64 CalcHashLevel(UINT_64 hash, UINT_32 level)
			{
				return hash & ((1 << level) - 1);
			}

			UINT_64 CalcHashSlot(UINT_64 hash)
			{
				UINT_64 slot = CalcHashLevel(hash, header_->level);
				if (slot < header_->split_slot)
				{
					slot = CalcHashLevel(hash, header_->level + 1);
				}
				return slot;
			}

			UINT_64 CalcNextHashSlot(const _KeyType & key)
			{
				return CalcHashLevel(CalcHashFunc(key), header_->level + 1);
			}

			File::AddressType GetPrimaryAddress(UINT_64 slot) const
			{
				return sizeof(LinearHashHeader) + slot * sizeof(PrimaryBucket);
			}

			_SourceType  * source_;
			OverflowPool   poolOverflow_;
			HeaderRecord   header_;
	};
}

#endif
