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

		bool isActive = true;
	};

	class Iterator : public std::iterator<
						std::forward_iterator_tag, 
						Node, 
						std::ptrdiff_t, 
						Node*, 
						Node&
						>
	{
	public:
		explicit Iterator(Node* node) :
			currentNode(node)
		{
		}

		T& operator*() const
		{
			return currentNode->m_data;
		}

		T* operator->() const
		{
			return &(currentNode->m_data);
		}

		Iterator& operator++()
		{
			currentNode = currentNode->m_next;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const Iterator& other) const
		{
			return currentNode == other.currentNode;
		}

		bool operator!=(const Iterator& other) const
		{
			return currentNode != other.currentNode;
		}

		// Copy constructibility
		Iterator(const Iterator&) = default;
		Iterator& operator=(const Iterator&) = default;

		// Move constructibility
		Iterator(Iterator&&) = default;
		Iterator& operator=(Iterator&&) = default; 

	private:
		Node* currentNode;
	};

	Iterator begin()
	{
		return Iterator(m_head);
	}

	Iterator cbegin() const
	{
		return Iterator(m_head);
	}

	Iterator end()
	{
		return Iterator(m_tail);
	}

	Iterator cend() const
	{
		return Iterator(m_tail);
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

	Iterator find(const T& value) const
	{
		return std::find(cbegin(), cend(), value);
	}

	Iterator find(const T& value)
	{
		return std::find(begin(), end(), value);
	}

	void activate(const T& value)
	{
		Node* travellerNode = m_head;

		while (travellerNode)
		{
			if (travellerNode->m_inverse && travellerNode->m_inverse->m_data == value)
			{
				travellerNode->m_inverse->isActive = true;
				break;
			}
			travellerNode = travellerNode->m_next;
		}
	}

	void deactivate(const T& value)
	{
		Node* travellerNode = m_head;

		while (travellerNode)
		{
			if (travellerNode->m_inverse && travellerNode->m_inverse->m_data == value)
			{
				travellerNode->m_inverse->isActive = false;
				break;
			}
			travellerNode = travellerNode->m_next;
		}
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

			std::cout << "Node: " << (current->isActive ? "X" : "_") << " " << current->m_data << std::endl;
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
