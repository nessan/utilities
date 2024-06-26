/// @brief Some utility functions for that can be used to read from streams.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <iostream>
#include <string>

namespace utilities {

/// @brief  Reads a 'line' from a stream.
/// @note   Unlike the @c std::getline(...) this version strips trailing comments (starting with a '#' by default).
///         It also completely ignores blank lines and assumes that lines ending with a '\' continue to the next.
/// @param  s The input stream to read from.
/// @param  line We overwrite this with the content we read from the input stream.
/// @param  comment_begin We ignore/strip out comments that start with this character ("#" by default).
/// @return The number of characters read.
/// @todo   In the code below we should escape comment delimiters that are special (e.g. '*')
static std::size_t
read_line(std::istream& s, std::string& line, std::string_view comment_begin = "#")
{
    // Lambda that trims a string in-place from leading/trailing space characters.
    auto trim = [](std::string& str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return !std::isspace(ch); }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) { return !std::isspace(ch); }).base(), str.end());
    };

    // Zap any existing content in the output parameter.
    line.clear();

    // We keep trying to read content until we hit the end-of-file (there are other early exits below).
    while (!s.eof()) {

        // Standard read-a-line from a stream.
        std::getline(s, line);

        // Remove any trailing comment (starts with the comment_begin).
        if (!comment_begin.empty()) {
            auto begin = line.find_first_of(comment_begin);
            if (begin != std::string::npos) line.erase(begin, line.length() - begin);
        }

        // Remove leading & trailing blanks.
        trim(line);

        // If after all that  we haven't captured anything skip on to try the next line.
        std::size_t n = line.length();
        if (n == 0) continue;

        // Handle continuation lines (we know that n != 0).
        if (line[n - 1] == '\\') {
            // Replace the continuation character with a space
            line[n - 1] = ' ';

            // Trim--we'll add one space back if there is a non-empty continuation.
            trim(line);

            // Recurse ...
            std::string continuation;
            read_line(s, continuation, comment_begin);
            if (!continuation.empty()) {
                line += " ";
                line += continuation;
            }
        }

        // If we have captured some content in `line` we can go home.
        if (!line.empty()) break;
    }

    return line.length();
}

/// @brief Reads one 'line' from a stream and returns that as a new std::string.
/// @note Unlike the @c std::getline(...) this version strips trailing comments (starting with a '#' by default).
/// It also completely ignores blank lines and assumes that lines ending with a '\' continue to the next.
/// @param s The input stream to read from.
/// @param comment_begin We ignore/strip out comments that start with this character ("#" by default)
/// @todo In the code below we should escape comment delimiters that are special (e.g. '*')
inline std::string
read_line(std::istream& s, std::string_view comment_begin = "#")
{
    std::string retval;
    read_line(s, retval, comment_begin);
    return retval;
}

/// @brief Rewind an input stream to the start
inline std::istream&
rewind(std::istream& is)
{
    is.clear();
    is.seekg(0, std::istream::beg);
    return is;
}

/// @brief Counts the number of lines in the input stream.
/// @note If the comment start string is empty we use @c std::getline to read the lines, otherwise we use our own
/// version @c read_line so comment lines are excluded etc.
inline std::size_t
line_count(std::istream& is, std::string_view comment_begin = "#")
{
    std::size_t retval = 0;
    std::string line;
    if (comment_begin.empty()) {
        while (std::getline(is, line)) ++retval;
    }
    else {
        while (read_line(is, line, comment_begin)) ++retval;
    }
    rewind(is);
    return retval;
}

} // namespace utilities
