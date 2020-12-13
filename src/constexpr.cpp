#include "constexpr.h"
#include <cstring> // for std::strlen
#include <cstdio> // for printf
#include <iostream> // for std::cout
#include <type_traits> // for std::is_same
#include <array>
#include <experimental/array>

using namespace std;

/* 

Literal types
-------------
A literal type is one whose layout can be determined at compile time. 
The following are the literal types:
    - void
    - scalar types (bool, int, float, double etc. char, wchar_t etc., pointers)
    - references (int&, int&&, etc.)
    - Arrays of void, scalar types or references (char [])
    - A class that has a trivial destructor, and one or more constexpr constructors 
    that are not move or copy constructors. Additionally, all its non-static data members 
    and base classes must be literal types and not volatile.

Constexpr variables
-------------------
The primary difference between const and constexpr variables is 
that the initialization of a const variable can be deferred until run time. 
A constexpr variable must be initialized at compile time. 

All constexpr variables are const.

    - A variable can be declared with constexpr, when it has 
    a literal type and is initialized. If the initialization is 
    performed by a constructor, the constructor must be declared as constexpr.

    - A reference may be declared as constexpr when both these conditions 
    are met: The referenced object is initialized by a constant expression, 
    and any implicit conversions invoked during initialization are also 
    constant expressions.

Constexpr Functions
-------------------
A constexpr function is one whose return value is computable 
at compile time when consuming code requires it. Consuming code 
requires the return value at compile time to initialize a 
constexpr variable, or to provide a non-type template argument. 
When its arguments are constexpr values, a constexpr function 
produces a compile-time constant. When called with non-constexpr 
arguments, or when its value isn't required at compile time, 
it produces a value at run time like a regular function. 
(This dual behavior saves you from having to write constexpr 
and non-constexpr versions of the same function.)

A constexpr function or constructor is implicitly inline.

The following rules apply to constexpr functions:
    - A constexpr function must accept and return only literal types.
    - A constexpr function can be recursive.
    - A constexpr member function cannot be virtual. 
    - A constructor cannot be defined as constexpr when 
    the enclosing class has any virtual base classes.
    - The body can be defined as = default or = delete.
    - The body can contain no goto statements or try blocks.
    - An explicit specialization of a non-constexpr template 
    can be declared as constexpr
    - An explicit specialization of a constexpr template 
    doesn't also have to be constexpr

*/

// Defines a constexpr max function
template<typename T>
constexpr const T& cx_max(const T& a, const T& b)
{
    return a < b ? b : a;
}

// Defines a constexpr min function
template<typename T>
constexpr const T& cx_min(const T& a, const T& b)
{
    return a < b ? a : b;
}

// Computes the min size from a given integer size and tolerance
constexpr size_t size_min(size_t size, size_t tolerance)
{
    return cx_max(size_t(0), size - tolerance);
}

// Computes bounds of the size range from a given integer size and tolerance
constexpr pair<size_t, size_t> size_range(size_t size, size_t tolerance)
{
    // Pair constructor is a constexpr function
    return pair<size_t, size_t>(size_min(size, tolerance), size + tolerance);
}

// Computes greatest common divisor recursively
constexpr uint gcd(uint a, uint b)
{
    return a ==0 || b==0 ? 0 : (
        a == b ? a : (a < b ? gcd(a, b-a) : gcd(a - b, b))
    );
}

// Recursive integer square root recursive helper
#define MID ((lo + hi + 1) / 2)
constexpr uint64_t sqrt_helper(uint64_t x, uint64_t lo, uint64_t hi)
{
  return lo == hi ? lo : ((x / MID < MID)
      ? sqrt_helper(x, lo, MID - 1) : sqrt_helper(x, MID, hi));
}
#undef MID

// Compute integer square root using binary search
constexpr uint64_t csqrt(uint64_t x)
{
  return sqrt_helper(x, 0, x / 2 + 1);
}


// Constexpr version of strlen 
constexpr size_t cx_strlen(const char* str)
{
    return (str == nullptr || *str == '\0') ? 0 : 1 + cx_strlen(str + 1);
}

// Constexpr version of strcmp for C-string literals of known size
// - does not work with constexpr const char* parameters ('\0'-ending but unknown size)
template<size_t N1, size_t N2>
constexpr int cx_safestrcmp(const char (&s1)[N1], const char (&s2)[N2], size_t K = 0)
{
    return (K >= N1 && K >= N2) ? 0 : ( K >= N1 ? -1 : (K >= N2 ? 1 :
        (s1[K] < s2[K] ? -1 : (s1[K] > s2[K] ? 1 : cx_safestrcmp(s1, s2, K+1) ))
    ));
}

