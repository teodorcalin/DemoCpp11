/*
 * lambdas.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: teodor
 */

#include "lambdas.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <string>


using namespace std;

void test_functor(void (*pFunc)())
{
	pFunc();
}

void test_functor(void (*pStringFunc)(const string&), const std::string& name)
{
	pStringFunc(name);
}

void test_functor(double (*pBinaryOp)(double, double), double a, double b)
{
	cout << "Result of " << a << " / " << b << " is " << pBinaryOp(a, b) << endl;
}

class ObjectLambdaTest
{
public:
	int _four = 4;
	void run()
	{
		int five = 5;

		// Capturing 'this' brings in all the data members by reference
		// Other local variables can be captures by value
		auto objLambda1 = [this, five](){
			_four = 8;
			cout << _four << " + " << five << " = "
					<< _four + five<< endl;
		};
		objLambda1();
		cout << "_four is now " << _four << endl;
		cout << "five is now " << five << endl;

		// Local variables can also be captured by reference like so.
		// 'this' can only be captured by value;
		auto objLambda2 = [&, this](){
			_four = 4;
			five = 10;
			cout << _four << " + " << five << " = "
					<< _four + five<< endl;
		};
		objLambda2();
		cout << "_four is now " << _four << endl;
		cout << "five is now " << five << endl;

	}
};

bool is_prime(unsigned int n)
{
	if(n < 4)
		return n >= 2;
	if(n % 2 == 0 || n % 3 == 0)
		return false;

	for(unsigned int i = 5; i*i <= n; i += 6)
	{
		if(n % i == 0 || n % (i+2) == 0)
			return false;
	}

	return true;
}

template <typename T>
size_t count_in_list(function<bool(T)> predicate, const list<T> & in_list)
{
	// Decorate the predicate using lambda
	function<bool(T)> predicate_and_print = [predicate](T t) {
		bool is_valid = predicate(t);
		if (is_valid)
			cout << t << " ";
		return is_valid;
	};
	// Apply the decorated predicate using the algorithm
	size_t total_count = count_if(in_list.begin(), in_list.end(), predicate_and_print);
	cout << endl;
	return total_count;
}

void demo_lambdas()
{
	// Lambda: Creating, calling and
	auto fn_hello = [](){ cout << "Hello !" << endl; };
	fn_hello();
	test_functor(fn_hello);
	test_functor([](){ cout << "Goodbye !" << endl; });

	auto fn_greet = [](const string& name){ cout << "Hello " << name << endl; };
	fn_greet("James");
	test_functor(fn_greet, "Katherin");

	// Since we have two return statements, each with a different inferred type
	// we need to specify '-> double' as a return type to the lambda
	auto fn_divide = [](double a, double b) -> double{
		if(b == 0.0)
			return 0;
		return a/b;
	};
	cout << fn_divide(10.0, 5.0) << endl;
	test_functor(fn_divide, 7, 3);

	int one = 1;
	int two = 2;
	int three = 3;

	// Capture one and two by value; call the lambda directly
	[one, two]() { cout << one << " + " << two << " = " << one+two << endl; }();

	// Capture all local variables by value
	[=]() { cout << one << " + " << two << " + " << three << " = "
		<< one+two+three << endl; }();

	// Capture all local variables by value, but capture three by reference
	[=, &three]() {
		three = 7;
		cout << one << " + " << two << " + " << three << " = "
			<< one+two+three << endl;
	}();
	cout << "three is now " << three << endl;

	// Capture all local variables by reference
	[&]() {
		one = 2;
		two = 4;
		three = 6;
		cout << one << " + " << two << " + " << three << " = "
		<< one+two+three << endl;
	}();
	cout << "one is now " << one << endl;
	cout << "two is now " << two << endl;
	cout << "three is now " << three << endl;

	// Capture all local variables by reference, except for two and three - by reference
	[&, two, three]() {
		one = 3;
		// Will not compile: local variables captured by value
		// two = 6; three = 9;
		cout << one << " + " << two << " + " << three << " = "
		<< one+two+three << endl;
	}();
	cout << "one is now " << one << endl;

	// Demonstrate the capturing of 'this' inside objects
	ObjectLambdaTest test1;
	test1.run();

	// Generate list of numbers using STL algorithm and lambda
	// Note that the variables that have static storage are
	// available in the body of the lambda as if captured by reference
	list<unsigned int> numbers(40);
	static unsigned int n = 0;
	generate(numbers.begin(), numbers.end(), []{ return n++; });
	cout << "Initial list: " << endl;
	for(unsigned int i : numbers)
	{
		cout << i << " ";
	}
	cout << endl;

	// Filter list of numbers using functor
	bool (*fptr_filter)(unsigned int) = &is_prime;
	cout << "Filtered list: " << endl;
	size_t n_primes = count_in_list<unsigned int>(fptr_filter, numbers);
	cout << "Total prime numbers : " << n_primes << endl;

	// By default variables captures by value are not modifiable inside the lambda:
	// Does not compile
	// [&, counter] (int a) mutable { sum = ++counter + a; }(3);

	// To be able to modify all variable captured by value
	// - like you would a parameter passed by value to a function -
	// you need to declare the lambda 'mutable'
	int sum = 0;
	int counter = 0;
	[&, counter] (int a) mutable { sum = ++counter + a; }(4);
	// Value of counter is unchanged
	cout << sum << endl << counter << endl;

}
