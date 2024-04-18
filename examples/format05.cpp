/// @brief Print the elements from a range.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/format.h"
#include "utilities/print.h"
#include <vector>

int
main()
{
    std::vector v = {1.123123, 2.1235, 3.555555};
    std::print("Unformatted vector: {}\n", v);
    std::print("Formatted vector:   {::3.2f}\n", v);
}