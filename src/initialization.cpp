#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "initialization.h"

using namespace std;

template<typename C>
void print_container(const C& c)
{
    for(const auto& x : c)
    {
        cout << x << " ";
    }
    cout << endl;
}

template<typename D>
void print_dictionary(const D& d)
{
    for(const auto& x : d)
    {
        cout << x.first << "->" << x.second << " ";
    }
    cout << endl;
}

template<typename T>
void print_array(const T* a, unsigned int size)
{
    for(const T* x = a; x < a+size; ++x)
    {
        cout << *x << " ";
    }
    cout << endl;
}

class C1
{
    // In-class initialized data members
    string _s {"abc"};
    double _d = 3.14;
    const char * _p {nullptr};
    int _y[4] {1,2,3,4};
public:
    // This is equivalent to C() : s("abc"), d(3.14), p(nullptr), y{1,2,3,4} {}
    C1() = default;
    // This constructor overrides the default initialization of two data members
    C1(double d, const char* p) : _d(d), _p(p) {}
    void print() const
    {
        cout << _s << " " << _d << " " << (_p ? _p : "") << endl;
        print_array(_y, 4);
    }
};

struct S1
{
    double _d;
    const char * _p;
    int _y[3];
    void print() const
    {
        cout << _d << " " << (_p ? _p : "") << endl;
        print_array(_y, 3);
    }
};

S1 transform(const S1& s_in)
{
    s_in.print();
    // Initialize function return object. Values must be constant expressions
    return {1.5, "Hello", {7, 8, 9}};
}

void demo_initialization()
{
    cout << endl << "*************** Initialization ***********" << endl;

    // Initializing arrays
    int aData[] {1, 2, 3, 4, 5};
    print_container(aData);

    // Initializing STL containers
    vector<int> vData {2, 3, 4, 5, 6};
    print_container(vData);

    set<int> sData {3, 4, 5, 6, 7};
    print_container(sData);

    map<int, string> mData {{0, "zero"}, {1, "one"}, {2, "two"}};
    print_dictionary(mData);

    // Dynamic array initialization
    double* aDblData = new double[4] {2.7183, 3.1416, 1.4142, 1.7321 };
    print_array(aDblData, 4);
    delete [] aDblData;

    // Use in-class member initialization, picked up by the default constructor
    C1 c_default;
    c_default.print();

    // Use a custom constructor to override some data members initialization
    C1 c_custom(2.71, "Hello");
    c_custom.print();

    // Use new brace initialization syntax to call the custom constructor
    C1 c_custom2 {1.73, "Halo"};
    c_custom2.print();

    // Default initialization of struct (aggregate initalization)
    S1 s_default {};
    s_default.print();

    // Partial initialization of struct (aggregate initalization)
    S1 s_partial {3.14};
    s_partial.print();

    // Custom initialization of struct (aggregate initalization)
    S1 s_custom {1.41, "Bye", {3, 4, 5}};
    s_custom.print();

    // Initialize function parameter object
    S1 s_returned = transform({2.71, "Greetings", {4, 5, 6}});
    s_returned.print();

    // When confronted with a brace initializer,
    // the compiler attempts the following, in order:
    // - initializer list constructor ( parameters in braces must all be
    // constants of the same type; no narrowing conversion - e.g. double to int)
    // - regular constructor (narrowing conversion allowed)
    // - aggregate initialization (all data members must be public)

    // Illegal: narrowing conversion
    // vector <int> v1 {4.5, 3};

    // Legal : initializer list constructor
    // => vector of size 2
    vector<double> v2 {4.5, 3};
    print_container(v2);

    // Legal but tricky: regular constructor vector<T>(size_t size, const T& value)
    // => vector of size size_t(4.5) == 4 (narrowing conversion)
    // => all elements are initialized with value 3
    vector<double> v3(4.5, 3);
    print_container(v3);
}

