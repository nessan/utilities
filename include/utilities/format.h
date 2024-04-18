/// @brief Connect any class with a `std::string to_string() const` method to `std::format`.
/// @note  We also include a work-around support for formatting ranges (shouldn't be needed in C++23).
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>
#include <format>
#include <cassert>

/// @brief A concept that matches any type that has an accessible `std::string to_string() const` method.
template<typename T>
concept has_to_string_method = requires(const T& x) {
    { x.to_string() } -> std::convertible_to<std::string>;
};

/// @brief Connect any type that has an accessible `std::string to_string() const` method to `std::format
template<has_to_string_method T>
struct std::formatter<T> {
    template<class FormatContext>
    auto format(const T& rhs, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}", rhs.to_string());
    }

    constexpr auto parse(const std::format_parse_context& ctx)
    {
        // Throw an error for anything that is not default formatted.
        auto it = ctx.begin();
        assert(it == ctx.end() || *it == '}');
        return it;
    }
};

// --------------------------------------------------------------------------------------------------------------------
// Work-around adds a std::formatter for std::ranges (explicitly avoiding strings which already have a formatter).
// This will go away once std::format properly supports std:ranges.
// Based On: https://github.com/Apress/beginning-cpp23/Workarounds/format_ranges_workaround.h with fixes.
// --------------------------------------------------------------------------------------------------------------------
#ifndef __cpp_lib_format_ranges

#include <ranges>
#include <utility>

namespace utilities {
/// @brief A concept that captures an associative container type.
template<typename T>
concept is_dictionary = requires { typename T::key_type; };

/// @brief A concept that captures an associative container type.
template<typename T>
concept is_char = std::same_as<T, char> || std::same_as<T, wchar_t>;

/// @brief A concept that roughly captures any string type.
/// @note  std::format already handles strings well so we want to NOT capture them below.
template <typename T>
	concept is_string =                                     // A "string" for our purposes
		is_char<std::ranges::range_value_t<T>>			    // is a range of characters
			&& (!requires { typename T::value_type; }		// that is either not a container (has no value_type member)
				|| requires { typename T::traits_type; });	// or a std::basic_string<> (has a traits_type member)
} // namespace utilities

/// @brief A formatter for a std::range (but deliberately excluding strings which already handled by std::format).
/// @note  By default arrays are printed [a0, a1, a3, ...]
/// @note  You can suppress the '[' & ']' delimiters by adding a 'n' to the format spec.
template<std::ranges::input_range T>
    requires(!utilities::is_string<T>)
struct std::formatter<T> : public std::formatter<std::ranges::range_value_t<T>> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        auto pos = ctx.begin();
        while (pos != ctx.end() && *pos != '}') {
            if (*pos == ':') {
                ctx.advance_to(++pos);
                return std::formatter<std::ranges::range_value_t<T>>::parse(ctx);
            }
            else if (*pos == 'n') {
                m_surround = false;
            }
            ++pos;
        }
        return pos;
    }

    template<class FormatContext>
    auto format(const T& range, FormatContext& ctx) const
    {
        auto pos = ctx.out();
        if (m_surround) {
            *pos++ = '[';
            ctx.advance_to(pos);
        }
        bool sep = false;
        for (auto&& value : range) {
            if (std::exchange(sep, true)) {
                *pos++ = ',';
                *pos++ = ' ';
            }
            pos = std::formatter<std::ranges::range_value_t<T>>::format(value, ctx);
            ctx.advance_to(pos);
        }
        if (m_surround) *pos++ = ']';
        return pos;
    }

    bool m_surround = true;
};

#endif // End of #ifndef __cpp_lib_format_ranges block.