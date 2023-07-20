#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <algorithm>
#include <cstddef>

template <typename T>
class LinkedList
{
public:
	using value_type = T;

	struct Node
	{
		/// Constructor.
		explicit Node(const T& value) :
			m_data(value)
		{
		}

		// Copy constructor.
		explicit Node(const Node& node) :
			m_data(node.m_data)
		{
		}

		T m_data;

		Node* m_next = nullptr;
		Node* m_inverse = nullptr;
	};

	class iterator : public std::iterator<
						std::forward_iterator_tag, 
						Node, 
						std::ptrdiff_t, 
						Node*, 
						Node&
						>
	{
	public:
		explicit iterator(Node* node) : m_currentNode(node)
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

	private:
		Node* m_currentNode;
	};

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

	~LinkedList()
	{
		clear();
	}

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

	void clear()
	{
		auto current = m_head;
		Node* previous = nullptr;
		while (current)
		{
			previous = current;
			current = current->m_next;
			delete previous;
		}
	}

	void insert(const T& value)
	{
		Node* newNode = new Node(value);

		// List is empty.
		if (!m_head)
		{
			m_head = newNode;	
			m_tail = newNode;
		}

		// Adding a new Node to the linked list.
		else
		{
			Node* prevNode = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
			m_tail->m_inverse = prevNode;
		}
		m_count++;
	}

	void remove(const T& value)
	{
		Node* currentNode = m_head;
		Node* previousNode = nullptr;

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

			delete currentNode;
			m_count--;
			remove(value);
		}
		else
		{
			return;
		}
	}

	void swap(LinkedList& other)
	{
		auto tempHead = m_head;
		auto tempTail = m_head;

		m_head = other.m_head;
		m_tail = other.m_tail;

		other.m_head = tempHead;
		other.m_tail = tempTail;
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

	// Pointing always to the first element.
	Node* m_head = nullptr;

	// Pointing always to the last element.
	Node* m_tail = nullptr;

	// Amount of Nodes in the linked list.
	size_t m_count = 0;
};

#endif
