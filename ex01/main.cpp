#include "Span.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	std::cout << "=== Subject Test Case ===" << std::endl;
	Span sp = Span(5);
	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	std::cout << "Shortest span: " << sp.shortestSpan() << " (expected 2)" << std::endl;
	std::cout << "Longest span:  " << sp.longestSpan() << " (expected 14)" << std::endl;

	std::cout << "\n=== Large Scale Test (15,000 Numbers) ===" << std::endl;
	const unsigned int bigSize = 15000;
	Span bigSpan(bigSize);

	std::vector<int> randomNumbers;
	randomNumbers.reserve(bigSize);

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	for (unsigned int i = 0; i < bigSize; ++i)
		randomNumbers.push_back(std::rand());

	bigSpan.addRange(randomNumbers.begin(), randomNumbers.end());

	std::cout << "Added " << bigSpan.size() << " elements successfully." << std::endl;
	std::cout << "Shortest span: " << bigSpan.shortestSpan() << std::endl;
	std::cout << "Longest span:  " << bigSpan.longestSpan() << std::endl;

	std::cout << "\n=== Exception Handling Tests ===" << std::endl;
	try
	{
		std::cout << "Attempting to add beyond capacity..." << std::endl;
		sp.addNumber(42);
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}

	try
	{
		Span emptySpan(10);
		std::cout << "Attempting shortestSpan on empty span..." << std::endl;
		emptySpan.shortestSpan();
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}

	return 0;
}
