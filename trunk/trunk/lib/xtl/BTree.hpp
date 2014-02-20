#ifndef XTL__BTREE_HPP__
#define XTL__BTREE_HPP__ 1

#include <string.h>

#include <algorithm> // std::swap()
#include <vector>

#include "FormatString.hpp"

namespace XTL
{
	struct PlainArrayManager
	{
		// COND: beginIndex <= endIndex
		template <typename T>
		static void Insert(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			if (count > 0)
			{
				::memmove(dest + (beginIndex + count), dest + beginIndex, (endIndex - beginIndex) * sizeof(T));
			}
		}

		// COND: beginIndex + count <= endIndex
		template <typename T>
		static void Delete(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			if (count > 0)
			{
				::memmove(dest + beginIndex, dest + (beginIndex + count), (endIndex - beginIndex - count) * sizeof(T));
			}
		}

		// COND: src != dest
		template <typename T>
		static void Move(T * dest, unsigned int toIndex, T * source, unsigned int beginIndex, unsigned int endIndex)
		{
			if (beginIndex < endIndex)
			{
				::memcpy(dest + toIndex, source + beginIndex, (endIndex - beginIndex) * sizeof(T));
			}
		}
	};

	struct ObjectArrayManager
	{
		template <typename T>
		static void Insert(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			if (count > 0)
			{
				for (unsigned int i = endIndex + count - 1; i >= beginIndex + count; --i)
				{
					std::swap(dest[i - count], dest[i]);
				}
			}
		}

		// COND: beginIndex + count <= endIndex
		template <typename T>
		static void Delete(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			if (count > 0)
			{
				for (unsigned int i = beginIndex; i < endIndex - count; ++i)
				{
					std::swap(dest[i], dest[i + count]);
				}
			}
		}

		// COND: source != dest
		template <typename T>
		static void Move(T * dest, unsigned int toIndex, T * source, unsigned int beginIndex, unsigned int endIndex)
		{
			for (unsigned int i = beginIndex, j = toIndex; i < endIndex; ++i, ++j)
			{
				std::swap(source[i], dest[j]);
			}
		}
	};

#define DECLARE_MANAGER_BODY(MANAGER, TYPE) \
	{ \
		static void Insert(TYPE * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count) \
		{ \
			MANAGER::Insert(dest, beginIndex, endIndex, count); \
		} \
		static void Delete(TYPE * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count) \
		{ \
			MANAGER::Delete(dest, beginIndex, endIndex, count); \
		} \
		static void Move(TYPE * dest, unsigned int toIndex, TYPE * source, unsigned int beginIndex, unsigned int endIndex) \
		{ \
			MANAGER::Move(dest, toIndex, source, beginIndex, endIndex); \
		} \
	}

	template <bool ITEM_IS_KEY, typename KeyType_, typename ItemType_, typename ArrayManagerItems_, typename ArrayManagerKeys_>
	struct ManagerPoolTemplate
	{
		/*
			Ugly Hack: добавлен шаблонный параметр Dummy, т.к. g++ не разрешает полностью специализировать шаблон внутри класса:
				"ошибка: явная специализация в не-namespace области"
				"error: Explicit specialization in non-namespace scope"
			При этом частичная специализация разрешена.
			Нельзя специализировать член шаблона, без полной специализации самого шаблона.
		*/
		template <typename T, typename Dummy> struct Manager        DECLARE_MANAGER_BODY(PlainArrayManager,  T);
		template <typename Dummy> struct Manager<ItemType_, Dummy>  DECLARE_MANAGER_BODY(ArrayManagerItems_, ItemType_);
		template <typename Dummy> struct Manager<KeyType_, Dummy>   DECLARE_MANAGER_BODY(ArrayManagerKeys_,  KeyType_);
	};

	template <typename ItemType_, typename KeyType_, typename ArrayManagerItems_, typename ArrayManagerKeys_>
	struct ManagerPoolTemplate<true, ItemType_, KeyType_, ArrayManagerItems_, ArrayManagerKeys_>
	{
		template <typename T, typename Dummy> struct Manager        DECLARE_MANAGER_BODY(PlainArrayManager,  T);
		template <typename Dummy> struct Manager<ItemType_, Dummy>  DECLARE_MANAGER_BODY(ArrayManagerItems_, ItemType_);
	};
#undef DECLARE_MANAGER_BODY

	template <typename X, typename Y>
	struct IsSame
	{
		enum { Value = false };
	};

	template <typename X>
	struct IsSame<X, X>
	{
		enum { Value = true };
	};

	template <typename ItemType_, typename KeyType_, typename ArrayManagerItems_, typename ArrayManagerKeys_>
	struct BTreeArrayManager
	{
		typedef ManagerPoolTemplate<IsSame<KeyType_, ItemType_>::Value, ItemType_, KeyType_, ArrayManagerItems_, ArrayManagerKeys_> ManagerPool;

		template <typename T>
		static void Insert(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			ManagerPool::template Manager<T, int>::Insert(dest, beginIndex, endIndex, count);
		}

		template <typename T>
		static void Delete(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
		{
			ManagerPool::template Manager<T, int>::Delete(dest, beginIndex, endIndex, count);
		}

		template <typename T>
		static void Move(T * dest, unsigned int toIndex, T * source, unsigned int beginIndex, unsigned int endIndex)
		{
			ManagerPool::template Manager<T, int>::Move(dest, toIndex, source, beginIndex, endIndex);
		}
	};

	static const unsigned int BTREE_DEFAULT_LEAF_CAPACITY   = 4;
	static const unsigned int BTREE_DEFAULT_BRANCH_CAPACITY = 4;

