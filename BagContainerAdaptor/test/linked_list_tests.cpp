#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

template <typename IteratorType>
class IteratorSTLTest : public ::testing::Test
{
protected:
	void isCopyConstructible()
	{
		EXPECT_TRUE(std::is_copy_constructible<IteratorType>::value);
	}

	void isCopyAssignable()
	{
		EXPECT_TRUE(std::is_copy_assignable<IteratorType>::value);
	}

	void isMoveConstructible()
	{
		EXPECT_TRUE(std::is_move_constructible<IteratorType>::value);
	}

	void isMoveAssignable()
	{
		EXPECT_TRUE(std::is_move_assignable<IteratorType>::value);
	}

	void isDestructible()
	{
		EXPECT_TRUE(std::is_destructible<IteratorType>::value);
	}

	void isDefaultConstructible()
	{
		EXPECT_TRUE(std::is_default_constructible<IteratorType>::value);
	}
};

using LinkedListIteratorTypes = ::testing::Types<
	LinkedList<int>::iterator,
	LinkedList<int>::const_iterator,
	LinkedList<int>::reverse_iterator,
	LinkedList<int>::const_reverse_iterator
>;

TYPED_TEST_SUITE(IteratorSTLTest, LinkedListIteratorTypes);

TYPED_TEST(IteratorSTLTest, isCopyConstructible)
{
	this->isCopyConstructible();
}

TYPED_TEST(IteratorSTLTest, isCopyAssignable)
{
	this->isCopyAssignable();
}

TYPED_TEST(IteratorSTLTest, isMoveConstructible)
{
	this->isMoveConstructible();
}

TYPED_TEST(IteratorSTLTest, isMoveAssignable)
{
	this->isMoveAssignable();
}

TYPED_TEST(IteratorSTLTest, isDestructible)
{
	this->isDestructible();
}

TYPED_TEST(IteratorSTLTest, isDefaultConstructible)
{
	this->isDefaultConstructible();
}

TEST(TestIterators, EqualityComparable)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };

	auto begin(list.begin());
	auto end(list.end());
	auto refIt(begin);

	EXPECT_TRUE(refIt == begin);
   	EXPECT_TRUE(begin == refIt);
    EXPECT_FALSE(begin == end);

    // Const iterators
    auto cbegin(list.cbegin());
    auto cend(list.cend());
    auto crefIt(cbegin);

    EXPECT_TRUE(crefIt == cbegin);
   	EXPECT_TRUE(cbegin == crefIt);
    EXPECT_FALSE(cbegin == cend);

	// Compare const and non-const iterators.
    EXPECT_TRUE(cbegin == begin);
    EXPECT_TRUE(begin == cbegin);
}

TEST(TestIterators, Swappable)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };

	auto begin = list.begin();
	auto end = list.end();
	auto ref = list.begin();

	// Swap begin and end, and vice verca with ref.
	std::swap(begin, end);
	EXPECT_EQ(ref, end);
	std::swap(ref, begin);
	EXPECT_EQ(end, begin);

	// Same with const-iterators.
	auto cbegin = list.cbegin();
	auto cend = list.cend();
	auto cRef = list.cbegin();

	std::swap(cbegin, cend);
	EXPECT_EQ(cRef, cend);
	std::swap(cRef, cbegin);
	EXPECT_EQ(cend, cbegin);
}

TEST(TestIterators, Inequality)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };
	LinkedList<int> otherList { 11, 22, 33, 44, 55, 66 };

	auto listBegin(list.begin());
	auto otherBegin(otherList.begin());

	// Same index but different containers.
	EXPECT_TRUE(listBegin != otherBegin);

	// Same buffer, but different indexes.
	auto snapshot(listBegin);
	EXPECT_TRUE(snapshot == listBegin);
	listBegin++;
	EXPECT_TRUE(snapshot != listBegin);

	// Same for constant iterators.
	auto constListBegin(list.cbegin());
	auto constOtherList(otherList.cbegin());

	EXPECT_TRUE(constListBegin != constOtherList);

	const auto constSnapshot(constListBegin);
	EXPECT_TRUE(constSnapshot == constListBegin);
	constListBegin++;
	EXPECT_TRUE(constSnapshot != constListBegin);
}

TEST(TestIterators, Dereferenceable)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };

	const auto begin(list.begin());

	// Implicit conversion to value_type.
	using T = typename LinkedList<int>::value_type;
	*begin = 5;
	T convertibleToThis = *begin;
	EXPECT_TRUE((std::is_same<decltype(convertibleToThis), typename LinkedList<int>::value_type>::value));

	auto end = list.end();

	EXPECT_EQ(convertibleToThis, 5);
}

