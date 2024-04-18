/// @brief Returns a string that represents the "type" of an object as seen by the compiler/preprocessor.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <iostream>

namespace utilities {

template <typename T>
constexpr auto type()
{
#ifdef __clang__
    std::string_view m_name = __PRETTY_FUNCTION__;
    std::string_view m_prefix  = "auto utilities::type() [T = ";
    std::string_view m_suffix  = "]";
#elif defined(__GNUC__)
    std::string_view m_name = __PRETTY_FUNCTION__;
    std::string_view m_prefix  = "constexpr auto utilities::type() [with T = ";
    std::string_view m_suffix  = "]";
#elif defined(_MSC_VER)
    std::string_view m_name = __FUNCSIG__;
    std::string_view m_prefix  = "auto __cdecl type_name<";
    std::string_view m_suffix  = ">(void)";
#endif

    m_name.remove_prefix(m_prefix.size());
    m_name.remove_suffix(m_suffix.size());
    return m_name;
}

template <typename T>
constexpr auto type(const T&)
{
    return type<T>();
}

} // namespace utilities
