#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <gtest/gtest.h>

#include <vector>
#include <chrono>
#include <list>

template <typename Container>
class PerformanceTest : public ::testing::Test
{
protected:
	void insert(int itemCount)
	{
		Container testContainer;
		BagContainerAdaptor<Container> testBagContainerAdaptor;

		auto startContainerAdd = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < itemCount; i++)
		{
			testContainer.push_back(static_cast<typename Container::value_type>(i));
		}
		auto endContainerAdd = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testContainer.size(), itemCount);

		auto startBagContainerAdaptorAdd = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < itemCount; i++)
		{
			testBagContainerAdaptor.insert(static_cast<typename Container::value_type>(i));
		}
		auto endBagContainerAdaptorAdd = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testBagContainerAdaptor.size(), itemCount);

		std::chrono::duration<double> listAddTime = endContainerAdd - startContainerAdd;
		std::chrono::duration<double> bagContainerAdaptorAddTime = endBagContainerAdaptorAdd - startBagContainerAdaptorAdd;

		auto containerAddTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds>(listAddTime);
		auto bagContainerAdaptorAddTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds> (bagContainerAdaptorAddTime);

		EXPECT_GT(containerAddTimeInMicro, bagContainerAdaptorAddTimeInMicro);

#ifdef ADD_BENCHMARK
		std::cout << "List performance for inserting:" << std::endl;
		std::cout << "std::list time for inserting " << itemCount << " elements: ";
		std::cout << containerAddTimeInMicro.count() << "ms" << std::endl;
		std::cout << "BagContainerAdaptor time for inserting " << itemCount << " elements: ";
		std::cout << bagContainerAdaptorAddTimeInMicro.count() << "ms" << std::endl;
#endif
	}

	/*
	void remove(int itemCount, std::vector<int> itemsToRemove)
	{
		Container testContainer;
		BagContainerAdaptor<Container> testBagContainerAdaptor;

		for(int i = 0; i < itemCount; i++)
		{
			testList.push_back(i);
		}

		EXPECT_EQ(testList.size(), itemCount);

		for(int i = 0; i < itemCount; i++)
		{
			testBagContainerAdaptor.insert(i);
		}

		EXPECT_EQ(testBagContainerAdaptor.size(), ITEM_COUNT);

		auto startListRemove = std::chrono::high_resolution_clock::now();
		for(auto item : itemsToRemove)
		{
			testList.remove(item);
		}
		auto endListRemove = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testList.size(), ITEM_COUNT - itemsToRemove.size());

		auto startBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();
		for(auto item : itemsToRemove)
		{
			testBagContainerAdaptor.remove(item);
		}
		auto endBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testBagContainerAdaptor.size(), ITEM_COUNT - itemsToRemove.size());

		std::chrono::duration<double> listRemoveTime = endListRemove - startListRemove;
		std::chrono::duration<double> bagContainerAdaptorRemoveTime = endBagContainerAdaptorRemove - startBagContainerAdaptorRemove;

		auto listRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds>(listRemoveTime);
		auto bagContainerAdaptorRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds> (bagContainerAdaptorRemoveTime);

		EXPECT_GT(listRemoveTimeInMicro, bagContainerAdaptorRemoveTimeInMicro);

#ifdef ADD_BENCHMARK
		std::cout << "List performance for removing:" << std::endl;
		std::cout << "std::list time for inserting " << ITEM_COUNT << " elements: ";
		std::cout << listRemoveTimeInMicro.count() << "ms" << std::endl;
		std::cout << "BagContainerAdaptor time for inserting " << ITEM_COUNT << " elements: ";
		std::cout << bagContainerAdaptorRemoveTimeInMicro.count() << "ms" << std::endl;
#endif
	}
	*/




};

typedef ::testing::Types<
	std::list<int>, 
	std::list<char>,
	std::list<double>,
	std::list<float>
> ContainerTypes;

TYPED_TEST_SUITE(PerformanceTest, ContainerTypes);

TYPED_TEST(PerformanceTest, InsertionTest)
{
	this->insert(10000);
}


