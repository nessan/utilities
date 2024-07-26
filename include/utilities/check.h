/// @brief `Two replacements for the standard `assert(condition)` macro that add an informational message.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <exception>
#include <format>
#include <iostream>
#include <string>

/// @brief Any failed checks will call this macro to exit the program via the @c utilities::exit(...) method.
/// @note  This macro automatically adds the needed location information to the passed payload.
#define check_failed(...) utilities::exit(__func__, __FILE__, __LINE__, std::format(__VA_ARGS__))

/// @def The `always_check` macro cannot be switched off with compiler flags.
#define always_check(cond, ...) \
    if (!(cond)) check_failed("Statement '{}' is NOT true: {}\n", #cond, std::format(__VA_ARGS__))

/// @def The `debug_check` macro expands to a no-op *unless* the `DEBUG` flag is set.
#ifdef DEBUG
    #define debug_check(cond, ...) always_check(cond, __VA_ARGS__)
#else
    #define debug_check(cond, ...) void(0)
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