// Constexpr version of strcmp
// Adapted from Ben Deane (github elbeno)
constexpr int cx_strcmp(const char* a, const char* b)
{
    return *a == 0 && *b == 0 ? 0 :
        *a == 0 ? -1 :
        *b == 0 ? 1 :
        *a < *b ? -1 :
        *a > *b ? 1 :
        strcmp(a+1, b+1);
}

// True if given string is present in given list (C-array) of strings
template<size_t N>
constexpr bool is_one_of(const char* x, const char* const (&list)[N], size_t K = 0)
{
    return K >= N ? false : (
        0 == cx_strcmp(x, list[K]) ? true : is_one_of(x, list, K+1)
    );
}

// Create std::array<T, N> from an initializer list of N values
// This allows initializing std::array without counting the rhs values
// example: auto a = make_array(1.41, 2.71, 3.14);
// Note: This is available from <experimental/array> in C++14
// Note: credit: Konrad Rudolph
template <typename... T>
constexpr auto make_array(T&&... values) ->
        std::array<
            typename std::decay<
                typename std::common_type<T...>::type>::type,
            sizeof...(T)> {
    return {std::forward<T>(values)...};
}

// Defines a constexpr abs function
template<typename T>
constexpr T cabs(const T& v)
{
    return v >= 0 ? v : -v;
}

// True if value is close enough to given value
constexpr bool is_almost_equal(double x1, double x0, double eps)
{
    return cabs(x1-x0) <= eps;
}

// True if value is close enough to the values listed in the N-size array
template<size_t N>
constexpr bool is_almost_one_of(double x1, const double (&xarray)[N], double eps, size_t K = 0)
{
    return K >= N ? false : (
        is_almost_equal(x1, xarray[K], eps) ? true : is_almost_one_of(x1, xarray, eps, K+1)
    );
}

// Consexpr-friendly string class
class cx_string
{
private:
    const char* const _str;
    const size_t _sz;
public:
    template<std::size_t N>
    constexpr cx_string(const char(&str)[N]): _str(str), _sz(N - 1) {}
 
