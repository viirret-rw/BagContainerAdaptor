#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

/// LinkedListNode represents a single node in the linked list.
/// \tparam T The type of data stored in the node.
template <typename T>
struct LinkedListNode
{
    /// Constructor.
    /// \param value The value of the data in the Node.
    /// \post The `m_data` is initialized with value.
    /// \exception noexcept No exceptions are thrown by this operation.
    explicit LinkedListNode(const T& value) noexcept
        : m_data(value)
    {
    }

    /// The data inside the Node.
    T m_data;

    /// Pointing towards the next item in the linked list.
    LinkedListNode<T>* m_next = nullptr;

    /// Pointing towards the previous item in the linked list.
    LinkedListNode<T>* m_inverse = nullptr;
};

/// This linked list is stl compatible a doubly linked list containing basic
/// functionality for container and four different iterator types.
/// \tparam T The type of elements stored in the linked list.
/// \tparam Allocator The type of allocator used in this linked list,
/// initialized as std::allocator by default.
template <typename T, typename Allocator = std::allocator<LinkedListNode<T>>>
class LinkedList
{
public:
	/// The type of items stored in the linked list.
    using value_type = T;

    /// A bidirectional iterator for traversing elements in the linked list.
	class iterator : public std::iterator<
                         std::bidirectional_iterator_tag,
                         LinkedListNode<T>,
                         std::ptrdiff_t,
                         LinkedListNode<T>*,
                         LinkedListNode<T>&>
    {
    public:
        /// Default constructor.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator() noexcept
        {
        }

        /// Constructor.
        /// \param node Pointer to the `LinkedListNode` to initialize the iterator with.
        /// \post The iterator is constructed with the given `LinkedListNode` as the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        explicit iterator(LinkedListNode<T>* node) noexcept
            : m_currentNode(node)
        {
        }

        /// Dereference operator for the iterator.
        /// \return A reference to the data of the current node.
        /// \post Returns a reference to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        T& operator*() const noexcept
        {
            return m_currentNode->m_data;
        }

        /// Arrow operator for the iterator.
        /// \return A pointer to the data of the current node.
        /// \post Returns a pointer to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        T* operator->() const noexcept
        {
            return &(m_currentNode->m_data);
        }

        /// Pre-increment operator for the iterator.
        /// \return A reference to the iterator after the increment.
        /// \post Moves the iterator to the next node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator& operator++() noexcept
        {
            m_currentNode = m_currentNode->m_next;
            return *this;
        }

        /// Post-increment operator for the iterator.
        /// \return An iterator pointing to the previous position before the increment (this).
        /// \post Moves the iterator to the next node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator operator++(int) noexcept
        {
            iterator temp = *this;
            m_currentNode = m_currentNode->m_next;
            return temp;
        }

        /// Pre-decrement operator for the iterator.
        /// \return A reference to the iterator after the decrement.
        /// \post Moves the iterator to the previous node in the linked list.
        iterator& operator--()
        {
            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_inverse;
            }
            else
            {
                std::cerr << "Cannot decrement iterator from end() iterator" << std::endl;
                std::terminate();
            }
            return *this;
        }

