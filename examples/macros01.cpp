/// @brief Exercise the OVERLOAD macro in a trivial way.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/macros.h"
#include <iostream>

/// @brief A trivial macro we will overload depending on the number of arguments it is given
#define COUT(...) OVERLOAD(COUT, __VA_ARGS__)

// The specific one, two, and three argument versions of the COUT test macro
#define COUT0(x)       std::cout << "Zero argument version:   " << '\n'
#define COUT1(x)       std::cout << "One argument version:    " << x << '\n'
#define COUT2(x, y)    std::cout << "Two arguments version:   " << x << ", " << y << '\n'
#define COUT3(x, y, z) std::cout << "Three arguments version: " << x << ", " << y << ", " << z << '\n'

int
main()
{
    std::cout << "Compiler: " << COMPILER_NAME << '\n';

    // NOTE: We only ever call COUT but expect to get the correct concrete version.
    COUT();
    COUT("x");
    COUT("x", 2);
    COUT("x", 2, 'z');
    return 0;
}