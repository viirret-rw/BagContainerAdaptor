#ifndef CONTAINER_ADAPTOR_HPP
#define CONTAINER_ADAPTOR_HPP

#include "../../dynamic/include/ring_buffer.hpp"

#include "linked_list.hpp"

#include <set>
#include <list>
#include <deque>
#include <vector>
#include <forward_list>
#include <unordered_set>

template <typename Container>
class BagContainerAdaptor
{
public:
	using value_type = typename Container::value_type;
	using iterator = typename Container::iterator;
	using const_iterator = typename Container::const_iterator;

	/// Constructor.
	BagContainerAdaptor() noexcept
	{
		initializeContainer(*this);
	}

	/// Destructor.
	~BagContainerAdaptor() noexcept
	{
		deallocateContainer(m_container);
	}

	/// Move constructor.
	/// \param container The underlying container from which the BagContainerAdaptor is constructed.
	BagContainerAdaptor(Container&& container) noexcept : m_container(std::move(container))
	{
	}

    // Move assignment operator.
	/// \param other The underlying container from which the BagContainerAdaptor is created.
	BagContainerAdaptor& operator=(BagContainerAdaptor&& other) noexcept
	{
		if (this != &other)
		{
			m_container = std::move(other.m_container);
		}
		return *this;
	}

	/// Forwarding constructor for self-instantiation.
	/// \param other The other BagContainerAdaptor from which we are initializing from.
	/// \tparam OtherContainer The template argument for the other container.
	template <typename OtherContainer, typename = std::enable_if<std::is_same<Container, OtherContainer>::value>>
	BagContainerAdaptor(BagContainerAdaptor<OtherContainer>&& other) noexcept :
		m_container(std::move(other.m_container))
	{
	}

	/// Copy constructor.
	/// \param other The other BagContainerAdaptor where we copy from.
	BagContainerAdaptor(const BagContainerAdaptor& other) = delete;

	/// Copy assignment operator.
	/// \param other The other BagContainerAdaptor where we copy from.
	BagContainerAdaptor& operator=(const BagContainerAdaptor& other) = delete;

	/// Insert element to specified position of the underlying container.
	/// \param pos The specified position where the element is inserted.
	/// \param value The value to be inserted.
	/// \tparam Iterator The iterator type of the underlying container.
	/// \return An iterator that points to the inserted element.
	template <typename Iterator>
	iterator insert(Iterator pos, const value_type& value)
	{
		return insertImpl(m_container, pos, value);
	}

	/// Insert element to the last position of the underlying container.
	/// \param value The value to be inserted.
	/// \return An iterator that points to the inserted element.
	iterator insert(const value_type& value)
	{
		return insertImpl(m_container, value);
	}
	
	/// Erase element from the specified position in the underlying container.
	/// \param pos The specified position where the element is erased.
	/// \return An iterator following the last removed element.
	iterator erase(iterator pos)
	{
		return eraseImpl(m_container, pos);
	}

	/// Erase all elements that have the specified value in the underlying container.
	/// \param value The value of the elements that are removed.
	/// \return An iterator following the last removed element.
	iterator erase(const value_type& value)
	{
		return eraseImpl(m_container, value);
	}

	/// Erase all elements between two iterators from the underlying container.
	/// \param first The first element in the range of elements being removed.
	/// \param last The last element of the range of the elements being removed.
	/// \tparam FirstType The type of iterator for the first element in the range.
	/// \tparam LastType The type of iterator for the last element in the range.
	/// \return An iterator following the last removed element.
	template <typename FirstType, typename LastType>
	iterator erase(FirstType first, LastType last)
	{
		return eraseImpl(m_container, first, last);
	}

	/// Swap the contents of two BagContainerAdaptors.
	/// \param other The other bag to be swapped with.
	void swap(BagContainerAdaptor& other) noexcept
	{
		m_container.swap(other.m_container);
	}

