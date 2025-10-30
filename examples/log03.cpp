/// Exercise the LOG/DBG macros.

#include "utilities/log.h"

template<typename T>
auto
fun(T x)
{
    LOG("value of x = {}", x);
    return x;
}

int
main(int, char*[])
{
    LOG("Hello world!");
    fun("Hello C++20!");
}