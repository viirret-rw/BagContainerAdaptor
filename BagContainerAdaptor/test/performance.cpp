#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <gtest/gtest.h>

#include <vector>
#include <chrono>
#include <list>
#include <deque>

template <typename Container>
class PerformanceTest : public ::testing::Test
{
protected:
	void TestBody() override {}
	
	void insert(int itemCount)
	{
		Container testContainer;
		BagContainerAdaptor<Container> testBagContainerAdaptor;

		auto startContainerAdd = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < itemCount; i++)
		{
			testContainer.push_back(static_cast<typename Container::value_type>(i));
		}
		auto endContainerAdd = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testContainer.size(), itemCount);

		auto startBagContainerAdaptorAdd = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < itemCount; i++)
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
		std::cout << "INSERT PERFORMANCE:" << std::endl;
		std::cout << "STL container time for inserting " << itemCount << " elements: ";
		std::cout << containerAddTimeInMicro.count() << "ms" << std::endl;
		std::cout << "BagContainerAdaptor time for inserting " << itemCount << " elements: ";
		std::cout << bagContainerAdaptorAddTimeInMicro.count() << "ms" << std::endl;
#endif
	}

public:
	void remove(int itemCount, std::vector<int> itemsToRemove)
	{
		Container testContainer;
		BagContainerAdaptor<Container> testBagContainerAdaptor;

		for (int i = 0; i < itemCount; i++)
		{
			testContainer.push_back(static_cast<typename Container::value_type>(i));
		}

		EXPECT_EQ(testContainer.size(), itemCount);

		for (int i = 0; i < itemCount; i++)
		{
			testBagContainerAdaptor.insert(static_cast<typename Container::value_type>(i));
		}

		EXPECT_EQ(testBagContainerAdaptor.size(), itemCount);

		auto startContainerRemove = std::chrono::high_resolution_clock::now();
		for (auto item : itemsToRemove)
		{
			testContainer.remove(item);
		}
		auto endContainerRemove = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testContainer.size(), itemCount - itemsToRemove.size());

		auto startBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();
		for (auto item : itemsToRemove)
		{
			testBagContainerAdaptor.remove(item);
		}
		auto endBagContainerAdaptorRemove = std::chrono::high_resolution_clock::now();

		EXPECT_EQ(testBagContainerAdaptor.size(), itemCount - itemsToRemove.size());

		std::chrono::duration<double> containerRemoveTime = endContainerRemove - startContainerRemove;
		std::chrono::duration<double> bagContainerAdaptorRemoveTime = endBagContainerAdaptorRemove - startBagContainerAdaptorRemove;

		auto containerRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds>(containerRemoveTime);
		auto bagContainerAdaptorRemoveTimeInMicro = std::chrono::duration_cast<std::chrono::microseconds> (bagContainerAdaptorRemoveTime);

		EXPECT_GT(containerRemoveTimeInMicro, bagContainerAdaptorRemoveTimeInMicro);

#ifdef ADD_BENCHMARK
		std::cout << "List performance for removing:" << std::endl;
		std::cout << "std::list time for inserting " << itemCount  << " elements: ";
		std::cout << containerRemoveTimeInMicro.count() << "ms" << std::endl;
		std::cout << "BagContainerAdaptor time for inserting " << itemCount << " elements: ";
		std::cout << bagContainerAdaptorRemoveTimeInMicro.count() << "ms" << std::endl;
#endif
	}
};

typedef ::testing::Types<
	std::list<int>,
	std::list<char>,
	std::list<double>,
	std::list<float>,
	std::vector<int>
> ContainerTypes;

TYPED_TEST_SUITE(PerformanceTest, ContainerTypes);

TYPED_TEST(PerformanceTest, InsertionTest)
{
	this->insert(10000);
}

TEST(PerformanceTest, RemoveTestList)
{
	PerformanceTest<std::list<int>> performanceTest;
	performanceTest.remove(10000, std::vector<int>{ 11, 22, 33, 44, 55, 66, 77, 88, 99, 100, 110, 120, 140, 6736 });
}

