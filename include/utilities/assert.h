/// @brief Assertion macros that check a boolean condition or equality of two values.
/// @link  https://nessan.github.io/utilities
///
/// These macros are replacements for the standard `assert` macro that improve on it in various ways.
/// In particular, you can add a message explaining why a check failed.
///
/// Assuming `utilities::exit_on_failure` is `true` (the default), the assertion macros will exit the program with a
/// non-zero status code if an assertion fails.
///
/// The `always_assert` and `always_assert_eq` macros are always enabled.
///
/// The `assert` and `assert_eq` macros are no-ops if the `NDEBUG` flag is set at compile time.
/// This works like the `NDEBUG` flag for the "standard" `assert` macro.
///
/// The `debug_assert` and `assert_eq` macros are no-ops if the `DEBUG` flag is **not** set at compile time.
///
/// SPDX-FileCopyrightText:  2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <format>
#include <print>
#include <string>
#include <source_location>

#ifdef assert
    #undef assert
#endif
#ifdef assert_eq
    #undef assert_eq
#endif
#ifdef debug_assert
    #undef debug_assert
#endif
#ifdef debug_assert_eq
    #undef debug_assert_eq
#endif
#ifdef always_assert
    #undef always_assert
#endif
#ifdef always_assert_eq
    #undef always_assert_eq
#endif

// @brief The `always_assert`macro is always enabled.
/// The first argument is the condition to check, the (optional) rest are passed to `std::format`.
#define always_assert(expr, ...)                                                                    \
    if (!(expr)) [[unlikely]] {                                                                     \
        utilities::assert_failed(#expr, std::source_location::current() __VA_OPT__(, __VA_ARGS__)); \
    }

/// @brief The `always_assert_eq` macro is always enabled.
/// The first two arguments are checked for equality, the (optional) rest are passed to `std::format`.
#define always_assert_eq(a, b, ...)                                                                               \
    if (!((a) == (b))) [[unlikely]] {                                                                             \
        utilities::assert_eq_failed(#a, #b, (a), (b), std::source_location::current() __VA_OPT__(, __VA_ARGS__)); \
    }

/// @brief The `debug_assert` & debug_assert_eq macros expand to a no-op **unless** the `DEBUG` flag is set.
#ifdef DEBUG
    #define debug_assert(expr, ...)    always_assert(expr __VA_OPT__(, __VA_ARGS__))
    #define debug_assert_eq(a, b, ...) always_assert_eq(a, b __VA_OPT__(, __VA_ARGS__))
#else
    #define debug_assert(expr, ...)    void(0)
    #define debug_assert_eq(a, b, ...) void(0)
#endif

/// @brief The `assert` & assert_eq macros expand to a no-op **only** when the `NDEBUG` flag is set.
#ifdef NDEBUG
    #define assert(expr, ...)    void(0)
    #define assert_eq(a, b, ...) void(0)
#else
    #define assert(expr, ...)    always_assert(expr __VA_OPT__(, __VA_ARGS__))
    #define assert_eq(a, b, ...) always_assert_eq(a, b __VA_OPT__(, __VA_ARGS__))
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Functions to handle assertion failures ...
// ---------------------------------------------------------------------------------------------------------------------
namespace utilities {

/// @brief Whether to exit the program on failure (default is `true`).
static constexpr auto exit_on_failure = true;

/// @brief Given a path like `/home/jj/dev/project/src/foo.cpp` this returns its "basename" `foo.cpp`
constexpr std::string
basename(std::string_view path)
{
    const auto pos = path.find_last_of("/\\\\");
    if (pos == std::string_view::npos) { return std::string(path); }
    return std::string(path.substr(pos + 1));
}

/// @brief Prints an error message with source code location information and optionally exits the program.
/// This handles simple boolean assert `assert(expr, ...)` failures.
void
assert_failed(std::string_view expr_str, std::source_location loc, std::string_view msg_fmt = "", auto... msg_args)
{
    std::println(stderr);
    std::println(stderr, "FAILED `assert({})` [{}:{}]", expr_str, basename(loc.file_name()), loc.line());
    if (!msg_fmt.empty()) {
        std::vprint_nonunicode(stderr, msg_fmt, std::make_format_args(msg_args...));
        std::println(stderr);
    }
    std::println(stderr);
    if (exit_on_failure) { ::exit(1); }
}

/// @brief Prints an error message with source code location information and optionally exits the program.
/// This handles fails for equality assertions `assert_eq(x, y, ...)`.
void
assert_eq_failed(std::string_view lhs_str, std::string_view rhs_str, auto lhs, auto rhs, std::source_location loc,
                 std::string_view msg_fmt = "", auto... msg_args)
{
    std::println(stderr);
    std::println(stderr, "FAILED `assert_eq({}, {})` [{}:{}]", lhs_str, rhs_str, basename(loc.file_name()), loc.line());
    if (!msg_fmt.empty()) {
        std::vprint_nonunicode(stderr, msg_fmt, std::make_format_args(msg_args...));
        std::println(stderr);
    }
    std::println(stderr, "lhs = {}", lhs);
    std::println(stderr, "rhs = {}", rhs);
    std::println(stderr);
    if (exit_on_failure) { ::exit(1); }
}

} // namespace utilities
