#ifndef BAG_CONTAINER_ADAPTOR_HPP
#define BAG_CONTAINER_ADAPTOR_HPP

#include <algorithm>
#include <deque>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <vector>

/// Bag is an abstract data type that can store a collection of elements without regard to their order.
/// Equal elements can appear multiple times in a bag. Although the elements container in a bag have no inherit order,
/// iterating over the bag elements is guaranteed to visit each element exactly once.
/// This bag takes in an stl container as a template argument and provides the same functionality for each container type
/// following the design pattern of an adapter.
/// \tparam Type the type of the items in the underlying type.
/// \tparam Container The underlying container type.
template <typename Type, typename Container = std::vector<Type>>
class BagContainerAdaptor
{
public:
    /// The value type of the underlying container.
    using value_type = typename Container::value_type;

    /// The iterator of the underlying container.
    using iterator = typename Container::iterator;

    /// The constant iterator of the underlying container.
    using const_iterator = typename Container::const_iterator;

    /// Default constructor.
    /// \exception noexcept The constructor is marked `noexcept` to guarantee no exceptions will be thrown during the
    ///                      construction, providing a strong exception safety guarantee.
    BagContainerAdaptor() noexcept
    {
    }

    /// Destructor.
    /// \post The `BagContainerAdaptor` object is destructed, and the `m_container` is deallocated, releasing any resources
    ///       held by the container.
    /// \exception noexcept The destructor is marked `noexcept` to guarantee no exceptions will be thrown during the
    ///                      destruction, providing a strong exception safety guarantee.
    ~BagContainerAdaptor() noexcept
    {
        deallocateContainer(m_container);
    }

    /// Move constructor.
    /// \param container The underlying container from which the `BagContainerAdaptor` is constructed.
    /// \pre The underlying container must have a move constructor to support moving its contents.
    /// \post The `BagContainerAdapter` is constructed, taking ownership of the contents of the underlying container.
    /// \exception noexcept The move constructor is marked noexcept to guarantee no exceptions will be
    /// 					thrown during the move operation, providing strong exception safety.
    BagContainerAdaptor(Container&& container) noexcept
        : m_container(std::move(container))
    {
    }

    /// Move assignment operator.
    /// \param other The underlying container from which the `BagContainerAdaptor` is created.
    /// \return Reference to the current `BagContainerAdaptor` object after the move assignment.
    /// \pre The `Container` type must have a move assignment operator to support moving its contents.
    /// \post The current `BagContainerAdaptor` object takes ownership of the contents of the other
    ///		  `BagContainerAdaptor`. The other `BagContainerAdaptor` is left in a valid but unspecified state.
    /// \exception noexcept The move assignment operator is marked noexcept to guarantee no exceptions will be
    /// 					thrown during the move assignment, providing a string exception safety.
    BagContainerAdaptor& operator=(BagContainerAdaptor&& other) noexcept
    {
        if (this != &other)
        {
            m_container = std::move(other.m_container);
        }
        return *this;
    }

    /// Copy constructor.
    /// \param other The other BagContainerAdaptor where we copy from.
    BagContainerAdaptor(const BagContainerAdaptor& other) noexcept
		: m_container(other.m_container)
	{
	}

    /// Copy assignment operator.
    /// \param other The other BagContainerAdaptor where we copy from.
    BagContainerAdaptor& operator=(const BagContainerAdaptor& other) noexcept
	{
		if (this != &other)
		{
			m_container = other.m_container;
		}
		return *this;
	}

    /// Insert element to specified position of the underlying container.
    /// \param pos The specified position where the element is inserted.
    /// \param value The value to be inserted.
    /// \tparam Iterator The iterator type of the underlying container.
    /// \return An iterator that points to the inserted element.
    /// \pre The `pos` iterator must be a valid iterator that points to a position within the underlying container.
    /// \post The `value` is inserted at the specified `pos` in the underlying container, and the `BagContainerAdaptor` object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's insertion operations, this function might throw exceptions like `std::bad_alloc`
    ///            if memory allocation fails.
    template <typename Iterator>
    iterator insert(Iterator pos, const value_type& value)
    {
        return insertImpl(m_container, pos, value);
    }

    /// Insert element to the last position of the underlying container.
    /// \param value The value to be inserted.
    /// \return An iterator that points to the inserted element.
    /// \post The `value` is inserted at the last position of the underlying container, and the `BagContainerAdaptor` object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's insertion operations, this function might throw exceptions like `std::bad_alloc`
    ///            if memory allocation fails.
    iterator insert(const value_type& value)
    {
        return insertImpl(m_container, value);
    }

