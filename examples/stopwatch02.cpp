/// @brief Another simple test of the stopwatch class
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/format.h"
#include "utilities/print.h"
#include "utilities/stopwatch.h"

#include <thread>

int
main()
{
    using namespace std::literals;

    utilities::stopwatch sw("Overhead stopwatch");
    for (auto sleep_duration = 0ms; sleep_duration <= 2s; sleep_duration += 200ms) {

        sw.click();
        std::this_thread::sleep_for(sleep_duration);
        sw.click();

        double sleep_ms = 1000 * utilities::to_seconds(sleep_duration);
        double actual_ms = 1000 * sw.lap();
        double diff = actual_ms - sleep_ms;
        double percent = sleep_ms != 0 ? 100 * diff / sleep_ms : 0;

        std::print("Requested sleep for {:8.2f}ms, measured wait was {:8.2f}ms => overhead {:.2f}ms ({:.2f}%)\n",
                   sleep_ms, actual_ms, diff, percent);
    }
    std::cout << sw << '\n';

    return 0;
}
