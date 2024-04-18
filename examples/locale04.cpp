/// @brief Checking support for std::locale in the standard libraries -- GCC is notoriously poor!
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"
#include <locale>

int
main(int argc, char* argv[])
{
    // Must have one arg -- the name of the locale we wish to use.
    if (argc != 2) {
        std::print("Missing argument -- usage: '{} locale-name (e.g. 'en_US')\n", argv[0]);
        exit(1);
    }

    int x = 1'000'000;
    std::cout << "Printing 1000000 to std::cout yields: " << x << '\n';

    try {
        std::locale loc(argv[1]);
        std::cout.imbue(loc);
        std::print("Setting the locale to '{}'\n", loc.name());
        std::cout << "Printing 1000000 to std::cout now yields: " << x << '\n';
    }
    catch (std::runtime_error& e) {
        std::print("Failed to set the locale to: '{}'\n", argv[1]);
        std::print("Exception raised: '{}'\n", e.what());
    }
    return 0;
}