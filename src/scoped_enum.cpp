#include <iostream> // for std::cout
#include <type_traits> // for std::underlying_type
#include <unordered_map>
#include "scoped_enum.h"

using namespace std;

// C++03 enums
// Unscoped enum; underlying type is compiler dependent (usually int)
enum Day { Monday, Tuesday, Wednesday };

// Compiler error: Wednesday already defined in global scope.
// All constant names in an inscoped enum can have collisions.
// enum AddamsFamily { Gomez, Morticia, Pugsley, Wednesday, Fester };


// C++11 enums
// Unscoped enums still exist; an underlying type can be defined
enum Month : char { Jan, Feb, Mar };
// No collision: constants live inside scope EMonth
enum class EMonth : char { Jan, Feb, Mar };
// Default underlying type is always int; whatever the compiler
enum class EDay { Monday, Tuesday, Wednesday };
// Since it has an underlying type, it can be forward-declared !
enum class EButtonState : bool;

// Problem: Scoped enums do not support operator<<
// (Unscoped enums do not either, but they are implicitely convertible to int)

// Solution1: Define operator<< for each constant
ostream& operator<<(ostream& out, EMonth m)
{
    // create a static container with all the values an their print
    static unordered_map<EMonth, const char*> values = {
        {EMonth::Jan, "January"},
        {EMonth::Feb, "February"},
        {EMonth::Mar, "March"}
    };
    auto it = values.find(m);
    if(it != values.end())
        out << it->second;
    else
        out << "Unknown month";

    return out;
}

// Solution2: Use operator<< defined for the underlying type
ostream& operator<<(ostream& out, EButtonState state)
{
    // Explicitely convert the enum value to its underlying type,
    // which is typically an plain old integral type, so printable
    out << static_cast<underlying_type<EButtonState>::type>(state);
    return out;
}

// Now I define the values of the forward-declared enum
enum class EButtonState : bool { Off, On };

void iterate_over_enum()
{
    // Iteration when the values are consecutive
    // Works for unscoped enums; need to define operator++ for scoped
    enum Note {Start, do_ = Start, re, mi, fa, sol, la, si, End};
    for(int n = Start; n < Note::End; ++n)
    {
        cout << "note " << Note(n) << endl;
    }

    // Iteration over a sequence of values. Need not be consecutive.
    // However requires to maintain a duplicate of the enum list
    constexpr initializer_list<Note> all_Notes =
        { do_, re, mi, fa, sol, la, si};
    for (auto n : all_Notes)
    {
        cout << "note " << n << endl;
    }
}

void demo_scoped_enum()
{
    cout << endl << "*************** Scoped Enum ***********" << endl;

    // C++03 : Two constants coming from different enums
    // can be compared, substracted, assigned to int variables etc.
    // because they are both convertible implicitly to int
    Day d = Monday;
    Month m = Jan;

    // Compiler warning for the comparison, but no compile time error
    if (d == m)
        cout << "Monday == January\n"; // This will be printed out
    else
        cout << "Monday != January\n";

    // Compiler error: invalid conversion from int to Day => OK
    // d = 5;

    // Compiler error: cannot convert Month to Day in assignment => OK
    // d = Mar;

    // No compiler warning: convertible to int => KO
    int day_nb = Monday + 3;
    cout << "Monday + 3 = " << day_nb << endl;

    // Convertible to int : uses operator<< for int
    cout << "Monday = " << Monday << endl;

    // Size of enum is 4 bytes, because underlying type is int;
    // But actually it is even worse: it depends on the compiler
    cout << "Size of Day enum: " << sizeof(Day) << endl;


    // C++11
    // For scoped enums, you need to explicitely write the scope -
    // - No name collision is possible,
    auto day = EDay::Tuesday;
    (void) day;
    // ... or explicitely build from an underlying type
    EMonth month = EMonth(1);
    EMonth bad_month = EMonth(13);
    // Warning: this will not detect a narrowing conversion
    EMonth narrowing = EMonth(67000);

    // No "==" operator is defined for the object type
    // So, in C++ 11, this is an compile time error => OK
    // if (day == month)
    //     cout << "Tuesday == February\n";
    // else
    //     cout << "Tuesday != February\n";

    // For unscoped enums, you can write the scope (compatible
    // with scoped enum) but also omit it (backward compatible)
    Month m2 = Month::Feb;
    m2 = Mar;
    (void) m2;

    // No operator+ : compile error => OK
    // day_nb = EDay::Monday + 3;
    // If need be, convert it explicively to int
    day_nb = static_cast<int>(EDay::Monday) + 3;

    // Size of enum is 1 byte : the size of the underlying type
    cout << "Size of Month unscoped enum: " << sizeof(Month) << endl;
    cout << "Size of EButtonState scoped enum: " << sizeof(EButtonState) << endl;

    // No operator <<; compile error => OK
    // cout << "Day is: " << day << endl;

    // Operator << defined => compiles and runs OK
    cout << "Month 1 is: " << month << endl;
    cout << "Month 13 is: " << bad_month << endl;
    cout << "Month 67000 is converted to "
         << static_cast<int>(narrowing) << endl;

    // Operator << defined => compiles and runs OK
    cout << "Button state is: " << EButtonState::On << endl;

    // Iterate over all defined values of an enum
    iterate_over_enum();

    cout << "End of Scoped Enum demo" << endl;
}
