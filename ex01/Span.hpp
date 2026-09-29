#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <iterator>
#include <stdexcept>

class Span
{
public:
	Span();
	Span(unsigned int n);
	Span(const Span& other);
	Span& operator=(const Span& rhs);
	~Span();

	void addNumber(int number);

	template <typename InputIterator>
	void addRange(InputIterator first, InputIterator last)
	{
		typename std::iterator_traits<InputIterator>::difference_type count = std::distance(first, last);
		if (count < 0 || _storage.size() + static_cast<std::size_t>(count) > _maxCapacity)
			throw std::length_error("Span capacity exceeded while adding range");
		_storage.insert(_storage.end(), first, last);
	}

	template <typename InputIterator>
	void addNumber(InputIterator first, InputIterator last)
	{
		addRange(first, last);
	}

	int shortestSpan() const;
	int longestSpan() const;

	std::size_t size() const;
	std::size_t capacity() const;

private:
	std::size_t _maxCapacity;
	std::vector<int> _storage;
};

#endif
