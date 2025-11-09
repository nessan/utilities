#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// Assertion macros that are replacements for the [standard attest](https://en.cppreference.com/w/cpp/error/attest)
/// macro, and which improve on it in various ways. <br>
/// See the [Attestations](docs/pages/Attestations.md) page for all the details.

#include <format>
#include <print>
#include <string>
#include <source_location>

// Avoid conflicts with other attest macros -- in particular, the ones defined in the standard library.
#ifdef attest
    #undef attest
#endif
#ifdef attest_eq
    #undef attest_eq
#endif
#ifdef debug_attest
    #undef debug_attest
#endif
#ifdef debug_attest_eq
    #undef debug_attest_eq
#endif
#ifdef always_attest
    #undef always_attest
#endif
#ifdef always_attest_eq
    #undef always_attest_eq
#endif

/// An attestion macro that checks a boolean condition `cond`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the condition
/// that failed.
///
/// The first argument is the condition to check, the (optional) rest are passed to `std::format` to form a custom
/// extra message that is printed if the condition fails.
#define always_attest(cond, ...) \
    if (!(cond)) [[unlikely]] { failed(#cond, std::source_location::current() __VA_OPT__(, __VA_ARGS__)); }

/// An attestion macro that checks the equality of two values `lhs` and `rhs`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the values that
/// failed the equality check.
///
/// The first two arguments are the values to check for equality, the (optional) rest are passed to `std::format` to
/// form a custom extra message that is printed if the values are not equal.
#define always_attest_eq(lhs, rhs, ...)                                                                 \
    if (!((lhs) == (rhs))) [[unlikely]] {                                                               \
        failed_eq(#lhs, #rhs, (lhs), (rhs), std::source_location::current() __VA_OPT__(, __VA_ARGS__)); \
    }

/// An attestion macro that checks a boolean condition `cond`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the condition
/// that failed.
///
/// **NOTE:** The `debug_attest` macro expands to a no-op **unless** the `DEBUG` flag is set.
///
/// The first argument is the condition to check, the (optional) rest are passed to `std::format` to form a custom
/// extra message that is printed if the condition fails.
#ifdef DEBUG
    #define debug_attest(cond, ...) always_attest(cond __VA_OPT__(, __VA_ARGS__))
#else
    #define debug_attest(cond, ...) void(0)
#endif

/// An attestion macro that checks the equality of two values `lhs` and `rhs`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the values that
/// failed the equality check.
///
/// **NOTE:** The `debug_attest_eq` macro expands to a no-op **unless** the `DEBUG` flag is set.
///
/// The first two arguments are the values to check for equality, the (optional) rest are passed to `std::format` to
/// form a custom extra message that is printed if the values are not equal.
#ifdef DEBUG
    #define debug_attest_eq(lhs, rhs, ...) always_attest_eq(lhs, rhs __VA_OPT__(, __VA_ARGS__))
#else
    #define debug_attest_eq(a, b, ...) void(0)
#endif

/// An attestion macro that checks a boolean condition `cond`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the condition
/// that failed.
///
/// **NOTE:** The `attest` macro expands to a no-op **only** when the `NDEBUG` flag is set.
///
/// The first argument is the condition to check, the (optional) rest are passed to `std::format` to form a custom
/// extra message that is printed if the condition fails.
#ifdef NDEBUG
    #define attest(cond, ...) void(0)
#else
    #define attest(cond, ...) always_attest(cond __VA_OPT__(, __VA_ARGS__))
#endif

/// An attestion macro that checks the equality of two values `lhs` and `rhs`. <br>
/// On failure, the attestion prints an error message and exits the program with a non-zero status code.
/// The message always includes the source code location of the attestion and a stringified version of the values that
/// failed the equality check.
///
/// **NOTE:** The `attest_eq` macro expands to a no-op **only** when the `NDEBUG` flag is set.
///
/// The first two arguments are the values to check for equality, the (optional) rest are passed to `std::format` to
/// form a custom extra message that is printed if the values are not equal.
#ifdef NDEBUG
    #define attest_eq(lhs, rhs, ...) void(0)
#else
    #define attest_eq(lhs, rhs, ...) always_attest_eq(lhs, rhs __VA_OPT__(, __VA_ARGS__))
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Functions to handle attestion failures in an anonymous namespace
// ---------------------------------------------------------------------------------------------------------------------
namespace {

// Given a path like `/home/jj/dev/project/src/foo.cpp` this returns its "basename" `foo.cpp`
constexpr std::string
basename(std::string_view path)
{
    const auto pos = path.find_last_of("/\\\\");
    if (pos == std::string_view::npos) { return std::string(path); }
    return std::string(path.substr(pos + 1));
}

// Prints an error message with source code location information and optionally exits the program.
// This handles simple boolean attest `attest(cond, ...)` failures.
constexpr void
failed(std::string_view cond_str, std::source_location loc, std::string_view msg_fmt = "", auto... msg_args)
{
    std::println(stderr);
    std::println(stderr, "FAILED `attest({})` [{}:{}]", cond_str, basename(loc.file_name()), loc.line());
    if (!msg_fmt.empty()) {
        std::vprint_nonunicode(stderr, msg_fmt, std::make_format_args(msg_args...));
        std::println(stderr);
    }
    std::println(stderr);
    ::exit(1);
}

/// Prints an error message with source code location information and optionally exits the program.
/// This handles fails for equality attestions `attest_eq(x, y, ...)`.
constexpr void
failed_eq(std::string_view lhs_str, std::string_view rhs_str, auto lhs, auto rhs, std::source_location loc,
          std::string_view msg_fmt = "", auto... msg_args)
{
    std::println(stderr);
    std::println(stderr, "FAILED `attest_eq({}, {})` [{}:{}]", lhs_str, rhs_str, basename(loc.file_name()), loc.line());
    if (!msg_fmt.empty()) {
        std::vprint_nonunicode(stderr, msg_fmt, std::make_format_args(msg_args...));
        std::println(stderr);
    }
    std::println(stderr, "lhs = {}", lhs);
    std::println(stderr, "rhs = {}", rhs);
    std::println(stderr);
    ::exit(1);
}

} // namespace
