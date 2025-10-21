/// Read from a file and tokenize all the lines.

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

    std::size_t n_line = 0;
    std::string line;
    while (utilities::read_line(file, line) != 0) {
        n_line++;
        std::print("Line {:2d}: '{}'\n", n_line, line);
        auto        tokens = utilities::split(line);
        std::size_t n_tokens = tokens.size();
        std::print("{} token(s):\n", n_tokens);
        std::print("{}\n", tokens);
    }

    return 0;
}
