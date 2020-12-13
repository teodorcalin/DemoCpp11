#include <iostream> // for cerr
#include <cstdlib> // for EXIT_SUCCESS
#include <functional> // for std::function
#include <limits>
#include "constexpr.h"
#include "bit_manipulation.h"
#include "scoped_enum.h"
#include "smart_pointers.h"
#include "type_support.h"
#include "range_based_loops.h"
#include "initialization.h"
#include "pitfalls.h"
#include "default_and_deleted_functions.h"
#include "lambdas.h"
#include "casts.h"
#include "rvalues.h"

using namespace std;

typedef function<void()> DemoFunction;

static const DemoFunction demos[] =
{
    demo_constexpr,
    demo_bit_manipulation,
    demo_scoped_enum,
    demo_smart_pointers,
    demo_type_support,
    demo_range_based_loops,
    demo_initialization,
    demo_default_and_deleted_functions,
    demo_lambdas,
    demo_rvalues,
    demo_casts,
    demo_pitfalls
};

int main(void)
{
    try
    {
        char c;
        // Can use for instance for(auto const& f : {demo_rvalues})
        for(auto const& f : demos)
        {
            f();
            cout << "======= Press any to continue, q to end =======" << endl;
            cin.get(c);
            if(c == 'q')
                break;
            cin.ignore(1);
        }

    } catch(...) {

        cerr << "Something bad happened." << endl;
        return EXIT_FAILURE;

    }
    return EXIT_SUCCESS;
}
