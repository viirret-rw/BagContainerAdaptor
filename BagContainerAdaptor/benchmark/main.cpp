#include "benchmark.hpp"
#include "custom_type.hpp"

#include <unordered_map>

long memoryUsage = 0;

// Overloading new and delete operators globally to track memory usage.
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

// Test runner function to track the time it takes to run the benchmarks.
template <typename CallBackType, typename... Args>
void run(const std::string& name, CallBackType callback, Args&&... args)
{
    auto begin = std::chrono::steady_clock::now();
    callback(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();

    std::cout << name << " took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " nanoseconds. ";
    std::cout << "Allocations: " << memoryUsage << " bytes." << std::endl;
    memoryUsage = 0;
}

// Run insert, remove and lookup for BagContainerAdapter and the underlying type.
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

// Run the same tests for std::forward_list.
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

// Run all the benchmarks for all the required types.
template <typename T>
void runBenchmarks(size_t amount, const T& value, const T& target)
{
    std::cout << "std::vector\n";
    BenchmarkRunner<std::vector<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "std::deque\n";
    BenchmarkRunner<std::deque<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "std::list\n";
    BenchmarkRunner<std::list<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "std::forward_list\n";
    ForwardListRunner<T>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "std::multiset\n";
    BenchmarkRunner<std::multiset<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "unordered_multiset\n";
    BenchmarkRunner<std::unordered_multiset<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "LinkedList\n";
    BenchmarkRunner<LinkedList<T>>::runTests(amount, value, target);
    std::cout << "\n";

    std::cout << "RingBuffer\n";
    BenchmarkRunner<ring_buffer<T>>::runTests(amount, value, target);
}

int main()
{
    // These runBenchmarks() calls are only made with primitives,
    // because std::unordered_multiset sucks.

    std::cout << "int, 10000 iterations\n";
    runBenchmarks<int>(10000, 5, 6);
    std::cout << "\n";

    std::cout << "double, 10000\n";
    runBenchmarks<double>(10000, 0.2, 0.5);
    std::cout << "\n";

    return 0;
}
