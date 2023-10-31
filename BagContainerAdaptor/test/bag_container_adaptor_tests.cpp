#include <gtest/gtest.h>

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <list>

// This is a special test for initializing BagContainerAdaptor with template argument of itself.

// When BagContainerAdaptor is initialized like this the underlying container for the 'adapter'
// is the template argument for the template argument BagContainerAdapter.

// This is because the value_type of the BagContainerAdapter is always it's template argument.
TEST(BagContainerAdaptor, InitializeBagContainerWithBagContainer)
{
    {
        BagContainerAdaptor<BagContainerAdaptor<int>, std::vector<int>> adapter;
        adapter.insert(1);
        adapter.insert(5);

        EXPECT_EQ(adapter.size(), 2);
    }

    {
        BagContainerAdaptor<int, std::multiset<int>> set;
        set.insert(1);
        set.insert(5);
        BagContainerAdaptor<int, std::multiset<int>> adapter2(std::move(set));
        EXPECT_EQ(adapter2.size(), 2);
    }

    {
        std::list<float> list = {1.f, 2.f, 66.f, 213.f};
        BagContainerAdaptor<float, std::list<float>> adapter = std::move(list);
        BagContainerAdaptor<float, std::list<float>> adapter2 = std::move(adapter);

        EXPECT_EQ(adapter2.size(), 4);
    }
}
