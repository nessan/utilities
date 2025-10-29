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

    std::print("Turning commas on for std::cout ...\n");
    RUN(utilities::imbue_stream_with_commas(std::cout));
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    std::print("Also turning commas on for the global locale ...\n");
    RUN(utilities::imbue_global_with_commas(true));
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    std::print("Turning commas off ...\n");
    RUN(utilities::imbue_stream_with_commas(std::cout, false));
    RUN(utilities::imbue_global_with_commas(false));
    std::cout << "Sending " << x_str << " to std::cout yields: " << x_val << '\n';
    std::print("std::print({}) with :L specifier yields: {:L}\n", x_str, x_val);
    std::print("std::print({}) w/o  :L specifier yields: {}\n\n", x_str, x_val);

    return 0;
}