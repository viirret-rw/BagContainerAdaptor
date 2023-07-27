#include "benchmark.hpp"
#include "custom_type.hpp"

#include <unordered_map>

long memoryUsage = 0;

void* operator new(size_t size)
{
	void* ptr = malloc(size);
	if (ptr != nullptr)
	{
		memoryUsage += size;
	}
	return ptr;
}

void operator delete(void* ptr) noexcept
{
	if (ptr != nullptr)
	{
		free(ptr);
	}
}

template <typename CallBackType, typename ...Args>	
void run(const std::string& name, CallBackType callback, Args&&...args)
{
	auto begin = std::chrono::steady_clock::now();
	callback(std::forward<Args>(args)...);
	auto end = std::chrono::steady_clock::now();

	std::cout << name << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " nanoseconds. ";
	std::cout << "Allocations: " << memoryUsage << " bytes." << std::endl;
	memoryUsage = 0;
}

template <typename Container>
class BenchmarkRunner
{
public:
	using value_type = typename Container::value_type;

	static void runTests(size_t amount, const value_type& value, const value_type& target)
	{
		Benchmark<Container> benchmark;
		
		run("Container insert", benchmark.containerInsert, amount, value);
		run("Bag insert", benchmark.bagInsert, amount, value);
		run("Container erase", benchmark.containerErase, amount, value);
		run("Bag erase", benchmark.bagErase, amount, value);
		run("Container lookup", benchmark.containerLookup, amount, target);
		run("Bag lookup", benchmark.bagLookup, amount, target);
	}
};

template <typename T>
class ForwardListRunner
{
public:
	static void runTests(size_t amount, const T& value, const T& target)
	{
		ForwardListBenchmark<T> forwardListBenchmark;
		Benchmark<std::forward_list<T>> benchmark;

		run("Container insert", forwardListBenchmark.insert, amount, value);
		run("Bag insert", benchmark.bagInsert, amount, value);
		run("Container erase", forwardListBenchmark.erase, amount, value);
		run("Bag erase", benchmark.bagErase, amount, value);
		run("Container lookup", forwardListBenchmark.lookup, amount, target);
		run("Bag lookup", benchmark.bagLookup, amount, target);
	}
};

int main()
{
	std::cout << "std::vector<int>" << std::endl;
	BenchmarkRunner<std::vector<int>>::runTests(100000, 3310, 323);
	std::cout << std::endl;

	std::cout << "std::vector<CustomType>" << std::endl;
	BenchmarkRunner<std::vector<CustomType>>::runTests(100, CustomType(), CustomType());
	std::cout << std::endl;

	std::cout << "std::vector<std::vector<std::string>>>" << std::endl;
	BenchmarkRunner<std::vector<std::vector<std::string>>>::runTests(1000, std::vector<std::string> { "hello", "how", "are", "you"}, std::vector<std::string> { "hey" });
	std::cout << std::endl;

	std::cout << "std::list<int>" << std::endl;
	BenchmarkRunner<std::list<int>>::runTests(100000, 323254, 311);
	std::cout << std::endl;

	std::cout << "std::list<std::unordered_map<int, std::string>>" << std::endl;
	BenchmarkRunner<std::list<std::unordered_map<int, std::string>>>::runTests(100000, std::unordered_map<int, std::string> {{ 1, "one" }, { 2, "two" }, { 3, "three" }},
	std::unordered_map<int, std::string> {{ 66, "moi" }});
	std::cout << std::endl;

	std::cout << "std::deque<size_t>" << std::endl;
	BenchmarkRunner<std::deque<size_t>>::runTests(100000, std::numeric_limits<size_t>::max(), 543543);
	std::cout << std::endl;

	std::cout << "std::deque<std::vector<std::vector<int>>>" << std::endl;
	BenchmarkRunner<std::deque<std::vector<std::vector<int>>>>::runTests(100000, std::vector<std::vector<int>> { std::vector<int> { 4, 2 }, std::vector<int> { 5, 8 }},
	std::vector<std::vector<int>> { std::vector<int> { 363 }});
	std::cout << std::endl;

	std::cout << "std::multiset<int>" << std::endl;
	BenchmarkRunner<std::multiset<int>>::runTests(1000000, 1, 65656);
	std::cout << std::endl;

	std::cout << "std::multiset<std::list<std::string>>" << std::endl;
	BenchmarkRunner<std::multiset<std::list<std::string>>>::runTests(10000, std::list<std::string> { "hey" }, std::list<std::string> { "hey hey" });
	std::cout << std::endl;

	std::cout << "multiset<std::list<CustomType>>" << std::endl;
	BenchmarkRunner<std::multiset<std::list<CustomType>>>::runTests(10000, std::list<CustomType> { CustomType() }, std::list<CustomType> { CustomType()});
	std::cout << std::endl;

	std::cout << "std::unordered_multiset<int>" << std::endl;
	BenchmarkRunner<std::unordered_multiset<int>>::runTests(10000, 10, 54);
	std::cout << std::endl;

	std::cout << "std::unordered_multiset<char>" << std::endl;
	BenchmarkRunner<std::unordered_multiset<char>>::runTests(10000, 'a', 'b');
	std::cout << std::endl;

	{
		std::cout << "BagContainerAdaptor<std::vector<int>>" << std::endl;
		BagContainerAdaptor<std::vector<int>> adaptor;
		BenchmarkRunner<BagContainerAdaptor<std::vector<int>>>::runTests(100000, 1, 2);
		std::cout << std::endl;
	}

	std::cout << "std::forward_list<int>" << std::endl;
	ForwardListRunner<int>::runTests(10000, 5, 2);
	std::cout << std::endl;

	std::cout << "std::forward_list<std::pair<int, std::string>" << std::endl;
	ForwardListRunner<std::pair<int, std::string>>::runTests(100000, std::pair<int, std::string>{ 1, "moi"}, std::pair<int, std::string>{ 2, "two"});
	std::cout << std::endl;

	return 0;
}
