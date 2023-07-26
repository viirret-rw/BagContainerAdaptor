#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <chrono>
#include <functional>

template <typename Container>
class Benchmark
{
public:
	using value_type = typename Container::value_type;

	static void containerInsert(size_t amount, value_type value)
	{
		Container container;

		for (size_t i = 0; i < amount; i++)	
		{
			container.insert(container.begin(), value);
		}
	}

	static void bagInsert(size_t amount, value_type value)
	{
		BagContainerAdaptor<Container> adapter;	

		for (size_t i = 0; i < amount; i++)
		{
			adapter.insert(adapter.begin(), value);
		}
	}

	static void containerErase(size_t amount, const value_type& value)
	{
		Container container;

		for (size_t i = 0; i < amount; i++)
		{
			container.insert(container.begin(), value);
		}

		for (size_t i = 0; i < amount; i++)
		{
			container.erase(container.begin());
		}
	}

	static void bagErase(size_t amount, const value_type& value)
	{
		BagContainerAdaptor<Container> adapter;

		for (size_t i = 0; i < amount; i++)
		{
			adapter.insert(adapter.begin(), value);
		}

		for (size_t i = 0; i < amount; i++)
		{
			adapter.erase(adapter.begin());
		}
	}

	static void containerLookup(size_t amount, value_type target)
	{
		Container container;

		size_t half = amount / 2;

		for (size_t i = 0; i < amount; i++)	
		{
			if (i == half)
			{
				container.insert(container.end(), target);
			}
			else
			{
				typename Container::value_type temp = {};
				container.insert(container.end(), temp);
			}
		}

		auto found = std::find(container.begin(), container.end(), target);

		if (found == container.end())
		{
			std::cerr << "Could not find target from container!" << std::endl;
		}
	}
	
	static void bagLookup(size_t amount, value_type target)
	{
		BagContainerAdaptor<Container> adapter;

		size_t half = amount / 2;

		for (size_t i = 0; i < amount; i++)
		{
			if (i == half)
			{
				adapter.insert(adapter.end(), target); 
			}
			else
			{
				adapter.insert(adapter.end(), {});
			}
		}

		auto found = adapter.find(target);

		if (found == adapter.end())
		{
			std::cerr << "Could not find target from bag!" << std::endl;
		}
	}
};

#endif
