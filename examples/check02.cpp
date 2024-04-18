/// @brief Throw and catch a utilities::check_error to see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/check.h"
#include <iostream>

int
subtract(int x, int y)
{
    always_check(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int
main()
{
    try {
        subtract(10, 11);
    }
    catch (const std::exception& e) {
        std::cout << "Caught an exception:";
        std::cout << e.what();
    }
    return 0;
}