	/// Get constant iterator pointing to the first element in the underlying container.
	/// \return Constant iterator pointing to the first element in the underlying container.
	const_iterator cbegin() const noexcept
	{
		return m_container.cbegin();
	}

	/// Get iterator pointing to the first element in the underlying container.
	/// \return Iterator pointing to the first element in the underlying container.
	iterator begin() noexcept
	{
		return m_container.begin();
	}

	/// Get constant iterator pointing to the last element in the underlying container.
	/// \return Contant iterator pointing to the last element in the underlying container.
	const_iterator cend() const noexcept
	{
		return m_container.cend();
	}

	/// Get iterator pointing to the last element in the underlying container.
	/// \return Iterator pointing to the last element in the underlying container.
	iterator end() noexcept
	{
		return m_container.end();
	}

	/// Get iterator pointing to the first instance of element with specified value.
	/// \param value The value to compare elements to.
	/// \return An iterator pointing to the first instance of element with specified value.
	iterator find(const value_type& value)
	{
		return std::find(m_container.begin(), m_container.end(), value);
	}

	/// Get iterator pointing to the first instance of element with specified value in const context.
	/// \param value The value to compare elements to.
	/// \return An iterator pointing to the first instance of element with specified value.
	const_iterator find(const value_type& value) const
	{
		return std::find(m_container.cbegin(), m_container.cend(), value);
	}

	/// Get reference to the first element in the underlying container.
	/// \return Reference to the first element in the underlying container.
	value_type& front() noexcept
	{
		return frontImpl(m_container);
	}
	
	/// Get reference to the first element in the underlying container in const context.
	/// \return Reference to the first element in the underlying container.
	const value_type& front() const noexcept
	{
		return frontImpl(m_container);
	}

	/// Get reference to the last element in the underlying container.
	/// \return Reference to the last element in the underlying container.
	value_type& back() noexcept
	{
		return backImpl(m_container);
	}

	/// Get reference to the last element in the underlying container.	
	/// \return Reference to the last element in the underlying container.
	const value_type& back() const noexcept
	{
		return m_container.back();
	}

	/// Get the amount of elements in the underlying container.
	/// \return The amount of elements in the underlying container.
	size_t size() const noexcept
	{
		return sizeImpl(m_container);
	}

	/// Get boolean describing if the underlying container is empty or not.
	/// \return Boolean describing if the underlying container is empty or not.
	bool empty() const noexcept
	{
		return m_container.empty();
	}
	
	// TODO remove me!
	// This is a testing function for my custom types.
	void debugInfo() const
	{
		m_container.debugInfo();
	}

private:
	/// \defgroup containerInitializations Initializations for different container types.

	/// Default initialization.
	/// \param bag Undefined container type.
	/// \tparam T The type of elements in undefined container type.
	/// \ingroup containerInitializations
	template <typename T>
	void initializeContainer(BagContainerAdaptor<T>& bag) = delete;

	/// Specialization for std::list.
	/// \param bag BagContainerAdaptor specialized for std::list<T>.
	/// \tparam T The type of elements in std::list.
	/// \ingroup containerInitializations
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::list<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// Specialization for std::vector.
	/// \param bag BagContainerAdaptor specialized for std::vector<T>.
	/// \tparam T The type of elements in std::vector.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::vector<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// Specialization for std::deque.
	/// \param bag BagContainerAdaptor specialized for std::deque<T>.
	/// \tparam T The type of elements in std::deque.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::deque<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// Specialization for std::forward_list.
	/// \param bag BagContainerAdaptor specialized for std::forward_list<T>.
	/// \tparam T The type of elements in std::forward_list.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::forward_list<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}
	