TEST(TestIterators, ConversionTest)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };

	// Creating constant iterators from normal iterators.
	{
		auto it = list.begin();

		// Conversion constructor.
		LinkedList<int>::const_iterator constructed(it);

		// Conversion assignment.
		LinkedList<int>::const_iterator assigned = list.begin();

		EXPECT_TRUE((std::is_same<decltype(constructed), typename LinkedList<int>::const_iterator>::value));
		EXPECT_TRUE((std::is_same<decltype(assigned), typename LinkedList<int>::const_iterator>::value));
		EXPECT_EQ(*constructed, *assigned);
	}
	
	// Creating normal iterators from constant iterators.
	{
		auto cit = list.cbegin();

		// Conversion constructor.
		LinkedList<int>::iterator constructed(cit);

		// Conversion assignment.
		LinkedList<int>::iterator assigned = list.cbegin();

		EXPECT_TRUE((std::is_same<decltype(constructed), typename LinkedList<int>::iterator>::value));
		EXPECT_TRUE((std::is_same<decltype(assigned), typename LinkedList<int>::iterator>::value));
		EXPECT_EQ(*constructed, *assigned);
	}
}

template <typename List, typename IteratorType>
class IteratorTest
{
public:
	static void iterationTest(List& list, IteratorType first, IteratorType last)
	{
		int counter = 0;
		auto f = first;
		for (auto i = first; i != last; i++)
		{
			EXPECT_EQ(i, f);
			counter++;
			f++;
		}
		EXPECT_EQ(list.size(), counter);
	}

	static void incrementTest(List& list, IteratorType iterator)
	{
		const auto pre = ++iterator;
		const auto post = iterator++;

		EXPECT_EQ(pre, post);

		auto controlIt = iterator;
		++iterator;
		EXPECT_NE(*controlIt++, *iterator);
	}

	static void decrementTest(List& list, IteratorType iterator)
	{
		auto next = std::next(iterator);

		const auto pre = --next;
		const auto post = next--;

		EXPECT_EQ(pre, post);
		EXPECT_EQ(pre, iterator);
		EXPECT_EQ(post, iterator);
	}
};

TEST(IteratorTest, TestNormalIteratorIteration)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5, 6 };
	IteratorTest<LinkedList<int>, LinkedList<int>::iterator>::iterationTest(list, list.begin(), list.end());
}

TEST(IteratorTest, TestConstantIteratorIteration)
{
	LinkedList<int> list = { 5, 4, 2, 5, 2, 9 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_iterator>::iterationTest(list, list.cbegin(), list.cend());
}

TEST(IteratorTest, TestReverseIteratorIteration)
{
	LinkedList<int> list = { 5, 554, 222, 22, 2, 9 };
	IteratorTest<LinkedList<int>, LinkedList<int>::reverse_iterator>::iterationTest(list, list.rend(), list.rbegin());
}

TEST(IteratorTest, TestConstantReverseIteratorIteration)
{
	LinkedList<int> list = { 8, 54, 212, 82, 12, 29 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_reverse_iterator>::iterationTest(list, list.crend(), list.crbegin());
}

TEST(IteratorTest, TestNormalIteratorIncrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::iterator>::incrementTest(list, list.begin());
}

TEST(IteratorTest, TestConstantIteratorIncrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_iterator>::incrementTest(list, list.cbegin());
}

TEST(IteratorTest, TestReverseIteratorIncrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::reverse_iterator>::incrementTest(list, list.rend());
}

TEST(IteratorTest, TestConstantReverseIteratorIncrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_reverse_iterator>::incrementTest(list, list.crend());
}

TEST(IteratorTest, TestNormalIteratorDecrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::iterator>::decrementTest(list, list.begin());
}

TEST(IteratorTest, TestConstantIterationDecrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_iterator>::decrementTest(list, list.cbegin());
}

TEST(IteratorTest, TestReverseIteratorDecrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::reverse_iterator>::decrementTest(list, list.rend());
}

TEST(IteratorTest, TestConstantReverseIteratorDecrement)
{
	LinkedList<int> list = { 1, 2, 3, 4, 5 };
	IteratorTest<LinkedList<int>, LinkedList<int>::const_reverse_iterator>::decrementTest(list, list.crend());
}

