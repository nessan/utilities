/// Trigger an verification error error and see what the message looks like.

#include "utilities/confirm.h"

int
subtract01(int x, int y)
{
    confirm(x == y, "x = {}, y = {}", x, y);
    return y - x;
}

int
main()
{
    return subtract01(10, 11);
}