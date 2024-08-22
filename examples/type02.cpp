/// @brief What do the various compilers do with default template parameters?
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"

// Trivial template with a default type
template<typename T = double>
struct trivial {};

int
main()
{
    std::print("Compiler: {}\n", COMPILER_NAME);
    std::print("trivial<int>:    '{}'\n", utilities::type<trivial<int>>());
    std::print("trivial<float>:  '{}'\n", utilities::type<trivial<float>>());
    std::print("trivial<double>: '{}'\n", utilities::type<trivial<double>>());

    // With gcc you can get different output if you pop the next line to the top
    std::print("trivial<>:       '{}'\n", utilities::type<trivial<>>());

    return 0;
}
