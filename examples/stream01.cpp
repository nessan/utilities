/// Read from a file line by line

#include "utilities/utilities.h"
#include <fstream>

int
main(int argc, char* argv[])
{
    // Must have exactly 1 argument (name of file to read from)
    always_assert_eq(argc, 2, "Usage: '{} <filename>' -- missing filename argument!", argv[0]);

    // Try to open the file
    std::ifstream file{argv[1]};
    always_assert(file, "Failed to open file '{}'", argv[1]);

    // Read all the lines in the file and the print them sans comments ...
    std::string line;
    std::size_t n_lines = 0;
    while (utilities::read_line(file, line) != 0) {
        n_lines++;
        std::print("Line #{:d}: '{}'\n", n_lines, line);
    }

    return 0;
}