    /// Erase element from the specified position in the underlying container.
    /// \param pos The specified position where the element is erased.
    /// \return An iterator following the last removed element.
    /// \pre The `pos` iterator must be a valid iterator that points to a position within the underlying container.
    /// \post The element at the specified `pos` in the underlying container is removed, and the `BagContainerAdaptor` object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's erase operation, this function might throw exceptions like:
    ///            - For std::vector: std::out_of_range if the pos iterator is invalid.
    /// \note The iterator returned points to the element that follows the erased element in the underlying container. If `pos`
    ///         points to the last element, the returned iterator is the `end()` iterator of the container.
    iterator erase(iterator pos)
    {
        return eraseImpl(m_container, pos);
    }

    /// Erase all elements that have the specified value in the underlying container.
    /// \param value The value of the elements that are removed.
    /// \return An iterator following the last removed element.
    /// \post All elements equal to the specified value in the underlying container are removed, and the BagContainerAdaptor object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's erase operation, this function might throw exceptions like:
    ///            - For std::vector with move iterator: std::out_of_range if the pos iterator is invalid or if the move constructor
    ///                                                of the contained type throws an exception.
    ///            - For other containers: No exceptions are thrown by the erase operation itself unless specified by the container.
    /// \note The iterator returned points to the element that follows the last removed element in the underlying container. If no
    ///         element with the specified value is found or if all occurrences of the value are removed, the returned iterator is
    ///         the end() iterator of the container.
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
    /// \post All elements between the specified range [first, last] in the underlying container are removed, and the
    ///       `BagContainerAdaptor` object is modified accordingly.
    /// \exception Depending on the underlying container's erase operation, this function might throw exceptions like:
    ///            - For std::vector with move iterator: std::out_of_range if the iterators are invalid or if the move constructor
    ///                                                of the contained type throws an exception.
    ///            - For other containers: No exceptions are thrown by the erase operation itself unless specified by the container.
    /// \note The iterator returned points to the element that follows the last removed element in the underlying container. If no
    ///       elements are removed, the returned iterator is the one pointed to by the "last" iterator parameter.
    template <typename FirstType, typename LastType>
    iterator erase(FirstType first, LastType last)
    {
        return eraseImpl(m_container, first, last);
    }

    /// Swap the contents of two BagContainerAdaptors.
    /// \param other The other bag to be swapped with.
    /// \post The contents of this BagContainerAdaptor are swapped with the contents of the "other" BagContainerAdaptor.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \note The swap operation is performed using the underlying container's swap operation, which is noexcept for most
    ///       standard containers (like std::vector, std::deque, std::list, std::forward_list, std::multiset, and std::unordered_multiset),
    ///       ensuring a fast and exception-safe swap.
    void swap(BagContainerAdaptor& other) noexcept
    {
        m_container.swap(other.m_container);
    }

    /// Get iterator pointing to the first element in the underlying container.
    /// \return Iterator pointing to the first element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator begin() noexcept
    {
        return m_container.begin();
    }

    /// Get iterator pointing to the last element in the underlying container.
    /// \return Iterator pointing to the last element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator end() noexcept
    {
        return m_container.end();
    }

    /// Get constant iterator pointing to the first element in the underlying container.
    /// \return Constant iterator pointing to the first element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cbegin() const noexcept
    {
        return m_container.cbegin();
    }

    /// Get constant iterator pointing to the last element in the underlying container.
    /// \return Contant iterator pointing to the last element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cend() const noexcept
    {
        return m_container.cend();
    }

    /// Get iterator pointing to the first instance of element with specified value.
    /// \param value The value to compare elements to.
    /// \return An iterator pointing to the first instance of element with specified value.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator find(const value_type& value) noexcept
    {
        return std::find(m_container.begin(), m_container.end(), value);
    }

    /// Get constant iterator pointing to the first instance of element with specified value in const context.
    /// \param value The value to compare elements to.
    /// \return An iterator pointing to the first instance of element with specified value.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator find(const value_type& value) const noexcept
    {
        return std::find(m_container.cbegin(), m_container.cend(), value);
    }

    /// Get reference to the first element in the underlying container.
    /// \return Reference to the first element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    value_type& front() noexcept
    {
        return frontImpl(m_container);
    }

    /// Get reference to the first element in the underlying container in const context.
    /// \return Reference to the first element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    const value_type& front() const noexcept
    {
        return frontImpl(m_container);
    }

    /// Get reference to the last element in the underlying container.
    /// \return Reference to the last element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    value_type& back() noexcept
    {
        return backImpl(m_container);
    }

    /// Get reference to the last element in the underlying container.
    /// \return Reference to the last element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    const value_type& back() const noexcept
    {
        return m_container.back();
    }

