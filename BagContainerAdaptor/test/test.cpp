#include <BagContainerAdaptor/bag_container_adaptor.hpp>

#include <list>
#include <iostream>


int main()
{
	BagContainerAdaptor<int, std::list<int>> bag;

	// Just testing the insert functionality.
	bag.insert(1);
	bag.insert(1);
	bag.insert(1);
	bag.insert(1);
	bag.insert(1);


	BagContainerAdaptor<int, std::multiset<int>> bag2;
	
	for (int i = 0; i < 1000; i++)
	{
		bag2.insert(i);
	}

	auto it = bag2.find(55);

	// This should just print 55.
	std::cout << *it << std::endl;


	// Doing the same thing for std::unordered_multiset.
	
	BagContainerAdaptor<int, std::unordered_multiset<int>> bag3;

	for (int i = 0; i < 1000; i++)
	{
		bag3.insert(i);
	}

	auto it2 = bag3.find(55);
	std::cout << *it << std::endl;





	return 0;
}
