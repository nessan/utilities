#include <utilities/macros.h>
#include <format>
#include <iostream>
#include <iterator>
#include <string>

int main()
{
    RUN(std::string s1);
    RUN(double x = 123456789.123456789);
    RUN(std::format_to(std::back_inserter(s1), "x = {:12.5f}", x), s1);
}