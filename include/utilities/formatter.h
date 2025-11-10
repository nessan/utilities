#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// Specialises [`std::formatter`](https://en.cppreference.com/w/cpp/utility/format/formatter) for types that have an
/// accessible `to_string()` method. <br>
/// See the [Formatter Specialisation](docs/pages/Formatter.md) page for all the details.

#include <concepts>
#include <format>
#include <cassert>

namespace utilities {

/// A concept that matches any type that has an accessible `std::string to_string() const` method.
template<typename T>
concept has_to_string = requires(const T& x) {
    { x.to_string() } -> std::convertible_to<std::string>;
};

} // namespace utilities

/// Specialisation of `std::formatter` for types that have an accessible `std::string to_string() const` method.
///
/// # Example
/// ```
/// struct Whatever {
///     std::string to_string() const { return "Whatever!"; }
/// };
/// Whatever whatever;
/// auto whatever_str = std::format("{}", whatever);
/// confirm_eq(whatever_str, "Whatever!");
/// ```
template<utilities::has_to_string T>
struct std::formatter<T> {
    template<class FormatContext>
    auto format(const T& rhs, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}", rhs.to_string());
    }

    constexpr auto parse(const std::format_parse_context& ctx)
    {
        // Throw an error for anything that is not default formatted.
        // The `to_string()` method might handle various formatting options, but we cannot know that here.
        auto it = ctx.begin();
        assert(it == ctx.end() || *it == '}');
        return it;
    }
};
