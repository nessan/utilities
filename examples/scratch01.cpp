#include <utilities/format.h>
#include <iostream>

struct Whatever {
    std::string to_string() const { return "Whatever!"; }
};

int main()
{
    Whatever w;
    std::cout << std::format("Struct with a to_string() method: '{}'\n", w);
    return 0;
}