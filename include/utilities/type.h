#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// Some utility functions that return a string representing an object's "type" as the compiler/preprocessor sees.
/// See the [Type Strings](docs/pages/TypeStrings.md) page for all the details.

#include <string>
#include <iostream>

namespace utilities {

/// Returns a string representing an object's "type" as the compiler/preprocessor sees.
template<typename T>
constexpr auto
type()
{
#ifdef __clang__
    std::string_view m_name = __PRETTY_FUNCTION__;
    std::string_view m_prefix = "auto utilities::type() [T = ";
    std::string_view m_suffix = "]";
#elif defined(__GNUC__)
    std::string_view m_name = __PRETTY_FUNCTION__;
    std::string_view m_prefix = "constexpr auto utilities::type() [with T = ";
    std::string_view m_suffix = "]";
#elif defined(_MSC_VER)
    std::string_view m_name = __FUNCSIG__;
    std::string_view m_prefix = "auto __cdecl utilities::type<";
    std::string_view m_suffix = ">(void)";
#endif

    m_name.remove_prefix(m_prefix.size());
    m_name.remove_suffix(m_suffix.size());
    return m_name;
}

/// Returns a string representing an object's "type" as the compiler/preprocessor sees.
template<typename T>
constexpr auto
type(const T&)
{
    return type<T>();
}

} // namespace utilities
