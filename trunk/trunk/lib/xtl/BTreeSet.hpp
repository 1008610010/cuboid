#ifndef XTL__BTREE_SET_HPP__
#define XTL__BTREE_SET_HPP__ 1

#include "BTree.hpp"

namespace XTL
{
	static const unsigned int BTREE_SET_DEFAULT_LEAF_CAPACITY   = 128;
	static const unsigned int BTREE_SET_DEFAULT_BRANCH_CAPACITY = 128;

	template <typename T,
	          typename Compare,
	          unsigned int LEAF_CAPACITY = BTREE_SET_DEFAULT_LEAF_CAPACITY,
	          unsigned int BRANCH_CAPACITY = BTREE_SET_DEFAULT_BRANCH_CAPACITY>
	class BTreeSet
	{
		typedef BTree<T, T, Compare, true, LEAF_CAPACITY, BRANCH_CAPACITY> BTreeType;

		public:

			BTreeSet()
				: tree_()
			{
				;;
			}

			unsigned int Size() const
			{
				return tree_.Size();
			}

			bool Empty() const
			{
				return tree_.Empty();
			}

			void Clear()
			{
				tree_.Clear();
			}

			bool Insert(const T & value)
			{
				bool inserted;

				tree_.Insert(value, inserted);

				return inserted;
			}

			unsigned long long int UsedMemory() const
			{
				return tree_.UsedMemory();
			}

			template <typename Functor>
			void ForEach(Functor & functor) const
			{
				for (typename BTreeType::ConstIterator itr = tree_.Begin(); !itr.AtEnd(); itr.Advance())
				{
					functor(*itr);
				}
			}

		private:

			BTreeType tree_;
	};
}

#endif

