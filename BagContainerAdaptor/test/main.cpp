#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <gtest/gtest.h>

template <typename Container>
class BagContainerAdaptorTest : public ::testing::Test
{
protected:
	void frontTest()
	{
		BagContainerAdaptor<Container> adapter;

		adapter.insert(adapter.begin(), 1);

		EXPECT_EQ(adapter.front(), 1);
	}
};

typedef ::testing::Types<
	std::list<int>,
	std::vector<int>,
	std::deque<int>,
	std::forward_list<int>,
	std::multiset<int>/*
	std::unordered_multiset<int>*/
> ContainerTypes;

TYPED_TEST_SUITE(BagContainerAdaptorTest, ContainerTypes);

TYPED_TEST(BagContainerAdaptorTest, frontTest)
{
	this->frontTest();
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

