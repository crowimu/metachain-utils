#pragma once

#ifndef __ARRAY_SLICE_H__
#define __ARRAY_SLICE_H__

#include <cstddef>

/**
* An read-only STL-style wrapper for array-style collections.
*
* This class allows std::array, std::vector, std::string, and c-style arrays
* to be used interchangeably in functions that expect an iterable container.
* This is not a substitute for move overloads. Consider also that a contiguous
* buffer (c-style array) is more performant than the iterator abstraction.
*/
template <typename Iterable>
class array_slice
{
public:
	template <typename Container>
	array_slice(const Container& container);

	array_slice(const Iterable* begin, const Iterable* end);

	const Iterable* begin() const;
	const Iterable* end() const;
	const Iterable* data() const;
	std::size_t size() const;
	bool empty() const;

private:
	const Iterable* begin_;
	const Iterable* end_;
};


/*
IMPLEMENTATION
*/
template <typename Iterable>
template <typename Container>
array_slice<Iterable>::array_slice(const Container& container)
	: begin_(container.data()), end_(container.data() + container.size())
{
}

template <typename Iterable>
array_slice<Iterable>::array_slice(const Iterable* begin, const Iterable* end)
	: begin_(begin), end_(end)
{
}

template <typename Iterable>
const Iterable* array_slice<Iterable>::begin() const
{
	return begin_;
}

template <typename Iterable>
const Iterable* array_slice<Iterable>::end() const
{
	return end_;
}

template <typename Iterable>
const Iterable* array_slice<Iterable>::data() const
{
	return begin_;
}

template <typename Iterable>
std::size_t array_slice<Iterable>::size() const
{
	return end_ - begin_;
}

template <typename Iterable>
bool array_slice<Iterable>::empty() const
{
	return end_ == begin_;
}

#endif

