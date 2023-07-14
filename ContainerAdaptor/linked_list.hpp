#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include <vector>
#include <list>

template <typename Container>
class LinkedList
{
public:
	using value_type = typename Container::value_type;

	template <typename T>
	struct Node
	{
		/// Constructor.
		explicit Node(const T& value) :
			m_data(value)
		{
		}

		// Copy constructor.
		explicit Node(const Node<T>& node) :
			m_data(node.m_data)
		{
		}

		T m_data;

		Node<T>* m_next = nullptr;
		Node<T>* m_inverse = nullptr;

		bool isActive = true;
	};

	~LinkedList()
	{
		auto current = m_head;
		Node<value_type>* previous = nullptr;
		while (current)
		{
			previous = current;
			current = current->m_next;
			delete previous;
		}
	}
	

	void insert(const value_type& value)
	{
		Node<value_type>* newNode = new Node<value_type>(value);

		// List is empty.
		if (!m_head)
		{
			m_head = newNode;	
			m_tail = newNode;
		}

		// Adding a new Node to the linked list.
		else
		{
			Node<value_type>* prevNode = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
			m_tail->m_inverse = prevNode;
		}
		m_count++;
	}

	void remove(const value_type& value)
	{
		Node<value_type>* currentNode = m_head;
		Node<value_type>* previousNode = nullptr;

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

	void activate(const value_type& value)
	{
		Node<value_type>* travellerNode = m_head;

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

	void deactivate(const value_type& value)
	{
		Node<value_type>* travellerNode = m_head;

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

	value_type& front()
	{
		return m_head->m_data;
	}

	const value_type& front() const
	{
		return m_head->m_data;
	}

	value_type& back()
	{
		return m_tail->m_data;
	}

	const value_type& back() const
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
	Node<value_type>* m_head = nullptr;

	// Pointing always to the last element.
	Node<value_type>* m_tail = nullptr;

	// Amount of Nodes in the linked list.
	size_t m_count = 0;
};

#endif
