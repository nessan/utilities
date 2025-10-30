/// Checking support for std::locale in the standard libraries -- GCC is notoriously poor!

#include "utilities/utilities.h"
#include <locale>

int
main()
{
    unsigned    x_val = 1'000'000;
    std::string x_str = "1000000";

    std::print("Using the default locale.\n");
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    std::print("Turning commas on for the usual output streams...\n");
    RUN(utilities::pretty_print_thousands());
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    std::print("Turning commas off ...\n");
    RUN(utilities::pretty_print_thousands(false));
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    return 0;
}