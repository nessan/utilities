/// Trigger an verification error error and see what the message looks like.

#include "utilities/confirm.h"

int
subtract(int x, int y)
{
    always_confirm_eq(x, y, "well that didn't work but x - y = {}", x - y);
    return y - x;
}

int
main()
{
    return subtract(10, 11);
}