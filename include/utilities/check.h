/// @brief Three replacements for the standard `assert(condition)` macro that add an informational message.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <exception>
#include <format>
#include <string>

/// @brief Throw a check_error exception defined below. Often not caught, so ends the program.
#define check_failed(...) \
    throw check_error { __func__, __FILE__, __LINE__, std::format(__VA_ARGS__) }

/// @def The `always_check` macro cannot be switched off with compiler flags.
#define always_check(cond, ...) \
    if (!(cond)) check_failed("Statement '{}' is NOT true: {}\n", #cond, std::format(__VA_ARGS__))

/// @def The `debug_check` macro expands to a no-op *unless* the `DEBUG` flag is set.
#ifdef DEBUG
    #define debug_check(cond, ...) always_check(cond, __VA_ARGS__)
#else
    #define debug_check(cond, ...) void(0)
#endif

/// @def The `check`macro expands to a no-op *only if* the `NDEBUG` flag is set.
#ifdef NDEBUG
    #define check(cond, ...) void(0)
#else
    #define check(cond, ...) always_check(cond, __VA_ARGS__)
#endif

/// @brief Our error exceptions capture the location where the exception was created and optionally a payload string.
/// @note  These are created by the `check_failed(...)` macro above that inserts the needed location information.
class check_error : public std::logic_error {
public:
    check_error(std::string_view func, std::string_view path, std::size_t line, std::string_view payload = "") :
        std::logic_error(to_string(func, path, line, payload))
    {
        // Empty body--we just needed to populate the parent `std::logic_error`'s `what()` string.
    }

    /// @brief Returns the whole exception as a string e.g. "[ERROR] 'foobar' foo.cpp line 25: x = 10, y = 11".
    static std::string to_string(std::string_view func, std::string_view path, std::size_t line,
                                 std::string_view payload)
    {
        auto retval = std::format("\n[ERROR] In function '{}' ({}, line {})", func, filename(path), line);
        if (!payload.empty()) {
            retval += ":\n";
            retval += payload;
        }
        return retval;
    }

    /// @brief Reduce a full path to just the filename.
    static std::string filename(std::string_view path)
    {
        char sep = '/';
#ifdef _WIN32
        sep = '\\';
#endif
        auto i = path.rfind(sep, path.length());
        if (i != std::string::npos) return std::string{path.substr(i + 1, path.length() - i)};
        return "";
    }
};
