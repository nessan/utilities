/// @brief Exercise std::format.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/format.h"
#include "utilities/macros.h"
#include <iterator>
#include <iostream>
#include <string>

int
main()
{
    RUN(std::string s1);
    RUN(double x = 123456789.123456789);
    RUN(std::format_to(std::back_inserter(s1), "x = {:12.5f}", x), s1);

    return 0;
}
