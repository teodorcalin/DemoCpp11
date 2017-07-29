/*
 * decltype_typeid_auto.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: teodor
 */

#include <array>
#include <iostream> // for cout
#include <iomanip> // for setw
#include <string>
#include <vector>
#include <typeinfo> // for typeid
#include <initializer_list> // for auto arrays
#include "type_support.h"

using namespace std;

// Generic addition template function with an auto return type
// computed from the template parameter types
template <class S, class T>
auto add(T v1, S v2) -> decltype(v1 + v2)
{
	return v1 + v2;
}

void demo_type_support()
{
	cout << "Mangled standard type names :" << endl;

	int a;
	unsigned int b;
	short c;
	unsigned short d;
	long e;
	unsigned long f;
	char g;
	unsigned char h;
	float i;
	double j;
	long double k;
	string s;
	wstring t;
	char* pC(nullptr);
	const char* pCC(nullptr);
	const char* const cpCC (nullptr);

	const unsigned int TYPE_LENGTH = 32;
	cout << left << setw(TYPE_LENGTH) << "int" << typeid(a).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "unsigned int" << typeid(b).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "short" << typeid(c).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "unsigned short" << typeid(d).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "long" << typeid(e).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "unsigned long" << typeid(f).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "char" << typeid(g).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "unsigned char" << typeid(h).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "float" << typeid(i).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "double" << typeid(j).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "long double" << typeid(k).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "string" << typeid(s).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "wstring" << typeid(t).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "char*" << typeid(pC).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "const char*" << typeid(pCC).name() << endl;
	cout << left << setw(TYPE_LENGTH) << "const char* const" << typeid(cpCC).name() << endl;

	decltype(cpCC) cpName = "Alice";
	cout << cpName << " - value of type " << typeid(cpName).name() << endl;

	auto int_value = 5;
	cout << int_value << " - value of type " << typeid(int_value).name() << endl;

	auto uint_value = 0u;
	cout << uint_value << " - value of type " << typeid(uint_value).name() << endl;

	auto long_value = -7l;
	cout << long_value << " - value of type " << typeid(long_value).name() << endl;

	auto ulong_value = 94ul;
	cout << ulong_value << " - value of type " << typeid(ulong_value).name() << endl;

	// Result of addition of an unsigned long with a float
	auto result_add = add(.6f, -7l);
	cout << ".6f + -7l = " << result_add  << " - value of type " << typeid(result_add).name() << endl;

	// Auto with floating point constants
	auto pi_rough = 3.14f;
	auto pi_coarse = 3.1415926;
	auto pi_accurate = 3.14159265359l;
	cout << pi_rough  << " - value of type " << typeid(pi_rough).name() << endl;
	cout << setprecision(8) << pi_coarse  << " - value of type " << typeid(pi_coarse).name() << endl;
	cout << setprecision(16) << pi_accurate  << " - value of type " << typeid(pi_accurate).name() << endl;

	// Auto with const and /or volatile
	volatile auto val = 5;				// volatile int
	auto volatile const flag = true;	// const volatile bool
	auto const answer = 'n';			// const char

#define SHOW_TYPE_OF_VAR(var) \
	cout << var << " - value of type " << typeid(var).name() << endl

	SHOW_TYPE_OF_VAR(val);
	SHOW_TYPE_OF_VAR(flag);
	SHOW_TYPE_OF_VAR(answer);

	// Auto with initializer lists (POD arrays)
	auto int_init_list = {0, 1, 2, 3};
	auto cstring_init_list = {"Earth", "Venus", "Mars" };

#define SHOW_TYPE_OF_ARRAY(arrayvar) \
	for(auto v : arrayvar) \
		cout << v << endl; \
	cout << " - values of type " << typeid(arrayvar).name() << endl


	SHOW_TYPE_OF_ARRAY(int_init_list);
	SHOW_TYPE_OF_ARRAY(cstring_init_list);

	// Build vector from initializer list
	vector<int> int_vect(int_init_list);
	SHOW_TYPE_OF_ARRAY(int_vect);


#undef SHOW_TYPE_OF_VAR
#undef SHOW_TYPE_OF_ARRAY
}