    // constexpr functions signal errors by throwing exceptions
    // in C++11, they must do so from the conditional operator ?:
    constexpr char operator[](std::size_t n) const
    {
        return n < _sz ? _str[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return _sz; }
    constexpr const char* const& c_str() const { return _str; }
};

// Main function
void demo_constexpr()
{
    cout << endl << "*************** Constexpr ***********" << endl;

    // Integers only
    constexpr size_t size1 = 13;
    constexpr size_t tolerance = 3;

    // Test that the return of size_min is usable at compile time
    array <int, size_min(size1, tolerance)> a0;
    cout << "Array of min size " << a0.size() << '\n';

    // Test that the return of size_range is usable at compile time
    constexpr auto sizes_min_max = size_range(size1, tolerance);
    array <int, sizes_min_max.second> a00;    
    cout << "Array of max size " << a00.size() << '\n';

    // Compute greatest common divisor of two integers at compile time
    static_assert( 24 == gcd(1440, 168), "invalid gcd");
    static_assert( 1 == gcd(15, 17), "invalid gcd");
    static_assert( 0 == gcd(12, 0), "invalid gcd");
    static_assert( 14 == gcd(14, 14), "invalid gcd");
    
    // Compute greatest common divisor of two integers at runtime
    cout << "GCD(" << 1440 << ',' << 168 << ") = " << gcd(1440, 168) << '\n';
    cout << "GCD(" << 15 << ',' << 17 << ") = " << gcd(15, 17) << '\n';
    cout << "GCD(" << 12 << ',' << 0 << ") = " << gcd(12, 0) << '\n';
    cout << "GCD(" << 14 << ',' << 14 << ") = " << gcd(14, 14) << '\n';

    // Compute square root at compile time
    static_assert( 13 == csqrt(169), "invalid sqrt");
    static_assert( 11 == csqrt(122), "invalid sqrt");
    static_assert( 4 == csqrt(24), "invalid sqrt");

    // Floating point values
    constexpr double xvalues[] = {1.41, 2.71, 3.14};
    constexpr double eps0 = 0.1;
    // Check at compile time if almost equal to constant within given tolerance
    static_assert(is_almost_equal(3.15, xvalues[2], eps0), "Not almost equal");
    // More interesting: Check if almost equal to one of the values in table !
    static_assert(is_almost_one_of(2.70, xvalues, eps0), "Not one of the values");
    static_assert(!is_almost_one_of(2.55, xvalues, eps0), "Not one of the values");


    // Constexpr C-string
    constexpr const char cs1[] = "Hello World!";
    // Constexpr function to determine length of C-string
    // Note: using raw string literal R"delimiter( raw_characters )delimiter"
    cout << R"(cx_strlen("Hello World!") = )" << cx_strlen("Hello World!") << '\n';
    // Construct array and copy C-string into it. 
    // Unfortunately cannot initialize with C-string auto-deducing char type and size.
    array<char, cx_strlen(cs1)> a1;
    std::move(std::begin(cs1), std::end(cs1), a1.begin());
    cout << "Array of size " << a1.size() << ": " << a1.data() << '\n';

    // Create int array like a C-array !
    auto a3 = make_array(2, 3, 5, 7, 11, 13, 17, 19);
    // Check that type is indeed an array<int, 8> !
    static_assert(is_same<decltype(a3), array<int, 8> >::value, "Not same type");
    // Display content
    cout << "Array of size " << a3.size() << ": ";
    for(const auto& elem : a3) cout << elem << ", ";
    cout << '\n';

    // Compare strings at compile time
    constexpr const char* scale[] = {"do", "re", "mi", "fa", "sol", "la", "si"};
    constexpr int do_vs_re = cx_safestrcmp("do", "re");
    constexpr int sol_vs_solo = cx_safestrcmp("sol", "solo");
    constexpr int fa_vs_fa = cx_safestrcmp("fa", "fa");
    cout << R"(safestrcmp("do", "re") = )" << do_vs_re << '\n';
    cout << R"(safestrcmp("sol", "solo") = )" << sol_vs_solo << '\n';
    cout << R"(safestrcmp("fa", "fa") = )" << fa_vs_fa << '\n';
    // constexpr int re_vs_mi = cx_safestrcmp(scale[1], scale[2]);
    // cout << "strcmp(re, mi) = " << re_vs_mi << '\n';
    // Note: Does not compile because scale[i] is decayed 
    // from a const char (&)[N] to a const char*& 
    // so the template cannot deduce N1 and N2

    constexpr int do_vs_sol = cx_strcmp(scale[0], scale[4]);
    cout << "strcmp(do, sol) = " << do_vs_sol << '\n';
    constexpr int sol_vs_si = cx_strcmp(scale[4], scale[5]);
    cout << "strcmp(sol, si) = " << sol_vs_si << '\n';
    // constexpr int sol_vs_null = cx_strcmp(scale[4], 0);
    // Does not compile : 'dereferencing null pointer' ==> OK
    // cout << cx_strcmp("sol", 0) << '\n';
    // Does compile ! And crashes at runtime

    // Search string in a list at compile time
    constexpr bool is_sol_in_scale = is_one_of("sol", scale);
    constexpr bool is_solo_in_scale = is_one_of("solo", scale);
    cout << "Is sol in scale ? " <<  is_sol_in_scale << '\n';
    cout << "Is solo in scale ? " <<  is_solo_in_scale << '\n';

    constexpr auto quote = cx_string("The state of law is equal for all people. "
        "It cannot depend on electoral politics. - Baltasar Garzon");
    cout << "Quote content : " << quote.c_str() << '\n';    
    cout << "Quote size : " << quote.size() << "; verification : " 
        << cx_strlen(quote.c_str()) << '\n';
    // constexpr char at_99 = quote[99];
    // breaks at compile time
    constexpr char at_98 = quote[quote.size()-1];
    cout << "Quote content last character : " << at_98 << '\n';

    
    // Note: In C++14, also in <experimental/array> we have std::experimental::to_array
    // double ca4[] = {1.41, 2.71, 3.14};
    // auto a4 = std::experimental::to_array(ca4);
    // static_assert(is_same<decltype(a4), array<double, 3> >::value, "Not same type");
    
    // Note: in C++17 deduction guide for array creation - no longer need to count rhs
    // std::array a4{3.0, 1.0, 4.0};  // -> std::array<double, 3>

    // Note: in C++20, <experimental> utilities are replaced by std::to_array from header <array>
    // auto a5 = std::to_array<long>({3, 4}); // OK: implicit conversion
    // auto a6 = std::to_array("foo");        // creates std::array<char, 4>{ 'f', 'o', 'o', '\0' }

    // Note: for more constexpr see https://github.com/elbeno/constexpr

    cout << "End of constexpr demo" << endl;
}
