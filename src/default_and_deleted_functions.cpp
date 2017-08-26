#include "default_and_deleted_functions.h"
#include <iostream>
#include <string>

using namespace std;

enum class MetroStopType : unsigned int
{
	Circle = 0,
	Square,
	Triangle,
	NumTypes // must be the last element
};

string to_string(MetroStopType type)
{
	string str_type("Unknown");
	switch(type)
	{
	case MetroStopType::Circle : str_type = "Circle"; break;
	case MetroStopType::Square : str_type = "Square"; break;
	case MetroStopType::Triangle: str_type = "Triangle"; break;
	default: break;
	}
	return str_type;
}


constexpr unsigned int nStopTypes
	= static_cast<unsigned int>(MetroStopType::NumTypes);

class MetroStop
{
private:
	MetroStopType _type = MetroStopType::Circle;
	int _locX = 0;
	int _locY = 0;
	double _passengerFrequenciesHz[nStopTypes] = {0.0, 0.5, 0.5};
public:
	// Keep the default constructor; that the compiler would otherwise
	// no longer generate given the definition of a custom constructor
	MetroStop() = default;
	// Delete the copy constructor
	MetroStop(const MetroStop& other) = delete;
	// Delete the copy-assignment operator
	MetroStop& operator=(const MetroStop& other) = delete;
	// Custom constructor
	MetroStop(MetroStopType type, int locX, int locY)
	: _type(type), _locX(locX), _locY(locY) {
		for (unsigned int i = 0; i < nStopTypes; ++i)
		{
			if(i == static_cast<unsigned int>(type))
				_passengerFrequenciesHz[i] = 0.0;
			else
				_passengerFrequenciesHz[i] = 0.5;
		}
	}
	void print()
	{
		cout << to_string(_type) << " at (" << _locX << ", " << _locY << ")" << endl;
		cout << "Passenger frequencies: " << endl;
		for (unsigned int i = 0; i < nStopTypes; ++i)
		{
			cout << "- to " << to_string(static_cast<MetroStopType>(i))
				<< " : " << _passengerFrequenciesHz[i] << " Hz" << endl;
		}

	}
	// deleted operator new prevents object from being dynamically allocated.
	void* operator new(size_t) = delete;
	// deleted operator array new prevents object arrays from being dynamically allocated
	void* operator new [] (size_t) = delete;
};

//prevent call through type promotion of any T to double from succeeding.
template < typename T >
void call_with_true_double_only(T) = delete;

// also define for const double, double&, etc. as needed.
void call_with_true_double_only(double param) {
	cout << "Called with true double parameter value " << param << endl;
}

void demo_default_and_deleted_functions()
{
	// Default constructed
	MetroStop cirStop1;
	cirStop1.print();
	// Custom constructed
	MetroStop sqrStop1(MetroStopType::Square, 4, 7);
	sqrStop1.print();
	// Will not compile: copy-constructor is deleted
	// MetroStop cirStop2(cirStop1);
	// Will not compile: copy-assignment operator is deleted
	// cirStop1 = sqrStop1;

	call_with_true_double_only(3.0);
	// Will not compile: overloaded function with float parameter deleted
	// call_with_true_double_only(-1.f);
	// Will not compile: overloaded function with int parameter deleted
	// call_with_true_double_only(5);

	// Will not compile : operator new deleted
	// MetroStop* pMS = new MetroStop;
	// pMS->print();
	// Will not compile : operator array new deleted
	// MetroStop* aMS = new MetroStop[3];
	// aMS[0].print();
}
