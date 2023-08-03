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
	/// \param value The value of the data in the Node.
	explicit LinkedListNode(const T& value) :
		m_data(value)
	{
	}

	/// The data inside the Node.
	T m_data;

	/// Pointing towards the next item in the linked list.
	LinkedListNode<T>* m_next = nullptr;

	/// Pointing towards the previous item in the linked list.
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
		/// Default constructor
		iterator() noexcept {}

		/// Constructor.
		/// \param node Pointer to the LinkedListNode to initialize the iterator with.
		/// \post The iterator is constructed with the given LinkedListNode as the current node.
		/// \exception No exceptions are thrown by this operation.
		explicit iterator(LinkedListNode<T>* node) noexcept : m_currentNode(node)
		{
		}
		
		/// Dereference operator for the iterator.
		/// \post Returns a reference to the data of the current node.
		/// \exception No exceptions are thrown by this operation.
		/// \return A reference to the data of the current node.
		T& operator*() const noexcept
		{
			return m_currentNode->m_data;
		}
		
		/// Arrow operator for the iterator.
		/// \post Returns a pointer to the data of the current node.
		/// \exception No exceptions are thrown by this operation.
		/// \return A pointer to the data of the current node.
		T* operator->() const noexcept
		{
			return &(m_currentNode->m_data);
		}

		/// Pre-increment operator for the iterator.
		/// \post Moves the iterator to the next node in the LinkedList.
		/// \exception No exceptions are thrown by this operation.
		/// \return A reference to the iterator after the increment.
		iterator& operator++() noexcept
		{
			m_currentNode = m_currentNode->m_next;
			return *this;
		}
		
		/// Post-increment operator for the iterator.
		/// \post Moves the iterator to the next node in the LinkedList.
		/// \exception No exceptions are thrown by this operation.
		/// \return An iterator pointing to the previous position before the increment.
		iterator operator++(int) noexcept
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}
		
		/// Equality comparison operator for the iterator.
		/// \param other The iterator to compare with.
		/// \post Checks if the current node of this iterator is equal to the current node of the other iterator.
		/// \exception No exceptions are thrown by this operation.
		/// \return True if both iterators point to the same node, otherwise false.
		bool operator==(const iterator& other) const noexcept
		{
			return m_currentNode == other.m_currentNode;
		}
		
		/// Inequality comparison operator for the iterator.
		/// \param other The iterator to compare with.
		/// \post Checks if the current node of this iterator is not equal to the current node of the other iterator.
		/// \exception No exceptions are thrown by this operation.
		/// \return True if both iterators do not point to the same node, otherwise false.
		bool operator!=(const iterator& other) const noexcept
		{
			return m_currentNode != other.m_currentNode;
		}

		/// Copy constructibility from const_iterator.
		/// \param it The const_iterator to copy construct from.
		/// \post The iterator is constructed with the same current node as the const_iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator(const typename LinkedList<T>::const_iterator& it) noexcept :
			m_currentNode(it.m_currentNode)
		{
		}

		/// Copy assignment from const_iterator.
		/// \param it The const_iterator to copy assign from.
		/// \post The iterator is assigned with the same current node as the const_iterator.
		/// \return A reference to the iterator after the assignment.
		/// \exception No exceptions are thrown by this operation.
		iterator& operator=(const typename LinkedList<T>::const_iterator& it) noexcept
		{
			m_currentNode = it.m_currentNode;
			return *this;
		}

		/// Move constructibility from const_iterator.
		/// \param it The const_iterator to move construct from.
		/// \post The iterator is constructed with the same current node as the const_iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator(typename LinkedList<T>::const_iterator&& it) noexcept :
			m_currentNode(const_cast<LinkedListNode<T>*>(it.m_currentNode))
		{
		}

		/// Move assignment from const_iterator.
		/// \post The iterator is assigned with the same current node as the const_iterator.
		/// \return A reference to the iterator after the assignment.
		/// \exception No exceptions are thrown by this operation.
		iterator& operator=(typename LinkedList<T>::const_iterator& it) noexcept
		{
			m_currentNode = const_cast<LinkedListNode<T>*>(it.currentNode);
			return *this;
		}

		/// Copy constructor.
		/// \return A reference to the iterator after the assignment.
		/// \post The iterator is constructed as a copy of the other iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator(const iterator&) noexcept = default;

		/// Copy assignment operator.
		/// \return A reference to the iterator after the assignment.
		/// \post The iterator is assigned as a copy of the other iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator& operator=(const iterator&) = default;

		/// Move constructor.
		/// \post The iterator is constructed by moving the other iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator(iterator&&) = default;

		/// Move assignment operator.
		/// \return A reference to the iterator after the assignment.
		/// \post The iterator is assigned by moving the other iterator.
		/// \exception No exceptions are thrown by this operation.
		iterator& operator=(iterator&&) = default; 
		
		/// Get the Node where the iterator is pointing.
		/// \return A pointer to the current node where the iterator is pointing.
		/// \post Returns a pointer to the current node where the iterator is pointing.
		/// \exception No exceptions are thrown by this operation.
		LinkedListNode<T>* getNode() const noexcept
		{
			return m_currentNode;
		}

	private:
		/// Pointer to the current node where the iterator is pointing.
		/// \note The iterator should always point to a valid node in the linked list, 
		/// 	or it should be nullptr if it has reached the end of the list.
		LinkedListNode<T>* m_currentNode;
	};

	class const_iterator : public std::iterator<
							std::forward_iterator_tag,
							const LinkedListNode<T>,
							std::ptrdiff_t,
							const LinkedListNode<T>*,
							const LinkedListNode<T>&
							>
	{
	public:
		const_iterator() noexcept {}

		explicit const_iterator(const LinkedListNode<T>* node) noexcept : m_currentNode(node)
		{
		}

		const T& operator*() const noexcept
		{
			return m_currentNode->m_data;
		}

		const T* operator->() const noexcept
		{
			return &(m_currentNode->m_data);
		}

		const_iterator& operator++() noexcept
		{
			m_currentNode = m_currentNode->m_next;
			return *this;
		}

		const_iterator operator++(int) noexcept
		{
			const_iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const const_iterator& other) const noexcept
		{
			return m_currentNode == other.m_currentNode;
		}

		bool operator!=(const const_iterator& other) const noexcept
		{
			return m_currentNode != other.m_currentNode;
		}

		/// Copy constructibility from non-const iterator.
		const_iterator(const typename LinkedList<T>::iterator& it) noexcept :
			m_currentNode(it.getNode())
		{
		}

		/// Copy assignment from non-const iterator.
		const_iterator& operator=(const typename LinkedList<T>::iterator& it) noexcept
		{
			m_currentNode = it.getNode();
			return *this;
		}

		/// Move constructibility from non-const iterator.
		/// \param it The non-const iterator to be moved from.
		/// \post The const_iterator is constructed, taking ownership of the internal
		/// 	pointer from the non-const iterator.
		/// \exception No exceptions are thrown by this operation.
		const_iterator(typename LinkedList<T>::iterator&& it) noexcept :
			m_currentNode(it.getNode())
		{
		}

		/// Move assingment from non-const iterator.
		/// \param it The non-const iterator to be moved from.
		/// \post The const_iterator is assigned the value of the non-const iterator, taking ownership of the internal pointer.
		/// \exception No exceptions are thrown by this operation.
		const_iterator& operator=(typename LinkedList<T>::iterator&& it) noexcept
		{
			m_currentNode = it.getNode();
			return *this;
		}

		/// Copy constructor.
		const_iterator(const const_iterator&) = default;

		/// Copy assignment operator.
		const_iterator& operator=(const const_iterator&) = default;

		/// Move constructor.
		const_iterator(const_iterator&&) = default;

		/// Move assignment operator.
		const_iterator& operator=(const_iterator&&) = default;

		/// Get the Node where the iterator is pointing.
		/// \return Node where the iterator is pointing.
		const LinkedListNode<T>* getNode() const noexcept
		{
			return m_currentNode;
		}

	private:
		LinkedListNode<T>* m_currentNode;

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

	LinkedList(const LinkedList<T>& other) = delete;

	LinkedList& operator=(const LinkedList<T>& other) = delete;

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
		return iterator(nullptr);
	}

	iterator cend() const
	{
		return iterator(nullptr);
	}

	void clear()
	{
		while (m_head) 
		{
        	auto* next = m_head->m_next;
        	m_allocator.deallocate(m_head, 1);
        	m_head = next;
    	}
    	m_count = 0;
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
		// Get Node where the first iterator is pointing.
		auto* firstNode = first.getNode();

		// Get Node where the last iterator is pointing.
		LinkedListNode<T>* lastNode;
		if (last == end())
		{
			lastNode = m_tail;
		}
		else
		{
			lastNode = last.getNode();
		}

		if (!firstNode || !lastNode || firstNode == lastNode)
		{
			return iterator(nullptr);
		}

		// Get the Nodes before and after range.
		LinkedListNode<T>* prevNode = nullptr;
		LinkedListNode<T>* nextNode = nullptr;

		if (first != begin())
		{
			prevNode = firstNode->m_inverse;
		}
		
		if (last != end() || lastNode != m_tail)
		{
			nextNode = lastNode->m_next;
		}

		// Reset m_head and m_tail pointers, connect the Nodes after range if possible.
		if (!prevNode && nextNode)
		{
			m_head = nextNode;
		}
		else if (prevNode && !nextNode)
		{
			m_tail = prevNode;
		}
		else if (!prevNode && !nextNode)
		{
			m_head = nullptr;
			m_tail = nullptr;
		}
		else
		{
			m_head = prevNode;
			m_tail = nextNode;
			prevNode->m_next = nextNode;
			nextNode->m_inverse = prevNode;
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

		// Deallocate the last element.
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
