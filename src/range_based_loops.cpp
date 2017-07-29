/*
 * range_based_loops.cpp
 *
 *  Created on: Jul 29, 2017
 *      Author: teodor
 */

#include "range_based_loops.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
using namespace std;

// Adaptor class to iterate on a container backwards
template <typename TContainer>
class Reversed
{
public:
	explicit Reversed(const TContainer& c)
	:_c(c) {}
	// The range-based "for" recongnizes containers that have
	// the begin() and end() methods. Note that we define only the
	// const versions here, so "for(auto & x : rev_c)" will not work
	typename TContainer::const_reverse_iterator begin() const
	{ return _c.rbegin(); }
	typename TContainer::const_reverse_iterator end() const
	{ return _c.rend(); }
private:
	const TContainer& _c;
};

// Template function that constructs the adaptor
template <typename TContainer>
Reversed<TContainer> make_reversed(const TContainer& c)
{
	return Reversed<TContainer>(c);
}

void demo_range_based_loops()
{
	// Basic 10-element integer array.
	int aData[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	// Access by value using a copy declared as a specific type.
	for( int y : aData ) {
		cout << y << " ";
	}
	cout << endl;

	// The auto keyword causes type inference to be used. Preferred.
	// Each element of 'aData' is copied int y, almost always undesirable
	for( auto y : aData ) {
		cout << y << " ";
	}
	cout << endl;

	// Type inference by reference. Preferred when modify is needed.
	for( auto& y : aData ) {
		y+=5;
		cout << y << " ";
	}
	cout << endl;

	// Type inference by constant reference. Preferred when no modify is needed.
	for( const auto& y : aData ) {
		cout << y << " ";
	}
	cout << endl;
	cout << "end of integer array test" << endl;
	cout << endl;

	// Create a vector object that contains 10 elements.
	vector<double> vData;
	for (int i = 0; i < 10; ++i) {
		vData.push_back(i + 0.14159);
	}

	// Range-based for loop to iterate through the vector, observing in-place.
	for( const auto &j : vData ) {
		cout << j << " ";
	}
	cout << endl;
	cout << "end of vector test" << endl;
	cout << endl;

	// Range-based for loop to iterate backwards through a list, observing in-place.
	list<unsigned int> lData { 2, 3, 5, 6, 11, 3, 17 };
	for (const auto & k : make_reversed(lData))
	{
		cout << k << " ";
	}
	cout << endl;
	cout << "end of reversed list test" << endl;
	cout << endl;

	map<int, char> m {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}};
	for(const auto & v : m)
	{
		cout << v.first << " -> " << v.second << endl;
	}

	cout << "end of map test" << endl;
	cout << endl;
}

