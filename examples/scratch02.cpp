#include "utilities/log.h"
#include <cstdint>

template<std::unsigned_integral B = std::uint64_t>
struct Foo {
    static std::string foo(std::string_view x)
    {
        LOG("Here I am with x = {}", x);
        return "ALPHA";
    }
};

int
main()
{
    Foo<>::foo("42");
    return 0;
}