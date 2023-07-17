#include <BagContainerAdaptor/container_adaptor.hpp>

template <typename T>
struct Example
{
	T* data;	
};

int main()
{
	//ContainerAdaptor<int, std::vector> adaptor1;
	//ContainerAdaptor<int, Example> adaptor2;

	ContainerAdaptor<std::vector<int>> f;

	f.insert(1);
	f.insert(2);
	f.insert(5);
	f.insert(5);
	f.insert(100);
	f.insert(1000);
	f.insert(33);

	f.debugInfo();

	f.insert(std::vector<int>{ 1, 2, 3, 4, 5 });

	f.debugInfo();

	// Testing member functions.
	std::cout << "Front: " << f.front() << std::endl;
	std::cout << "Back: " << f.back() << std::endl;
	std::cout << "Size: " << f.size() << std::endl;
	std::cout << "Empty: " << f.empty() << std::endl;


	//std::vector<int> vec { 1, 2, 3, 4, 5};

	//ContainerAdaptor<vec> g;

	//ContainerAdaptor<std::vector<int>, SequentialList<int>> a;
	//ContainerAdaptor<std::list<int>> g;


	//f.remove(1);


	return 0;
}
