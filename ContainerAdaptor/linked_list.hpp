#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include <vector>
#include <list>

template <typename Value>
struct Node
{
	/// Constructor.
	explicit Node(const Value& value) :
		m_data(value)
	{
	}

	Value m_data;
	Node<Value>* m_next = nullptr;
};

template <typename Value>
class LinkedList
{
public:
	void insert(const Value& value)
	{
		auto newNode = new Node<Value>(value);

		// List is empty
		if (!m_head)
		{
			m_head = newNode;	
			m_tail = newNode;
		}
		else
		{
			m_tail->m_next = newNode;
			m_tail = newNode;
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
	Node<Value>* m_head = nullptr;
	Node<Value>* m_tail = nullptr;
};

#endif
