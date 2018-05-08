/* Reference: 'https://www.cprogramming.com/c++11/
rvalue-references-and-move-semantics-in-c++11.html'
*/

#include <iostream>
#include "rvalues.h"

using namespace std;

int fn()
{
    return 5;
}

void check(const int& ref)
{
    cout << "L-value version " << ref << endl;
}

void check(int&& ref)
{
    cout << "R-value version " << ref << endl;
}

class ArrayWrapper
{
public:
    // Constructor from size with default value
    explicit ArrayWrapper (size_t n)
        : _p_vals( new int[ n ] )
        , _size( n )
    {}
    // Destructor
    ~ArrayWrapper() 
    {
        delete [] _p_vals;
    }
    // Copy constructor - copies from another object of the same type
    ArrayWrapper(ArrayWrapper const& other)
    :_p_vals(new int [other._size])
    ,_size(other._size)
    {
        // Must allocate a new array and perform a deep copy
        for ( size_t i = 0; i < _size; ++i )
            _p_vals[ i ] = other._p_vals[ i ];
        cout << "Performed a deep copy" << endl;
    }
    // Move constructor - plunders from a temporary object of the same type
    ArrayWrapper(ArrayWrapper && temp_other)
    :_p_vals(temp_other._p_vals)
    ,_size(temp_other._size)
    {
        // Since temp_other is a temporary, the program will no longer
        // refer to it : it is ok to "steal" its internal array,
        // no need to allocate a new array and perform a deep copy.

        // However, since the descructor will be called on the temporary,
        // we must reassign its _p_vals to avoid deleting the stolen array.
        temp_other._p_vals = nullptr;
        // Reassigning _size is not strictly necessary
        temp_other._size = 0;

        // Note that both _p_vals and _size are built-in data types.
        // If the class had a field that is an object, - for instance
        // Metadata _metadata, or std::string _name - then that field 
        // must be transfered using with std::move (which invokes 
        // its move constructor), like so: _metadata(std::move(other._metadata))
        // The reason for this is that temp_other - and hence also
        // temp_other._metadata - is an l-value for the duration of this 
        // constructor, so we need to convert it to an r-value using std::move.

        cout << "Performed a 'move' copy" << endl;
    }
    // Const accessor
    int const& at(size_t i) const { return _p_vals[i]; }
    // Mutable accessor
    int& at(size_t i) 
    { 
        auto const_this = const_cast<ArrayWrapper const*>(this);
        return const_cast<int&>(const_this->_p_vals[i]); 
    }
    // print method
    void print() const
    {
        for ( size_t i = 0; i < _size; ++i )
            cout << _p_vals[ i ] << " ";
        cout << endl;
    }

private:
    int* _p_vals;
    size_t _size;
};

ArrayWrapper makeArray(size_t n, int value)
{
    ArrayWrapper array(n);
    for(size_t i=0; i<n; ++i)
        array.at(i) = value;
    return array;
}

void printAddress (const int& v) // const ref to allow binding to rvalues
{
    cout << reinterpret_cast<const void*>( & v ) << endl;
}

int x;

int getInt ()
{
    // Return by value
    return x;
}

int && getRvalueInt ()
{
    // Return an r-value reference
    return std::move( x );
}

void demo_rvalues()
{
    cout << endl << "*************** R-values *****************" << endl;

    // Rule: whatever we can get an address of is an l-value
    // Otherwise it is an r-value
    int i = 8;
    const int j = 9;
    int* k = new int(10);
    // Ok: variables are l-values
    int* ptr1 = &i;
    cout << *ptr1 << endl;
    // Ok: named constants are l-values
    const int* ptr2 = &j;
    cout << *ptr2 << endl;
    // Ok: pointers (variables holding an address) are l-values
    int** ptr3 = &k;
    cout << *ptr3 << endl;

    // KO: literal constants are r-values
    // compiler says: "error: lvalue required as unary ‘&’ operand"
    // int* ptr4 = &8;

    // KO: function return values are r-values
    // int* ptr5 = &fn();

    // Ok: The l-value is the variable after incrementation
    int* ptr6 = &++i;
    cout << "Prefix incrementation returns l-value : " << *ptr6 << endl;

    // KO: The temporary value before incrementation is an r-value
    // int* ptr7 = &i++;

    // KO: The temporary value after expression evaluation is an r-value
    // int* ptr8 = &(3+i);

    // Ok: references are l-values
    int& refK = *k;
    int* ptr9 = &refK;
    cout << "A reference is an l-value : " << *ptr9 << endl;

    // Rule: references (called l-value references in modern C++)
    // can only be bound to l-values
    int& refI = i;
    cout << "Reference bound to an l-value : " << refI << endl;

    // KO: return of fn is an r-value
    // compiler says; "error: invalid initialization of non-const
    // reference of type ‘int&’ from an rvalue of type ‘int’"
    // int& refRet = fn();

    // Rule: references to const can be bound to r-values.
    // The life of the r-value is then extended 
    // for as long as the reference exists
    const int& refRet = fn();
    cout << "Reference to const bound to an r-value : " << refRet << endl;
    // Note: references to non-const can only be bound to mutable l-values.
    // References to const can be bound to l-values as well as r-values.

    // Rule: r-value references can be bound only to r-values, i.e. temporaries
    int &&rvrefN = 7;
    cout << "R-value reference to literal constant : " << rvrefN << endl;

    // KO: i is an l-value
    // compiler says "error: cannot bind ‘int’ lvalue to ‘int&&’"
    // int &&rvrefI = i;

    int &&rvrefRet = fn();
    cout << "R-value reference to return value: " << rvrefRet << endl;

    // Using the overloaded function check(), one can determine if
    // a reference variablerefers to a terporary object (r-value) or to a 
    // permanent object (constant or mutable l-value)
    cout << "Literal constant               : ";
    check(6);
    cout << "Function return value          : ";
    check(fn());
    cout << "Postfix increment return value : ";
    check(i++);
    cout << "Prefix increment return value  : ";
    check(++i);
    cout << "Variable                       : ";
    check(j);
    cout << "Reference                      : ";
    check(refK);

    // Application : move constructor and move assignment operator
    ArrayWrapper a1(4);
    for(size_t i=0; i < 4; ++i)
        a1.at(i) = i;
    cout <<"array a1 :" << endl;
    a1.print();
    
    // Classic C++03 syntax, causes call to copy constructor
    ArrayWrapper a2 = a1;
    cout <<"array a2 (copied from a1) :" << endl;
    a2.print();
    
    // We transform a1 to an r-value, thus forcing call to move constructor
    ArrayWrapper a3  = std::move(a1);
    cout << "array a3 (moved from a1) :" << endl;
    a3.print();
    // Since we have plundered object a1, the following causes a segfault:
    // a1.print();
    
    // Temporary object returned from a factory function :
    // here the compiler optimizes to avoid copy and move constructors
    ArrayWrapper a4  = makeArray(4, 7);
    cout <<"array a4 (gotten from factory function) :" << endl;
    a4.print();
    // Note: the reason is the returned object is local to the factory function

    // Question: Should I return an explicit r-value reference from a function ?
    // Answer: It definitely avoids a copy - see getInt vs getRvalueInt - 
    // but 1. If the object is local to the function, the compiler optimizes 
    // the copy anyway and 2. If not, there is a risk of a dangling reference.
    // The legitimate use cases are very rare.

    // Different addresses: a copy was performed
    printAddress( getInt() ); 
    printAddress( x );

    // Same address, because we explicitely returned an r-value
    printAddress( getRvalueInt() ); 
    printAddress( x );

}