        /// Post-decrement operator for the iterator.
        /// \return An iterator pointing to the previous position before the decrement (this).
        /// \post Moves the iterator to the previous node in the linked list.
        iterator operator--(int)
        {
            iterator temp = *this;

            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_inverse;
            }
            else
            {
                std::cerr << "Cannot decrement iterator from end() iterator" << std::endl;
                std::terminate();
            }
            return temp;
        }

        /// Equality comparison operator for the iterator.
        /// \param other The iterator to compare with.
        /// \return True if both iterators point to the same node, otherwise false.
        /// \post Checks if the current node of this iterator is equal to the current node of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator==(const iterator& other) const noexcept
        {
            return m_currentNode == other.m_currentNode;
        }

        /// Inequality comparison operator for the iterator.
        /// \param other The iterator to compare with.
        /// \return True if both iterators do not point to the same node, otherwise false.
        /// \post Checks if the current node of this iterator is not equal to the current node of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator!=(const iterator& other) const noexcept
        {
            return m_currentNode != other.m_currentNode;
        }

        /// Copy constructibility from const_iterator.
        /// \param it The constant iterator to copy construct from.
        /// \post The iterator is constructed with the same current node as the constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator(const typename LinkedList<T>::const_iterator& it) noexcept
            : m_currentNode(const_cast<LinkedListNode<T>*>(it.getNode()))
        {
        }

        /// Copy assignment from const_iterator.
        /// \param it The constant iterator to copy assign from.
        /// \return A reference to the iterator after the assignment.
        /// \post The iterator is assigned with the same current node as the constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator& operator=(const typename LinkedList<T>::const_iterator& it) noexcept
        {
            m_currentNode = const_cast<LinkedListNode<T>*>(it.getNode());
            return *this;
        }

        /// Move constructibility from const_iterator.
        /// \param it The constant iterator to move construct from.
        /// \post The iterator is constructed with the same current node as the constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator(typename LinkedList<T>::const_iterator&& it) noexcept
            : m_currentNode(const_cast<LinkedListNode<T>*>(it.getNode()))
        {
        }

        /// Move assignment from const_iterator.
        /// \param it The constant iterator to move assign from.
        /// \return A reference to the iterator after the assignment.
        /// \post The iterator is assigned with the same current node as the constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator& operator=(typename LinkedList<T>::const_iterator&& it) noexcept
        {
            m_currentNode = const_cast<LinkedListNode<T>*>(it.getNode());
            return *this;
        }

        /// Copy constructor.
        /// \param other The iterator to be copied.
        /// \post The iterator is constructed as a copy of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator(const iterator& other) noexcept = default;

        /// Copy assignment operator.
        /// \param other The iterator to be copied.
        /// \return A reference to the iterator after the assignment.
        /// \post The iterator is assigned as a copy of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator& operator=(const iterator& other) noexcept = default;

        /// Move constructor.
        /// \param other The iterator to be moved.
        /// \post The iterator is constructed by moving the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator(iterator&& other) noexcept = default;

        /// Move assignment operator.
        /// \param other The iterator to be moved.
        /// \return A reference to the iterator after the assignment.
        /// \post The iterator is assigned by moving the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        iterator& operator=(iterator&& other) noexcept = default;

        /// Get the Node where the iterator is pointing.
        /// \return A pointer to the current node where the iterator is pointing.
        /// \post Returns a pointer to the current node where the iterator is pointing.
        /// \exception noexcept No exceptions are thrown by this operation.
        LinkedListNode<T>* getNode() const noexcept
        {
            return m_currentNode;
        }

    private:
        /// Pointer to the current node where the iterator is pointing.
        /// \note The iterator should always point to a valid node in the linked list,
        /// 	or it should be nullptr if it has reached the end of the list.
        LinkedListNode<T>* m_currentNode;
    };

	/// A bidirectional constant iterator for traversing elements in the linked list.
    class const_iterator : public std::iterator<
                               std::bidirectional_iterator_tag,
                               const LinkedListNode<T>,
                               std::ptrdiff_t,
                               const LinkedListNode<T>*,
                               const LinkedListNode<T>&>
    {
    public:
        /// Default constructor.
        const_iterator() noexcept
        {
        }

        /// Constructor.
        /// \param node Pointer to the LinkedListNode to initialize constant iterator with.
        /// \post The iterator is constructed with the given LinkedListNode as the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        explicit const_iterator(LinkedListNode<T>* node) noexcept
            : m_currentNode(node)
        {
        }

        /// Deference operator for the constant iterator.
        /// \return A constant reference to the data of the current node.
        /// \post Returns a constant reference to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        const T& operator*() const noexcept
        {
            return m_currentNode->m_data;
        }

        /// Arrow operator for the constant iterator.
        /// \return A constant pointer to the data of the current node.
        /// \post Returns a constant pointer to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        const T* operator->() const noexcept
        {
            return &(m_currentNode->m_data);
        }

        /// Pre-increment operator for the constant iterator.
        /// \return A reference to the constant iterator after the increment.
        /// \post Moves the constant iterator to the next node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator& operator++() noexcept
        {
            m_currentNode = m_currentNode->m_next;
            return *this;
        }

        /// Post-increment operator for the constant iterator.
        /// \return An iterator pointing to the previous position before the increment.
        /// \post Moves the constant iterator to the next node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator operator++(int) noexcept
        {
            const_iterator temp = *this;
            m_currentNode = m_currentNode->m_next;
            return temp;
        }

        /// Pre-decrement operator for the constant iterator.
        /// \return A reference to the constant iterator after the decrement.
        /// \post Moves the iterator to the previous node in the linked list.
        const_iterator& operator--()
        {
            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_inverse;
            }
            else
            {
                std::cerr << "Cannot decrement constant iterator from end() iterator" << std::endl;
                std::terminate();
            }
            return *this;
        }

        /// Post-decrement operator for the constant iterator.
        /// \return An iterator pointing to the previous position before the decrement (this).
        /// \post Moves the iterator to the previous node in the linked list.
        const_iterator operator--(int)
        {
            const_iterator temp = *this;
            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_inverse;
            }
            else
            {
                std::cerr << "Cannot decrement constant iterator from end() iterator" << std::endl;
                std::terminate();
            }
            return temp;
        }

        /// Equality comparison operator for the iterator.
        /// \param other The constant iterator to compare with.
        /// \return True if both of the constant iterators point to the same node, otherwise false.
        /// \post Checks if the current node of this constant iterator is equal to the current node of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator==(const const_iterator& other) const noexcept
        {
            return m_currentNode == other.m_currentNode;
        }

        /// Inequality comparison operator for the iterator.
        /// \param other The constant iterator to compare with.
        /// \return True if both iterators do not point to the same node, otherwise false.
        /// \post Checks if the current node of this constant iterator is not equal to the current node of the other constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator!=(const const_iterator& other) const noexcept
        {
            return m_currentNode != other.m_currentNode;
        }

        /// Copy constructibility from non-const iterator.
        /// \param it The `iterator` to copy construct from.
        /// \post The constant iterator is constructed with the same current node as the `iterator`.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator(const typename LinkedList<T>::iterator& it) noexcept
            : m_currentNode(it.getNode())
        {
        }

        /// Copy assignment from non-const iterator.
        /// \param it The `iterator` to copy assign from.
        /// \return A reference to the constant iterator after the assignment.
        /// \post The constant iterator is assigned with the same current node as the `iterator`.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator& operator=(const typename LinkedList<T>::iterator& it) noexcept
        {
            m_currentNode = it.getNode();
            return *this;
        }

        /// Move constructibility from non-const iterator.
        /// \param it The non-const iterator to be moved from.
        /// \post The const_iterator is constructed, taking ownership of the internal
        /// 	pointer from the non-const iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator(typename LinkedList<T>::iterator&& it) noexcept
            : m_currentNode(it.getNode())
        {
        }

        /// Move assingment from non-const iterator.
        /// \param it The non-const iterator to be moved from.
        /// \return A reference to the constant iterator after the assignment.
        /// \post The const_iterator is assigned the value of the non-const iterator, taking ownership of the internal pointer.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator& operator=(typename LinkedList<T>::iterator&& it) noexcept
        {
            m_currentNode = it.getNode();
            return *this;
        }

        /// Copy constructor.
        /// \param other The constant iterator to be copied.
        /// \post The iterator is constructed as a copy of the other constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator(const const_iterator& other) noexcept = default;

        /// Copy assignment operator.
        /// \param other The constant iterator to be copied.
        /// \return A reference to the constant iterator after the assignment.
        /// \post The constant iterator as assigned as a copy of the other constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator& operator=(const const_iterator& other) noexcept = default;

        /// Move constructor.
        /// \param other The constant iterator to be moved.
        /// \post The constant iterator is constructed by moving the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator(const_iterator&& other) noexcept = default;

        /// Move assignment operator.
        /// \param other The constant iterator to be moved.
        /// \return A reference to the constant iterator after the assignment.
        /// \post The constant iterator is assigned by moving the other constant iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_iterator& operator=(const_iterator&& other) noexcept = default;

        /// Get the Node where the constant iterator is pointing.
        /// \return A pointer to the current node where the constant iterator is pointing.
        /// \post Returns a pointer to the current node where the constant iterator is pointing.
        /// \exception noexcept No exceptions are thrown by this operation.
        const LinkedListNode<T>* getNode() const noexcept
        {
            return m_currentNode;
        }

    private:
        /// Pointer to the current node where the iterator is pointing.
        /// \note The iterator should always point to a valid node in the linked list,
        /// 	or it should be nullptr if it has reached the end of the list.
        LinkedListNode<T>* m_currentNode;
    };

    /// A bidirectional reverse iterator for traversing items backwards in the linked list.
	class reverse_iterator : public std::iterator<
                                 std::bidirectional_iterator_tag,
                                 const LinkedListNode<T>,
                                 std::ptrdiff_t,
                                 const LinkedListNode<T>*,
                                 const LinkedListNode<T>&>
    {
    public:
        /// Default constructor.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator() noexcept
        {
        }

        /// Constructor.
        /// \param node Pointer to the `LinkedListNode` to initialize reverse iterator with.
        /// \post The reverse iterator is constructed with the given `LinkedListNode` as the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        explicit reverse_iterator(LinkedListNode<T>* node) noexcept
            : m_currentNode(node)
        {
        }

        /// Dereference operator for the reverse iterator.
        /// \return A reference to the data of the current node.
        /// \post Returns a reference to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        T& operator*() const noexcept
        {
            return m_currentNode->m_data;
        }

        /// Arrow operator for the reverse iterator.
        /// \return A pointer to the data of the current node.
        /// \post Returns a pointer to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        T* operator->() const noexcept
        {
            return &(m_currentNode->m_data);
        }

        /// Pre-increment operator for the reverse iterator.
        /// \return A reference to the reverse iterator after the increment.
        /// \post Moves the reverse iterator to the previous node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator& operator++()
        {
            m_currentNode = m_currentNode->m_inverse;
            return *this;
        }

        /// Post-increment operator for the reverse iterator.
        /// \return A reference to the iterator after the increment.
        /// \post Moves the iterator to the previous node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator operator++(int)
        {
            reverse_iterator temp = *this;
            m_currentNode = m_currentNode->m_inverse;
            return temp;
        }

        /// Pre-decrement operator for the reverse iterator.
        /// \return A reference to the reverse iterator after the decrement.
        /// \post Moves the reverse iterator to the next node in the linked list.
        reverse_iterator& operator--()
        {
            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_next;
            }
            else
            {
                std::cerr << "Cannot decrement reverse iterator from rbegin iterator" << std::endl;
                std::terminate();
            }
            return *this;
        }

        /// Post decrement operator for the reverse linked list.
        /// \return A reference to the reverse iterator after the decrement.
        /// \post Moves the reverse iterator to the next node in the linked list.
        reverse_iterator operator--(int)
        {
            reverse_iterator temp = *this;

            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_next;
            }
            else
            {
                std::cerr << "Cannot decrement reverse iterator from rbegin iterator" << std::endl;
                std::terminate();
            }
            return temp;
        }

        /// Equality comparison operator for the iterator.
        /// \param other The reverse iterator to compared with.
        /// \return True of both reverse iterators point to the same node, otherwise false.
        /// \post Checks if the current node of this reverse iterator is equal to the current node of the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator==(const reverse_iterator& other) const noexcept
        {
            return m_currentNode == other.m_currentNode;
        }

        /// Inequality comparison operator for the iterator.
        /// \param other The reverse iterator to compare with.
        /// \return True if both reverse iterators do not point to the same node, otherwise false.
        /// \post Checks if the current node of this reverse iterator is not equal to the current node of the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator!=(const reverse_iterator& other) const noexcept
        {
            return m_currentNode != other.m_currentNode;
        }

        /// Copy constructibility from constant reverse iterator.
        /// \param it The constant reverse iterator to copy construct from.
        /// \post The reverse iterator is constructed with the same current node as the constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator(const typename LinkedList<T>::const_reverse_iterator& it) noexcept
            : m_currentNode(const_cast<LinkedListNode<T>*>(it.getNode()))
        {
        }

        /// Copy assignment from constant reverse iterator.
        /// \param it The constant reverse iterator to copy assign from.
        /// \return A reference to the reverse iterator after the assignment.
        /// \post The reverse iterator is assigned with the same current node as the constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator& operator=(const typename LinkedList<T>::const_reverse_iterator& it) noexcept
        {
            m_currentNode = const_cast<LinkedListNode<T>*>(it.getNode());
            return *this;
        }

        /// Move constructibility from constant reverse iterator.
        /// \param it The constant reverse iterator to move construct from.
        /// \post The reverse iterator is constructed with the same current node as the constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator(typename LinkedList<T>::const_reverse_iterator&& it) noexcept
            : m_currentNode(const_cast<LinkedListNode<T>*>(it.getNode()))
        {
        }

        /// Move assignment from constant reverse iterator.
        /// \param it The constant reverse iterator to move assign from.
        /// \return A reference to the reverse iterator after the assignment.
        /// \post The reverse iterator is assigned with the same current node as the constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator& operator=(typename LinkedList<T>::const_reverse_iterator&& it) noexcept
        {
            m_currentNode = const_cast<LinkedListNode<T>*>(it.getNode());
            return *this;
        }

        /// Copy constructor.
        /// \param other The reverse iterator to be copied.
        /// \post The reverse iterator is constructed as a copy of the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator(const reverse_iterator& other) noexcept = default;

        /// Copy assignment operator.
        /// \param other The reverse iterator to be copied.
        /// \return A reference to the reverse iterator after the assignment.
        /// \post The reverse iterator is assigned as a copy of the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator& operator=(const reverse_iterator& other) noexcept = default;

        /// Move constructor
        /// \param other The reverse iterator to be moved.
        /// \post The reverse iterator is constructed by moving the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator(reverse_iterator&& other) noexcept = default;

        /// Move assignment operator.
        /// \param other The reverse iterator to be moved.
        /// \return A reference to the reverse iterator after the assignment.
        /// \post The reverse iterator is assigned by moving the other reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        reverse_iterator& operator=(reverse_iterator&& other) noexcept = default;

        /// Get the node where the iterator is pointing.
        /// \return A pointer to the current node where the reverse iterator is pointing.
        /// \post Returns a pointer to the current node where the reverse iterator is pointing.
        /// \exception noexcept No exceptions are thrown by this operation.
        LinkedListNode<T>* getNode() const noexcept
        {
            return m_currentNode;
        }

    private:
        /// Pointer to the current node where the reverse iterator is pointing.
        /// \note The reverse iterator should always point to a valid node in the linked list,
        /// 	or it should be nullptr if it has reached the end of the list.
        LinkedListNode<T>* m_currentNode;
    };

    /// A bidirectional constant reverse iterator for traversing items backwards in the linked list.
	class const_reverse_iterator : public std::iterator<
                                       std::bidirectional_iterator_tag,
                                       const LinkedListNode<T>,
                                       std::ptrdiff_t,
                                       const LinkedListNode<T>*,
                                       const LinkedListNode<T>&>
    {
    public:
        /// Default constructor.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator() noexcept
        {
        }

        /// Constructor.
        /// \param node Pointer to the `LinkedListNode` to initialize constant reverse iterator with.
        /// \post The constant reverse iterator is constructed with the given `LinkedListNode` as the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        explicit const_reverse_iterator(LinkedListNode<T>* node) noexcept
            : m_currentNode(node)
        {
        }

        /// Dereference operator for the constant reverse iterator.
        /// \return A constant reference to the data of the current node.
        /// \post Returns a constant reference to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        const T& operator*() const noexcept
        {
            return m_currentNode->m_data;
        }

        /// Arrow operator for the constant reverse iterator.
        /// \return A constant pointer to data of the current node.
        /// \post Returns a constant pointer to the data of the current node.
        /// \exception noexcept No exceptions are thrown by this operation.
        const T* operator->() const noexcept
        {
            return &(m_currentNode->m_data);
        }

        /// Pre-increment operator for the constant reverse iterator.
        /// \return A reference to the constant reverse iterator after the increment.
        /// \post Moves the constant reverse iterator to the previous node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator& operator++()
        {
            m_currentNode = m_currentNode->m_inverse;
            return *this;
        }

        /// Post-increment operator for the constant reverse iterator.
        /// \return A constant reverse iterator pointing to the previous position before the increment.
        /// \post Moves the constant reverse iterator to the previous node in the linked list.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator temp = *this;
            m_currentNode = m_currentNode->m_inverse;
            return temp;
        }

        /// Pre-decrement operator for the constant iterator.
        /// \return A reference to the constant reverse iterator after the decrement.
        /// \post Moves the constant reverse iterator to the next node in the linked list.
        const_reverse_iterator& operator--()
        {
            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_next;
            }
            else
            {
                std::cerr << "Cannot decrement reverse iterator from rbegin iterator" << std::endl;
                std::terminate();
            }
            return *this;
        }

        /// Post-decrement operator for the constant reverse iterator.
        /// \return A constant reverse iterator pointing to the previous position before the decrement.
        /// \post Moves the constant reverse iterator to the next node in the linked list.
        const_reverse_iterator operator--(int) noexcept
        {
            const_reverse_iterator temp = *this;

            if (m_currentNode)
            {
                m_currentNode = m_currentNode->m_next;
            }
            else
            {
                std::cerr << "Cannot decrement reverse iterator from rbegin iterator" << std::endl;
                std::terminate();
            }
            return temp;
        }

        /// Equality comparison operator for the constant reverse iterator.
        /// \param other The constant reverse iterator to compare with.
        /// \return True if both of the constant reverse iterators point to the same node, otherwise false.
        /// \post Checks if the current node if this constant reverse iterator is equal to the current of the other iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator==(const const_reverse_iterator& other) const noexcept
        {
            return m_currentNode == other.m_currentNode;
        }

        /// Inequality comparison operator for the constant reverse iterator.
        /// \param other The constant reverse iterator to compare with.
        /// \return True of both constant reverse iterators do not point to the same node, otherwise false.
        /// \exception noexcept No exceptions are thrown by this operation.
        bool operator!=(const const_reverse_iterator& other) const noexcept
        {
            return m_currentNode != other.m_currentNode;
        }

        /// Copy constructibility from non-const reverse iterator.
        /// \param it The reverse iterator to copy construct from.
        /// \post The constant reverse iterator is constructed with the same current node as the non-const reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator(const typename LinkedList<T>::reverse_iterator& it) noexcept
            : m_currentNode(it.getNode())
        {
        }

        /// Copy assignment from non-const reverse iterator.
        /// \param it The reverse iterator to copy assign from.
        /// \return A reference to the constant reverse iterator after the assignment.
        /// \post The constant reverse iterator is assigned with the same current node as the non-const reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator& operator=(const typename LinkedList<T>::reverse_iterator& it) noexcept
        {
            m_currentNode = it.getNode();
            return *this;
        }

        /// Move constructibility from non-const reverse iterator.
        /// \param it The reverse iterator to be moved from.
        /// \post The constant reverse iterator is constructed, taking ownership of the internal
        /// 	pointer from the non-const reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator(typename LinkedList<T>::reverse_iterator&& it) noexcept
            : m_currentNode(it.getNode())
        {
        }

        /// Move assignment from non-const reverse iterator.
        /// \param it The reverse iterator to be moved from.
        /// \return A reference to the constant reverse iterator after the assignment.
        /// \post The constant reverse iterator is assigned with the value of the reverse iterator,
        /// 	taking ownership of the internal pointer.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator& operator=(typename LinkedList<T>::reverse_iterator&& it) noexcept
        {
            m_currentNode = it.getNode();
            return *this;
        }

        /// Copy constructor.
        /// \param other The constant reverse iterator to be copied.
        /// \post The constant reverse iterator is constructed as a copy of the other constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator(const const_reverse_iterator& other) noexcept = default;

        /// Copy assignment operator.
        /// \param other The constant reverse iterator to be copied.
        /// \return A reference to the constant reverse iterator after the assignment.
        /// \post The constant reverse iterator is assigned as a copy of the other constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator& operator=(const const_reverse_iterator& other) noexcept = default;

        /// Move constructor
        /// \param other The constant reverse iterator to be moved.
        /// \post The constant reverse iterator is constructed by moving the other constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator(const_reverse_iterator&& other) noexcept = default;

        /// Move assignment operator.
        /// \param other The constant reverse iterator to be moved.
        /// \return A reference to the constant reverse iterator after the assignment.
        /// \post The constant reverse iterator is assigned by moving the other constant reverse iterator.
        /// \exception noexcept No exceptions are thrown by this operation.
        const_reverse_iterator& operator=(const_reverse_iterator&& other) noexcept = default;

        /// Get the node where the constant reverse iterator is pointing.
        /// \return A pointer to the current node where the constant reverse iterator is pointing.
        /// \post Returns a pointer to the current node where the iterator is pointing.
        /// \exception noexcept No exceptions are thrown by this operation.
        LinkedListNode<T>* getNode() const noexcept
        {
            return m_currentNode;
        }

    private:
        /// Pointer to the node where the constant reverse iterator is pointing.
        /// \note The constant reverse iterator should always point to a valid node in the linked list,
        /// 	or it should be nullptr if it has reached the end of the list.
        LinkedListNode<T>* m_currentNode;
    };

    /// Default constructor.
    /// \post Constructs a new `LinkedList` object with no elements.
    /// \exception noexcept No exceptions are thrown by this operation.
    LinkedList() noexcept
    {
    }

    /// Destructor.
    /// \post Destroys the `LinkedList` object, freeing all associated resources.
    /// \exception noexcept No exceptions are thrown by this operation.
    ~LinkedList() noexcept
    {
        clear();
    }

    /// Initializer list constructor.
    /// \param list An initializer list containing values to initialize the LinkedList with.
    /// \post Constructs a new LinkedList object with elements from the initializer list.
    /// \exception The `insert` function may throw exceptions if memory allocation fails or if an exception is thrown by the element's constructor.
    LinkedList(std::initializer_list<value_type> list)
    {
        for (const value_type& value : list)
        {
            insert(value);
        }
    }

    /// Move constructor.
    /// \param other The LinkedList to be moved from.
    /// \post Constructs a new LinkedList by moving the content from the other LinkedList.
    ///       The other LinkedList will be left in a valid but unspecified state.
    /// \exception noexcept No exceptions are thrown by this operation.
    LinkedList(LinkedList&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_count(other.m_count)
    {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_count = 0;
    }

    /// Move assingment operator.
    /// \param other The `LinkedList` to be moved from.
    /// \return A reference to the LinkedList after the move assignment.
    /// \post Moves the content from the other LinkedList to this LinkedList.
    ///       The other LinkedList will be left in a valid but unspecified state.
    /// \exception noexcept No exceptions are thrown by this operation.
    LinkedList& operator=(LinkedList&& other) noexcept
    {
        if (this != &other)
        {
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_count = other.m_count;

            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_count = 0;
        }
        return *this;
    }

    /// Copy constructor.
    /// \param other The LinkedList to be copied from.
    /// \post Constructs a new LinkedList by copying the content from the other LinkedList.
    /// \exception noexcept No exceptions are thrown by this operation.
    LinkedList(const LinkedList<T>& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_count(other.m_count)
    {
    }

    /// Copy assignment operator.
    /// \param other The LinkedList to be assigned from.
    /// \return A reference to the LinkedList after the attempted copy assignment.
    /// \post Copies the content from the other LinkedList to this LinkedList.
    /// \exception noexcept No exceptions are thrown by this operation.
    LinkedList& operator=(const LinkedList<T>& other) noexcept
    {
        if (this != &other)
        {
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_count = other.m_count;
        }
        return *this;
    }

    /// Get an iterator to the beginning of the linked list.
    /// \return An iterator pointing to the first element in the linked list.
    /// \note If the linked list is empty, the iterator will be equal to the end iterator.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator begin() noexcept
    {
        return iterator(m_head);
    }

    /// Get an iterator to the end of the linked list.
    /// \return An iterator pointing to the position past the last element in the linked list.
    /// \note This iterator acts as a sentinel and should not be dereferenced.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator end() noexcept
    {
        return iterator(nullptr);
    }

    /// Get a const iterator to the beginning of the linked list.
    /// \return A const iterator pointing to the first element in the linked list.
    /// \note If the linked list is empty, the iterator will be equal to the end iterator.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cbegin() const noexcept
    {
        return const_iterator(m_head);
    }

    /// Get a const iterator to the end of the linked list.
    /// \return A const iterator pointing to the position past the last element in the linked list.
    /// \note This constant iterator acts as a sentinel and should not be dereferenced.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

    /// Get a reverse iterator to the beginning of the linked list.
    /// \return A reverse iterator pointing past the first element in the linked list.
    /// \note This reverse iterator acts as a sentinel and should not be dereferenced.
    /// \exception noexcept No exceptions are thrown by this operation.
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(nullptr);
    }

    /// Get a reverse iterator to the beginning of the linked list.
    /// \return A reverse iterator pointing to the last element in the linked list.
    /// \note If linked list is empty, this reverse iterator will be equal to the crbegin iterator.
    /// \exception noexcept No exceptions are thrown by this operation.
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(m_tail);
    }

    /// Get a constant reverse iterator to the beginning of the linked list.
    /// \return A constant reverse iterator pointing past the first element in the linked list.
    /// \note If constant reverse iterator acts as a sentinel and should not be dereferenced.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(nullptr);
    }

    /// Get a constant reverse iterator to end of the linked list.
    /// \return A constant reverse iterator pointing to the last element in the linked list.
    /// \note If linked list is empty, this constant reverse iterator will be equal to the crend iterator.
    /// \exception noexcept No exceptions are thrown by this operation.
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(m_tail);
    }

    /// Clear the elements in the LinkedList and deallocate memory.
    /// \post Removes all elements from the LinkedList, and deallocates the memory used by each element.
    /// \exception noexcept No exceptions are thrown by this operation.
    void clear() noexcept
    {
        while (m_head)
        {
            auto* next = m_head->m_next;
            m_allocator.deallocate(m_head, 1);
            m_head = next;
        }
        m_count = 0;
    }

    /// Insert a new element with the given value at the end of the linked list.
    /// \param value The value of the element to be inserted.
    /// \return An iterator that points to the newly inserted element.
    /// \pre The value_type of the linked list must be copy constructible.
    /// \post The element with the specified value is inserted at the end of the linked list.
    /// \exception May throw std::bad_alloc if memory allocation fails during the operation.
    /// \note If the value_type of the linked list is not copy constructible, this function will not compile.
    iterator insert(const T& value)
    {
        auto* newNode = m_allocator.allocate(1);
        m_allocator.construct(newNode, value);

        // List is empty.
        if (!m_head)
        {
            m_head = newNode;
            m_tail = newNode;
        }

        // Adding a new Node to the linked list.
        else
        {
            LinkedListNode<T>* prevNode = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
            m_tail->m_inverse = prevNode;
        }
        m_count++;

        return iterator(m_tail);
    }

    /// Insert a new element with the given value at the specified position in the linked list.
    /// \param pos An iterator pointing to the position where the element is inserted.
    /// \param value The value of the element to be inserted.
    /// \return An iterator that points to the newly inserted element.
    /// \pre The value_type of the linked list must be copy constructible.
    /// \pre The iterator `pos` must be a valid iterator within the linked list.
    /// \post The element with the specified value is inserted at the position indicated by `pos`.
    /// \exception May throw std::bad_alloc if memory allocation fails during the operation.
    /// \note If the value_type of the linked list is not copy constructible, this function will not compile.
    iterator insert(iterator pos, const T& value)
    {
        auto* newNode = m_allocator.allocate(1);
        m_allocator.construct(newNode, value);

        // List is empty.
        if (!m_head)
        {
            m_head = newNode;
            m_tail = newNode;
        }
        // Inserting to beginning.
        else if (pos == begin() && begin() != end())
        {
            newNode->m_next = m_head;
            m_head->m_inverse = newNode;
            m_head = newNode;
        }
        // Inserting to end.
        else if (pos == end())
        {
            newNode->m_inverse = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
        // Inserting to any other position.
        else
        {
            auto* ogNode = pos.getNode();
            ogNode->m_inverse->m_next = newNode;
            ogNode->m_inverse = newNode;

            newNode->m_next = ogNode;
            newNode->m_inverse = ogNode->m_inverse;
        }

        m_count++;
        return iterator(newNode);
    }

    /// Remove all occurrences of the specified value from the linked list.
    /// \param value The value of the elements to be removed.
    /// \return An iterator that points to the element following the last removed element, or the end() iterator if no element was removed.
    /// \pre The value_type of the linked list must support equality comparison.
    /// \post All elements with the specified value are removed from the linked list.
    /// \exception May throw an exception if memory deallocation fails (depends on the allocator).
    iterator erase(const T& value)
    {
        auto* currentNode = m_head;
        LinkedListNode<T>* previousNode = nullptr;

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

            LinkedListNode<T>* returnNode = currentNode->m_next;
            m_allocator.deallocate(currentNode, 1);
            m_count--;
            return iterator(returnNode);
        }
        else
        {
            return iterator(m_tail);
        }
    }

    /// Remove the element at the specified position in the linked list.
    /// \param pos An iterator pointing to the element to be removed.
    /// \return An iterator that points to the element following the removed element, or the end() iterator if the last element was removed.
    /// \pre The provided iterator must be valid and dereferenceable.
    /// \pre The linked list must not be empty.
    /// \post The element at the position specified by the iterator is removed from the linked list.
    /// \post The iterator following the removed element is returned.
    /// \exception May throw an exception if memory deallocation fails (depends on the allocator).
    iterator erase(iterator pos)
    {
        auto* removable = pos.getNode();
        auto* nextNode = removable->m_next;

        if (removable == nullptr)
        {
            return iterator(m_tail);
        }
        else if (removable == m_head)
        {
            m_head = removable->m_next;

            if (m_head)
            {
                m_head->m_inverse = nullptr;
            }
            else
            {
                m_tail = nullptr;
            }
        }
        else if (removable == m_tail)
        {
            m_tail = removable->m_inverse;
            m_tail->m_next = nullptr;
        }
        else
        {
            LinkedListNode<T>* prevNode = removable->m_inverse;
            prevNode->m_next = nextNode;
            nextNode->m_inverse = prevNode;
        }

        m_count--;
        m_allocator.deallocate(removable, 1);
        return iterator(nextNode);
    }

    /// Remove the elements in the range [first, last] from the linked list.
    /// \param first An iterator pointing to the first element of the range to be removed.
    /// \param last An iterator pointing to the element just beyond the last element of the range to be removed.
    /// \return An iterator that points to the element following the last removed element, or the end() iterator if the last element was removed.
    /// \pre The provided iterators must be valid and dereferenceable.
    /// \pre The range [first, last] must be a valid range within the linked list.
    /// \post The elements in the range [first, last] are removed from the linked list.
    /// \post The iterator following the last removed element is returned.
    /// \exception May throw an exception if the deallocation of memory fails.
    iterator erase(iterator first, iterator last)
    {
        // Get Node where the first iterator is pointing.
        auto* firstNode = first.getNode();

        // Get Node where the last iterator is pointing.
        LinkedListNode<T>* lastNode;
        if (last == end())
        {
            lastNode = m_tail;
        }
        else
        {
            lastNode = last.getNode();
        }

        if (!firstNode || !lastNode || firstNode == lastNode)
        {
            return iterator(nullptr);
        }

        // Get the Nodes before and after range.
        LinkedListNode<T>* prevNode = nullptr;
        LinkedListNode<T>* nextNode = nullptr;

        if (first != begin())
        {
            prevNode = firstNode->m_inverse;
        }

        if (last != end() || lastNode != m_tail)
        {
            nextNode = lastNode->m_next;
        }

        // Reset m_head and m_tail pointers, connect the Nodes after range if possible.
        if (!prevNode && nextNode)
        {
            m_head = nextNode;
        }
        else if (prevNode && !nextNode)
        {
            m_tail = prevNode;
        }
        else if (!prevNode && !nextNode)
        {
            m_head = nullptr;
            m_tail = nullptr;
        }
        else
        {
            m_head = prevNode;
            m_tail = nextNode;
            prevNode->m_next = nextNode;
            nextNode->m_inverse = prevNode;
        }

        // Delete the Nodes within the range.
        LinkedListNode<T>* currentNode = firstNode;

        while (currentNode != lastNode)
        {
            auto* next = currentNode->m_next;

            m_allocator.deallocate(currentNode, 1);
            currentNode = next;
            m_count--;
        }

        // Deallocate the last element.
        m_allocator.deallocate(lastNode, 1);
        m_count--;

        return iterator(nextNode);
    }

    /// Swap the contents of this linked list with another linked list.
    /// \param other The other linked list to swap with.
    /// \post The contents of this linked list are exchanged with the contents of the other linked list.
    /// \exception noexcept No exceptions are thrown by this operation.
    void swap(LinkedList& other) noexcept
    {
        auto tempHead = m_head;
        auto tempTail = m_tail;
        auto tempCount = m_count;

        m_head = other.m_head;
        m_tail = other.m_tail;
        m_count = other.m_count;

        other.m_head = tempHead;
        other.m_tail = tempTail;
        other.m_count = tempCount;
    }

    /// Find the first occurrence of a value in the linked list.
    /// \param value The value to search for.
    /// \return An iterator to the first occurrence of the value in the linked list, or the end() iterator if the value is not found.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator find(const T& value) noexcept
    {
        return std::find(begin(), end(), value);
    }

    /// Find the first occurrence of a value in the linked list in a constant context.
    /// \param value The value to search for.
    /// \return An iterator to the first occurrence of the value in the linked list, or the end() iterator if the value is not found.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    iterator find(const T& value) const noexcept
    {
        return std::find(cbegin(), cend(), value);
    }

    /// Returns a reference to the first element in the linked list.
    /// \return A reference to the first element.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    T& front() noexcept
    {
        return m_head->m_data;
    }

    /// Returns a constant reference to the first element in the linked list in const context.
    /// \return A constant reference to the first element.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    const T& front() const noexcept
    {
        return m_head->m_data;
    }

    /// Returns a reference to the last element in the linked list.
    /// \return A reference to the last element.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    T& back() noexcept
    {
        return m_tail->m_data;
    }

    /// Returns a constant reference to the last element in the linked list in const context.
    /// \return A constant reference to the last element.
    /// \pre The linked list must not be empty.
    /// \exception noexcept No exceptions are thrown by this operation.
    const T& back() const noexcept
    {
        return m_tail->m_data;
    }

    /// Returns the number of elements in the linked list.
    /// \return The number of elements in the linked list.
    /// \exception noexcept No exceptions are thrown by this operation.
    size_t size() const noexcept
    {
        return m_count;
    }

    /// Checks whether the linked list is empty.
    /// \return True if the linked list is empty, otherwise false.
    /// \exception noexcept No exceptions are thrown by this operation.
    bool empty() const
    {
        return !m_head;
    }

private:
    /// Pointing always to the first element.
    LinkedListNode<T>* m_head = nullptr;

    /// Pointing always to the last element.
    LinkedListNode<T>* m_tail = nullptr;

    /// Amount of nodes in the linked list.
    size_t m_count = 0;

    /// Allocator for memory management, default is std::allocator.
    Allocator m_allocator;
};

#endif