    /// Get the amount of elements in the underlying container.
    /// \return The amount of elements in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
	std::size_t size() const noexcept
    {
        return sizeImpl(m_container);
    }

    /// Get boolean describing if the underlying container is empty or not.
    /// \return Boolean describing if the underlying container is empty or not.
    /// \exception noexcept No exceptions are thrown by this operation.
    bool empty() const noexcept
    {
        return m_container.empty();
    }

private:
    /// \defgroup containerDestructors Functions called by destructor for different underlying container types.

    /// Default destructor implementation.
    /// \param bag Default container type.
    /// \tparam C The underlying container type.
    /// \post The function does nothing as the default implementation for deallocation.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup containerDestructors
    template <typename C>
    void deallocateContainer(C&) noexcept
    {
    }

    /// Destructor specialization for std::deque.
    /// \param bag BagContainerAdapter specialized for std::deque.
    /// \post The internal container of BagContainerAdaptor (std::deque) is cleared.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup containerDestructors.
    void deallocateContainer(BagContainerAdaptor<std::deque<value_type>>& bag) noexcept
    {
        bag.clear();
    }

    /// \defgroup insertImplementations Insert functionality for various underlying container types.

    /// Insert element to the last position of the underlying container type.
    /// \param container The underlying container type where the element is inserted.
    /// \param value The value of the inserted element.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return An iterator that points to the inserted element.
    /// \pre The `container` must be a valid instance of the underlying container type.
    /// \post The element `value` is inserted at the last position of the `container`.
    /// \exception std::bad_alloc if memory allocation fails during elements insertion.
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
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \post The element `value` is inserted at the last position of the `container`.
    /// \exception std::bad_alloc if memory allocation fails during elements insertion.
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
    /// \param Iterator The position in the underlying container where the element is inserted.
    /// \param value The value of the inserted element.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \tparam Iterator The iterator type of the underlying container.
    /// \pre The `container` must be a valid instance of the underlying container type.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element `value` is inserted at the specified position in the `container`.
    /// \exception std::bad_alloc if memory allocation fails (applicable to std::vector, std::deque).
    /// \exception std::length_error if the underlying container exceeds its maximum size (applicable to std::forward_list, std::list, std::deque).
    /// \return An iterator that points to the inserted element.
    /// \ingroup insertImplementations
    template <typename C, typename Iterator>
    iterator insertImpl(C& container, Iterator pos, const value_type& value)
    {
        return container.insert(pos, value);
    }

    /// Insert element to a specified position specialized for std::forward_list.
    /// \param container The underlying container type where the element is inserted.
    /// \param pos Iterator to the position in the underlying container where the element is inserted.
    /// \param value The value of the inserted element.
    /// \tparam Iterator The position in the underlying container where the element is inserted.
    /// \return An iterator that points to the inserted element.
    /// \pre The `container` must be a valid instance of std::forward_list<value_type>.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element `value` is inserted at the specified position in the `container`.
    /// \exception std::bad_alloc if memory allocation fails.
    /// \ingroup insertImplementations
    template <typename Iterator>
    iterator insertImpl(std::forward_list<value_type>& container, Iterator pos, const value_type& value)
    {
        auto itPrev = container.before_begin();
        for (auto it = container.begin(); it != pos; ++it)
        {
            itPrev = it;
        }
        return container.insert_after(itPrev, value);
    }

    /// \defgroup eraseImplementations Erase functionality for various underlying container types.

    /// Erase item from the underlying container in the position of the iterator.
    /// \param container The underlying container type where the element is erased.
    /// \param pos The position where the item is removed from the container.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return An iterator following the removed element.
    /// \pre The `container` must be a valid instance of the specified container type.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element at the position specified by `pos` is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    /// 		   This includes exceptions such as `std::bad_alloc` when memory allocation fails.
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
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element after the `pos` iterator is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception Any exception that may be thrown by the underlying container's `erase_after` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
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
    /// \pre The `container` must be a valid instance of the specified container type.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
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
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
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
    /// \pre The `container` must be a valid instance of a container type that supports range erasure,
    ///      and the range defined by `first` and `last` must be valid within the container.
    /// \post All elements within the specified range are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \ingroup eraseImplementations
    template <typename C, typename FirstType, typename LastType>
    iterator eraseImpl(C& container, FirstType first, LastType last)
    {
        return container.erase(first, last);
    }

