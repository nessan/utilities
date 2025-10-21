/// Exercise the LOG/DBG macros.

#include <cstdint>
#include "utilities/log.h"

template<std::unsigned_integral Block = std::uint64_t>
struct Foo {
    static std::string foo(std::string_view x)
    {
        LOG("Here I am with x = {}", x);
        return "RANDOM STRING";
    }
};

int
main()
{
    Foo<>::foo("42");
    return 0;
}