	/// Specialization for std::multiset.
	/// \param bag BagContainerAdaptor specialized for std::multiset<T>.
	/// \tparam T The type of elements in std::multiset.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::multiset<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// Specialization for std::unordered_multiset.
	/// \param bag BagContainerAdaptor specialized for std::unordered_multiset<T>.
	/// \tparam T The type of elements in std::unordered_multiset.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<std::unordered_multiset<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// Specialization for BagContainerAdaptor.
	/// \param bag BagContainerAdaptor specialized for BagContainerAdaptor<T>.
	/// \tparam T The type of elements in BagContainerAdaptor argument.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<BagContainerAdaptor<T>>& bag) noexcept
	{
		m_container = std::move(bag.m_container);
	}

	/// Specialization for LinkedList.
	/// \param bag BagContainerAdaptor specialized for LinkedList<T>.
	/// \tparam T The type of elements in LinkedList.
	/// \ingroup containerInitializations.
	template <typename T>
	void initializeContainer(BagContainerAdaptor<LinkedList<T>>& bag) noexcept
	{
		m_container = std::move(bag.m_container);
	}

	/// Specialization for Ring buffer.
	/// \param bag BagContainerAdapter specialized for ring_buffer<T>.
	/// \tparam T The type of elements in Ring buffer.
	/// \ingroup containerInitializations
	template <typename T>
	void initializeContainer(BagContainerAdaptor<ring_buffer<T>>& bag) noexcept
	{
		m_container = bag.m_container;
	}

	/// \defgroup containerDestructors	

	/// Default destructor implementation.
	/// \param bag Default container type.
	/// \tparam C The underlying container type.
	/// \ingroup containerDestructors
	template <typename C>
	void deallocateContainer(C& bag) noexcept
	{
	}

	/// Specialization for std::deque.
	/// \param bag BagContainerAdapter specialized for std::deque.
	/// \ingroup containerDestructors.
	void deallocateContainer(BagContainerAdaptor<std::deque<value_type>> bag) noexcept
	{
        bag.clear();
	}

	/// \defgroup insertImplementations

	/// Insert element to the last position of the underlying container type.
	/// \param container The underlying container type where the element is inserted.
	/// \param value The value of the inserted element.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return An iterator that points to the inserted element.
	/// \ingroup insertImplementations
	template <typename C>
	iterator insertImpl(C& container, const value_type& value)
	{
		return container.insert(container.end(), value);
	}

	/// Insert element to the last position specialized for std::forward_list.
	/// \param container The underlying container type where the element is inserted.
	/// \param value The value of the inserted element.
	/// \return An iterator that points to the inserted element.
	/// \ingroup insertImplementations
	iterator insertImpl(std::forward_list<value_type>& container, const value_type& value)
	{
		auto itPrev = container.before_begin();
		for (auto it = container.begin(); it != container.end(); ++it)
		{
			itPrev = it;
		}
		return container.insert_after(itPrev, value);
	}

	/// Insert element to a specified position in the underlying container.
	/// \param container The underlying container type where the element is inserted.
	/// \param pos The position in the underlying container where the element is inserted.
	/// \param value The value of the inserted element.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \tparam Iterator The iterator type of the underlying container.
	/// \return An iterator that points to the inserted element.
	/// \ingroup insertImplementations
	template <typename C, typename Iterator>
	typename std::enable_if<!std::is_same<C, std::forward_list<value_type>>::value, iterator>::type
	insertImpl(C& container, Iterator pos, const value_type& value)
	{
		return container.insert(pos, value);
	}

	/// Insert element to a specified position specialized for std::forward_list.
	/// \param container The underlying container type where the element is inserted.
	/// \param pos Constant iterator to the position in the underlying container where the element is inserted.
	// \param value The value of the inserted element.
	/// \return An iterator that points to the inserted element.
	/// \ingroup insertImplementations
	iterator insertImpl(std::forward_list<value_type>& container, const_iterator pos, const value_type& value)
	{
		auto itPrev = container.before_begin();
		for (auto it = container.begin(); it != pos; ++it)
		{
			itPrev = it;
		}
		return container.insert_after(itPrev, value);
	}

	/// \defgroup eraseImplementations

