#include "pitfalls.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
using namespace std;

int increment( int& nValue )
{
    ++nValue;
    cout << "Incremented value is " << nValue << endl;
    return nValue;
}

int makeDouble( int &nValue )
{
    nValue *= 2;
    cout << "Doubled value is " << nValue << endl;
    return nValue;
}

int multiply( int x, int y )
{
    return x * y;
}

bool incrAndTestEqual2(int& w)
{
    return ++w == 2;
}


namespace Color
{
    enum Color
    {
        BLUE = 1,
        PURPLE,
        GREEN,
        RED
    };
}

class Base
{
private:
    int m_nID;
    virtual int generateClassID() { return 1; }
public:
    virtual ~Base() {}
    Base()
    {
        m_nID = generateClassID();
    }
    int getID() { return m_nID; }
};

class Derived : public Base
{
private:
    virtual int generateClassID() { return 2; }
public:
    Derived()
    {}
};

// version 1
void processing(string a, bool flag = true)
{
    cout << "call to processing version 1 with arguments " 
        << a << " and " << flag << endl;
}
// version 2
void processing(string a, string b, bool flag = true)
{
    cout << "call to processing version 2 with arguments " 
        << a << ", " << b << " and " << flag << endl;
}

class Shape // an abstract class
{
public:
    virtual ~Shape() {}
    Shape() { init(); }
private:
    void init() { reset(); }
    virtual void reset() = 0;
    int _color;
}; 

class Point : public Shape // a concrete derived class
{  
public:
    virtual void reset() { _x = _y = 0.0; }
private:
    double _x, _y;
};


void demo_pitfalls()
{
    cout << endl << "*************** Pitfalls *****************" << endl;

    // Uninitialized value incremented => no compiler warning
    // The compiler will warn 'Wuninitialized' only if variable is used 
    // in this same scope where it is defined
    int nX;
    increment(nX);

    // Integer division => no compiler warning
    int num = 7, den = 2;
    int fraction = num/den;
    (void) fraction;

    // Assignment used in place of equality operator => no compiler warning
    int nY = -1;
    bool isValid = (nY = 0);
    if(isValid)
        nY += 10;
    else
        nY += 100;

    // Automated conversion from signed to unsigned => no compiler warning
    // int constant is promoted to unsigned int; result is unsigned int
    cout << "Value should be -5 : " << 10 - 15u << endl;

    // 'delete' used in place of 'delete[]' => memory leak; no compiler warning
    // Only first element of the array is properly destroyed
    char* aName = new char[10];
    delete aName;

    // Side efects in compound expression => 'Wsequence-point' compiler warning sometimes
    // Result of the expression depends of order in which the side effects complete.
    // Here both multiply and makeDouble have a side effect => no compiler warning
    int nZ = 5;
    cout << "Is value 18 or 21 ? " <<  increment(nZ) + makeDouble(nZ) << endl;
    // cout << "Is value 30 or 36 ? " <<  multiply(nZ, ++nZ) << endl; // warning Wsequence-point

    // '++nW' will be evaluated only if first operant of && is true => no compiler warning
    // Any operator that has a side effect (e.g. ++, --, += etc.) should be in its own statement
    int nT = 0, nW = 1;
    if( nT == 1 && incrAndTestEqual2(nW))
        cout << "Yes : nT is " << nT << " and nW is " << nW << endl;
    else
        cout << "No : nT is " << nT << " and nW is " << nW << endl;

    // Missing break statement at the end of a switch-case block => no compiler warning
    int nValue = 2;
    Color::Color eColor = Color::BLUE;
    switch (nValue)
    {
        case 1: eColor = Color::BLUE;
        case 2: eColor = Color::PURPLE;
        case 3: eColor = Color::GREEN;
        default: eColor = Color::RED;
    }
    cout << "Color should be 2 (PURPLE) it is " << eColor << endl;

    // Virtual method is called in constructor => no compiler warning
    // Since derived class is not yet fully constructed when virtual method is called
    // the base class version of virtual method 'generateClassID' is called
    Derived xDerived;
    cout << "ID should be 2 it is " << xDerived.getID() << endl;


    // Version 1 of the overloaded function is called, not version 2 => no compiler warning
    // Explanation: arguments passed are of type const char*; the 2nd argument is more readily 
    // converted from const char* to bool than to string. Go figure.
    processing("Hello", "World");

    // b is actually a function, not a variable
    string a("Hello");
    string b();
    // Remember to omit the () for the default constructor.
    // The compiler will complain here:
    // cout << "b is " << b << endl;

    // Crashes the program with "pure virtual method called"
    //Point p;

    list<int> lSource, lTarget(3u, 10);
    lSource.push_back(4);
    lSource.push_back(5);
    lSource.push_back(6);
    // Should be find(lSource.begin(), lSource.end())
    // Like this it compiles without warning and generates undefined behavior
    // - may be a infinite loop as lTarget.end() is never reached.
    // list<int>::iterator it = find(lSource.begin(), lTarget.end(), 7);
    // if(it != lSource.end())
    //    lTarget.push_back(*it);
}
