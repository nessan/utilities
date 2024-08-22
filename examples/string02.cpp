/// @brief Basic check of some functions in utilities/string.h
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"

int
main()
{
    std::string x;
    while (std::cout << "Text with whitespace to compact (x to exit)? " && std::getline(std::cin, x)) {
        if (x == "X" || x == "x") break;
        std::print("condensed('{}') = '{}'\n", x, utilities::condensed(x));
    }
    return 0;
}