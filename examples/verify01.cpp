/// @brief Trigger an verification error error and see what the message looks like.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/verify.h"

int subtract(int x, int y)
{
    verify(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int main()
{
    return subtract(10, 11);
}