/*
TEST(PerformanceTest, ListPerformanceAdd)
{
	int ITEM_COUNT = 10000;

	std::list<int> testList;
	BagContainerAdaptor<std::list<int>> testBagContainerAdaptor;

	auto startListAdd = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < ITEM_COUNT; i++)
	{
		testList.push_back(i);
	}
	auto endListAdd = std::chrono::high_resolution_clock::now();

	EXPECT_EQ(testList.size(), ITEM_COUNT);

	auto startBagContainerAdaptorAdd = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < ITEM_COUNT; i++)
	{
		testBagContainerAdaptor.insert(i);
	}
	auto endBagContainerAdaptorAdd = std::chrono::high_resolution_clock::now();

	EXPECT_EQ(testBagContainerAdaptor.size(), ITEM_COUNT);

	std::chrono::duration<double> listAddTime = endListAdd - startListAdd;
	std::chrono::duration<double> bagContainerAdaptorAddTime = endBagContainerAdaptorAdd - startBagContainerAdaptorAdd;

	auto listAddTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds>(listAddTime);
	auto bagContainerAdaptorAddTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds> (bagContainerAdaptorAddTime);

	EXPECT_GT(listAddTimeInMicro, bagContainerAdaptorAddTimeInMicro);

#ifdef ADD_BENCHMARK
	std::cout << "List performance for inserting:" << std::endl;
	std::cout << "std::list time for inserting " << ITEM_COUNT << " elements: ";
	std::cout << listAddTimeInMicro.count() << "ms" << std::endl;
	std::cout << "BagContainerAdaptor time for inserting " << ITEM_COUNT << " elements: ";
	std::cout << bagContainerAdaptorAddTimeInMicro.count() << "ms" << std::endl;
#endif
}

TEST(PerformanceTest, ListPerformanceRemove)
{
	int ITEM_COUNT = 10000;
	std::vector<int> itemsToRemove { 1, 43, 33, 434, 5454, 323, 5545, 54, 343, 654, 10 };

	std::list<int> testList;
	BagContainerAdaptor<std::list<int>> testBagContainerAdaptor;

	for(int i = 0; i < ITEM_COUNT; i++)
	{
		testList.push_back(i);
	}

	EXPECT_EQ(testList.size(), ITEM_COUNT);

	for(int i = 0; i < ITEM_COUNT; i++)
	{
		testBagContainerAdaptor.insert(i);
	}

	EXPECT_EQ(testBagContainerAdaptor.size(), ITEM_COUNT);

	auto startListRemove = std::chrono::high_resolution_clock::now();
	for(auto item : itemsToRemove)
	{
		testList.remove(item);
	}
	auto endListRemove = std::chrono::high_resolution_clock::now();

	EXPECT_EQ(testList.size(), ITEM_COUNT - itemsToRemove.size());

	auto startBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();
	for(auto item : itemsToRemove)
	{
		testBagContainerAdaptor.remove(item);
	}
	auto endBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();

	EXPECT_EQ(testBagContainerAdaptor.size(), ITEM_COUNT - itemsToRemove.size());

	std::chrono::duration<double> listRemoveTime = endListRemove - startListRemove;
	std::chrono::duration<double> bagContainerAdaptorRemoveTime = endBagContainerAdaptorRemove - startBagContainerAdaptorRemove;

	auto listRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds>(listRemoveTime);
	auto bagContainerAdaptorRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds> (bagContainerAdaptorRemoveTime);

	EXPECT_GT(listRemoveTimeInMicro, bagContainerAdaptorRemoveTimeInMicro);

#ifdef ADD_BENCHMARK
	std::cout << "List performance for removing:" << std::endl;
	std::cout << "std::list time for inserting " << ITEM_COUNT << " elements: ";
	std::cout << listRemoveTimeInMicro.count() << "ms" << std::endl;
	std::cout << "BagContainerAdaptor time for inserting " << ITEM_COUNT << " elements: ";
	std::cout << bagContainerAdaptorRemoveTimeInMicro.count() << "ms" << std::endl;
#endif
}

*/



