#include "easyfind.hpp"
#include <iostream>
#include <vector>
#include <list>

int main()
{
	std::cout << "=== Testing with std::vector ===" << std::endl;
	std::vector<int> numbers;
	for (int i = 1; i <= 5; ++i)
		numbers.push_back(i * 10);

	try
	{
		std::vector<int>::iterator it = easyfind(numbers, 30);
		std::cout << "Found element in vector: " << *it << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Vector search failed: " << e.what() << std::endl;
	}

	std::cout << "\n=== Testing with std::list ===" << std::endl;
	std::list<int> values;
	values.push_back(100);
	values.push_back(200);
	values.push_back(300);

	try
	{
		std::list<int>::iterator it = easyfind(values, 200);
		std::cout << "Found element in list: " << *it << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "List search failed: " << e.what() << std::endl;
	}

	std::cout << "\n=== Testing Element Not Found ===" << std::endl;
	try
	{
		std::cout << "Searching for 999 in vector..." << std::endl;
		easyfind(numbers, 999);
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}

	return 0;
}
