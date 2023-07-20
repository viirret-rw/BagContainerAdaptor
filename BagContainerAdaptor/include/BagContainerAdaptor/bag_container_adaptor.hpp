#ifndef CONTAINER_ADAPTOR_HPP
#define CONTAINER_ADAPTOR_HPP

#include "linked_list.hpp"
#include "hash_map.hpp"

#include <deque>
#include <forward_list>
#include <set>
#include <unordered_set>

template <typename T, typename U>
struct is_same_type
{
	static constexpr bool value = false;
};

template <typename T>
struct is_same_type<T, T>
{
	static constexpr bool value = true;
};

template <typename Container>
class BagContainerAdaptor
{
public:
	using value_type = typename Container::value_type;
	using iterator = typename Container::iterator;
	using const_iterator = typename Container::const_iterator;

	BagContainerAdaptor()
	{
		initializeContainer(*this);
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

	iterator insert(const_iterator pos, const value_type& value)
	{
		return insertImpl(m_container, pos, value);
	}

	iterator erase(iterator pos)
	{
		return m_container.erase(pos);
	}

	iterator erase(const_iterator pos)
	{
		return m_container.erase(pos);
	}

	iterator erase(iterator first, iterator last)
	{
		return m_container.erase(first, last);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return m_container.erase(first, last);
	}

	void swap(BagContainerAdaptor& other)
	{
		m_container.swap(other.m_container);
	}

	const_iterator cbegin() const
	{
		return m_container.cbegin();
	}

	iterator begin()
	{
		return m_container.begin();
	}

	const_iterator cend() const
	{
		return m_container.cend();
	}

	iterator end()
	{
		return m_container.end();
	}

	iterator find(const value_type& value)
	{
		return std::find(m_container.begin(), m_container.end(), value);
	}

	const_iterator find(const value_type& value) const
	{
		return std::find(m_container.cbegin(), m_container.cend(), value);
	}

	value_type& front()
	{
		return frontImpl(m_container);
	}

	const value_type& front() const
	{
		return frontImpl(m_container);
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
	
	// This is a testing function for my custom types.
	void debugInfo() const
	{
		m_container.debugInfo();
	}

private:
	// Initializations for different container types.

	// Default initialization.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<T>& bag) = delete;

	// Specialization for std::list.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::list<T>>& bag)
	{
		m_container = std::move(std::list<T>());
	}

	// Specialization for std::vector.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::vector<T>>& bag)
	{
		m_container = std::move(std::vector<T>());
	}

	// Specialization for std::deque.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::deque<T>>& bag)
	{
		m_container = std::move(std::deque<T>());
	}

	// Specialization for std::forward_list.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::forward_list<T>>& bag)
	{
		m_container = std::move(std::forward_list<T>());
	}
	
	// Specialization for std::multiset.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::multiset<T>>& bag)
	{
		m_container = std::move(std::multiset<T>());
	}

	// Specialization for std::unordered_multiset.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::unordered_multiset<T>>& bag)
	{
		m_container = std::move(std::unordered_multiset<T>());
	}

	// Specialization for LinkedList.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<LinkedList<T>>& bag)
	{
		m_container = std::move(LinkedList<T>());
	}

	// Specialization for HashMap.
	template <typename Key, typename Value>
	void initializeContainer(BagContainerAdaptor<HashMap<Key, Value>>& bag)
	{
		m_container = HashMap<Key, Value>();
	}

private:
	// Front implementation for containers with front() member function.
    template <typename C>
    typename std::enable_if<!std::is_same<C, std::unordered_multiset<value_type>>::value, value_type&>::type
    frontImpl(C& container) 
	{
        return container.front();
    }

    // Front implementation for std::unordered_multiset.
    template <typename C>
    typename std::enable_if<std::is_same<C, std::unordered_multiset<value_type>>::value, value_type&>::type
    frontImpl(C& container) 
	{
		return const_cast<value_type&>(*container.begin());
    }

	// Insert implementation for containers with insert() member function.
	template <typename C>
	typename std::enable_if<!std::is_same<C, std::forward_list<value_type>>::value, iterator>::type
	insertImpl(C& container, const_iterator& pos, const value_type& value)
	{
		return m_container.insert(pos, value);
	}

	// Insert implementation for std::forward_list.
	template <typename C>
	typename std::enable_if<std::is_same<C, std::forward_list<value_type>>::value, iterator>::type
	insertImpl(C& container, const_iterator& pos, const value_type& value)
	{
		// TODO This will need more work, but it kinda works.
		if(container.empty())
		{
			return container.insert_after(container.before_begin(), value);
		}
		else
		{
			return container.insert_after(pos, value);
		}
	}

private:
	Container m_container;
};

#endif
