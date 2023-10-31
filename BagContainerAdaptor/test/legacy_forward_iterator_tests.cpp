#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>
#include <BagContainerAdaptor/linked_list.hpp>

#include <type_traits>
#include <list>

template <typename Iterator>
class PassLegacyForwardIteratorTest : public ::testing::Test
{
protected:
	void isForwardIterator()
	{
		if (!std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value)
		{
			FAIL();
		}
	}

    void isCopyConstructible()
    {
        EXPECT_TRUE(std::is_copy_constructible<Iterator>::value);
    }

    void isCopyAssignable()
    {
        EXPECT_TRUE(std::is_copy_assignable<Iterator>::value);
    }

    void isMoveConstructible()
    {
        EXPECT_TRUE(std::is_move_constructible<Iterator>::value);
    }

    void isMoveAssignable()
    {
        EXPECT_TRUE(std::is_move_assignable<Iterator>::value);
    }

    void isDestructible()
    {
        EXPECT_TRUE(std::is_destructible<Iterator>::value);
    }

    void isDefaultConstructible()
    {
        EXPECT_TRUE(std::is_default_constructible<Iterator>::value);
    }

	void isSwappable()
	{
		EXPECT_TRUE(std::is_swappable<Iterator>::value);
	}

	void isEqualityComparable()
	{
		bool value = AreIteratorsEqual<Iterator, Iterator>::check();
		EXPECT_TRUE(value);
	}

private:
	template <typename Iterator1, typename Iterator2>
	struct AreIteratorsEqual
	{
		static bool check()
		{
			return std::is_same<typename std::iterator_traits<Iterator1>::iterator_category,
									  typename std::iterator_traits<Iterator2>::iterator_category>::value &&
						 std::is_same<typename std::iterator_traits<Iterator1>::value_type,
									  typename std::iterator_traits<Iterator2>::value_type>::value;
		}
	};

	template <typename Iterator1>
	struct AreIteratorsEqual<Iterator1, Iterator1>
	{
		static bool check()
		{
			return true;
		}
	};
};

using IteratorTypes = ::testing::Types<

    /// STL types.
	typename BagContainerAdaptor<int, std::vector<int>>::iterator,
	typename BagContainerAdaptor<int, std::vector<int>>::const_iterator,
	typename BagContainerAdaptor<int, std::list<int>>::iterator,
	typename BagContainerAdaptor<int, std::list<int>>::const_iterator,
	typename BagContainerAdaptor<int, std::deque<int>>::iterator,
	typename BagContainerAdaptor<int, std::deque<int>>::const_iterator,
	typename BagContainerAdaptor<int, std::forward_list<int>>::iterator,
	typename BagContainerAdaptor<int, std::forward_list<int>>::const_iterator,
	typename BagContainerAdaptor<int, std::multiset<int>>::iterator,
	typename BagContainerAdaptor<int, std::multiset<int>>::const_iterator,
	typename BagContainerAdaptor<int, std::unordered_multiset<int>>::iterator,
	typename BagContainerAdaptor<int, std::unordered_multiset<int>>::const_iterator,

    // Custom types.
	typename BagContainerAdaptor<LinkedList<int>>::iterator,
	typename BagContainerAdaptor<LinkedList<int>>::const_iterator
	>;

TYPED_TEST_SUITE(PassLegacyForwardIteratorTest, IteratorTypes);

TYPED_TEST(PassLegacyForwardIteratorTest, isForwardIterator)
{
	this->isForwardIterator();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isCopyConstructible)
{
    this->isCopyConstructible();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isCopyAssignable)
{
	this->isCopyAssignable();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isMoveConstructible)
{
	this->isMoveConstructible();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isMoveAssignable)
{
	this->isMoveAssignable();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isDestructible)
{
	this->isDestructible();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isDefaultConstructible)
{
	this->isDefaultConstructible();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isSwappable)
{
	this->isSwappable();
}

TYPED_TEST(PassLegacyForwardIteratorTest, isEqualityComparable)
{
	this->isEqualityComparable();
}

