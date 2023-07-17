#ifndef CONTAINER_ADAPTOR_HH
#define CONTAINER_ADAPTOR_HH

#include "linked_list.hpp"

template <typename Container, typename Implementation = LinkedList<Container>>
class BagContainerAdaptor
{
public:
	using value_type = typename Container::value_type;

	// Move constructor.
	BagContainerAdaptor(Container&& container) : m_container(std::move(container))
	{
	}

	BagContainerAdaptor()
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

	void insert(const Container& container)
	{
		for (const auto& item : container)
		{
			m_implementation.insert(item);
		}
	}

	typename Implementation::Iterator begin()
	{
		return m_implementation.begin();
	}

	typename Implementation::Iterator end()
	{
		return m_implementation.end();
	}

	void remove(const value_type& value)
	{
		m_implementation.remove(value);
	}

	void activate(const value_type& value)
	{
		m_implementation.activate(value);
	}

	void deactivate(const value_type& value)
	{
		m_implementation.deactivate(value);
	}

	value_type& front()
	{
		return m_implementation.front();
	}

	const value_type& front() const
	{
		return m_implementation.front();
	}

	value_type& back()
	{
		return m_implementation.back();
	}

	const value_type& back() const
	{
		return m_implementation.back();
	}

	size_t size() const
	{
		return m_implementation.size();
	}

	bool empty() const
	{
		return m_implementation.empty();
	}

	void debugInfo() const
	{
		m_implementation.debugInfo();
	}

private:
	Container m_container;
	Implementation m_implementation;
};

#endif
