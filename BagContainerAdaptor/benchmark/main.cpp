#include "benchmark.hpp"

template <typename Container>
class TestRunner
{
public:
	using value_type = typename Container::value_type;

	static void runTests(size_t amount, value_type value)
	{
		Benchmark<Container> m_benchmark;
		m_benchmark.run("Container insert", amount, value, m_benchmark.containerInsert);
		m_benchmark.run("Bag insert", amount, value, m_benchmark.bagInsert);
		m_benchmark.run("Container erase", amount, value, m_benchmark.containerErase);
		m_benchmark.run("Bag erase", amount, value, m_benchmark.bagErase);
		m_benchmark.run("Container lookup", amount, value, m_benchmark.containerLookup);
		m_benchmark.run("Bag lookup", amount, value, m_benchmark.bagLookup);
	}
};

#define SIZE 1000000

int main()
{
	TestRunner<std::list<std::vector<int>>>::runTests(SIZE, std::vector<int> { 1, 2, 3, 4 });
	TestRunner<std::list<int>>::runTests(SIZE, 4);


	return 0;
}
