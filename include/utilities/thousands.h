#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// Some utility functions that help you print large numbers in a readable format by forcing a stream or locale to
/// insert appropriate commas.
/// See the [Pretty Printing](docs/pages/PrettyPrinting.md) page for all the details.

#include <iostream>
#include <locale>
#include <string>

namespace utilities {

/// A `std::numpunct` facet that puts the commas in the thousand spots so 10000.5 -> 10,000.5
struct commas_facet : std::numpunct<char> {
    using numpunct::numpunct;
    char        do_thousands_sep() const override { return ','; }
    std::string do_grouping() const override { return "\003"; }
};

// We do our own memory management for the commas_facet (set the refs arg to 1).
static commas_facet our_commas_facet{1};

// NOTE: GCC on the Mac (or more accurately GCC's libstdc++) is poor at handling std::locale.
// The only locale it seems to know about is "C" or "POSIX"
// The stdlib for clang (libc++) seems to be much more compliant.
static const std::locale default_locale{"C"};
static const std::locale commas_locale(default_locale, &our_commas_facet);

/// Force a stream to insert commas into large numbers for readability so that 23456.7 is printed as 23,456.7
///
/// @param strm The stream you want to have this property -- defaults to `std::cout`.
/// @param on   You can set this second argument to `false` to return the stream to its default behaviour.
inline void
imbue_stream_with_commas(std::ios_base& strm = std::cout, bool on = true)
{
    on ? strm.imbue(commas_locale) : strm.imbue(default_locale);
}

/// Force the global locale to insert commas into large numbers so that 23456.7 is printed as 23,456.7
///
/// You can set the argument to `false` to return the locale to its default behaviour.
///
/// This function is primarily used to get `std::format` & friends to work correctly with the `{:L}` specifier.
inline void
imbue_global_with_commas(bool on = true)
{
    on ? std::locale::global(commas_locale) : std::locale::global(default_locale);
}

/// Force the global locale & the usual output streams to insert commas into large numbers.
///
/// You can set the argument to `false` to return the locale to its default behaviour.
///
/// # Example
/// ```
/// utilities::pretty_print_thousands();
/// auto x_str = std::format("x = {:L}", 123456789.9);
/// attest_eq(x_str, "x = 123,456,789.9");
/// ```
inline void
pretty_print_thousands(bool on = true)
{
    imbue_global_with_commas(on);
    imbue_stream_with_commas(std::cout, on);
    imbue_stream_with_commas(std::cerr, on);
    imbue_stream_with_commas(std::clog, on);
}

} // namespace utilities
