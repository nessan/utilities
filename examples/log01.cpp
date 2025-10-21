/// Exercise the LOG/DBG macros.

#include "utilities/log.h"

int
add(int x, int y)
{
    DBG("This is a message that only gets printed if the DEBUG flag was set at compile time.");
    LOG("This is a log message that gets printed *unless* the NO_LOGS flag is set at compile time.");
    return y + x;
}

int
subtract(int x, int y)
{
    // This should put out the source location followed by ": ..."
    LOG("x = {}, y = {}", x, y);
    return y - x;
}

int
main()
{
#ifdef DEBUG
    std::cout << "The DEBUG flag is set!\n";
#endif
    // LOG without args just prints the source location.
    LOG();
    return add(10, 11) + subtract(10, 11);
}