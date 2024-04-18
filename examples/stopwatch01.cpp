/// @brief Simple test of the stopwatch class.
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/format.h"
#include "utilities/print.h"
#include "utilities/stopwatch.h"

#include <iostream>
#include <thread>

int
main()
{
    using namespace std::literals;

    auto sleep_duration = 1s;
    std::cout << "Sleeping for " << utilities::to_seconds(sleep_duration) << "s ...\n";
    const utilities::stopwatch stopwatch;
    std::this_thread::sleep_for(sleep_duration);
    std::cout << "Elapsed time: " << stopwatch << '\n';

    /// Check that the fmt library can handle a stopwatch
    std::print("And here is what 'std::print' puts out for elapsed time: {}\n", stopwatch);
}