	/// Erase item from the underlying container in the position of the iterator.
	/// \param container The underlying container type where the element is erased.
	/// \param pos The position where the item is removed from the container.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return An iterator following the removed element.
	/// \ingroup eraseImplementations
	template <typename C>
	iterator eraseImpl(C& container, iterator pos)
	{
		return container.erase(pos);
	}

	/// Erase item from the underlying container in the position of the iterator specialized for std::forward_list.
	/// \param container The underlying container type where the element is erased.
	/// \param pos The position where the element is erased.
	/// \return An iterator following the removed element.
	/// \ingroup eraseImplementations
	iterator eraseImpl(std::forward_list<value_type>& container, iterator pos)
	{
		return container.erase_after(pos);
	}

	/// Erase items from the underlying container that have specified value.
	/// \param container The underlying container type where the elements are erased.
	/// \param value The value of the removed elements.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return An iterator following the last removed element.
	/// \ingroup eraseImplementations
	template <typename C>
	iterator eraseImpl(C& container, const value_type& value)
	{
		auto it = find(value);

		while (it != container.end())
		{
			it = container.erase(it);
			it = find(value);
		}
		return it;
	}

	/// Erase items from the underlying container type that have specified value specialized for std::forward_list.
	/// \param container The underlying container type where the elements are erased.
	/// \param value The value of the removed elements.
	/// \return An iterator following the last removed element.
	/// \ingroup eraseImplementations
	iterator eraseImpl(std::forward_list<value_type>& container, const value_type& value)
	{
		auto previous = container.before_begin();
		auto current = container.begin();

		while (current != container.end())
		{
			if (*current == value)
			{
				current = container.erase_after(previous);
			}
			else
			{
				previous = current;
				++current;
			}
		}
		return current;
	}

	/// Erase items from the underlying container type between two iterators.
	/// \param container The underlying container type where the elements are erased.
	/// \param first The first element in the range of elements to be removed.
	/// \param last The last element in the range of elements to be removed.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \tparam FirstType The type of iterator for the first removed element.
	/// \tparam LastType The type of iterator for the last removed element.
	/// \return An iterator following the last removed element.
	/// \ingroup eraseImplementations
	template <typename C, typename FirstType, typename LastType>
	typename std::enable_if<!std::is_same<C, std::forward_list<value_type>>::value, iterator>::type
	eraseImpl(C& container, FirstType first, LastType last)
	{
		return container.erase(first, last);
	}
	
	/// Erase items from the underlying container type between two iterator specialized for std::forward_list.
	/// \param container The underlying container type where the elements are erased.
	/// \param first The first element in the range of elements to be removed.
	/// \param last The last element in the range of elements to be removed.
	/// \return An iterator following the last removed element.
	/// \ingroup eraseImplementations
	iterator eraseImpl(std::forward_list<value_type>& container, const_iterator first, const_iterator last)
	{
		return container.erase_after(first, last);
	}

	/// \defgroup frontImplementations

	/// Front function implementation for container types that have the front() member function.
	/// \param container The underlying container type where the element is accessed.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	template <typename C>
	value_type& frontImpl(C& container) noexcept
	{
		return container.front();
	}

	/// Front function implementation for container types that have the front() member function in const context.
	/// \param container The underlying container type where the element is accessed.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	template <typename C>
	const value_type& frontImpl(const C& container) const noexcept
	{
		return container.front();
	}

	/// Front function specialization for std::forward_list.
	/// \param container The underlying container type for BagContainerAdaptor that is std::forward_list.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	value_type& frontImpl(std::forward_list<value_type>& container) noexcept
	{
		return *container.begin();
	}

	/// Front function specialization for std::forward_list in const context.
	/// \param container The underlying container type for BagContainerAdaptor that is std::forward_list.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	const value_type& frontImpl(const std::forward_list<value_type>& container) const noexcept
	{
		return *container.cbegin();
	}

