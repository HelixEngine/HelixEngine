#pragma once
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <cassert>

namespace helix
{
	//感谢Kiwano IntrusiveList的源码支持

	template<typename PointerType>
	class IntrusiveList
	{
	public:
		using value_type = typename std::pointer_traits<PointerType>::pointer;
		using pointer = value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;

		IntrusiveList() :
			first()
			, last()
		{
		}

		~IntrusiveList()
		{
			clear();
		}

		const value_type& getFirst() const
		{
			return first;
		}

		value_type& getFirst()
		{
			return first;
		}

		const value_type& getLast() const
		{
			return last;
		}

		value_type& getLast()
		{
			return last;
		}

		[[nodiscard]] bool isEmpty() const
		{
			return first == nullptr;
		}

		void pushBack(const_reference child)
		{
			if (child->getPrev())
				child->getPrev()->getNext() = child->getNext();
			if (child->getNext())
				child->getNext()->getPrev() = child->getPrev();

			child->getPrev() = last;
			child->getNext() = nullptr;

			if (first)
			{
				last->getNext() = child;
			} else
			{
				first = child;
			}

			last = child;
		}

		void pushFront(const_reference child)
		{
			if (child->getPrev())
				child->getPrev()->getNext() = child->getNext();
			if (child->getNext())
				child->getNext()->getPrev() = child->getPrev();

			child->getPrev() = nullptr;
			child->getNext() = first;

			if (first)
			{
				first->getPrev() = child;
			} else
			{
				last = child;
			}

			first = child;
		}

		void insertBefore(reference child, reference before)
		{
			if (child->getPrev())
				child->getPrev()->getNext() = child->getNext();
			if (child->getNext())
				child->getNext()->getPrev() = child->getPrev();

			if (before->getPrev())
				before->getPrev()->getNext() = child;
			else
				first = child;

			child->getPrev() = before->getPrev();
			child->getNext() = before;
			before->getPrev() = child;
		}

		void insertAfter(reference child, reference after)
		{
			if (child->getPrev())
				child->getPrev()->getNext() = child->getNext();
			if (child->getNext())
				child->getNext()->getPrev() = child->getPrev();

			if (after->getNext())
				after->getNext()->getPrev() = child;
			else
				last = child;

			child->getNext() = after->getNext();
			child->getPrev() = after;
			after->getNext() = child;
		}

		void remove(reference child)
		{
			if (child->getNext())
			{
				child->getNext()->getPrev() = child->getPrev();
			} else
			{
				last = child->getPrev();
			}

			if (child->getPrev())
			{
				child->getPrev()->getNext() = child->getNext();
			} else
			{
				first = child->getNext();
			}

			child->getPrev() = nullptr;
			child->getNext() = nullptr;
		}

		void clear()
		{
			value_type p = first;
			while (p)
			{
				value_type tmp = p;
				p = p->getNext();
				if (tmp)
				{
					tmp->getNext() = nullptr;
					tmp->getPrev() = nullptr;
				}
			}
			first = nullptr;
			last = nullptr;
		}

		bool checkValid()
		{
			if (!first)
				return true;

			int pos = 0;

			value_type p = first;
			value_type tmp = p;
			do
			{
				tmp = p;
				p = p->getNext();
				++pos;

				if (p)
				{
					if (p->getPrev() != tmp)
						return false;
				} else
				{
					if (tmp != last)
						return false;
				}
			} while (p);
			return true;
		}
	public:
		template<typename IteratorPointerType>
		struct Iterator
		{
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = IteratorPointerType;
			using pointer = IteratorPointerType*;
			using reference = IteratorPointerType&;
			using difference_type = ptrdiff_t;

			Iterator(value_type ptr = nullptr, bool isEnd = false) :
				base(ptr)
				, isEnd(isEnd)
			{
			}

			reference operator*() const
			{
				(base && !isEnd);
				return const_cast<reference>(base);
			}

			pointer operator->() const
			{
				return std::pointer_traits<pointer>::pointer_to(**this);
			}

			Iterator& operator++()
			{
				assert(base && !isEnd);
				value_type next = base->getNext();
				if (next)
					base = next;
				else
					isEnd = true;
				return (*this);
			}

			Iterator operator++(int)
			{
				Iterator old = (*this);
				++(*this);
				return old;
			}

			Iterator& operator--()
			{
				assert(base);
				if (isEnd)
					isEnd = false;
				else
					base = base->getPrev();
				return (*this);
			}

			Iterator operator--(int)
			{
				Iterator old = (*this);
				--(*this);
				return old;
			}

			bool operator==(const Iterator& other) const
			{
				return base == other.base && isEnd == other.isEnd;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			operator bool() const
			{
				return base != nullptr && !isEnd;
			}
		private:
			bool isEnd;

			typename std::remove_const<value_type>::type base;
		};
	public:
		using iterator = Iterator<value_type>;
		using const_iterator = Iterator<const value_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin()
		{
			return iterator(first, first == nullptr);
		}

		const_iterator begin() const
		{
			return const_iterator(first, first == nullptr);
		}

		const_iterator cbegin() const
		{
			return begin();
		}

		iterator end()
		{
			return iterator(last, true);
		}

		const_iterator end() const
		{
			return const_iterator(last, true);
		}

		const_iterator cend() const
		{
			return end();
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const
		{
			return rbegin();
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const
		{
			return rend();
		}

		value_type& front()
		{
			if (isEmpty())
				throw std::out_of_range("front() called on empty list");
			return first;
		}

		const value_type& front() const
		{
			if (isEmpty())
				throw std::out_of_range("front() called on empty list");
			return first;
		}

		value_type& back()
		{
			if (isEmpty())
				throw std::out_of_range("back() called on empty list");
			return last;
		}

		const value_type& back() const
		{
			if (isEmpty())
				throw std::out_of_range("back() called on empty list");
			return last;
		}
	private:
		value_type first;
		value_type last;
	};

	template<typename PointerType>
	class IntrusiveNode
	{
	public:
		using value_type = typename std::pointer_traits<PointerType>::pointer;
		using reference = value_type&;
		using pointer = value_type*;

		IntrusiveNode() :
			prev(nullptr)
			, next(nullptr)
		{
		}

		explicit IntrusiveNode(value_type rhs) :
			prev(nullptr)
			, next(nullptr)
		{
			if (rhs)
			{
				prev = rhs->getPrev();
				next = rhs->getNext();
			}
		}

		const value_type& getPrev() const
		{
			return prev;
		}

		value_type& getPrev()
		{
			return prev;
		}

		const value_type& getNext() const
		{
			return next;
		}

		value_type& getNext()
		{
			return next;
		}
	private:
		value_type prev;
		value_type next;

		friend class IntrusiveList<PointerType>;
	};

}
