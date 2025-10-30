/// Print the elements from a range.

#include "utilities/formatter.h"
#include <print>
#include <vector>

int
main()
{
    std::vector v = {1.123123, 2.1235, 3.555555};
    std::print("Unformatted vector: {}\n", v);
    std::print("Formatted vector:   {::3.2f}\n", v);
}