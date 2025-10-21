/// @brief Exercise std::format a little
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/format.h"
#include <print>

// Here is a completely trivial class with a `to_string()` method.
struct Whatever {
    std::string to_string() const { return "Whatever!"; }
};

int
main()
{
    std::print("As int: {},    {},    {},    {}\n", -55, -66, -77, -88);
    std::print("As hex: {:#x}, {:#x}, {:#x}, {:#x}\n", -55, -66, -77, -88);
    std::print("As HEX: {:#X}, {:#X}, {:#X}, {:#X}\n", -55, -66, -77, -88);

    bool a = true;
    bool b = false;
    std::print("Booleans by default:  {} and {}\n", a, b);
    std::print("Booleans as strings:  {:s} and {:s}\n", a, b);
    std::print("Booleans as integers: {:d} and {:d}\n", a, b);

    std::print("Struct with a to_string() method: '{}'\n", Whatever{});

    return 0;
}