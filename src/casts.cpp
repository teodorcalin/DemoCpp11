// References: https://arne-mertz.de/2015/01/a-casting-show/
#include "casts.h"
#include <climits>
#include <iostream>

#define UNUSED_VAR(x) (void)(x)
using namespace std;

// Utilities

void takesAnInt(int a)
{
    cout << a << " - this is the overload that takes an int" << endl;
}

void takesAnInt(long int b)
{
    cout << b << " - this is the overload that takes a long int " << endl;
}

class Pet {
public:
    // At least one virtual function is needed to make base class plymorphic, 
    // for dynamic cast. A base-class virtual destructor is good practice.
    virtual ~Pet() = default;
};
class MouseEater {};
class Cat : public Pet, public MouseEater {};
class Ferret : public Pet, public MouseEater {};
class Dog : public Pet {};

void feedTheCat(Pet& b)
{
    // I know for a fact, at compile time, that b is a Cat
    Cat& d = static_cast<Cat&>(b);
    UNUSED_VAR(d);
}

void feedItAMouse(Pet& b)
{
    cout << "Your pet says : 'A mouse ? ";
    try
    {
        MouseEater& f = dynamic_cast<MouseEater&>(b);
        UNUSED_VAR(f);
        cout << " Yum-yum !'" << endl;
    }catch(/*std::bad_cast const& bc*/...){
        cout << " I hate you !'" << endl;
    }
}

class B1 {}; 
class B2 { int i; }; 
class B3 { int j; };
class D : public B2, public B3 {};

class MyContainer
{
    size_t _n;
    int* _data;
public:
    MyContainer(size_t n, int* data)
    :_n(n)
    {
        _data = new int[n];
        for (size_t i=0; i<n; ++i)
            _data[i] = data[i];
    }
    ~MyContainer() { delete [] _data; }
    /// RHV accessor: for when the container is constant: we may only read data
    int const& at(size_t index) const
    {
        return _data[index];
    }
    /// LHV accessor: for when the container is mutable - we can change data
    int& at(size_t index)
    {
        MyContainer const* const_this = static_cast<MyContainer const*>(this);
        return const_cast<int&>(const_this->at(index));
    }
    /// Method that emulates poor legacy code: should have been declared const
    void println()
    {
        for (size_t i=0; i<_n; ++i)
            cout << _data[i] << " ";
        cout << endl;
    }
};

