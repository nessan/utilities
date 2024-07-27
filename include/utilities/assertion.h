/// @brief Two replacements for the standard @c assert(condition) macro that add an informational message.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <format>
#include <iostream>
#include <string>

/// @brief Failed assertions call this macro to exit the program using the @c utilities::exit(...) function.
/// @note  Macro automatically adds the needed source code location information to the passed message payload.
#define exit_with_message(...) utilities::exit(__func__, __FILE__, __LINE__, std::format(__VA_ARGS__))

/// @brief The @c debug_assertion macro expands to a no-op @b unless the @c DEBUG flag is set.
#ifdef DEBUG
    #define debug_assertion(cond, ...) \
        exit_with_message("Statement '{}' is NOT true: {}\n", #cond, std::format(__VA_ARGS__))
#else
    #define debug_assertion(cond, ...) void(0)
#endif

/// @brief The @c assertion macro expands to a no-op @b if the @c NDEBUG flag is set.
/// @note  This is the same behaviour as the like the standard @c assert macro.
#ifdef NDEBUG
    #define assertion(cond, ...) void(0)
#else
    #define assertion(cond, ...) exit_with_message("Statement '{}' is NOT true: {}\n", #cond, std::format(__VA_ARGS__))
#endif

namespace utilities {

/// @brief Given a path like `/home/jj/dev/project/src/foo.cpp` this returns its basename `foo.cpp`
inline std::string
basename(std::string_view path)
{
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    auto i = path.rfind(sep, path.length());
    if (i != std::string::npos) return std::string{path.substr(i + 1, path.length() - i)};
    return "";
}

/// @brief This function prints an error message with source code location information and exits the program.
/// @note  Generally this is only called from some macro which adds the needed location info.
inline void
exit(std::string_view func, std::string_view path, std::size_t line, std::string_view payload = "")
{
    std::cerr << std::format("\n[ERROR] In function '{}' ({}, line {})", func, basename(path), line);
    if (!payload.empty()) std::cerr << ":\n" << payload;
    std::cerr << '\n' << std::endl;
    ::exit(1);
}

} // namespace utilities