	/*
		TODO: При вставке, если лист заполнен, то сначала попробовать перераспределить элементы
		между соседним левым и правым листами. Дробить его, только в случае, если соседние тоже заполнены.

		Получение ключа:
		operator const KeyType_ & (const ItemType_ &);

		Инициализация ключом:
		ItemType_ & ItemType_::operator= (const KeyType_ &);

		Сравнение ключей:
		bool KeyComparator::operator() (const KeyType &, const KeyType &);


		LEAF_CAPACITY >= 2
		BRANCH_CAPACITY >= 2
	*/
	template <typename ItemType_,
	          typename KeyType_,
	          typename KeyComparator_,
	          bool UNIQUE_KEYS = false,
	          unsigned int LEAF_CAPACITY = BTREE_DEFAULT_LEAF_CAPACITY,
	          unsigned int BRANCH_CAPACITY = BTREE_DEFAULT_BRANCH_CAPACITY>
	class BTree
	{
		public:

			typedef ItemType_ ItemType;
			typedef KeyType_  KeyType;

			typedef PlainArrayManager ArrayManagerKeys;
			typedef PlainArrayManager ArrayManagerItems;

		private:

			static const KeyComparator_ & KEY_COMPARATOR()
			{
				static KeyComparator_ comp;
				return comp;
			}

			static bool Less(const KeyType & left, const KeyType & right)
			{
				return KEY_COMPARATOR()(left, right);
			}

			/*
				index = BinarySearch(...)
					Если ключ key найден:
						items[i] <  key, для 0 <= i < index (или "<=", если ключи могут повторяться);
						items[i] == key, для i == index;
						items[i] >  key, для i > index (или ">=", если ключи могут повторяться);
					Если ключ key не найден:
						items[i] <  key, для 0 <= i < index;
						items[i] >  key, для i >= index;
			 */
			template <typename T>
			static unsigned int BinarySearch(const KeyType & key, const T * items, unsigned int size, bool & found)
			{
				unsigned int left = 0;
				unsigned int right = size;

				while (left < right)
				{
					unsigned int middle = (left + right) >> 1;
					const KeyType & middleKey = static_cast<const KeyType &>(items[middle]);
					if (Less(middleKey, key))
					{
						left = middle + 1;
					}
					else if (Less(key, middleKey))
					{
						right = middle;
					}
					else
					{
						found = true;
						return middle;
					}
				}

				found = false;
				return left;
			}

			typedef BTreeArrayManager<ItemType, KeyType, ArrayManagerItems, ArrayManagerKeys> ArrayManager;

