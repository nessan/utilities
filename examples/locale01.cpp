/// @brief Checking support for std::locale in the standard libraries -- GCC is notoriously poor!
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"
#include <iostream>
#include <locale>

int
main()
{

    std::locale loc;
    std::print("Compiler {}: Default constructed locale is named '{}'\n", COMPILER_NAME, loc.name());

    int x = 1'000'000;
    std::cout << "Printing 1000000 to std::cout yields: " << x << '\n';

    return 0;
}