#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <chrono>
#include <functional>
#include <iostream>

// Insert, remove and lookup functions for BagContainerAdaptor and the underlying type.
template <typename Container>
class Benchmark
{
public:
    using value_type = typename Container::value_type;

    static void containerInsert(size_t amount, const value_type& value)
    {
        Container container;

        for (size_t i = 0; i < amount; i++)
        {
            container.insert(container.begin(), value);
        }
    }

    static void bagInsert(size_t amount, const value_type& value)
    {
        BagContainerAdaptor<Container> adapter;

        for (size_t i = 0; i < amount; i++)
        {
            adapter.insert(value);
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
            adapter.insert(value);
        }

		for (size_t i = 0; i < amount; i++)
		{
			adapter.erase(adapter.begin());
		}
    }

    static void containerLookup(size_t amount, const value_type& target)
    {
        Container container;

        size_t half = amount / 2;

        for (size_t i = 0; i < amount; i++)
        {
            if (i == half)
            {
                container.insert(container.begin(), target);
            }
            else
            {
                typename Container::value_type temp = {};
                container.insert(temp);
            }
        }

        auto found = std::find(container.begin(), container.end(), target);

        if (found == container.end())
        {
            std::cerr << "Could not find target from container!" << std::endl;
        }
    }

    static void bagLookup(size_t amount, const value_type& target)
    {
        BagContainerAdaptor<Container> adapter;

        size_t half = amount / 2;

        for (size_t i = 0; i < amount; i++)
        {
            if (i == half)
            {
                adapter.insert(target);
            }
            else
            {
                typename Container::value_type temp = {};
                adapter.insert(temp);
            }
        }

        auto found = adapter.find(target);

        if (found == adapter.end())
        {
            std::cerr << "Could not find target from bag!" << std::endl;
        }
    }
};

// Same functions but for std::forward_list.
template <typename T>
class ForwardListBenchmark
{
public:
    static void insert(size_t amount, T value)
    {
        std::forward_list<T> list;
        auto it = list.before_begin();

        for (size_t i = 0; i < amount; i++)
        {
            list.insert_after(it, value);
        }
    }

    static void erase(size_t amount, T value)
    {
        std::forward_list<T> list;
        auto it = list.before_begin();

        for (size_t i = 0; i < amount; i++)
        {
            list.insert_after(it, value);
        }

        list.erase_after(it, list.end());
    }

    static void lookup(size_t amount, T target)
    {
        std::forward_list<T> list;
        auto it = list.before_begin();

        size_t half = amount / 2;

        for (size_t i = 0; i < amount; i++)
        {
            if (i == half)
            {
                list.insert_after(it, target);
            }
            else
            {
                T temp = {};
                list.insert_after(it, temp);
            }
        }

        auto found = std::find(list.begin(), list.end(), target);

        if (found == list.end())
        {
            std::cerr << "Could not find target from container!" << std::endl;
        }
    }
};

#endif
