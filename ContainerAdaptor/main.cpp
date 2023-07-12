#include "container_adaptor.hpp"

template <typename T>
struct Example
{
	T* data;	
};

int main()
{
	//ContainerAdaptor<int, std::vector> adaptor1;
	//ContainerAdaptor<int, Example> adaptor2;

	ContainerAdaptor<std::vector<float>> f;

	f.insert(1.0f);
	f.insert(2.0f);
	f.insert(5.0f);

	std::vector<int> vec { 1, 2, 3, 4, 5};

	//ContainerAdaptor<vec> g;

	ContainerAdaptor<std::vector<int>, LinkedList<int>> a;
	//ContainerAdaptor<std::list<int>> g;

	f.print();

	return 0;
}

