#ifndef CONTAINER_ADAPTOR_HH
#define CONTAINER_ADAPTOR_HH

#include "linked_list.hpp"

template <typename Container, typename Implementation = LinkedList<typename Container::value_type>>
class ContainerAdaptor
{
public:
	using value_type = typename Container::value_type;

	// Move constructor.
	ContainerAdaptor(Container&& container) : m_container(std::move(container))
	{
	}

	ContainerAdaptor()
	{
		// Container type is std::vector.
		if (std::is_same<Container, std::vector<value_type>>::value)
		{
		}

		// Container type is std::list.
		else if (std::is_same<Container, std::list<value_type>>::value)
		{
		}
	}

	void insert(const value_type& value)
	{
		m_implementation.insert(value);
	}

	void remove(const value_type& value)
	{
		m_implementation.remove(value);
	}

	void print()
	{
		m_implementation.print();
	}

private:
	Container m_container;
	Implementation m_implementation;
};

#endif