    /// Erase items from the underlying container type between two iterator specialized for std::forward_list.
    /// \param container The underlying container type where the elements are erased.
    /// \param first The first element in the range of elements to be removed.
    /// \param last The last element in the range of elements to be removed.
    /// \tparam FirstType The iterator type pointing to the first element to be removed.
    /// \tparam LastType The iterator type pointing to the last element to be removed.
    /// \return An iterator following the last removed element.
    /// \pre The `container` must be a valid instance of std::forward_list<value_type> or a compatible type,
    ///      and the range defined by `first` and `last` must be valid within the container.
    /// \post All elements within the specified range are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase_after` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \ingroup eraseImplementations
    template <typename FirstType, typename LastType>
    iterator eraseImpl(std::forward_list<value_type>& container, FirstType first, LastType last)
    {
        return container.erase_after(first, last);
    }

    /// \defgroup frontImplementations Functionality for getting the first element for various container types.

    /// Front function implementation for container types that have the front() member function.
    /// \param container The underlying container type where the element is accessed.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \pre The `container` must be a valid instance of the specified container type, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
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
    /// \pre The `container` must be a valid instance of the specified container type, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    template <typename C>
    const value_type& frontImpl(const C& container) const noexcept
    {
        return container.front();
    }

    /// Front function specialization for std::forward_list.
    /// \param container The underlying container type for BagContainerAdaptor that is std::forward_list.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of std::forward_list, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    value_type& frontImpl(std::forward_list<value_type>& container) noexcept
    {
        return *container.begin();
    }

    /// Front function specialization for std::forward_list in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::forward_list.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of const std::forward_list, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    const value_type& frontImpl(const std::forward_list<value_type>& container) const noexcept
    {
        return *container.cbegin();
    }

    /// Front function specialization for std::multiset.
    /// \param container The underlying container type for BagContainerAdaptor that is std::multiset.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of std::multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    value_type& frontImpl(std::multiset<value_type>& container) noexcept
    {
        // Using const_cast here because the value_type of std::multiset is constant.
        return const_cast<value_type&>(*container.begin());
    }

    /// Front function specialization for std::multiset in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::multiset.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of const std::multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    const value_type& frontImpl(const std::multiset<value_type>& container) const noexcept
    {
        return *container.cbegin();
    }

    /// Front function specialization for std::unordered_multiset.
    /// \param container The underlying container type for BagContainerAdaptor that is std::unordered_multiset.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of std::unordered_multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    value_type& frontImpl(std::unordered_multiset<value_type>& container) noexcept
    {
        // Using const_cast here because the value_type of std::unordered_multiset is constant.
        return const_cast<value_type&>(*container.begin());
    }

    /// Front function specialization for std::unordered_multiset in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::unordered_multiset.
    /// \return Reference to the first item in underlying container.
    /// \pre The `container` must be a valid instance of const std::unordered_multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    const value_type& frontImpl(const std::unordered_multiset<value_type>& container) const noexcept
    {
        return *container.cbegin();
    }

    // \defgroup backImplementations Functionality for getting the last element for various container types.

    /// Back function implementation for container types that have the back() member function.
    /// \param container The underlying container type where the element is accessed.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return Reference to the last item in underlying container.
    /// \pre The `container` must be a valid instance of a container type that supports the back() member function, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup backImplementations
    template <typename C>
    value_type& backImpl(C& container) noexcept
    {
        return container.back();
    }

    /// Back function specialization for std::forward_list.
    /// \param container The underlying container type where the element is accessed.
    /// \return Reference the the last item in underlying container.
    /// \pre The `container` must be a valid instance of std::forward_list, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
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
    /// \pre The `container` must be a valid instance of std::multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
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
    /// \pre The `container` must be a valid instance of std::unordered_multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
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

    /// \defgroup sizeImplementations Funcionality for getting the amount of elements for various container types.

    /// Get the amount of elements in the underlying container type that has size() member function.
    /// \param container The underlying container that we get the amount of elements from.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return The amount of elements in the underlying container type.
    /// \pre The `container` must be a valid instance of the specified container type.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup sizeImplementations
    template <typename C>
    std::size_t sizeImpl(C& container) const noexcept
    {
        return container.size();
    }

    /// Get the amount of elements specialized for std::forward_list.
    /// \param container The underlying container that we get the amount of elements from.
    /// \return The amount of elements in the underlying container type.
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup sizeImplementations
	std::size_t sizeImpl(std::forward_list<value_type>& container) const noexcept
    {
        return std::distance(container.begin(), container.end());
    }

    /// Get the amount of elements specialized for const std::forward_list.
    /// \param container The underlying container that we get the amount of elements from.
    /// \return The amount of elements in the underlying container type.
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup sizeImplementations
	std::size_t sizeImpl(const std::forward_list<value_type>& container) const noexcept
    {
        return std::distance(container.begin(), container.end());
    }

private:
    /// The underlying container type as member.
    Container m_container;
};

#endif
