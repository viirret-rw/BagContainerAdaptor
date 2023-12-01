#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>
#include <BagContainerAdaptor/linked_list.hpp>

// Testing front() and back() member functions for types in bag container adaptor that
// have normal order for items in the container.
template <typename Container>
class FrontAndBackTest : public ::testing::Test
{
protected:
    void frontTest()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.front(), 1);
    }

    void backTest()
    {
        BagContainerAdaptor<int, Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.back(), 3);
    }
};

// Note that we are missing std::unordered_multiset.
using FrontAndBackContainerTypes = ::testing::Types<
    std::list<int>,
    std::vector<int>,
    std::deque<int>,
    std::multiset<int>>;

TYPED_TEST_SUITE(FrontAndBackTest, FrontAndBackContainerTypes);

TYPED_TEST(FrontAndBackTest, frontTest)
{
    this->frontTest();
}

TYPED_TEST(FrontAndBackTest, backTest)
{
    this->backTest();
}

// Testing front() and back() member functions for types in bag container adaptor that
// have reversed order for items in the container.
template <typename Container>
class FrontAndBackTestReversed : public ::testing::Test
{
protected:
    void frontTest()
    {
        Container testSet;
        BagContainerAdaptor<int, Container> adaptor;

        for (int i = 1; i < 4; i++)
        {
            insertValues(testSet, i);
            adaptor.insert(i);
        }

        EXPECT_EQ(*testSet.begin(), adaptor.front());
        EXPECT_EQ(3, adaptor.front());
    }

    void backTest()
    {
        Container testSet;
        BagContainerAdaptor<int, Container> adaptor;

        for (int i = 1; i < 4; i++)
        {
            insertValues(testSet, i);
            adaptor.insert(i);
        }

        auto testSetLast = testSet.begin();
        for (auto it = testSetLast; it != testSet.end(); it++)
        {
            testSetLast = it;
        }

        EXPECT_EQ(*testSetLast, adaptor.back());
        EXPECT_EQ(1, adaptor.back());
    }

private:
    template <typename C = Container>
    typename std::enable_if<std::is_same<C, std::forward_list<typename C::value_type>>::value>::type
    insertValues(Container& testSet, typename C::value_type value)
    {
        testSet.insert_after(testSet.before_begin(), value);
    }

    template <typename C = Container>
    typename std::enable_if<!std::is_same<C, std::forward_list<typename C::value_type>>::value>::type
    insertValues(Container& testSet, typename C::value_type value)
    {
        testSet.insert(value);
    }
};

using FrontAndBackReverseContainerTypes = ::testing::Types<
    std::forward_list<int>,
    std::unordered_multiset<int>>;

TYPED_TEST_SUITE(FrontAndBackTestReversed, FrontAndBackReverseContainerTypes);

TYPED_TEST(FrontAndBackTestReversed, frontTest)
{
    this->frontTest();
}

TYPED_TEST(FrontAndBackTestReversed, backTest)
{
    this->backTest();
}