void demo_casts()
{
    cout << endl << "*************** Casts ********************" << endl;
    /// Static cast
    /// A a; B b = static_cast<B>(a);
    /// The most straightforward check-at-compile-time cast operator in C++
    //
    // Use conditions :
    // - source type A and target type B are not unrelated (see reinterpret_cast)
    // but there is no implicit conversion from A to B (like int to long int)
    //
    // Alternatives :
    // - when B is a user-defined class type, it is common to use function-style
    // cast or call the conversion constructor : b = B(a) or (C++11) b = B{a}
    //
    // Common use cases:
    // 1. narrowing conversions between numbers (int to short, double to int etc.)
    // 2. conversions between integrals and enums
    // 3. make a widening coversion explicit (e.g. int to long) to force the
    // compiler to pick a specific function overload
    // 4. downcast pointers or references in class hierarchies when you know
    // the type of the object (e.g. void* to int*, Base& to Derived& etc.)
    // 5. store pointers as void* (e.g. to use polymorphism in C legacy code)

    int j = 5;
    // misuse: Does not compile - unrelated types
    // double* pDbl = static_cast<double*>(&j);
    // Use reinterpret_cast if you really want to do this.

    // Use case 1 - narrowing
    short k = (j < SHRT_MIN ? SHRT_MIN : 
                (j > SHRT_MAX ? SHRT_MAX : static_cast<short>(j))
              );
    UNUSED_VAR(k);

    // Use case 2 - enuming
    enum ESpanishNumerals
    {
        E_UNO = 1,
        E_DOS,
        E_TRES,
        E_QUATRO,
        E_CINCO
    };
    ESpanishNumerals l = static_cast<ESpanishNumerals>(j);
    UNUSED_VAR(l);

    // Use case 3 - forcing overload choice
    cout << "j value is " << j << endl;
    takesAnInt(j);
    takesAnInt(static_cast<long>(j)); 

    // Use case 4 - downcasting when 100% sure
    Cat d;          ///< d is a Cat which is a Pet
    Pet* b = &d;    ///< Implicit conversion from Cat* to Pet*
                    ///< Maybe to store the cat in a generic list vector<Pet*>
    feedTheCat(*b); ///< Contains an explicit static cast from Pet& to Cat&

    // Use case 4 - misuse
    cout << "Choose a pet : input < 0 for Cat, "
        "0 <= input < 5 for Ferret, "
        "input >= 5 for Dog" << endl;
    short petChoice = 0;
    cin >> petChoice;
    Pet* pD(nullptr);
    if(petChoice < 0)
        pD = new Cat;
    else if (petChoice < 5)
        pD = new Ferret;
    else
        pD = new Dog;
    feedTheCat(*pD);  ///< Here we may be feeding cat food to a dog or ferret
    // Here one should use dynamic_cast instead

    // Use case 4 - misuse
    // Old C-style type erasure, pervasive in legacy code
    void* pV = &j;
    // Compiles, no warning -> bogus value is printed
    double* pDbl = static_cast<double*>(pV);
    cout << "Int reinterpreted as double : " << j << " == " << *pDbl << endl;
    // One may as well use reinterpret_cast if not sure that pV is a double*

    // Use case 5
    void* vpJ = &j;
    int* p2J = static_cast<int*>(vpJ);
    cout << "j value is still " << *p2J << endl;

    /// Dynamic cast
    /// A a; B b = dynamic_cast<B>(a);
    /// Check-at-runtime cast operator in C++
    //
    // Use conditions :
    // - A and B are pointer or reference types to types XA and XB
    // in a polymorphic hierarchy ( base class has virtual functions )
    // AND
    // - downcast : XA* points to a base class, XB* points to a derived class
    // - OR cross-cast : types XA and XB have a common ancestor base class XC
    //
    // Alternatives :
    // - dynamic_cast is often (not always) a hint to a flawed design. Many 
    // naive applications of dynamic_cast can be solved more cleanly with 
    // virtual functions. Try virtual functions before using dynamic_cast.
    //
    // Common use cases:
    // 1. downcast of pointers or references in class hierarchies when the type
    // can only be determined at runtime - e.g. objects are built by a factory
    // 2. cross-cast towards a virtual interface - facet - of an object, when 
    // that facet can only be determined at runtime.

    // Use case 1
    Dog* pDog = dynamic_cast<Dog*>(pD);
    if(pDog)
        cout << "Dog says 'woof!'" << endl;
    else
        cout << "Your pet is not a dog." << endl;

    // Use case 2
    feedItAMouse(*pD); ///< Contains a dynamic cast from Pet& to MouseEater&

    delete pD;

    /// Reinterpret cast
    /// A a; B b = reinterpret_cast<B>(a);
    /// Cast-blindly-never-check cast operator in C++
    //
    // Use conditions :
    // - source type A and target type B are unrelated (e.g. long and double*)
    // and A and B are numeric types or pointer types (not user-defined types)
    //
    // Alternatives :
    // - Try the other types of cast before considering reinterpret_cast
    //
    // Common use cases:
    // 1. store pointers in an integer type (e.g. to log them in legacy code)

    // Use case 1
    intptr_t pJ = reinterpret_cast<intptr_t>(&j);
    UNUSED_VAR(pJ);

    // Misuse 1 - assumed downcast
    // You assume D is derived from B1 and try to downcast
    // and the compiler lets you do it, although D is not related to B1
    B1* p2B1 = nullptr;
    D* p2D = reinterpret_cast<D*>(p2B1);
    // In this case you should use static or dynamic cast, for type checking
    // D* p2D = static_cast<D*>(p2B); ///< This does not compile
    UNUSED_VAR(p2D);

    // Misuse 2 - downcast
    D d0;
    B3* p2B3 = &d0;     // implicit conversion from D* to B3*
                        // with offset of sizeof(B2) = sizeof(int) = 4 bytes
    D* p2D_ok = static_cast<D*>(p2B3);          // correctly removes the offset
    D* p2D_ko = reinterpret_cast<D*>(p2B3);     // keeps the same address
    cout << "Address of d is " << &d0 << endl;
    cout << "p2D_ok points to " << p2D_ok << endl;
    cout << "p2D_ko points to " << p2D_ko << endl;
    // For downcasts one should use either static_cast or dynamic_cast, 
    // never reinterpret_cast.

    /// Const cast
    /// A a; B b = const_cast<B>(a);
    ///  cast operator in C++
    //
    // Use conditions :
    // - A is pointer-to-constant or reference-to-constant type and B is 
    // the pointer or reference to the same type without the constness
    //
    // Alternatives :
    // - Try to correctly enforce constness and use mutable instead
    //
    // Common use cases:
    // 1. cast away constness to be able to use const-incorrect legacy code
    // 2. accessor methods with a const and non-const version
    
    // Use case 1
    int table[] = {2, 3, 5, 7, 11};
    const MyContainer cmc(5, table);
    // Does not compile: println() method should have been declared const
    // cmc.println()
    const_cast<MyContainer*>(&cmc)->println();

    // Use case 2
    cout << "Element at index 2: " << cmc.at(2) << endl;
    // Does not compile: OK: only the RHV accessor works on a constant container
    // cmc.at(2) = 19;
    MyContainer mc(5, table);
    cout << "Element at index 2: " << mc.at(2) << endl;
    // Here, the LHV accessor is using the const_cast; see class definition
    mc.at(2) = 13;
    cout << "Element at index 2: " << mc.at(2) << endl;

    /// C-style cast
    /// A a; B b = (B) a;
    /// The universal cast operator in C; superseded in C++
    //
    // Use conditions :
    // - you are constrained to work with a C-only compiler
    // 
    // Mechanism :
    // at compile-time, tries the following casts in order, 
    // the first which succeeds is it:
    // 1. const_cast
    // 2. static_cast
    // 3. static_cast followed by const_cast
    // 4. reinterpret_cast
    // 5. reinterpret_cast followed by const_cast

    // Misuse
    const int m = j;
    pDbl = (double*)&m;
    cout << "j value as a double is " << *pDbl <<endl;
    // Here one should prefer either static_cast or reinterpret_cast.
    // C-style cast casts away constness - rarely wanted - and may
    // degrade in a reinterpret_cast, which does no type checking and is unsafe
    // while downcasting. It is also hard to find in the code. No advantages.

}

