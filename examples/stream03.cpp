/// @brief Read from a file see how long it takes to account for comment lines vs. ignoring them
/// @copyright Copyright (c) 2024 Nessan Fitzmaurice
#include "utilities/utilities.h"
#include <fstream>

int
main(int argc, char* argv[])
{
    // Must have exactly 1 argument (name of file to read from)
    if (argc != 2) check_failed("Usage: '{} <filename>' -- missing filename argument!", argv[0]);

    // Try to open the file
    std::ifstream file{argv[1]};
    if (!file) check_failed("Failed to open file '{}'", argv[1]);

    // And we're off ...
    utilities::imbue_stream_with_commas(std::cout);
    std::print("File '{}' has {} lines, of which {} are non-comment lines.\n", argv[1], utilities::line_count(file, ""),
               utilities::line_count(file, "#"));

    utilities::stopwatch sw;
    std::size_t          n_trials = 100'000;
    std::cout << "Trials: " << n_trials << '\n';
    std::print("Running {:L} trials where each trial counts ALL the lines              ", n_trials, argv[1]);
    sw.click();
    for (std::size_t i = 0; i < n_trials; ++i) {
        utilities::line_count(file, "");
        utilities::rewind(file);
    }
    sw.click();
    std::print("took: {:8.2f}ms.\n", 1000 * sw.lap());

    std::print("Running {:L} trials where each trial counts the non-comment lines only ", n_trials, argv[1]);
    sw.click();
    for (std::size_t i = 0; i < n_trials; ++i) {
        utilities::line_count(file, "#");
        utilities::rewind(file);
    }
    sw.click();
    std::print("took: {:8.2f}ms.\n", 1000 * sw.lap());

    return 0;
}
