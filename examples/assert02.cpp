/// @brief Trigger an verification error error and see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/assert.h"

int subtract(int x, int y)
{
    always_assert_eq(x, y, "well that didn't work but x - y = {}", x - y);
    return y - x;
}

int main()
{
    return subtract(10, 11);
}