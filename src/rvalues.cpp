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
    //int* ptr8 = &(3+i);

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

    // Rule: references to const can be bound to r-values
    // The life of the r-value is then extended for as long as the reference exists
    const int& refRet = fn();
    cout << "Reference to const bound to an r-value : " << refRet << endl;

    // Rule: r-value references can be bound only to r-values
    int &&rvrefN = 7;
    cout << "R-value reference to literal constant : " << rvrefN << endl;

    // KO: i is an l-value
    // compiler says "error: cannot bind ‘int’ lvalue to ‘int&&’"
    // int &&rvrefI = i;

    int &&rvrefRet = fn();
    cout << "R-value reference to return value: " << rvrefRet << endl;

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


}



