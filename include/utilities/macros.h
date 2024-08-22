/// @brief Some useful common macros.
/// @link  https://nessan.github.io/utilities/
/// NOTE:  MSVC's traditional preprocessor barfs on these macros but their newer cross platform compatible one is fine.
///        To use the upgrade, add the '/Zc:preprocessor' flag at compile time.
///        Our CMake module `compiler_init` does that for you.
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

/// @brief Invoke the pre-processor stringizing operator but fully expanding any macro argument first!
#define STRINGIZE(s)      STRINGIZE_IMPL(s)
#define STRINGIZE_IMPL(s) #s

/// @brief Concatenate two symbols but making sure to fully expand those symbols if they happen to be macros themselves.
#define CONCAT(a, b)      CONCAT_IMPL(a, b)
#define CONCAT_IMPL(a, b) a##b

/// @brief Turn a semantic version into a string (we overload to handle 1, 2, or 3 argument versions)
#define VERSION_STRING(...) OVERLOAD(VERSION_STRING, __VA_ARGS__)

// The actual one, two, and three argument versions of that macro
// NOTE: In C++ contiguous strings are concatenated so "2" "." "3" is the same as "2.3"
#define VERSION_STRING1(major)               STRINGIZE(major)
#define VERSION_STRING2(major, minor)        STRINGIZE(major) "." STRINGIZE(minor)
#define VERSION_STRING3(major, minor, patch) STRINGIZE(major) "." STRINGIZE(minor) "." STRINGIZE(patch)

/// @brief RUN(code); prints the line of code to the console and then executes it.
/// @note This is a an overloaded macro that is used in some test/example codes to show what specific code is getting
/// executed, optionally followed by one or two results from that call.
#define RUN(...) OVERLOAD(RUN, __VA_ARGS__)

// The one, two,and three argument versions of RUN
#define RUN1(code)                         \
    std::cout << "[CODE]   " << #code << "\n"; \
    code

#define RUN2(code, val) \
    RUN1(code);         \
    std::cout << "[RESULT] " << #val << ": " << val << '\n'

#define RUN3(code, val1, val2) \
    RUN1(code);                \
    std::cout << "[RESULT] " << #val1 << ": " << val1 << " and " << #val2 << ": " << val2 << '\n'

/// @brief   Preprocessor trickery to allow for macros that can be overloaded by the number of passed arguments.
/// @example #define FOO(...) OVERLOAD(FOO, __VA_ARGS__) will make 'FOO' overloaded on the number of passed args.
////         So FOO() will call the zero arg version FOO0(), FOO(a) will call FOO1(a), FOO(a,b) will call FOO2(a,b) etc.
/// @note    You supply whichever specific FOO0(), FOO1(a), FOO2(a,b), FOO2(a,b,c), implementations that make sense,
///          but the consumer can just call FOO(...) and automatically get the correct one.
#define OVERLOAD(macro, ...) CONCAT(macro, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

/// @brief ARG_COUNT(...) expands to the count of its arguments e.g. ARG_COUNT(x,y,z) will expands to 3.
#define ARG_COUNT(...) ARG_COUNT_IMPL(__VA_ARGS__ __VA_OPT__(, ) 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, count, ...) count

/// @brief Compiler name & version as a string -- occasionally useful to have around to annotate test output etc.
/// @note Could add more compilers from e.g. https://github.com/cpredef/predef/blob/master/Compilers.md)
#if defined(_MSC_VER)
    #define COMPILER_NAME "MSC " STRINGIZE(_MSC_FULL_VER)
#elif defined(__clang__)
    #define COMPILER_NAME "clang " VERSION_STRING(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__)
    #define COMPILER_NAME "gcc " VERSION_STRING(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
    #define COMPILER_NAME "Unidentified Compiler"
#endif
