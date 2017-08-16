#pragma once

#ifndef __COLLECTION_H__
#define __COLLECTION_H__

#include <iterator>
#include <iostream>
#include <vector>


#define BC_SENTENCE_DELIMITER " "

/**
* Cast vector/enumerable elements into a new vector.
* @param      <Source>  The source element type.
* @param      <Target>  The target element type.
* @param[in]  source    The enumeration of Source elements to cast.
* @returns              A new enumeration with elements cast to Target.
*/
template <typename Source, typename Target>
std::vector<Target> cast(const std::vector<Source>& source);

/**
* Obtain the sorted distinct elements of the list.
* @param      <Element>  The list element type.
* @param[in]  list       The list.
* @return                The sorted list reduced to its distinct elements.
*/
template <typename Element>
std::vector<Element>& distinct(std::vector<Element>& list);

/**
* Find the position of a pair in an ordered list.
* @param      <Pair>  The type of list member elements.
* @param[in]  list    The list to search.
* @param[in]  key     The key to the element to find.
* @return             The position or -1 if not found.
*/
template <typename Pair, typename Key>
int find_pair_position(const std::vector<const Pair>& list, Key& key);

/**
* Find the position of an element in an ordered list.
* @param      <Element>  The type of list member elements.
* @param[in]  list       The list to search.
* @param[in]  value      The value of the element to find.
* @return                The position or -1 if not found.
*/
template <typename Element, typename Container>
int find_position(const Container& list, const Element& value);

/**
* Facilitate a list insertion sort by inserting into a sorted position.
* @param      <Type>       The type of list member elements.
* @param      <Predicate>  The sort predicate function signature.
* @param[in]  list         The list to modify.
* @param[in]  element      The element to insert.
* @param[in]  predicate    The sort predicate.
* @return                  The vector iterator.
*/
template <typename Type, typename Predicate>
typename std::vector<Type>::iterator insert_sorted(std::vector<Type>& list,
	const Type& element, Predicate predicate);

/**
* Move members of a source list to end of a target list. Source is cleared.
* @param      <Type>     The type of list member elements.
* @param[in]  target     The target list.
* @param[in]  source     The source list
*/
template <typename Type>
void move_append(std::vector<Type>& target, std::vector<Type>& source);

template <typename Source, typename Target>
std::vector<Target> cast(const std::vector<Source>& source)
{
	std::vector<Target> target(source.size());
	target.assign(source.begin(), source.end());
	return target;
}

template <typename Element>
std::vector<Element>& distinct(std::vector<Element>& list)
{
	std::sort(list.begin(), list.end());
	list.erase(std::unique(list.begin(), list.end()), list.end());
	list.shrink_to_fit();
	return list;
}

template <typename Pair, typename Key>
int find_pair_position(const std::vector<Pair>& list, const Key& key)
{
	const auto predicate = [&](const Pair& pair)
	{
		return pair.first == key;
	};

	auto it = std::find_if(list.begin(), list.end(), predicate);

	if (it == list.end())
		return -1;

	return static_cast<int>(distance(list.begin(), it));
}

template <typename Element, typename Container>
int find_position(const Container& list, const Element& value)
{
	const auto it = std::find(std::begin(list), std::end(list), value);

	if (it == std::end(list))
		return -1;

	return static_cast<int>(std::distance(list.begin(), it));
}

template <typename Type, typename Predicate>
typename std::vector<Type>::iterator insert_sorted(std::vector<Type>& list,
	Type& element, Predicate predicate)
{
	return list.insert(std::upper_bound(list.begin(), list.end(), element,
		predicate), element);
}

template <typename Type>
void move_append(std::vector<Type>& target, std::vector<Type>& source)
{
	target.reserve(target.size() + source.size());
	std::move(source.begin(), source.end(), std::back_inserter(target));
	source.clear();
}

#endif
