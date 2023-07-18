#include <gtest/gtest.h>

#define ADD_BENCHMARK

#include "performance.cpp"

#include <algorithm>

TEST(mainTests, SwapTest)
{
	BagContainerAdaptor<std::list<int>> adapter1;

	adapter1.insert(1);
	adapter1.insert(2);
	adapter1.insert(3);

	BagContainerAdaptor<std::list<int>> adapter2;
	adapter2.insert(4);
	adapter2.insert(5);
	adapter2.insert(6);


	adapter1.swap(adapter2);

	// Show contents of the adapter1
	adapter1.debugInfo();
	adapter2.debugInfo();
}

TEST(mainTests, MoveAssingmentTest)
{
	BagContainerAdaptor<std::list<int>> adapter;

	adapter.insert(1);
	adapter.insert(2);
	adapter.insert(5);
	adapter.insert(7);
	adapter.insert(11);
	adapter.insert(30);

	BagContainerAdaptor<std::list<int>> adapter2;
	adapter2 = std::move(adapter);

	adapter2.debugInfo();

	std::cout << "the next should be empty" << std::endl;
	adapter.debugInfo();
}

TEST(mainTests, IteratorTest)
{
	BagContainerAdaptor<std::list<int>> adapter;

	adapter.insert(1);
	adapter.insert(2);
	adapter.insert(5);
	adapter.insert(7);
	adapter.insert(11);
	adapter.insert(30);

	auto it = adapter.find(11);

	if(it != adapter.end())
	{
		std::cout << "Value found" << std::endl;
	}
	else
	{
		std::cout << "Value not found" << std::endl;
	}
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
