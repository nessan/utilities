#pragma once
// Some useful commonly used macros.
//
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// Several useful and generally well known macros. <br>
/// See the [Macros](docs/pages/Macros.md) page for all the details.

#include <array>
#include <cstring>
#include <print>
#include <string>
#include <utility>

namespace {

// Remove surrounding quotes from a stringified literal when present.
template<std::size_t N>
[[nodiscard]] inline const char*
stringise_literal(const char (&input)[N])
{
    if (N > 2 && input[0] == '"' && input[N - 2] == '"') {
        thread_local char buffer[N - 1]{};
        for (std::size_t i = 0; i < N - 3; ++i) buffer[i] = input[i + 1];
        buffer[N - 3] = '\0';
        return buffer;
    }
    return input;
}

// Concatenate multiple C-strings into a single thread-local buffer.
template<typename... Parts>
[[nodiscard]] inline const char*
concat_cstr(const Parts*... parts)
{
    thread_local std::array<std::string, 4> buffers{};
    thread_local std::size_t                next = 0;
    auto&                                   buffer = buffers[next++ % buffers.size()];
    buffer.clear();
    std::size_t total = 0;
    auto        accumulate = [&](const char* part) { total += std::strlen(part); };
    (accumulate(parts), ...);
    buffer.reserve(total);
    auto append = [&](const char* part) { buffer.append(part); };
    (append(parts), ...);
    return buffer.c_str();
}

} // anonymous namespace

/// `STRINGISE` invokes the pre-processor stringising operator, fully expanding any macro argument first!
///
/// # Example
/// ```
/// #define FOO1 "abc"
/// #define BAR1 FOO1
/// auto s = STRINGISE(BAR1);
/// confirm(std::strcmp(s, "abc") == 0, "s = {}", s);
/// ```
#define STRINGISE(s)      stringise_literal(STRINGISE_IMPL(s))
#define STRINGISE_IMPL(s) #s

/// `CONCAT` concatenates two symbols making sure to fully expand those symbols if they happen to be macros themselves.
///
/// # Example
/// ```
/// #define FOO2 foo
/// #define BAR2 FOO2
/// int foofoo = 42;
/// auto value = CONCAT(FOO2, BAR2);
/// confirm(value == foofoo);
/// ```
#define CONCAT(a, b)      CONCAT_IMPL(a, b)
#define CONCAT_IMPL(a, b) a##b

/// `VERSION_STRING` creates a semantic version string from 1, 2, or 3 arguments.
///
/// # Example
/// ```
/// #define major 3
/// #define minor 2
/// #define patch 1
/// auto s3 = VERSION_STRING(major, minor, patch);
/// auto s2 = VERSION_STRING(major, minor);
/// auto s1 = VERSION_STRING(major);
/// confirm(std::strcmp(s3, "3.2.1") == 0);
/// confirm(std::strcmp(s2, "3.2") == 0);
/// confirm(std::strcmp(s1, "3") == 0);
/// ```
#define VERSION_STRING(...) OVERLOAD(VERSION_STRING, __VA_ARGS__)

// The actual one, two, and three argument versions of that macro.
// In C++, contiguous strings are concatenated so "2" "." "3" is the same as "2.3"
#define VERSION_STRING1(major)               STRINGISE(major)
#define VERSION_STRING2(major, minor)        concat_cstr(STRINGISE(major), ".", STRINGISE(minor))
#define VERSION_STRING3(major, minor, patch) concat_cstr(STRINGISE(major), ".", STRINGISE(minor), ".", STRINGISE(patch))

/// `RUN` prints the line of code to the console, executes it, then optionally outputs one or two results.
///
/// This is often used in example code to show the specific line of code getting executed, possibly followed by the
/// values of one or two variables that were changed by that code.
///
/// | Version         | Description                                                          |
/// | --------------- | -------------------------------------------------------------------- |
/// | `RUN(code)`     | Prints a stringifed `code` to the screen and then executes it        |
/// | `RUN(code,u)`   | Follows that with a result line showing the value for `u`.           |
/// | `RUN(code,u,v)` | Follows that with a result line showing the values for `u` and `v`.  |
#define RUN(...) OVERLOAD(RUN, __VA_ARGS__)

// Concrete implementations for `RUN` called with 1, 2, or 3 arguments.
#define RUN1(code)                      \
    std::println("[CODE]   {}", #code); \
    code

#define RUN2(code, val) \
    RUN1(code);         \
    std::println("[RESULT] {}: {}", #val, val);

#define RUN3(code, val1, val2) \
    RUN1(code);                \
    std::println("[RESULT] {}: {} and {}: {}", #val1, val1, #val2, val2);

/// `OVERLOAD` makes it appear that a macro can be overloaded by the number of passed arguments.
///
/// `#define FOO(...) OVERLOAD(FOO, __VA_ARGS__)` will make 'FOO' appear overloaded on the number of passed args:
///
/// - `FOO()`    calls the zero arg version which must be called `FOO0`
/// - `FOO(a)`   calls the one arg version `FOO1(a)`, which must be called `FOO1`.
/// - `FOO(a,b)` calls the two arg version `FOO2(a, b)`. That version must be called `FOO2`.
/// - etc. etc.
///
/// You supply whichever specific FOO0(), FOO1(a), FOO2(a,b), FOO2(a,b,c), implementations that make sense, but the
/// macro consumer can just call FOO(...) and automatically get the correct one.
#define OVERLOAD(macro, ...) CONCAT(macro, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

// ARG_COUNT(...) expands to the count of its arguments e.g. ARG_COUNT(x,y,z) will expands to 3.
#define ARG_COUNT(...)                                                 ARG_COUNT_IMPL(__VA_ARGS__ __VA_OPT__(, ) 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, count, ...) count

/// `COMPILER_NAME` is a macro that expands to the current compiler name & version as a string.
///
/// Sometimes useful to annotate test or benchmark output etc.
///
/// **Note:** We can add more [compilers](https://github.com/cpredef/predef/blob/master/Compilers.md) if needed.
#if defined(_MSC_VER)
    #define COMPILER_NAME concat_cstr("MSC ", STRINGISE(_MSC_FULL_VER))
#elif defined(__clang__)
    #define COMPILER_NAME concat_cstr("clang ", VERSION_STRING(__clang_major__, __clang_minor__, __clang_patchlevel__))
#elif defined(__GNUC__)
    #define COMPILER_NAME concat_cstr("gcc ", VERSION_STRING(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__))
#else
    #define COMPILER_NAME "Unidentified Compiler"
#endif
