/// Simple test of the stopwatch class.

#include <utilities/namespace.h>

#include <print>
#include <thread>

int
main()
{
    using namespace std::literals;

    auto sleep_duration = 1s;
    std::println("Sleeping for {}s ...", utilities::to_seconds(sleep_duration));
    const stopwatch sw;
    std::this_thread::sleep_for(sleep_duration);
    std::println("Elapsed time: {}", sw);

    /// Check that the fmt library can handle a stopwatch
    std::println("And here is what 'std::print' puts out for elapsed time: {}", sw);
}
