#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include <vector>
#include <list>

#define DEBUG

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

// Just checking stuff before I have created actual tests.
#ifdef DEBUG
		std::uintptr_t headAddress = reinterpret_cast<std::uintptr_t>(m_head);
		std::uintptr_t tailAddress = reinterpret_cast<std::uintptr_t>(m_tail);
		std::uintptr_t inverseAddress = reinterpret_cast<std::uintptr_t>(m_tail->m_inverse);

		std::cout << "Head Address: " << headAddress << std::endl;
		std::cout << "Tail Address: " << tailAddress << std::endl;
		std::cout << "Inverse address: " << inverseAddress << std::endl;
		std::cout << std::endl;
#endif
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
			// This means that the target Node is the head Node.
			if (previousNode == nullptr)
			{
				m_head = currentNode->m_next;
			}
			else
			{
				previousNode->m_next = currentNode->m_next;
				previousNode->m_inverse = previousNode;
			}
			delete currentNode;
		}
		else
		{
			std::cout << "Could not find the Node to delete" << std::endl;
		}
	}

	void print()
	{
		auto current = m_head;
		while (current)
		{
			std::cout << current->m_data << " ";
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
