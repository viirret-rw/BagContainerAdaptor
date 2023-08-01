#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <memory>
#include <iostream>
#include <algorithm>
#include <cstddef>

// I would this to be a nested class in the LinkedList but
// with this implementation it doesn't seem possible.
template <typename T>
struct LinkedListNode
{
	/// Constructor.
	explicit LinkedListNode(const T& value) :
		m_data(value)
	{
	}

	T m_data;

	LinkedListNode<T>* m_next = nullptr;
	LinkedListNode<T>* m_inverse = nullptr;
};


template <typename T, typename Allocator = std::allocator<LinkedListNode<T>>>
class LinkedList
{
public:
	using value_type = T;

	class iterator : public std::iterator<
						std::forward_iterator_tag, 
						LinkedListNode<T>, 
						std::ptrdiff_t, 
						LinkedListNode<T>*, 
						LinkedListNode<T>&
						>
	{
	public:
		iterator(){}

		explicit iterator(LinkedListNode<T>* node) : m_currentNode(node)
		{
		}

		T& operator*() const
		{
			return m_currentNode->m_data;
		}

		T* operator->() const
		{
			return &(m_currentNode->m_data);
		}

		iterator& operator++()
		{
			m_currentNode = m_currentNode->m_next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const iterator& other) const
		{
			return m_currentNode == other.m_currentNode;
		}

		bool operator!=(const iterator& other) const
		{
			return m_currentNode != other.m_currentNode;
		}

		// Copy constructibility
		iterator(const iterator&) = default;
		iterator& operator=(const iterator&) = default;

		// Move constructibility
		iterator(iterator&&) = default;
		iterator& operator=(iterator&&) = default; 
		
		LinkedListNode<T>* getNode() const
		{
			return m_currentNode;
		}

	private:
		LinkedListNode<T>* m_currentNode;
	};

	class const_iterator
	{
	};

	/// Default constructor.
	LinkedList()
	{
	}

	/// Destructor.
	~LinkedList()
	{
		clear();
	}

	/// Initializer list constructor.
	LinkedList(std::initializer_list<value_type> list) 
	{
		for (const value_type& value : list)
		{
			insert(value);
		}
	}

	/// Move constructor.
	LinkedList(LinkedList&& other) : 
		m_head(other.m_head), m_tail(other.m_tail), m_count(other.m_count)
	{
		other.m_head = nullptr;
		other.m_tail = nullptr;
		other.m_count = 0;
	}

	/// Move assingment operator.
	LinkedList& operator=(LinkedList&& other)
	{
		if (this != &other)
		{
			m_head = other.m_head;
			m_tail = other.m_tail;
			m_count = other.m_count;

			other.m_head = nullptr;
			other.m_tail = nullptr;
		}
		return *this;
	}

	iterator begin()
	{
		return iterator(m_head);
	}

	iterator cbegin() const
	{
		return iterator(m_head);
	}

	iterator end()
	{
		return iterator(m_tail);
	}

	iterator cend() const
	{
		return iterator(m_tail);
	}

	void clear()
	{
		m_allocator.deallocate(m_head, size());
	}

	iterator insert(const T& value) noexcept
	{
		auto* newNode = m_allocator.allocate(1);
		m_allocator.construct(newNode, value);

		// List is empty.
		if (!m_head)
		{
			m_head = newNode;	
			m_tail = newNode;
		}

		// Adding a new Node to the linked list.
		else
		{
			LinkedListNode<T>* prevNode = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
			m_tail->m_inverse = prevNode;
		}
		m_count++;

		return iterator(m_tail);
	}

	iterator insert(iterator pos, const T& value) noexcept
	{
		auto* newNode = m_allocator.allocate(1);
		m_allocator.construct(newNode, value);

		// List is empty.
		if (!m_head)
		{
			m_head = newNode;
			m_tail = newNode;
		}
		// Inserting to beginning.
		else if (pos == begin() && begin() != end())
		{
			newNode->m_next = m_head;
			m_head->m_inverse = newNode;
			m_head = newNode;
		}
		// Inserting to end.
		else if (pos == end())
		{
			newNode->m_inverse = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
		}
		// Inserting to any other position.
		else
		{
			LinkedListNode<T>* ogNode = pos.getNode();
			ogNode->m_inverse->m_next = newNode;
			ogNode->m_inverse = newNode;

			newNode->m_next = ogNode;
			newNode->m_inverse = ogNode->m_inverse;
		}

		m_count++;
		return iterator(newNode);
	}

