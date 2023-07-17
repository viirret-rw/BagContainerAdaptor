#include <gtest/gtest.h>

#define ADD_BENCHMARK

#include "performance.cpp"

#include <algorithm>

TEST(mainTests, IteratorTest)
{
	BagContainerAdaptor<std::list<int>> adapter;

	adapter.insert(1);
	adapter.insert(2);
	adapter.insert(5);
	adapter.insert(7);
	adapter.insert(11);
	adapter.insert(30);

	auto it = std::find(adapter.begin(), adapter.end(), 7);

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