	/// Front function specialization for std::multiset.
	/// \param container The underlying container type for BagContainerAdaptor that is std::multiset.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	value_type& frontImpl(std::multiset<value_type>& container) noexcept
	{
		// Using const_cast here because the value_type of std::multiset is constant.
		return const_cast<value_type&>(*container.begin());
	}

	/// Front function specialization for std::multiset in const context.
	/// \param container The underlying container type for BagContainerAdaptor that is std::multiset.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	const value_type& frontImpl(const std::multiset<value_type>& container) const noexcept
	{
		return *container.cbegin();
	}
	
	/// Front function specialization for std::unordered_multiset.
	/// \param container The underlying container type for BagContainerAdaptor that is std::unordered_multiset.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	value_type& frontImpl(std::unordered_multiset<value_type>& container) noexcept
	{
		// Using const_cast here because the value_type of std::unordered_multiset is constant.
		return const_cast<value_type&>(*container.begin());
	}
	
	/// Front function specialization for std::unordered_multiset in const context.
	/// \param container The underlying container type for BagContainerAdaptor that is std::unordered_multiset.
	/// \return Reference to the first item in underlying container.
	/// \ingroup frontImplementations
	const value_type& frontImpl(const std::unordered_multiset<value_type>& container) const noexcept
	{
		return *container.cbegin();
	}

	// \defgroup backImplementations
	
	/// Back function implementation for container types that have the back() member function.
	/// \param container The underlying container type where the element is accessed.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return Reference to the last item in underlying container.
	/// \ingroup backImplementations
	template <typename C>
	value_type& backImpl(C& container) noexcept
	{
		return container.back();
	}

	/// Back function specialization for std::forward_list.
	/// \param container The underlying container type where the element is accessed.
	/// \return Reference the the last item in underlying container.
	/// \ingroup backImplementations
	value_type& backImpl(std::forward_list<value_type>& container) noexcept
	{
		auto itLast = container.begin();

		while (std::next(itLast) != container.end())
		{
			++itLast;
		}
		return *itLast;
	}

	/// Back function specialization for std::multiset.
	/// \param container The underlying container type where the element is accessed.
	/// \return Reference to the last item in underlying container.
	/// \ingroup backImplementations
	value_type& backImpl(std::multiset<value_type>& container) noexcept
	{
		auto itLast = container.begin();
		std::advance(itLast, container.size() - 1);
		return const_cast<value_type&>(*itLast);
	}

	/// Back function specialization for std::unordered_multiset.
	/// \param container The underlying container type where the element is accessed.
	/// \return Reference to the last item in underlying container.
	/// \ingroup backImplementations
	value_type& backImpl(std::unordered_multiset<value_type>& container) noexcept
	{
		auto itLast = container.begin();

		while (std::next(itLast) != container.end())
		{
			++itLast;
		}
		return const_cast<value_type&>(*itLast);
	}

	/// \defgroup sizeImplementations

	/// Get the amount of elements in the underlying container type that has size() member function.
	/// \param container The underlying container that we get the amount of elements from.
	/// \tparam C The underlying container type used for BagContainerAdaptor.
	/// \return The amount of elements in the underlying container type.
	/// \ingroup sizeImplementations
	template <typename C>
	size_t sizeImpl(C& container) const noexcept
	{
		return container.size();
	}

	/// Get the amount of elements specialized for std::forward_list.
	/// \param container The underlying container that we get the amount of elements from.
	/// \return The amount of elements in the underlying container type.
	/// \ingroup sizeImplementations
	size_t sizeImpl(std::forward_list<value_type>& container) const noexcept
	{
		return std::distance(container.begin(), container.end());
	}

	/// Get the amount of elements specialized for const std::forward_list.
	/// \param container The underlying container that we get the amount of elements from.
	/// \return The amount of elements in the underlying container type.
	/// \ingroup sizeImplementations
	size_t sizeImpl(const std::forward_list<value_type>& container) const noexcept
	{
		return std::distance(container.begin(), container.end());
	}

private:
	/// The underlying container type as member.
	Container m_container;
};

#endif
