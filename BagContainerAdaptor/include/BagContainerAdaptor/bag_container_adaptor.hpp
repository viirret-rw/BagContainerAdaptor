#ifndef BAG_CONTAINER_ADAPTOR_HPP
#define BAG_CONTAINER_ADAPTOR_HPP

#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

/// Bag is an abstract data type that can store a collection of elements without regard to their order.
/// Equal elements can appear multiple times in a bag. Although the elements container in a bag have no inherit order,
/// iterating over the bag elements is guaranteed to visit each element exactly once.
/// This bag takes in a type and an STL-compatible container as a template arguments and provides the same functionality for each container type
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
    /// \exception noexcept No exceptions are thrown by this operation.
    BagContainerAdaptor() noexcept = default;

    /// Move constructor.
    /// \param container The underlying container from which the `BagContainerAdaptor` is constructed.
    /// \pre The underlying container must have a move constructor to support moving its contents.
    /// \post The `BagContainerAdapter` is constructed, taking ownership of the contents of the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
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
    /// \exception noexcept No exceptions are thrown by this operation.
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
    /// \pre Assumes the `other` BagContainerAdaptor is a valid instance
    /// \post Creates a new BagContainerAdaptor instance that is a copy of `other`.
    /// \exception noexcept No exceptions are thrown by this operation.
    BagContainerAdaptor(const BagContainerAdaptor& other) noexcept
        : m_container(other.m_container)
    {
    }

    /// Copy assignment operator.
    /// \param other The other BagContainerAdaptor where we copy from.
    /// \pre Assumes that both `this` and `other` BagContainerAdaptors are valid instances.
    /// \post Copies the content of `other` into `this` BagContainerAdaptor.
    /// \exception noexcept No exceptions are thrown by this operation.
    BagContainerAdaptor& operator=(const BagContainerAdaptor& other) noexcept
    {
        if (this != &other)
        {
            m_container = other.m_container;
        }
        return *this;
    }

    /// Insert element to the underlying container.
    /// \param value The value to be inserted.
    /// \return An iterator that points to the inserted element.
    /// \post The `value` is inserted to the underlying container, and the `BagContainerAdaptor` object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's insertion operations, this function might throw exceptions like `std::bad_alloc`
    ///            if memory allocation fails.
    iterator insert(const value_type& value)
    {
        return insertImpl(m_container, value);
    }

    /// Removes a specified element from the underlying container.
    /// \param elem An iterator pointing to the element to be removed from the underlying container.
    /// \pre The `elem` iterator must be a valid iterator that points to a position within the underlying container.
    /// \post The element at the specified `elem` in the underlying container is removed, and the `BagContainerAdaptor` object
    ///       is modified accordingly.
    /// \exception Depending on the underlying container's erase operation, this function might throw exceptions like:
    ///            - For std::vector: std::out_of_range if the `elem` iterator is invalid.
    /// \par Time complexity:
    /// - O(1) For containers with constant-time erase operation (e.g., std::vector, std::unordered_set, std::unordered_map).
    /// - O(n) For containers with linear-time erase operation (e.g., std::list, std::forward_list) where n is the number of elements.
    void erase(iterator elem)
    {
        return eraseImpl(m_container, elem);
    }

    /// Erase all elements that have the specified value in the underlying container.
    /// \param value The value of the elements that are removed.
    /// \post All elements equal to the specified value in the underlying container are removed, and the BagContainerAdaptor object
    ///       is modified accordingly.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    void erase(const value_type& value)
    {
        return eraseImpl(m_container, value);
    }

    /// Swap the contents of two BagContainerAdaptors.
    /// \param other The other bag to be swapped with.
    /// \post The contents of this BagContainerAdaptor are swapped with the contents of the `other` BagContainerAdaptor.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \note The swap operation is performed using the underlying container's swap operation, which is noexcept for most
    ///       standard containers (like std::vector, std::deque, std::list, std::forward_list, std::multiset, and std::unordered_multiset),
    ///       ensuring a fast and exception-safe swap.
    void swap(BagContainerAdaptor& other) noexcept
    {
        m_container.swap(other.m_container);
    }

    /// Get iterator pointing to the first element in the underlying container.
    /// "First" in reference to the implied iteration order withing the container.
    /// \return An iterator pointing to the first element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator begin() noexcept
    {
        return m_container.begin();
    }

    /// Get iterator pointing to the one past the last element in the underlying container.
    /// "Last" in reference to the implied iteration order withing the container.
    /// \return An iterator pointing one past the last element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator end() noexcept
    {
        return m_container.end();
    }

    /// Get a constant iterator pointing to the first element in the underlying container.
    /// "First" in reference to the implied iteration order withing the container.
    /// \return A constant iterator pointing to the first element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cbegin() const noexcept
    {
        return m_container.cbegin();
    }

    /// Get a constant iterator pointing one past the last element in the underlying container.
    /// "Last" in reference to the implied iteration order withing the container.
    /// \return A contant iterator pointing one past the last element in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cend() const noexcept
    {
        return m_container.cend();
    }

    /// Get iterator pointing to instance of element with specified value.
    /// \param value The value to compare elements to.
    /// \return An iterator pointing to the instance of element with specified value.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator find(const value_type& value) noexcept
    {
        return findImpl(m_container, value);
    }

    /// Get constant iterator pointing to instance of element with specified value in const context.
    /// \param value The value to compare elements to.
    /// \return An iterator pointing to the instance of element with specified value.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator find(const value_type& value) const noexcept
    {
        return findImpl(m_container, value);
    }

    /// Get reference to the implied first element in the underlying container in const context.
    /// \return Reference to the implied first element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    const value_type& front() const noexcept
    {
        return frontImpl(m_container);
    }

    /// Get reference to the implied last element in the underlying container.
    /// \return Reference to the implied last element in the underlying container.
    /// \pre The container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \par Time complexity:
    /// - 0(1) For containers that support the .back() member function, and std::multiset.
    /// - 0(n) For std::forward_list and std::unordered_multiset.
    const value_type& back() const noexcept
    {
        return backImpl(m_container);
    }

    /// Get the amount of elements in the underlying container.
    /// \return The amount of elements in the underlying container.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \par Time complexity:
    /// - O(1) For containers with constant-time size retrieval, such as std::unordered_multiset and std::queue.
    /// These containers maintain an internal count of elements, allowing direct and constant-time access to the size.
    /// - O(n) For containers with linear-time size retrieval, such as std::vector, std::deque, std::list and std::multiset.
    /// These containers do not maintain an internal count of elements, requiring iteration through the elements
    /// to determine the size, resulting in linear time complexity.
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
    /// \defgroup insertImplementations Insert functionality for various underlying container types.

    /// Insert element to the underlying container type.
    /// \param container The underlying container type where the element is inserted.
    /// \param value The value of the inserted element.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return An iterator that points to the inserted element.
    /// \pre The `container` must be a valid instance of the underlying container type.
    /// \post The element `value` is inserted at the last position of the `container`.
    /// \exception std::bad_alloc if memory allocation fails during elements insertion.
    /// \note Iterators might be invalidated in certain cases or specific container types,
    /// 	especially if reallocation occurs due to insufficient capacity.
    /// \ingroup insertImplementations
    template <typename C>
    iterator insertImpl(C& container, const value_type& value)
    {
        return container.insert(container.end(), value);
    }

    /// Insert element to the underlying container type specialized for std::forward_list.
    /// \param container The underlying container type where the element is inserted.
    /// \param value The value of the inserted element.
    /// \return An iterator that points to the inserted element.
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \post The element `value` is inserted at the last position of the `container`.
    /// \exception std::bad_alloc if memory allocation fails during elements insertion.
    /// \note Inserting elements to the beginning of std::forward_list invalidates iterators.
    /// \ingroup insertImplementations
    iterator insertImpl(std::forward_list<value_type>& container, const value_type& value)
    {
        return container.insert_after(container.before_begin(), value);
    }

    /// \defgroup eraseImplementations Erase functionality for various underlying container types.

    /// Erase item from the underlying container at the implied position of the iterator.
    /// \param container The underlying container type where the element is erased.
    /// \param pos The implied position where the item is removed from the container.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return Assumed next iterator following the deleted element.
    /// \pre The `container` must be a valid instance of the specified container type.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element at the position specified by `pos` is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    /// 		   This includes exceptions such as `std::bad_alloc` when memory allocation fails.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    template <typename C>
    void eraseImpl(C& container, iterator pos)
    {
        container.erase(pos);
    }

    /// Erase item from the underlying container at the implied position of the iterator specialized for std::forward_list.
    /// \param container The underlying container type where the element is erased.
    /// \param pos The position where the element is erased.
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element after the `pos` iterator is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception Any exception that may be thrown by the underlying container's `erase_after` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::forward_list<value_type>& container, iterator pos)
    {
        if (pos == container.begin())
        {
            container.pop_front();
            container.begin();
        }
        else
        {
            auto prev = container.before_begin();
            for (auto it = container.begin(); it != container.end(); ++it)
            {
                if (it == pos)
                {
                    container.erase_after(prev);
                }
                prev = it;
            }
        }
    }

    /// Erase item from the underlying container at the implied position of the iterator specialized for std::deque.
    /// \param container The underlying container type where the element is erased.
    /// \param pos The position where the element is erased.
    /// \pre The `container` must be a valid instance of std::deque.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element after the `pos` iterator is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception std::runtime_error If an error occurs during element removal.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::deque<value_type>& container, iterator pos)
    {
        auto end = container.end();
        auto it = std::find(begin(), end, *pos);

        if (it != end)
        {
            if (it != end - 1)
            {
                std::iter_swap(it, end - 1);
            }
            container.pop_back();
        }
    }

    /// Erase item from the underlying container at the implied position of the iterator specialized for std::vector.
    /// \param container The underlying container type where the element is erased.
    /// \param pos The position where the element is erased.
    /// \pre The `container` must be a valid instance of std::vector.
    /// \pre The `pos` iterator must be a valid iterator within the `container`.
    /// \post The element after the `pos` iterator is removed from the `container`.
    /// \exception std::out_of_range If the `pos` iterator is invalid or out of range for the container.
    /// \exception std::runtime_error If an error occurs during element removal.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::vector<value_type>& container, iterator pos)
    {
        auto end = container.end();
        auto it = std::find(begin(), end, *pos);

        if (it != end)
        {
            if (it != end - 1)
            {
                std::iter_swap(it, end - 1);
            }
            container.pop_back();
        }
    }

    /// Erase items from the underlying container that have a specified value.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the removed elements.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \pre The `container` must be a valid instance of the specified container type.
    /// \pre The `container` must have a member function erase.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    template <typename C>
    void eraseImpl(C& container, const value_type& value)
    {
        container.erase(value);
    }

    /// Removes all occurrences of a specified value specialized for std::forward_list.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the removed elements.
    /// \pre The `container` must be a valid instance of std::forward_list.
    /// \post All elements with the specified `value` are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::forward_list<value_type>& container, const value_type& value)
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
    }

    /// Removes all occurrences of a specified value specialized for std::deque.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the to be removed elements.
    /// \pre The `container` must be a valid instance of std::deque.
    /// \pre The `container` must not be in an invalid state or uninitialized.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::deque<value_type>& container, const value_type& value)
    {
        for (auto it = container.begin(); it != container.end();)
        {
            if (*it == value)
            {
                while (it != container.end() - 1 && container.back() == value)
                {
                    container.pop_back();
                }
                if (it != container.end() - 1)
                {
                    std::iter_swap(it, container.end() - 1);
                    container.pop_back();
                }
                else
                {
                    container.pop_back();
                }
            }
            else
            {
                ++it;
            }
        }
    }

    /// Removes all occurrences of a specified value specialized for std::list.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the to be removed elements.
    /// \pre The `container` must be a valid instance of std::list.
    /// \pre The `container` must not be in an invalid state or uninitialized.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::list<value_type>& container, const value_type& value)
    {
        for (auto it = container.begin(); it != container.end();)
        {
            if (*it == value)
            {
                it = container.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    /// Removes all occurrences of a specified value specialized for std::multiset.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the to be removed elements.
    /// \pre The `container` must be a valid instance of std::multiset.
    /// \pre The `container` must not be in an invalid state or uninitialized.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::multiset<value_type>& container, const value_type& value)
    {
        container.erase(container.equal_range(value).first, container.equal_range(value).second);
    }

    /// Removes all occurrences of a specified value specialized for std::unordered_multiset.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the to be removed elements.
    /// \pre The `container` must be a valid instance of std::unordered_multiset.
    /// \pre The `container` must not be in an invalid state or uninitialized.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::unordered_multiset<value_type>& container, const value_type& value)
    {
        container.erase(container.equal_range(value).first, container.equal_range(value).second);
    }

    /// Removes all occurrences of a specified value specialized for std::vector.
    /// \param container The underlying container type where the elements are erased.
    /// \param value The value of the to be removed elements.
    /// \pre The `container` must be a valid instance of std::vector.
    /// \pre The `container` must not be in an invalid state or uninitialized.
    /// \post All elements with the specified value are removed from the `container`.
    /// \exception Any exception that may be thrown by the underlying container's `erase` function.
    ///         This typically includes exceptions like those related to invalid iterators or invalid operations on the container.
    /// \note This function may invalidate all iterators pointing to elements within the container.
    /// \ingroup eraseImplementations
    void eraseImpl(std::vector<value_type>& container, const value_type& value)
    {
        for (auto it = container.begin(); it != container.end();)
        {
            if (*it == value)
            {
                while (it != container.end() - 1 && container.back() == value)
                {
                    container.pop_back();
                }
                if (it != container.end() - 1)
                {
                    std::iter_swap(it, container.end() - 1);
                    container.pop_back();
                }
                else
                {
                    container.pop_back();
                }
            }
            else
            {
                ++it;
            }
        }
    }

    /// \defgroup frontImplementations Functionality for getting the implied first element for various container types.

    /// Front function implementation for container types that have the front() member function in const context.
    /// \param container The underlying container type where the element is accessed.
    /// \tparam C The underlying container type used for BagContainerAdaptor.
    /// \return Reference to the implied first item in underlying container.
    /// \pre The `container` must be a valid instance of the specified container type, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    template <typename C>
    const value_type& frontImpl(const C& container) const noexcept
    {
        return container.front();
    }

    /// Front function specialization for std::forward_list in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::forward_list.
    /// \return Reference to the assumed first item in underlying container.
    /// \pre The `container` must be a valid instance of const std::forward_list, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    const value_type& frontImpl(const std::forward_list<value_type>& container) const noexcept
    {
        return *container.cbegin();
    }

    /// Front function specialization for std::multiset in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::multiset.
    /// \return Reference to the assumed first item in underlying container.
    /// \pre The `container` must be a valid instance of const std::multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup frontImplementations
    const value_type& frontImpl(const std::multiset<value_type>& container) const noexcept
    {
        return *container.cbegin();
    }

    /// Front function specialization for std::unordered_multiset in const context.
    /// \param container The underlying container type for BagContainerAdaptor that is std::unordered_multiset.
    /// \return Reference to the assumed first item in underlying container.
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
    const value_type& backImpl(const C& container) const noexcept
    {
        return container.back();
    }

    /// Back function specialization for std::forward_list.
    /// \param container The underlying container type where the element is accessed.
    /// \return Reference the the last item in underlying container.
    /// \pre The `container` must be a valid instance of std::forward_list, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup backImplementations
    const value_type& backImpl(const std::forward_list<value_type>& container) const noexcept
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
    const value_type& backImpl(const std::multiset<value_type>& container) const noexcept
    {
        return *container.crbegin();
    }

    /// Back function specialization for std::unordered_multiset.
    /// \param container The underlying container type where the element is accessed.
    /// \return Reference to the last item in underlying container.
    /// \pre The `container` must be a valid instance of std::unordered_multiset, and the container must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup backImplementations
    const value_type& backImpl(const std::unordered_multiset<value_type>& container) const noexcept
    {
        auto itLast = container.cbegin();

        while (std::next(itLast) != container.cend())
        {
            ++itLast;
        }
        return *itLast;
    }

    /// \defgroup findImplementations Functionality for looking up elements in the underlying container

    /// Find element from the underlying container.
    /// \param container The underlying container type where the element is looked up.
    /// \param value The value that is looked up from the container.
    /// \return Iterator to the found element if found or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    template <typename C>
    iterator findImpl(C& container, const value_type& value) noexcept
    {
        return std::find(container.begin(), container.end(), value);
    }

    /// Find element from the underlying container, when the container type is std::multiset.
    /// \param container The underlying container type that is std::multiset.
    /// \param value The value that is looked up from the container.
    /// \return Iterator to the found element if found, or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    iterator findImpl(std::multiset<value_type>& container, const value_type& value) noexcept
    {
        return container.find(value);
    }

    /// Find the element from the underlying container, when the container type is std::unordered_multiset.
    /// \param container The undelying container type that is std::unordered_multiset.
    /// \param value The value that is looked up from the container.
    /// \return Iterator to the found element if found, or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    iterator findImpl(std::unordered_multiset<value_type>& container, const value_type& value) noexcept
    {
        return container.find(value);
    }

    /// Find element from the underlying container in const context.
    /// \param container The underlying container type where the element is looked up.
    /// \param value The value that is looked up from the container.
    /// \return Constant iterator to the found element if found, or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    template <typename C>
    const_iterator findImpl(const C& container, const value_type& value) const noexcept
    {
        return std::find(container.cbegin(), container.cend(), value);
    }

    /// Find element from the underlying container that is std::multiset in const context.
    /// \param container The underlying container type where the element is looked up.
    /// \param value The value that is looked up from the container.
    /// \return Constant iterator to the found element if found, or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    const_iterator findImpl(const std::multiset<value_type>& container, const value_type& value) const noexcept
    {
        return container.find(value);
    }

    /// Find element from the underlying container that is std::unordered_multiset in const context.
    /// \param container The underlying container that is const std::unordered_multiset, where the element is looked up.
    /// \param value The value that is looked up from the container.
    /// \return Constant iterator to the found element if found, or container.end().
    /// \exception noexcept No exceptions are thrown by this operation.
    /// \ingroup findImplementations
    const_iterator findImpl(const std::unordered_multiset<value_type>& container, const value_type& value) const noexcept
    {
        return container.find(value);
    }

    /// \defgroup sizeImplementations Functionality for getting the amount of elements for various container types.

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
    /// The underlying container type.
    Container m_container;
};

#endif
