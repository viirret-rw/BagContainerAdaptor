#ifndef CONTAINER_ADAPTOR_HH
#define CONTAINER_ADAPTOR_HH

#include "linked_list.hpp"

template <typename Container>
class BagContainerAdaptor
{
public:
	using value_type = typename Container::value_type;

	BagContainerAdaptor()
	{
		initializeContainer();
	}

	// Move constructor.
	BagContainerAdaptor(Container&& container) : m_container(std::move(container))
	{
	}

	// Move assignment operator.
	BagContainerAdaptor& operator=(BagContainerAdaptor&& other)
	{
		if (this != &other)
		{
			m_container = std::move(other.m_container);
		}
		return *this;
	}

	void insert(const value_type& value)
	{
		m_container.insert(value);
	}

	void insert(const Container& container)
	{
		m_container.insert(container);
	}

	void remove(const value_type& value)
	{
		m_container.remove(value);
	}

	void swap(BagContainerAdaptor& other)
	{
		m_container.swap(other.m_container);
	}

	typename Container::Iterator cbegin() const
	{
		return m_container.cbegin();
	}

	typename Container::Iterator begin()
	{
		return m_container.begin();
	}

	typename Container::Iterator cend() const
	{
		return m_container.cend();
	}

	typename Container::Iterator end()
	{
		return m_container.end();
	}

	typename Container::Iterator find(const value_type& value) const
	{
		return m_container.find(value);
	}

	typename Container::Iterator find(const value_type& value)
	{
		return m_container.find(value);
	}

	void activate(const value_type& value)
	{
		m_container.activate(value);
	}

	void deactivate(const value_type& value)
	{
		m_container.deactivate(value);
	}

	value_type& front()
	{
		return m_container.front();
	}

	const value_type& front() const
	{
		return m_container.front();
	}

	value_type& back()
	{
		return m_container.back();
	}

	const value_type& back() const
	{
		return m_container.back();
	}

	size_t size() const
	{
		return m_container.size();
	}

	bool empty() const
	{
		return m_container.empty();
	}

	void debugInfo() const
	{
		m_container.debugInfo();
	}

	void initializeContainer()
	{
		m_container = LinkedList<int>();
	}

private:
	Container m_container;
};

#endif
