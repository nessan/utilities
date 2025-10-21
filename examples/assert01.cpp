/// @brief Trigger an verification error error and see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/assert.h"

int subtract01(int x, int y)
{
    assert(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int main()
{
    return subtract01(10, 11);
}