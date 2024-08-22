/// @brief Basic check of the utilities::type(...) function
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"

int
main()
{
    utilities::stopwatch         sw_default;
    utilities::precise_stopwatch sw_precise;
    utilities::steady_stopwatch  sw_steady;
    utilities::system_stopwatch  sw_system;

    std::cout << "Compiler: " << COMPILER_NAME << '\n';
    std::print("sw_default has type '{}'\n", utilities::type(sw_default));
    std::print("sw_precise has type '{}'\n", utilities::type(sw_precise));
    std::print("sw_steady  has type '{}'\n", utilities::type(sw_steady));
    std::print("sw_system  has type '{}'\n", utilities::type(sw_system));

    return 0;
}