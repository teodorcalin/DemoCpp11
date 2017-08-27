/*
 * casts.cpp
 *
 *  Created on: Aug 27, 2017
 *      Author: teodor
 */

#include "casts.h"
#include <iostream>
using namespace std;

void demo_casts()
{
	int j = 7;
	// Does not compile
	// double* pDbl = static_cast<double*>(&j);

	// Old C-style type erasure, pervasive in legacy code
	void* pV = &j;
	// Compiles, no warning -> bogus value is printed
	double* pDbl = static_cast<double*>(pV);
	cout << "Int to void to double : " << *pDbl << endl;

	// Compiles, no warning -> same bogus value is printed
	double* pDbl2 = reinterpret_cast<double*>(&j);
	cout << "Int to double : " << *pDbl2 << endl;
}
