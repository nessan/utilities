/// @brief Trigger an assertion error error and see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#undef NDEBUG
#include "utilities/assertion.h"

int subtract(int x, int y)
{
    assertion(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int main()
{
    return subtract(10, 11);
}