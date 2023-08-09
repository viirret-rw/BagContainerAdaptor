#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

template <typename Container>
class FrontAndBackTest : public ::testing::Test
{
protected:
    void frontTest()
    {
        BagContainerAdaptor<Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.front(), 1);
    }

    void backTest()
    {
        BagContainerAdaptor<Container> adapter;

        adapter.insert(1);
        adapter.insert(2);
        adapter.insert(3);

        EXPECT_EQ(adapter.back(), 3);
    }
};

using FrontAndBackContainerTypes = ::testing::Types<
    std::list<int>,
    std::vector<int>,
    std::deque<int>,
    std::forward_list<int>,
    std::multiset<int>,
    ring_buffer<int>,
    LinkedList<int>>;

TYPED_TEST_SUITE(FrontAndBackTest, FrontAndBackContainerTypes);

TYPED_TEST(FrontAndBackTest, frontTest)
{
    this->frontTest();
}

TYPED_TEST(FrontAndBackTest, backTest)
{
    this->backTest();
}

TEST(FrontAndBackTests, FrontTestUnorderedMultiset)
{
    std::unordered_multiset<int> testSet;
    BagContainerAdaptor<std::unordered_multiset<int>> adaptor;

    testSet.insert(1);
    adaptor.insert(1);

    testSet.insert(2);
    adaptor.insert(2);

    testSet.insert(3);
    adaptor.insert(3);

    EXPECT_EQ(*testSet.begin(), adaptor.front());
    EXPECT_EQ(3, adaptor.front());
}

TEST(FrontAndBackTests, BackTestUnorderedMultiset)
{
    std::unordered_multiset<int> testSet;
    BagContainerAdaptor<std::unordered_multiset<int>> adaptor;

    testSet.insert(1);
    adaptor.insert(1);

    testSet.insert(2);
    adaptor.insert(2);

    testSet.insert(3);
    adaptor.insert(3);

    auto testSetLast = testSet.begin();
    for (auto it = testSetLast; it != testSet.end(); it++)
    {
        testSetLast = it;
    }

    EXPECT_EQ(*testSetLast, adaptor.back());
    EXPECT_EQ(1, adaptor.back());
}
