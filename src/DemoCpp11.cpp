//============================================================================
// Name        : DemoCpp11.cpp
// Author      : Teodor Calin
// Version     :
// Copyright   : CC
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream> // for cerr
#include <cstdlib> // for EXIT_SUCCESS
#include "smart_pointers.h"
#include "type_support.h"
#include "range_based_loops.h"
#include "initialization.h"
#include "pitfalls.h"

using namespace std;

int main(void)
{
	try
	{
		//demo_smart_pointers();
		//demo_type_support();
		//demo_range_based_loops();
		//demo_initialization();
        (void)demo_pitfalls();
	} catch(...){
		cerr << "Something bad happened." << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
