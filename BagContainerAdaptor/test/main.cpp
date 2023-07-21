#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <gtest/gtest.h>

template <typename Container>
class BagContainerAdaptorTest : public ::testing::Test
{
protected:
	void insertTest()
	{
		BagContainerAdaptor<Container> adapter;

		adapter.insert(1);
		adapter.insert(2);
		adapter.insert(3);
	}

	void findTest()
	{
		BagContainerAdaptor<Container> adapter;

		adapter.insert(1);
		adapter.insert(2);
		adapter.insert(3);

		auto it = adapter.find(2);

		EXPECT_TRUE(it != adapter.end());
	}

	void frontTest()
	{
		BagContainerAdaptor<Container> adapter;

		adapter.insert(1);

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

	void sizeTest()
	{
		BagContainerAdaptor<Container> adapter;

		adapter.insert(1);
		EXPECT_EQ(adapter.size(), 1);
		adapter.insert(2);
		EXPECT_EQ(adapter.size(), 2);
		adapter.insert(3);
		EXPECT_EQ(adapter.size(), 3);
	}

	void emptyTest()
	{
		BagContainerAdaptor<Container> adapter;
		EXPECT_TRUE(adapter.empty());
		adapter.insert(1);
		EXPECT_FALSE(adapter.empty());
	}

	void swapTest()
	{
		BagContainerAdaptor<Container> adapter1;
		adapter1.insert(1);
		adapter1.insert(2);
		adapter1.insert(3);
		adapter1.insert(4);

		BagContainerAdaptor<Container> adapter2;
		adapter2.insert(1);

		EXPECT_EQ(adapter1.size(), 4);
		EXPECT_EQ(adapter2.size(), 1);

		adapter1.swap(adapter2);

		EXPECT_EQ(adapter1.size(), 1);
		EXPECT_EQ(adapter2.size(), 4);
	}
};

typedef ::testing::Types<
	std::list<int>,
	std::vector<int>,
	std::deque<int>,
	std::forward_list<int>,
	std::multiset<int>,
	std::unordered_multiset<int>
> ContainerTypes;

TYPED_TEST_SUITE(BagContainerAdaptorTest, ContainerTypes);

TYPED_TEST(BagContainerAdaptorTest, insertTest)
{
	this->insertTest();
}

TYPED_TEST(BagContainerAdaptorTest, findTest)
{
	this->findTest();
}

TYPED_TEST(BagContainerAdaptorTest, frontTest)
{
	this->frontTest();
}

TYPED_TEST(BagContainerAdaptorTest, backTest)
{
	this->backTest();
}

TYPED_TEST(BagContainerAdaptorTest, sizeTest)
{
	this->sizeTest();
}

TYPED_TEST(BagContainerAdaptorTest, emptyTest)
{
	this->emptyTest();
}

TYPED_TEST(BagContainerAdaptorTest, swapTest)
{
	this->swapTest();
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

