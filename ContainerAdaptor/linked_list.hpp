#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include <vector>
#include <list>

// TODO this class should be inside the linked list.
template <typename Value>
struct Node
{
	/// Constructor.
	explicit Node(const Value& value) :
		m_data(value)
	{
	}

	// Copy constructor.
	explicit Node(const Node<Value>& node) :
		m_data(node.m_data)
	{
	}

	Value m_data;

	Node<Value>* m_next = nullptr;
	Node<Value>* m_inverse = nullptr;

	bool isActive = true;
};

template <typename Value>
class LinkedList
{
public:
	~LinkedList()
	{
		auto current = m_head;
		Node<Value>* previous = nullptr;
		while (current)
		{
			previous = current;
			current = current->m_next;
			delete previous;
		}
	}
	

	void insert(const Value& value)
	{
		Node<Value>* newNode = new Node<Value>(value);

		// List is empty.
		if (!m_head)
		{
			m_head = newNode;	
			m_tail = newNode;
		}

		// Adding a new Node to the linked list.
		else
		{
			Node<Value>* prevNode = m_tail;
			m_tail->m_next = newNode;
			m_tail = newNode;
			m_tail->m_inverse = prevNode;
		}
	}

	void remove(const Value& value)
	{
		Node<Value>* currentNode = m_head;
		Node<Value>* previousNode = nullptr;

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
			remove(value);
		}
		else
		{
			return;
		}
	}

	void activate(const Value& value)
	{
		Node<Value>* travellerNode = m_head;

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

	void deactivate(const Value& value)
	{
		Node<Value>* travellerNode = m_head;

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
	Node<Value>* m_head = nullptr;

	// Pointing always to the last element.
	Node<Value>* m_tail = nullptr;
};

#endif
