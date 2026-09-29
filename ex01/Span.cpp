#include "Span.hpp"
#include <algorithm>

Span::Span()
	: _maxCapacity(0)
{
}

Span::Span(unsigned int n)
	: _maxCapacity(n)
{
}

Span::Span(const Span& other)
	: _maxCapacity(other._maxCapacity), _storage(other._storage)
{
}

Span& Span::operator=(const Span& rhs)
{
	if (this != &rhs)
	{
		_maxCapacity = rhs._maxCapacity;
		_storage = rhs._storage;
	}
	return *this;
}

Span::~Span()
{
}

void Span::addNumber(int number)
{
	if (_storage.size() >= _maxCapacity)
		throw std::length_error("Span is already at full capacity");
	_storage.push_back(number);
}

int Span::shortestSpan() const
{
	if (_storage.size() < 2)
		throw std::logic_error("At least two numbers are required to compute a span");

	std::vector<int> sorted = _storage;
	std::sort(sorted.begin(), sorted.end());

	int minDistance = sorted[1] - sorted[0];
	for (std::size_t i = 2; i < sorted.size(); ++i)
	{
		int diff = sorted[i] - sorted[i - 1];
		if (diff < minDistance)
			minDistance = diff;
	}
	return minDistance;
}

int Span::longestSpan() const
{
	if (_storage.size() < 2)
		throw std::logic_error("At least two numbers are required to compute a span");

	int minVal = *std::min_element(_storage.begin(), _storage.end());
	int maxVal = *std::max_element(_storage.begin(), _storage.end());

	return maxVal - minVal;
}

std::size_t Span::size() const
{
	return _storage.size();
}

std::size_t Span::capacity() const
{
	return _maxCapacity;
}
