/// @brief Basic check of some functions in utilities/string.h
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"

int
main()
{
    std::string str;
    while (std::cout << "Text (x to exit)? " && std::getline(std::cin, str)) {
        if (str == "X" || str == "x") break;
        std::print("upper_cased('{}')  = '{}'\n", str, utilities::upper_cased(str));
        std::print("standardized('{}') = '{}'\n", str, utilities::standardized(str));
    }
    return 0;
}