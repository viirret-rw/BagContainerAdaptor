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

	static void containerLookup(size_t amount, const value_type& value)
	{
		Container container;

		for (size_t i = 0; i < amount; i++)
		{
			container.insert(container.begin(), value);
		}

		auto res = std::find(container.begin(), container.end(), value);
	}

	static void bagLookup(size_t amount, const value_type& value)
	{
		BagContainerAdaptor<Container> adapter;

		for (size_t i = 0; i < amount; i++)
		{
			adapter.insert(adapter.begin(), value);
		}

		auto res = adapter.find(value);
	}

	static void run(const std::string& name, size_t amount, const value_type& value, const std::function<void(size_t, value_type)>& test)
	{
		auto begin = std::chrono::steady_clock::now();

		test(amount, value);

		auto end = std::chrono::steady_clock::now();

		std::cout << name << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " nanoseconds" << std::endl;
	}

};

#endif
