#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <list>

// This is a special test for initializing BagContainerAdaptor with template argument of itself.

TEST(BagContainerAdaptor, InitializeBagContainerWithBagContainer)
{
    {
        // Adaptor underlying container type: std::vector<BagContainerAdaptor<int>>.
        BagContainerAdaptor<BagContainerAdaptor<int>> adapter;

        adapter.insert(BagContainerAdaptor<int>());
        adapter.insert(BagContainerAdaptor<int>());
        adapter.insert(BagContainerAdaptor<int>());

        EXPECT_EQ(adapter.size(), 3);
    }

    {
        // Adaptor underlying container type: std::list<BagContainerAdaptor<int>>.
        BagContainerAdaptor<BagContainerAdaptor<int>, std::list<BagContainerAdaptor<int>>> adapter;

        adapter.insert(BagContainerAdaptor<int>());
        adapter.insert(BagContainerAdaptor<int>());
        adapter.insert(BagContainerAdaptor<int>());

        EXPECT_EQ(adapter.size(), 3);
    }
}
