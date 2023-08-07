#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

template <typename IteratorType>
class IteratorTest : public ::testing::Test
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

typedef ::testing::Types<
	LinkedList<int>::iterator,
	LinkedList<int>::const_iterator
> LinkedListIteratorTypes;

TYPED_TEST_SUITE(IteratorTest, LinkedListIteratorTypes);

TYPED_TEST(IteratorTest, isCopyConstructible)
{
	this->isCopyConstructible();
}

TYPED_TEST(IteratorTest, isCopyAssignable)
{
	this->isCopyAssignable();
}

TYPED_TEST(IteratorTest, isMoveConstructible)
{
	this->isMoveConstructible();
}

TYPED_TEST(IteratorTest, isMoveAssignable)
{
	this->isMoveAssignable();
}

TYPED_TEST(IteratorTest, isDestructible)
{
	this->isDestructible();
}

TYPED_TEST(IteratorTest, isDefaultConstructible)
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

TEST(TestIterators, IncrementOperators)
{
	LinkedList<int> list { 1, 2, 3, 4, 5, 6 };

	auto begin = list.begin();
	auto cbegin = list.cbegin();

	const auto pre = ++begin;
	const auto post = begin++;

	EXPECT_EQ(pre, post);

	// Same for constant iterators.
	auto cpre = ++cbegin;
	auto cpost = cbegin++;

	EXPECT_EQ(cpre, cpost);

	auto controlIt = begin;
	++begin;
	EXPECT_NE(*controlIt++, *begin);
	EXPECT_EQ(*controlIt, *begin);
}