	iterator erase(const T& value) noexcept
	{
		auto* currentNode = m_head;
		LinkedListNode<T>* previousNode = nullptr;

		while (currentNode && currentNode->m_data != value)
		{
			previousNode = currentNode;
			currentNode = currentNode->m_next;
		}

		if (currentNode)
		{
			// Target Node is the head Node.
			if (!previousNode)
			{
				m_head = currentNode->m_next;
				currentNode->m_next->m_inverse = nullptr;
			}

			// Target Node is the tail Node.
			else if (!currentNode->m_next)
			{
				m_tail = previousNode;
				previousNode->m_next = nullptr;
			}

			// Target Node is normal Node.
			else
			{
				previousNode->m_next = currentNode->m_next;
				currentNode->m_next->m_inverse = previousNode;
			}
			
			LinkedListNode<T>* returnNode = currentNode->m_next;		
			m_allocator.deallocate(currentNode, 1);
			m_count--;
			return iterator(returnNode);
		}
		else
		{
			return iterator(m_tail);
		}
	}

	iterator erase(iterator pos) noexcept
	{
		auto* removable = pos.getNode();
		auto* nextNode = removable->m_next;

		if (removable == nullptr)
		{
			return iterator(m_tail);
		}
		else if (removable == m_head)
		{
			m_head = removable->m_next;

			if (m_head)
			{
				m_head->m_inverse = nullptr;
			}
			else
			{
				m_tail = nullptr;
			}
		}
		else if (removable == m_tail)
		{
			m_tail = removable->m_inverse;
			m_tail->m_next = nullptr;
		}
		else
		{
			LinkedListNode<T>* prevNode = removable->m_inverse;
			prevNode->m_next = nextNode;
			nextNode->m_inverse = prevNode;
		}
			
		m_count--;
		m_allocator.deallocate(removable, 1);
		return iterator(nextNode);
	}

	iterator erase(iterator first, iterator last) noexcept
	{
		auto* firstNode = first.getNode();
		auto* lastNode = last.getNode();

		if (!firstNode || !lastNode || firstNode == lastNode)
			return iterator(m_tail);

		auto* prevNode = firstNode->m_inverse;
		auto* nextNode = lastNode->m_next;

		// Connect the Nodes before and after the range.
		if (prevNode)
		{
			prevNode->m_next = nextNode;
		}
		else
		{
			m_head = nextNode;
		}

		if (nextNode)
		{
			nextNode->m_inverse = prevNode;
		}
		else
		{
			m_tail = prevNode;
		}

		// Delete the Nodes within the range.
		LinkedListNode<T>* currentNode = firstNode;
		while (currentNode != lastNode)
		{
			auto* next = currentNode->m_next;
			m_allocator.deallocate(currentNode, 1);
			currentNode = next;
			m_count--;
		}

		m_allocator.deallocate(lastNode, 1);
		m_count--;

		return iterator(nextNode);
	}

	// Swap head and tail pointers and the m_count variable.
	void swap(LinkedList& other)
	{
		auto tempHead = m_head;
		auto tempTail = m_tail;
		auto tempCount = m_count;

		m_head = other.m_head;
		m_tail = other.m_tail;
		m_count = other.m_count;

		other.m_head = tempHead;
		other.m_tail = tempTail;
		other.m_count = tempCount;
	}

	iterator find(const T& value) const
	{
		return std::find(cbegin(), cend(), value);
	}

	iterator find(const T& value)
	{
		return std::find(begin(), end(), value);
	}

	T& front()
	{
		return m_head->m_data;
	}

	const T& front() const
	{
		return m_head->m_data;
	}

	T& back()
	{
		return m_tail->m_data;
	}

	const T& back() const
	{
		return m_tail->m_data;
	}

	size_t size() const
	{
		return m_count;
	}

	bool empty() const
	{
		return !m_head;
	}

	void debugInfo() const
	{
		auto current = m_head;
		while (current)
		{
			if (current == m_head)
			{
				std::cout << "Head ";
			}
			if (current == m_tail)
			{
				std::cout << "Tail ";
			}

			std::cout << "Node: " << current->m_data << std::endl;
			std::cout << "Address: " << current << std::endl;
			std::cout << "Inverse address: " << current->m_inverse << std::endl;
			std::cout << "Next address: " << current->m_next << std::endl;
			std::cout << std::endl;

			current = current->m_next;
		}
		std::cout << std::endl;
	}

private:

	/// Pointing always to the first element.
	LinkedListNode<T>* m_head = nullptr;

	/// Pointing always to the last element.
	LinkedListNode<T>* m_tail = nullptr;

	/// Amount of Nodes in the linked list.
	size_t m_count = 0;

	/// Allocator for memory management, default is std::allocator.
	Allocator m_allocator;
};

#endif
