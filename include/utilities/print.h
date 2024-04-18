/// @brief Work-around support for `std::print` (shouldn't be needed in C++23).
/// @link  https://github.com/Apress/beginning-cpp23/blob/main/Workarounds/print_workaround.h
/// @link  https://nessan.github.io/utilities/
#pragma once

#ifdef __cpp_lib_print
    #include <print>
#else

// clang-format off
#include <format>
#include <iostream>
// clang-format on

namespace std {
template<typename... Args>
void
print(const format_string<Args...> format, Args&&... args)
{
    std::format_to(std::ostreambuf_iterator<char>(std::cout), format, std::forward<Args>(args)...);
}

template<typename... Args>
void
println(const format_string<Args...> format, Args&&... args)
{
    print(format, std::forward<Args>(args)...);
    std::cout << '\n';
}

template<typename... Args>
void
print(const wformat_string<Args...> format, Args&&... args)
{
    std::format_to(std::ostreambuf_iterator<wchar_t>(std::wcout), format, std::forward<Args>(args)...);
}

template<typename... Args>
void
println(const wformat_string<Args...> format, Args&&... args)
{
    print(format, std::forward<Args>(args)...);
    std::wcout << L'\n';
}
} // namespace std

#endif // End of the __cpp_lib_print == false block.