			template <typename T>
			static void ArrayInsert(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
			{
				ArrayManager::Insert(dest, beginIndex, endIndex, count);
			}

			template <typename T>
			static void ArrayDelete(T * dest, unsigned int beginIndex, unsigned int endIndex, unsigned int count)
			{
				ArrayManager::Delete(dest, beginIndex, endIndex, count);
			}

			template <typename T>
			static void ArrayMove(T * dest, unsigned int toIndex, T * source, unsigned int beginIndex, unsigned int endIndex)
			{
				ArrayManager::Move(dest, toIndex, source, beginIndex, endIndex);
			}

			class Leaf;
			class Branch;

			struct Node
			{
				unsigned int   level;
				unsigned int   slot;
				Branch       * parent;

				explicit Node(unsigned int level_)
					: level(level_), slot(0), parent(0) { ;; }

				virtual ~Node() throw()
				{
					;;
				}

				virtual bool IsLeaf() const = 0;

				virtual const KeyType & GetMaxKey() const = 0;

				void SetParent(Branch * parent, unsigned int slot)
				{
					this->parent = parent;
					this->slot = slot;
				}

				const Node * GetNextSibling() const
				{
					if (this->parent == 0)
					{
						return 0;
					}

					if (this->slot < this->parent->size)
					{
						return this->parent->childs[slot + 1];
					}

					const Branch * nextParent = static_cast<const Branch *>(this->parent->GetNextSibling());
					return nextParent == 0 ? 0 : nextParent->childs[0];
				}

				const Node * GetPreviousSibling() const
				{
					if (this->parent == 0)
					{
						return 0;
					}

					if (this->slot > 0)
					{
						return this->parent->childs(this->slot - 1);
					}

					const Branch * prevParent = static_cast<const Branch *>(this->parent->GetPreviousSibling());
					return prevParent == 0 ? 0 : prevParent->childs[prevParent->size];
				}
			};

			struct Leaf : public Node
			{
				Leaf         * next;
				Leaf         * prev;
				unsigned int   size;
				ItemType       items[LEAF_CAPACITY];

				explicit Leaf()
					: Node(0), next(0), prev(0), size(0) { ;; }

				virtual ~Leaf() throw()
				{
					;;
				}

				virtual bool IsLeaf() const
				{
					return true;
				}

				virtual const KeyType & GetMaxKey() const
				{
					return GetKey(this->size - 1);
				}

				const KeyType & GetKey(unsigned int slot) const
				{
					return static_cast<const KeyType &>(this->items[slot]);
				}

				unsigned int FindSlot(const KeyType & key, bool & found) const
				{
					return BinarySearch(key, this->items, this->size, found);
				}

				ItemType & Insert(unsigned int slot, const KeyType & key)
				{
					// TODO: сделать глобальное множество флагов: расставить в различные методы флаги

					ArrayInsert(this->items, slot, this->size, 1);

					// TODO: нахера тут вызов деструктора?
					this->items[slot].~ItemType();
					new (this->items + slot) ItemType(key);

					++this->size;

					/*
					if (slot == this->size - 1 && this->parent != 0)
					{
						this->parent->UpdateKey(this->slot, GetMaxKey());
					}
					*/

					return this->items[slot];
				}

				ItemType & InsertAtEnd(const KeyType & key, KeyType & newKey, Node *& newNode)
				{
					if (this->size < LEAF_CAPACITY)
					{
						ItemType & newItem = this->items[this->size];
						newItem = key;
						++this->size;

						newNode = 0;

						return newItem;
					}

					Leaf * newLeaf = new Leaf();
					newLeaf->next = 0;
					newLeaf->prev = this;
					this->next = newLeaf;

					newLeaf->items[0] = key;
					newLeaf->size = 1;

					newKey = this->GetMaxKey();
					newNode = newLeaf;

					return newLeaf->items[0];
				}

				ItemType & InsertAndSplit(unsigned int slot, const KeyType & key, KeyType & newKey, Node *& newNode)
				{
					const unsigned int newSize = (this->size + 1) / 2;

					Leaf * newLeaf = new Leaf();
					newLeaf->next = this->next;
					newLeaf->prev = this;
					if (this->next != 0)
					{
						this->next->prev = newLeaf;
					}
					this->next = newLeaf;

					Leaf * leafToInsert = this;
					if (slot < newSize)
					{
						ArrayMove(newLeaf->items, 0, this->items, newSize - 1, this->size);
						ArrayInsert(this->items, slot, newSize - 1, 1);
					}
					else
					{
						slot -= newSize;
						ArrayMove(newLeaf->items, 0, this->items, newSize, newSize + slot);
						ArrayMove(newLeaf->items, slot + 1, this->items, newSize + slot, this->size);
						leafToInsert = newLeaf;
					}

					leafToInsert->items[slot] = key;

					newLeaf->size = this->size + 1 - newSize;
					this->size = newSize;

					newNode = newLeaf;
					newKey = this->GetMaxKey();

					return leafToInsert->items[slot];
				}

				ItemType & InsertAndRearrangeWithRight(unsigned int slot, const KeyType & key, Leaf * rightLeaf)
				{
					// printf("InsertAndRearrangeWithRight\n");
					const unsigned int totalSize = this->size + rightLeaf->size + 1;
					const unsigned int newSize = totalSize / 2;
					const unsigned int itemsToMove = this->size + 1 - newSize;
					Leaf * leafToInsert = this;

					if (slot < newSize)
					{
						/*
							[ X X N X X X | X X X Y Y Y ]
							  0 1   2 3 4   5 6 7 8 9 a
							[ X X X X X N | X X X Y Y Y ]
							  0 1 2 3 4     5 6 7 8 9 a
						*/

						ArrayInsert(rightLeaf->items, 0, rightLeaf->size, itemsToMove);
						ArrayMove(rightLeaf->items, 0, this->items, newSize - 1, this->size);
						ArrayInsert(this->items, slot, newSize - 1, 1);
					}
					else // slot >= newSize
					{
						/*
							[ X X X X X X | N X X Y Y Y ]
							[ X X X X X X | X N X Y Y Y ]
						*/

						ArrayInsert(rightLeaf->items, 0, rightLeaf->size, itemsToMove);
						ArrayMove(rightLeaf->items, 0, this->items, newSize, slot);
						ArrayMove(rightLeaf->items, slot - newSize + 1, this->items, slot, this->size);

						leafToInsert = rightLeaf;
						slot -= newSize;
					}

					leafToInsert->items[slot] = key;

					this->size = newSize;
					rightLeaf->size = totalSize - newSize;

					this->parent->UpdateKey(this->slot, this->GetMaxKey());

					return leafToInsert->items[slot];
				}

				ItemType & InsertAndRearrangeWithLeft(unsigned int slot, const KeyType & key, Leaf * leftLeaf)
				{
					// printf("InsertAndRearraneWithLeft\n");
					const unsigned int totalSize = this->size + leftLeaf->size + 1;
					const unsigned int newSize = totalSize / 2;
					const unsigned int itemsToMove = this->size + 1 - newSize;
					Leaf * leafToInsert = this;

					if (slot >= itemsToMove)
					{
						/*
							[ Y Y Y X X X | X X N X X X ]

							[ . . . A A B B B ]
							[ A A . . . B B B ]
							[ A A . B B B ]
						*/
						ArrayMove(leftLeaf->items, leftLeaf->size, this->items, 0, itemsToMove);
						ArrayDelete(this->items, 0, slot, itemsToMove);
						ArrayDelete(this->items, slot - itemsToMove + 1, this->size, itemsToMove - 1);
						slot -= itemsToMove;
					}
					else // slot < itemsToMove
					{
						/*
							[ Y Y Y X N X | X X X X X X ]
						*/
						ArrayMove(leftLeaf->items, leftLeaf->size, this->items, 0, slot);
						ArrayMove(leftLeaf->items, leftLeaf->size + slot + 1, this->items, slot, itemsToMove - 1);
						ArrayDelete(this->items, 0, this->size, itemsToMove - 1);

						leafToInsert = leftLeaf;
						slot += leftLeaf->size;
					}

					leafToInsert->items[slot] = key;

					this->size = newSize;
					leftLeaf->size = totalSize - newSize;

					leftLeaf->parent->UpdateKey(leftLeaf->slot, leftLeaf->GetMaxKey());

					return leafToInsert->items[slot];
				}

				void Delete(unsigned int slot)
				{
					this->items[slot] = ItemType();

					ArrayDelete(this->items, slot, this->size, 1);

					--this->size;

					/*
						Если удалялся последний элемент из листа и дерево состоит из более чем одного листа,
						необходимо обновить ключ листа в родительской ветке.
					*/
					if (slot == this->size && this->parent != 0)
					{
						if (this->size != 0)
						{
							this->parent->UpdateKey(this->slot, GetMaxKey());
						}
						else
						{
							/*
								После удаления лист оказался пустым (возможно в случае LEAF_CAPACITY = 2).
								Тогда значение ключа в родительской ветке берем из левого соседнего листа.
								В случае если левого соседа нет (this == firstLeaf_), не делаем ничего. Так как этот лист впоследсвие
								будет перераспределен с соседом справа и, либо его ключ обновится, либо будет удален.
							*/
							Leaf * leftLeaf = this->prev;
							if (leftLeaf != 0)
							{
								this->parent->UpdateKey(this->slot, leftLeaf->GetMaxKey());
							}
						}
					}
				}

				Leaf * RearrangeItems()
				{
					Leaf * leftLeaf = 0;
					Leaf * rightLeaf = 0;
					if (this->slot == 0)
					{
						leftLeaf = this;
						rightLeaf = static_cast<Leaf *>(this->parent->childs[this->slot + 1]);
					}
					else
					{
						leftLeaf = static_cast<Leaf *>(this->parent->childs[this->slot - 1]);
						rightLeaf = this;
					}

					const unsigned int totalSize = leftLeaf->size + rightLeaf->size;

					if (totalSize <= LEAF_CAPACITY)
					{
						/*
							Все элементы из обоих листов помещаются в один. Тогда перемещаем все элементы из правого листа в левый.
						*/
						ArrayMove(leftLeaf->items, leftLeaf->size, rightLeaf->items, 0, rightLeaf->size);
						leftLeaf->size = totalSize;
						rightLeaf->size = 0;
					}
					else
					{
						const unsigned int leftSize = totalSize / 2;
						const unsigned int rightSize = totalSize - leftSize; // = leftLeaf->size + rightLeaf->size - leftSize

						if (leftSize < leftLeaf->size)
						{
							ArrayInsert(rightLeaf->items, 0, rightLeaf->size, leftLeaf->size - leftSize);
							ArrayMove(rightLeaf->items, 0, leftLeaf->items, leftSize, leftLeaf->size - leftSize);
						}
						else
						{
							/*
								totalSize > LEAF_CAPACITY =>
									leftSize  >= [ LEAF_CAPACITY / 2 ]
									rightSize >= [ LEAF_CAPACITY / 2 ]

								Но метод RearrangeItems вызывается только в случае leaf->size < [ LEAF_CAPACITY / 2 ].
								Таким образом leftSize - leftLeaf->size = rightLeaf->size - rightSize != 0.
								Поэтому здесь leftSize > leafLeaf->size.
							*/
							ArrayMove(leftLeaf->items, leftLeaf->size, rightLeaf->items, 0, rightLeaf->size - rightSize);
							ArrayDelete(rightLeaf->items, 0, rightLeaf->size, rightLeaf->size - rightSize);
						}

						leftLeaf->size = leftSize;
						rightLeaf->size = rightSize;

						/*
							Максимальный элемент левого листа изменился, необходимо обновить ключ в родительской ветке.
						*/
						leftLeaf->parent->UpdateKey(leftLeaf->slot, leftLeaf->GetMaxKey());
					}

					return rightLeaf;
				}
			};

			struct Branch : public Node
			{
				unsigned int   size;
				KeyType        keys[BRANCH_CAPACITY];
				Node         * childs[BRANCH_CAPACITY + 1]; // Childs! NOT children, bu-ga-ga...

				explicit Branch(unsigned int level)
					: Node(level), size(0) { ;; }

				Branch(unsigned int level, const KeyType & key, Node * first, Node * second)
					: Node(level), size(1)
				{
					this->keys[0] = key;
					this->childs[0] = first;
					this->childs[1] = second;
					UpdateChildrenLinks();
				}

				virtual ~Branch() throw()
				{
					for (unsigned int i = 0; i <= this->size; ++i)
					{
						delete this->childs[i];
					}
				}

				virtual bool IsLeaf() const
				{
					return false;
				}

				virtual const KeyType & GetMaxKey() const
				{
					return this->childs[this->size]->GetMaxKey();
				}

				unsigned int FindSlot(const KeyType & key, bool & found) const
				{
					return BinarySearch(key, this->keys, this->size, found);
				}

				void Insert(unsigned int slot, const KeyType & key, Node * child)
				{
					ArrayInsert(this->keys, slot, this->size, 1);
					this->keys[slot] = key;

					ArrayInsert(this->childs, slot + 1, this->size + 1, 1);
					this->childs[slot + 1] = child;

					++this->size;

					UpdateChildrenLinks(slot + 1, this->size);
				}

				void InsertAndSplit(unsigned int slot, const KeyType & key, Node * node, KeyType & newKey, Node *& newNode)
				{
					const unsigned int totalSize = this->size + 1;
					const unsigned int newSize = totalSize / 2;

					Branch * newBranch = new Branch(this->level);
					newBranch->size = totalSize - newSize - 1;

					/*
						Необходимо разбить массив размером totalSize:
							K[i] = this->keys[i],     0 <= i < slot
							K[i] = key,               i = slot
							K[i] = this->keys[i - 1], slot < i < totalSize
						на два массива с newSize и newBranch->size элементами.
					*/
					if (slot < newSize)
					{
						std::swap(newKey, this->keys[newSize - 1]);

						ArrayMove(newBranch->keys, 0, this->keys, newSize, this->size);
						ArrayInsert(this->keys, slot, newSize - 1, 1);
						this->keys[slot] = key;
					}
					else if (slot > newSize)
					{
						std::swap(newKey, this->keys[newSize]);

						ArrayMove(newBranch->keys, 0, this->keys, newSize + 1, slot);
						ArrayMove(newBranch->keys, slot - newSize, this->keys, slot, this->size);
						newBranch->keys[slot - newSize - 1] = key;
					}
					else // slot == newSize
					{
						newKey = key;

						ArrayMove(newBranch->keys, 0, this->keys, newSize, this->size);
					}

					/*
						Необходимо разбить массив размером (totalSize + 1)
							C[i] = this->childs[i],     0 <= i < slot + 1
							C[i] = node,                i = slot + 1
							C[i] = this->childs[i + 1], slot + 1 < i < totalSize + 1
						на два массива размерами (newSize + 1) и (newBranch->size + 1).
					*/
					if (slot < newSize)
					{
						ArrayMove(newBranch->childs, 0, this->childs, newSize, this->size + 1);
						ArrayInsert(this->childs, slot + 1, newSize, 1);
						this->childs[slot + 1] = node;
						this->UpdateChildrenLinks(slot + 1, newSize);
					}
					else // slot >= newSize
					{
						ArrayMove(newBranch->childs, 0, this->childs, newSize + 1, slot + 1);
						ArrayMove(newBranch->childs, slot - newSize + 1, this->childs, slot + 1, this->size + 1);
						newBranch->childs[slot - newSize] = node;
					}

					this->size = newSize;

					newBranch->UpdateChildrenLinks();

					newNode = newBranch;
				}

				void Delete(unsigned int slot)
				{
					// HERE: 1 <= slot <= size_
					// Удаляем потомка с индексом slot и ключ с индексом (slot - 1)

					ArrayDelete(this->keys,   slot - 1, this->size,     1);
					ArrayDelete(this->childs, slot,     this->size + 1, 1);

					--this->size;

					UpdateChildrenLinks(slot, this->size);
				}

				Branch * RearrangeKeys()
				{
					Branch * leftBranch = 0;
					Branch * rightBranch = 0;
					if (this->slot == 0)
					{
						leftBranch = this;
						rightBranch = static_cast<Branch *>(this->parent->childs[this->slot + 1]);
					}
					else
					{
						leftBranch = static_cast<Branch *>(this->parent->childs[this->slot - 1]);
						rightBranch = this;
					}

					unsigned int totalSize = leftBranch->size + rightBranch->size;

					if (totalSize < BRANCH_CAPACITY)
					{
						// Перемещаем все ключи из правой ветки в левую, правая после этого становится пустой.

						std::swap(leftBranch->keys[leftBranch->size], leftBranch->GetParentKey());
						// Ключ у родителя для ветки leftBranch стал пустым

						leftBranch->MoveItems(leftBranch->size + 1, rightBranch, 0, rightBranch->size);

						leftBranch->UpdateChildrenLinks(leftBranch->size + 1, totalSize + 1);

						leftBranch->size = totalSize + 1;
						rightBranch->size = 0;
					}
					else
					{
						unsigned int leftSize = totalSize / 2;
						unsigned int rightSize = totalSize - leftSize;

						if (leftSize < leftBranch->size)
						{
							// Перемещаем ключи с левой ветки на правую

							rightBranch->Unshift(leftBranch->size - leftSize);

							std::swap(rightBranch->keys[leftBranch->size - leftSize - 1], leftBranch->GetParentKey());
							std::swap(leftBranch->GetParentKey(), leftBranch->keys[leftSize]);

							rightBranch->MoveItems(0, leftBranch, leftSize + 1, leftBranch->size);

							leftBranch->size = leftSize;
							rightBranch->size = rightSize;
							rightBranch->UpdateChildrenLinks();
						}
						else // leftSize > leftBranch->Size()
						{
							// Перемещаем ключи с правой ветки на левую
							std::swap(leftBranch->keys[leftBranch->size], leftBranch->GetParentKey());
							std::swap(leftBranch->GetParentKey(), rightBranch->keys[rightBranch->size - rightSize - 1]);

							leftBranch->MoveItems(leftBranch->size + 1, rightBranch, 0, rightBranch->size - rightSize - 1);
							rightBranch->Shift(rightBranch->size - rightSize);

							leftBranch->UpdateChildrenLinks(leftBranch->size + 1, leftSize);
							leftBranch->size = leftSize;
							rightBranch->size = rightSize;
							rightBranch->UpdateChildrenLinks();
						}
					}

					return rightBranch;
				}

				KeyType & GetParentKey() const
				{
					return this->parent->keys[this->slot];
				}

				void UpdateChildLink(unsigned int slot)
				{
					this->childs[slot]->SetParent(this, slot);
				}

				void UpdateChildrenLinks(unsigned int beginIndex, unsigned int endIndex)
				{
					for (unsigned int i = beginIndex; i <= endIndex; ++i)
					{
						UpdateChildLink(i);
					}
				}

				void UpdateChildrenLinks()
				{
					UpdateChildrenLinks(0, this->size);
				}

				void UpdateKey(unsigned int slot, const KeyType & key)
				{
					if (slot < this->size)
					{
						this->keys[slot] = key;
					}
					else if (this->parent != 0)
					{
						this->parent->UpdateKey(this->slot, key);
					}
				}

				void Shift(unsigned int count)
				{
					ArrayDelete(this->keys,   0, this->size,     count);
					ArrayDelete(this->childs, 0, this->size + 1, count);
				}

				void Unshift(unsigned int count)
				{
					ArrayInsert(this->keys,   0, this->size,     count);
					ArrayInsert(this->childs, 0, this->size + 1, count);
				}

				void MoveItems(unsigned int destIndex, Branch * branch, unsigned int beginIndex, unsigned int endIndex)
				{
					ArrayMove(this->keys,   destIndex, branch->keys,   beginIndex, endIndex);
					ArrayMove(this->childs, destIndex, branch->childs, beginIndex, endIndex + 1);
				}
			};

		public:

			BTree()
				: root_(0),
				  firstLeaf_(0),
				  size_(0),
				  leavesCount_(0),
				  branchesCount_(0)
			{
				;;
			}

			~BTree() throw()
			{
				delete root_;
			}

			unsigned int Size() const
			{
				return size_;
			}

			bool Empty() const
			{
				return size_ == 0;
			}

			unsigned int LeavesCount() const
			{
				return leavesCount_;
			}

			unsigned int BranchesCount() const
			{
				return branchesCount_;
			}

			unsigned long long int UsedMemory() const
			{
				return static_cast<unsigned long long int>(leavesCount_) * sizeof(Leaf) +
				       static_cast<unsigned long long int>(branchesCount_) * sizeof(Branch);
			}

			void Clear()
			{
				delete root_;
				root_ = 0;
				firstLeaf_ = 0;
				size_ = 0;
				leavesCount_ = 0;
				branchesCount_ = 0;
			}

			ItemType & Insert(const KeyType & key, bool & inserted)
			{
				if (root_ == 0)
				{
					firstLeaf_ = new Leaf();
					++leavesCount_;
					root_ = firstLeaf_;
				}

				inserted = false;

				Node * newNode = 0;
				KeyType newKey = KeyType();

				ItemType & newItem = InsertIntoNode(root_, key, inserted, newKey, newNode);

				if (inserted)
				{
					++size_;

					if (newNode != 0)
					{
						root_ = new Branch(root_->level + 1, newKey, root_, newNode);
						++branchesCount_;
					}
				}

				return newItem;
			}

			bool Delete(const KeyType & key)
			{
				unsigned int slot;
				Leaf * leaf = Find(key, slot);

				if (leaf == 0)
				{
					return false;
				}

				DeleteFromLeaf(leaf, slot);
				return true;
			}

			void DebugPrint() const
			{
				if (root_ == 0)
				{
					fprintf(stdout, "\n");
					return;
				}

				std::vector<std::string> lines(root_->level + 1);
				DebugPrintNode(root_, lines.begin(), lines.end());

				for (unsigned int i = 0; i < lines.size(); ++i)
				{
					fprintf(stdout, "%s\n", lines[i].c_str());
				}

				fprintf(stdout, "\n");
			}

			static void AlignString(std::string & dest, const std::string & src)
			{
				if (src.length() > dest.length())
				{
					dest += std::string(src.length() - dest.length(), ' ');
				}
			}

			void DebugPrintNode(const Node * node, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) const
			{
				if (node->IsLeaf())
				{
					// printf("DebugPrint Leaf %p\n", node);
					const Leaf * leaf = static_cast<const Leaf *>(node);

					*begin += "["; // + FormatString("%p(%u) ", leaf, leaf->slot);

					for (unsigned int i = 0; i < leaf->size; ++i)
					{
						*begin += FormatString("%i ", leaf->items[i]);
					}

					if (leaf->size > 0)
					{
						begin->erase(begin->length() - 1, 1);
					}
					*begin += "]";
				}
				else
				{
					const Branch * branch = static_cast<const Branch *>(node);
					// printf("DebugPrint Branch %p with %u children\n", node, branch->size);

					*begin += "( "; // + FormatString("%p(%u)", branch, branch->slot);

					// printf("  Child #0\n");
					DebugPrintNode(branch->childs[0], begin + 1, end);

					for (unsigned int i = 0; i < branch->size; ++i)
					{
						AlignString(*begin, *(begin + 1));
						*begin += FormatString("%i", branch->keys[i]);
						for (std::vector<std::string>::iterator itr = begin + 1; itr != end; ++itr)
						{
							AlignString(*itr, *begin);
						}
						// printf("  Child #%u\n", i + 1);
						DebugPrintNode(branch->childs[i + 1], begin + 1, end);
					}

					AlignString(*begin, *(begin + 1));
					begin->erase(begin->length() - 1, 1);
					*begin += ")";
					for (std::vector<std::string>::iterator itr = begin + 1; itr != end; ++itr)
					{
						AlignString(*itr, *begin);
					}
				}
			}

			class ConstIterator
			{
				public:

					ConstIterator(const Leaf * leaf, unsigned int slot)
						: leaf_(leaf), slot_(slot) { ;; }

					bool AtEnd() const
					{
						return leaf_ == 0;
					}

					const ItemType & operator* () const
					{
						return leaf_->items[slot_];
					}

					void Advance()
					{
						if (leaf_ == 0)
						{
							return;
						}

						++slot_;
						if (slot_ >= leaf_->size)
						{
							slot_ = 0;
							// leaf_ = static_cast<const Leaf *>(leaf_->GetNextSibling());
							leaf_ = leaf_->next;
						}
					}

				private:

					const Leaf   * leaf_;
					unsigned int   slot_;
			};

			const ConstIterator Begin() const
			{
				return ConstIterator(firstLeaf_, 0);
			}

			bool SelfTest(bool checkLeafTail = false, bool checkBranchTail = false) const
			{
				Tester tester(*this, checkLeafTail, checkBranchTail);

				return tester.Run();
				/*
					Пройтись рекурсивно
					1. Посчитать количество листов - сверить с leavesCount_;
					2. Посчитать количество веток - сверить с branchesCount_;
					3. Посчитать сумму количества элементов во всех листах - сверить с size_;
					Проверить this->parent == 0 || this->parent->level = this->level + 1
					this->IsLeaf() == (this->level == 0)
				*/
			}

			// TODO: realize it
			class Tester
			{
				public:

					Tester(const BTree & tree, bool checkLeafTail, bool checkBranchTail)
						: tree_(tree),
						  checkLeafTail_(checkLeafTail),
						  checkBranchTail_(checkBranchTail),
						  isFirstLeaf_(true),
						  prevLeaf_(0),
						  nextLeaf_(0),
						  currentKey_(),
						  itemsCount_(0),
						  leavesCount_(0),
						  branchesCount_(0)
					{
						;;
					}

					bool Run()
					{
						if (tree_.root_ == 0)
						{
							return tree_.firstLeaf_ == 0 && tree_.size_ == 0 && tree_.leavesCount_ == 0 && tree_.branchesCount_ == 0;
						}

						if (!TestNode(tree_.root_, 0, 0, tree_.root_->level + 1))
						{
							return false;
						}

						if (nextLeaf_ != 0)
						{
							// throw std::runtime_error("Last leaf does not point to null\n");
							return false;
						}

						if (itemsCount_ != tree_.size_)
						{
							return false;
						}

						if (leavesCount_ != tree_.leavesCount_)
						{
							return false;
						}

						if (branchesCount_ != tree_.branchesCount_)
						{
							return false;
						}

						return true;
					}

				private:

					bool TestNode(const Node * node, const Branch * parent, unsigned int slot, unsigned int parentLevel)
					{
						if (node->parent != parent)
						{
							return false;
						}

						if (node->parent == 0 && node->slot != 0)
						{
							return false;
						}

						if (node->parent != 0 && node->slot != slot)
						{
							return false;
						}

						if (node->level + 1 != parentLevel)
						{
							return false;
						}

						if (node->IsLeaf())
						{
							return TestLeaf(static_cast<const Leaf *>(node));
						}
						else
						{
							return TestBranch(static_cast<const Branch *>(node));
						}
					}

					bool TestLeaf(const Leaf * leaf)
					{
						++leavesCount_;
						if (leaf->parent != 0 && leaf->size < LEAF_CAPACITY / 2)
						{
							return false;
						}

						if (leaf->size == 0)
						{
							return false;
						}

						if (leaf->level != 0)
						{
							return false;
						}

						itemsCount_ += leaf->size;

						unsigned int from = 0;

						if (isFirstLeaf_)
						{
							from = 1;
							currentKey_ = leaf->GetKey(0);
							isFirstLeaf_ = false;
							if (leaf->prev != 0)
							{
								return false;
							}
						}
						else
						{
							if (leaf->prev != prevLeaf_)
							{
								return false;
							}

							if (leaf != nextLeaf_)
							{
								return false;
							}
						}

						prevLeaf_ = leaf;
						nextLeaf_ = leaf->next;

						for (unsigned int i = from; i < leaf->size; ++i)
						{
							const KeyType & leafKey = leaf->GetKey(i);

							bool isOrdered = Less(currentKey_, leafKey);
							if (!UNIQUE_KEYS)
							{
								isOrdered = isOrdered || !Less(leafKey, currentKey_);
							}

							if (!isOrdered)
							{
								return false;
							}

							currentKey_ = leafKey;
						}

						if (checkLeafTail_)
						{
							static const ItemType EMPTY_ITEM = ItemType();

							for (unsigned int i = leaf->size; i < LEAF_CAPACITY; ++i)
							{
								if (!(leaf->items[i] == EMPTY_ITEM))
								{
									return false;
								}
							}
						}

						return true;
					}

					bool TestBranch(const Branch * branch)
					{
						++branchesCount_;

						if (branch->parent != 0 && branch->size < BRANCH_CAPACITY / 2)
						{
							return false;
						}

						if (branch->level == 0)
						{
							return false;
						}

						if (!TestNode(branch->childs[0], branch, 0, branch->level))
						{
							return false;
						}

						if (isFirstLeaf_)
						{
							return false;
						}

						for (unsigned int i = 0; i < branch->size; ++i)
						{
							if (Less(currentKey_, branch->keys[i]) || Less(branch->keys[i], currentKey_))
							{
								return false;
							}

							if (!TestNode(branch->childs[i + 1], branch, i + 1, branch->level))
							{
								return false;
							}
						}

						if (checkBranchTail_)
						{
							static const KeyType EMPTY_KEY = KeyType();

							for (unsigned int i = branch->size; i < BRANCH_CAPACITY; ++i)
							{
								if (!(branch->keys[i] == EMPTY_KEY))
								{
									return false;
								}
							}
						}

						return true;
					}

					const BTree  & tree_;
					const bool     checkLeafTail_;
					const bool     checkBranchTail_;
					bool           isFirstLeaf_;
					const Leaf   * prevLeaf_;
					const Leaf   * nextLeaf_;
					KeyType        currentKey_;
					unsigned int   itemsCount_;
					unsigned int   leavesCount_;
					unsigned int   branchesCount_;
			};

		private:

			ItemType & InsertIntoNode(Node * node, const KeyType & key, bool & inserted, KeyType & newKey, Node *& newNode)
			{
				if (node->IsLeaf())
				{
					return InsertIntoLeaf(static_cast<Leaf *>(node), key, inserted, newKey, newNode);
				}
				else
				{
					return InsertIntoBranch(static_cast<Branch *>(node), key, inserted, newKey, newNode);
				}
			}

			ItemType & InsertIntoLeaf(Leaf * leaf, const KeyType & key, bool & inserted, KeyType & newKey, Node *& newNode)
			{
				// printf("InsertIntoLeaf %p\n", (void *) leaf);
				/*
				// TODO: сделать быструю загрузку отсортированных значений в конец дерева
				inserted = true;
				ItemType & newItem = leaf->InsertAtEnd(key, newKey, newNode);
				if (newNode != 0)
				{
					++leavesCount_;
				}
				return newItem;
				*/

				bool found = false;
				unsigned int slot = leaf->FindSlot(key, found);

				if (found && UNIQUE_KEYS)
				{
					inserted = false;
					return leaf->items[slot];
				}

				inserted = true;

				if (leaf->size < LEAF_CAPACITY)
				{
					/*
						В соответствии с реализацией метода FindSlot, в конец листа можем вставить, только если это последний лист (с максимальными значениями ключей).
						Таким образом leaf->GetMaxKey() не изменяется, и нет необходимости обновлять ключ вышестоящей ветки.

							leaf->items[i] < key, для всех 0 <= i < slot

							       slot
							       +
							(      c     x     x    )
							[a b c] [...] [...] [...]
					*/

					newNode = 0;

					return leaf->Insert(slot, key);
				}

				/*
					Здесь leaf->size == LEAF_CAPACITY.
					Перед разбивкой пробуем перераспределить значения между соседними листами, чтобы освободить место под вставляемый элемент.
				*/
				if (leaf->parent != 0)
				{
					Leaf * leftLeaf = leaf->prev;
					Leaf * rightLeaf = leaf->next;

					if (leftLeaf == 0)
					{
						if (rightLeaf->size < LEAF_CAPACITY)
						{
							return leaf->InsertAndRearrangeWithRight(slot, key, rightLeaf);
						}
					}
					else if (rightLeaf == 0)
					{
						if (leftLeaf->size < LEAF_CAPACITY)
						{
							return leaf->InsertAndRearrangeWithLeft(slot, key, leftLeaf);
						}
					}
					else if (leftLeaf->size < rightLeaf->size)
					{
						return leaf->InsertAndRearrangeWithLeft(slot, key, leftLeaf);
					}
					else if (rightLeaf->size < LEAF_CAPACITY)
					{
						return leaf->InsertAndRearrangeWithRight(slot, key, rightLeaf);
					}
				}

				// Ничего не вышло. Придется добавлять новый лист.
				++leavesCount_;

				return leaf->InsertAndSplit(slot, key, newKey, newNode);
			}

			ItemType & InsertIntoBranch(Branch * branch, const KeyType & key, bool & inserted, KeyType & newKey, Node *& newNode)
			{
				// printf("InsertIntoBranch %p\n", (void *) branch);
				bool found = false;
				unsigned int slot = branch->FindSlot(key, found);

				Node * newChildNode = 0;
				KeyType newChildKey = KeyType();

				ItemType & newItem = InsertIntoNode(branch->childs[slot], key, inserted, newChildKey, newChildNode);

				if (!inserted || newChildNode == 0)
				{
					return newItem;
				}

				if (branch->size < BRANCH_CAPACITY)
				{
					branch->Insert(slot, newChildKey, newChildNode);
				}
				else
				{
					++branchesCount_;
					branch->InsertAndSplit(slot, newChildKey, newChildNode, newKey, newNode);
				}

				return newItem;
			}

			void DeleteFromLeaf(Leaf * leaf, unsigned int slot)
			{
				// printf("DeleteFromLeaf: %p %u\n", leaf, slot);
				leaf->Delete(slot);
				--size_;

				if (leaf->size >= LEAF_CAPACITY / 2)
				{
					return;
				}

				if (leaf->parent == 0)
				{
					/*
						Дерево состоит из одного листа. Если после удаления элемента из листа он стал пуст, то удаляем лист.
					*/
					if (leaf->size == 0)
					{
						delete root_;
						--leavesCount_;

						root_ = 0;
						firstLeaf_ = 0;
					}
				}
				else
				{
					Leaf * rightLeaf = leaf->RearrangeItems();
					if (rightLeaf->size == 0)
					{
						Leaf * leftLeaf = rightLeaf->prev;
						DeleteFromBranch(rightLeaf->parent, rightLeaf->slot);

						leftLeaf->next = rightLeaf->next;
						if (rightLeaf->next != 0)
						{
							rightLeaf->next->prev = leftLeaf;
						}

						delete rightLeaf;
						--leavesCount_;
					}
				}
			}

			void DeleteFromBranch(Branch * branch, unsigned int slot)
			{
				// printf("DeleteFromBranch %p @ %u\n", (void *) branch, slot);
				branch->Delete(slot);

				if (branch->size >= BRANCH_CAPACITY / 2)
				{
					return;
				}

				if (branch->parent == 0)
				{
					if (branch->size == 0)
					{
						root_ = branch->childs[0];
						root_->SetParent(0, 0);

						branch->childs[0] = 0;
						delete branch;

						--branchesCount_;
					}
				}
				else
				{
					// HERE: branch->parent->size >= [BRANCH_CAPACITY / 2] >= 1, т.е. у родителя как минимум два потомка.
					Branch * rightBranch = branch->RearrangeKeys();
					if (rightBranch->size == 0)
					{
						DeleteFromBranch(rightBranch->parent, rightBranch->slot);

						rightBranch->childs[0] = 0;
						delete rightBranch;

						--branchesCount_;
					}
				}
			}

			Leaf * Find(const KeyType & key, unsigned int & slot) const
			{
				return root_ == 0 ? 0 : FindInNode(root_, key, slot);
			}

			Leaf * FindInNode(Node * node, const KeyType & key, unsigned int & slot) const
			{
				bool found = false;
				// TODO: create virtual method Node::Find(key, & slot)
				if (node->IsLeaf())
				{
					Leaf * leaf = static_cast<Leaf *>(node);
					slot = leaf->FindSlot(key, found);
					return found ? leaf : 0;
				}
				else
				{
					const Branch * branch = static_cast<const Branch *>(node);
					unsigned int branchSlot = branch->FindSlot(key, found);
					return FindInNode(branch->childs[branchSlot], key, slot);
				}
			}

			Node         * root_;
			Leaf         * firstLeaf_;
			unsigned int   size_;
			unsigned int   leavesCount_;
			unsigned int   branchesCount_;
	};
}

#endif

