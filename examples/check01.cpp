/// @brief Throw an uncaught utilities::check_error and see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/check.h"

int
subtract(int x, int y)
{
    check(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int
main()
{
    return subtract(10, 11);
}