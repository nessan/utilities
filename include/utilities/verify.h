/// @brief Verification macros that check a condition and exit with a message if that check fails.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <format>
#include <iostream>
#include <string>

/// @brief Failed verifications call this macro to exit the program using the @c utilities::exit(...) function.
/// @note  This macro automatically adds the needed source code location information to the exit call.
#define exit_with_message(...) utilities::exit(__func__, __FILE__, __LINE__, std::format(__VA_ARGS__))

/// @brief The @c verify macro checks that a condition is true and if not, exits the program with a message.
/// @param cond The condition that you want checked.
/// @param args The other arguments hold the message to print on fails -- will be formatted by @c std::format
#define verify(cond, ...) \
    if (!(cond)) exit_with_message("Statement '{}' is NOT true: {}\n", #cond, std::format(__VA_ARGS__))

/// @brief If the @c DEBUG flag is set this checks that a condition is true and if not, exits with a message.
/// @param cond The condition that you want checked if the  @c DEBUG flag is set.
/// @param args The other arguments hold the message to print on fails -- will be formatted by @c std::format
/// @note  The @c debug_verify macro is no-op @b unless the @c DEBUG flag is set.
///        Use it for "expensive" checks appropriate in development that should never be "on" in production.
#ifdef DEBUG
    #define debug_verify(cond, ...) verify(cond, __VA_ARGS__)
#else
    #define debug_verify(cond, ...) void(0)
#endif

/// @brief The @c assertion macro expands to a no-op @b if the @c NDEBUG flag is set.
/// @param cond The condition that you want checked if the  @c DEBUG flag is set.
/// @param args The other arguments hold the message to print on fails -- will be formatted by @c std::format
/// @note  This is a replacement for the standard @c assert macro -- same behaviour but with a message.
#ifdef NDEBUG
    #define assertion(cond, ...) void(0)
#else
    #define assertion(cond, ...) verify(cond, __VA_ARGS__)
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
    return i != std::string::npos ? std::string{path.substr(i + 1, path.length() - i)} : "";
}

/// @brief This function prints an error message with source code location information and exits the program.
/// @note  Generally this is only called from some macro which adds the needed location info.
inline void
exit(std::string_view func, std::string_view path, std::size_t line, std::string_view payload = "")
{
    std::cerr << std::format("\n[VERIFY FAILED] In function '{}' ({}, line {})", func, basename(path), line);
    if (!payload.empty()) std::cerr << ":\n" << payload;
    std::cerr << '\n' << std::endl;
    ::exit(1);
}

} // namespace utilities
