#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// A collectionm of utility functions that work on strings.
/// See the [String Functions](docs/pages/StringFunctions.md) page for all the details.

#include <algorithm>
#include <cctype>
#include <charconv>
#include <format>
#include <iterator>
#include <optional>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

namespace utilities {

// --------------------------------------------------------------------------------------------------------------------
// We start with the convert-an-input-string-in-place versions which only work on *non-const* input strings.
// --------------------------------------------------------------------------------------------------------------------

/// Converts a wide character to its upper case equivalent if it is a lowercase letter.
///
/// Faster than the standard C-library `toupper(...)` function but only works for ASCII characters.
///
/// # Example
/// ```
/// attest(utilities::to_upper('a') == 'A');
/// attest(utilities::to_upper('z') == 'Z');
/// attest(utilities::to_upper('A') == 'A');
/// attest(utilities::to_upper('Z') == 'Z');
/// attest(utilities::to_upper('=') == '=');
/// ```
[[nodiscard]] constexpr char32_t
to_upper(char32_t cp) noexcept
{
    return (cp >= 97 && cp <= 122) ? (cp ^ 0b100000) : cp;
}

/// Converts a wide character to its lower case equivalent if it is a uppercase letter.
///
/// Faster than the standard C-library `tolower(...)` function but only works for ASCII characters.
///
/// # Example
/// ```
/// attest(utilities::to_lower('A') == 'a');
/// attest(utilities::to_lower('Z') == 'z');
/// attest(utilities::to_lower('a') == 'a');
/// attest(utilities::to_lower('z') == 'z');
/// attest(utilities::to_lower('=') == '=');
/// ```
[[nodiscard]] constexpr char32_t
to_lower(char32_t cp) noexcept
{
    return (cp >= 65 && cp <= 90) ? (cp | 0b0100000) : cp;
}

/// Converts a string to upper case in-place.
///
/// Uses the `to_upper(...)` function to convert each character to its upper case equivalent if it is a lowercase
/// letter. Any non-ASCII, non-letter characters are left unchanged.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// utilities::upper_case(str);
/// attest_eq(str, "HELLO, WORLD!");
/// ```
inline void
upper_case(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](char ch) {
        return static_cast<char>(to_upper(static_cast<char32_t>(static_cast<unsigned char>(ch))));
    });
}

/// Converts a string to lower case in place.
///
/// Uses the `to_lower(...)` function to convert each character to its lower case equivalent if it is an uppercase
/// letter. Any non-ASCII, non-letter characters are left unchanged.
///
/// # Example
/// ```
/// std::string str = "HELLO, WORLD!";
/// utilities::lower_case(str);
/// attest_eq(str, "hello, world!");
/// ```
inline void
lower_case(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](char ch) {
        return static_cast<char>(to_lower(static_cast<char32_t>(static_cast<unsigned char>(ch))));
    });
}

/// Removes any leading white-space from a string in-place.
///
/// # Example
/// ```
/// std::string str = "  Hello, World!";
/// utilities::trim_left(str);
/// attest_eq(str, "Hello, World!");
/// ```
inline void
trim_left(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return !std::isspace(ch); }));
}

/// Remove any trailing white-space from a string in-place.
///
/// # Example
/// ```
/// std::string str = "Hello, World!  ";
/// utilities::trim_right(str);
/// attest_eq(str, "Hello, World!");
/// ```
inline void
trim_right(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) { return !std::isspace(ch); }).base(), str.end());
}

/// Removes all leading & trailing white-space from a string in-place.
///
/// # Example
/// ```
/// std::string str = "  Hello, World!  ";
/// utilities::trim(str);
/// attest_eq(str, "Hello, World!");
/// ```
inline void
trim(std::string& str)
{
    trim_left(str);
    trim_right(str);
}

/// Replace the first occurrence of a target substring with some other string in-place.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// utilities::replace_left(str, "World", "Universe");
/// attest_eq(str, "Hello, Universe!");
/// ```
inline void
replace_left(std::string& str, std::string_view target, std::string_view replacement)
{
    auto p = str.find(target);
    if (p != std::string::npos) str.replace(p, target.length(), replacement);
}

/// Replace the final occurrence of a target substring with some other string in-place.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// utilities::replace_right(str, "World", "Universe");
/// attest_eq(str, "Hello, Universe!");
/// ```
inline void
replace_right(std::string& str, std::string_view target, std::string_view replacement)
{
    auto p = str.rfind(target);
    if (p != std::string::npos) str.replace(p, target.length(), replacement);
}

/// Replace all occurrences of a target substring with some other string in-place.
///
/// # Example
/// ```
/// std::string str = "Hello, World! Hello, Universe!";
/// utilities::replace(str, "Hello", "Goodbye");
/// attest_eq(str, "Goodbye, World! Goodbye, Universe!");
/// ```
inline void
replace(std::string& str, std::string_view target, std::string_view replacement)
{
    std::size_t p = 0;
    while ((p = str.find(target, p)) != std::string::npos) {
        str.replace(p, target.length(), replacement);
        p += replacement.length();
    }
}

/// Replace all contiguous white space sequences in a string in-place.
///
/// By default they are replaced with a single space character. YUse the `with` argument to change that.
///
/// By default any white space at the beginning and end is removed entirely. Use the `also_trim` argument to change
/// that.
///
/// # Example
/// ```
/// std::string str = "    Hello   World!  ";
/// utilities::replace_space(str);
/// attest_eq(str, "Hello World!");
/// ```
inline void
replace_space(std::string& s, const std::string& with = " ", bool also_trim = true)
{
    if (also_trim) trim(s);
    std::regex ws{R"(\s+)"};
    s = std::regex_replace(s, ws, with);
}

/// Condense contiguous white space sequences in a string in-place.
///
/// By default any white space at the beginning and end is removed entirely. Use the `also_trim` argument to change
/// that.
///
/// # Example
/// ```
/// std::string str = "Hello,   World!  ";
/// utilities::condense(str);
/// attest_eq(str, "Hello, World!");
/// ```
inline void
condense(std::string& s, bool also_trim = true)
{
    replace_space(s, " ", also_trim);
}

/// Erase the first occurrence of a target substring.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// utilities::erase_left(str, "World");
/// attest_eq(str, "Hello, !");
/// ```
inline void
erase_left(std::string& str, std::string_view target)
{
    auto p = str.find(target);
    if (p != std::string::npos) str.erase(p, target.length());
}

/// Erase the last occurrence of a target substring.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// utilities:: erase_right(str, "World");
/// attest_eq(str, "Hello, !");
/// ```
inline void
erase_right(std::string& str, std::string_view target)
{
    auto p = str.rfind(target);
    if (p != std::string::npos) str.erase(p, target.length());
}

/// Erase all occurrences of a target substring.
///
/// # Example
/// ```
/// std::string str = "abcdefghijklmnopqrstuvwxyz";
/// utilities::erase(str, "def");
/// attest_eq(str, "abcghijklmnopqrstuvwxyz");
/// ```
inline void
erase(std::string& str, std::string_view target)
{
    std::size_t p = 0;
    while ((p = str.find(target, p)) != std::string::npos) str.erase(p, target.length());
}

/// Removes "surrounds" from a string in-place so for example: (text) -> text.
///
/// Multiple surrounds also work so `<<<text>>>` -> `text`.
/// The "surrounds" are only removed if they are correctly balanced.
///
/// # Example
/// ```
/// std::string str = "(Hello, World!)";
/// utilities::remove_surrounds(str);
/// attest_eq(str, "Hello, World!");
/// ```
inline void
remove_surrounds(std::string& s)
{
    std::size_t len = s.length();
    while (len > 1) {
        // If the first character is alpha-numeric we are done.
        char first = s[0];
        if (isalnum(first)) return;

        // First character is not alpha-numeric.
        // Grab the last character & check for a match.
        char last = s[len - 1];
        bool match = false;

        // Handle cases [text], {text}, <text>, and (text) and then all others
        switch (first) {
            case '(':
                if (last == ')') match = true;
                break;
            case '[':
                if (last == ']') match = true;
                break;
            case '{':
                if (last == '}') match = true;
                break;
            case '<':
                if (last == '>') match = true;
                break;
            default:
                if (last == first) match = true;
                break;
        }

        if (match) {
            // Shrink the string and continue
            s = s.substr(1, len - 2);
            len -= 2;
        }
        else {
            // No match => no surround so we can exit.
            return;
        }
    }
}

/// "Standardize" a string -- turns "[ hallo   world ]  " or "   Hallo World" into "HALLO WORLD"
///
/// # Example
/// ```
/// std::string str = "[ hallo   world ]  ";
/// utilities::standardize(str);
/// attest_eq(str, "HALLO WORLD");
/// ```
inline void
standardize(std::string& s)
{
    condense(s);
    upper_case(s);
    remove_surrounds(s);
    trim(s);
}

// --------------------------------------------------------------------------------------------------------------------
// Next we have all the counterpart create-a-new-string that is a copy of input-string with the appropriate conversion.
// These happily work on *const* input strings as the inputs are left unaltered.
// --------------------------------------------------------------------------------------------------------------------

/// Returns a new string that is a copy of the input converted to upper case.
///
/// Uses the standard C-library `toupper(...)` function (so will not work for wide character sets).
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto upper_str = utilities::upper_cased(str);
/// attest_eq(upper_str, "HELLO, WORLD!");
/// ```
inline std::string
upper_cased(std::string_view input)
{
    std::string s{input};
    upper_case(s);
    return s;
}

/// Returns a new string that is a copy of the input converted to lower case.
///
/// Uses the standard C-library `tolower(...)` function (so will not work for wide character sets).
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto lower_str = utilities::lower_cased(str);
/// attest_eq(lower_str, "hello, world!");
/// ```
inline std::string
lower_cased(std::string_view input)
{
    std::string s{input};
    lower_case(s);
    return s;
}

/// Returns a new string that is a copy of the input with leading white-space removed.
///
/// # Example
/// ```
/// std::string str = "  Hello, World!";
/// auto trimmed_left_str = utilities::trimmed_left(str);
/// attest_eq(trimmed_left_str, "Hello, World!");
/// ```
inline std::string
trimmed_left(std::string_view input)
{
    std::string s{input};
    trim_left(s);
    return s;
}

/// Returns a new string that is a copy of the input with trailing white-space removed.
///
/// # Example
/// ```
/// std::string str = "Hello, World!  ";
/// auto trimmed_right_str = utilities::trimmed_right(str);
/// attest_eq(trimmed_right_str, "Hello, World!");
/// ```
inline std::string
trimmed_right(std::string_view input)
{
    std::string s{input};
    trim_right(s);
    return s;
}

/// Returns a new string that is a copy of the input with all leading and trailing white-space removed.
///
/// # Example
/// ```
/// std::string str = "  Hello, World!  ";
/// auto trimmed_str = utilities::trimmed(str);
/// attest_eq(trimmed_str, "Hello, World!");
/// ```
inline std::string
trimmed(std::string_view input)
{
    std::string s{input};
    trim(s);
    return s;
}

/// Returns a new string that is a copy of the input with the first occurrence of a target substring replaced.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto replaced_str = utilities::replaced_left(str, "World", "Universe");
/// attest_eq(replaced_str, "Hello, Universe!");
/// ```
inline std::string
replaced_left(std::string_view input, std::string_view target, std::string_view replacement)
{
    std::string s{input};
    replace_left(s, target, replacement);
    return s;
}

/// Returns a new string that is a copy of the input with the final occurrence of a target substring replaced.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto replaced_str = utilities::replaced_right(str, "World", "Universe");
/// attest_eq(replaced_str, "Hello, Universe!");
/// ```
inline std::string
replaced_right(std::string_view input, std::string_view target, std::string_view replacement)
{
    std::string s{input};
    replace_right(s, target, replacement);
    return s;
}

/// Returns a new string that is a copy of the input with all occurrences of a target substring replaced.
///
/// # Example
/// ```
/// std::string str = "Hello World! Hello Universe!";
/// auto replaced_str = utilities::replaced(str, "Hello", "Goodbye");
/// attest_eq(replaced_str, "Goodbye World! Goodbye Universe!");
/// ```
inline std::string
replaced(std::string_view input, std::string_view target, std::string_view replacement)
{
    std::string s{input};
    replace(s, target, replacement);
    return s;
}

/// Returns a new string that is a copy of the input with all contiguous white space sequences replaced.
///
/// - By default they are replaced with a single space character. Use the `with` argument to change that.
/// - By default any white space at the beginning and end is removed entirely. Use the `also_trim` argument to change
/// that.
///
/// # Example
/// ```
/// std::string str = "  Hello    World!  ";
/// auto replaced_space_str = utilities::replaced_space(str);
/// attest_eq(replaced_space_str, "Hello World!");
/// ```
inline std::string
replaced_space(std::string_view input, const std::string& with = " ", bool also_trim = true)
{
    std::string s{input};
    replace_space(s, with, also_trim);
    return s;
}

/// Returns a copy of the input with all contiguous white space sequences replaced with one space.
///
/// By default any white space at the beginning and end is removed entirely. Use the `also_trim` argument to change
/// that.
///
/// # Example
/// ```
/// std::string str = "Hello,   World!  ";
/// auto condensed_str = utilities::condensed(str);
/// attest_eq(condensed_str, "Hello, World!");
/// ```
inline std::string
condensed(std::string_view input, bool also_trim = true)
{
    std::string s{input};
    condense(s, also_trim);
    return s;
}

/// Returns a new string that is a copy of the input with the first occurrence of a target substring erased.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto erased_left_str = utilities::erased_left(str, "World");
/// attest_eq(erased_left_str, "Hello, !");
/// ```
inline std::string
erased_left(std::string_view input, std::string_view target)
{
    std::string s{input};
    erase_left(s, target);
    return s;
}

/// Returns a new string that is a copy of the input with the last occurrence of a target substring erased.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// auto erased_right_str = utilities::erased_right(str, "World");
/// attest_eq(erased_right_str, "Hello, !");
/// ```
inline std::string
erased_right(std::string_view input, std::string_view target)
{
    std::string s{input};
    erase_right(s, target);
    return s;
}

/// Returns a new string that is a copy of the input with the all occurrence of a target substring erased.
///
/// # Example
/// ```
/// std::string str = "abcdefghijklmnopqrstuvwxyz";
/// auto erased_str = utilities::erased(str, "def");
/// attest_eq(erased_str, "abcghijklmnopqrstuvwxyz");
/// ```
inline std::string
erased(std::string_view input, std::string_view target)
{
    std::string s{input};
    erase(s, target);
    return s;
}

/// Returns a new string that is a copy of the input with "surrounds" stripped from it.
///
/// - Multiple surrounds also work so `<<<text>>>` -> `text`.
/// - The "surrounds" are only removed if they are correctly balanced.
///
/// # Example
/// ```
/// std::string str = "(Hello, World!)";
/// auto removed_surrounds_str = utilities::removed_surrounds(str);
/// attest_eq(removed_surrounds_str, "Hello, World!");
/// ```
inline std::string
removed_surrounds(std::string_view input)
{
    std::string s{input};
    remove_surrounds(s);
    return s;
}

/// Returns a "standardized" string that is a copy of the input.
///
/// For example, input "[ hallo world ]" or "   Hallo World" is returned as  "HALLO WORLD"
///
/// # Example
/// ```
/// std::string str = "[ hallo world ]";
/// auto standardized_str = utilities::standardized(str);
/// attest_eq(standardized_str, "HALLO WORLD");
/// ```
inline std::string
standardized(std::string_view input)
{
    std::string s{input};
    standardize(s);
    return s;
}

// --------------------------------------------------------------------------------------------------------------------
// Next some functions that have no 'in-place' versus 'out-of-place' versions.
// --------------------------------------------------------------------------------------------------------------------

/// Check if a string starts with a particular prefix string.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// bool starts_with_hello = utilities::starts_with(str, "Hello");
/// attest_eq(starts_with_hello, true);
/// ```
inline bool
starts_with(std::string_view str, std::string_view prefix)
{
    return str.find(prefix) == 0;
}

/// Check if a string ends with a particular suffix string.
///
/// # Example
/// ```
/// std::string str = "Hello, World!";
/// bool ends_with_world = utilities::ends_with(str, "World!");
/// attest_eq(ends_with_world, true);
/// ```
inline bool
ends_with(std::string_view str, std::string_view suffix)
{
    const auto pos = str.rfind(suffix);
    return (pos != std::string::npos) && (pos == (str.length() - suffix.length()));
}

/// Try to read a value of a particular type from a string.
///
/// Uses `std::from_chars(...)` to retrieve a possible integral type from a string.
///
/// # Example
/// ```
/// std::string str = "123.456";
/// auto x = utilities::possible<double>(str);
/// attest_eq(*x, 123.456);
/// ```
template<typename T>
constexpr std::optional<T>
possible(std::string_view in, const char** next = nullptr)
{
    in.remove_prefix(in.find_first_not_of("+ "));
    T    retval;
    auto ec = std::from_chars(in.cbegin(), in.cend(), retval);
    if (next) *next = ec.ptr;
    if (ec.ec != std::errc{}) return std::nullopt;
    return retval;
}

/// Given input text and delimiters, tokenize the text and then passes the tokens to a function you supply.
///
/// - If the text is in `str` this parameter might be `cbegin(str)`.
/// - If the text is in `str` this parameter might be `cend(str)`.
/// - If the possible delimiters are in `delims` this might be `cbegin(delims)`.
/// - If the possible delimiters are in `delims` this might be `cend(delims)`.
/// - Will be called with a token like this `function(token_begin, token_end)`.
///
/// Credit to [blog article](https://tristanbrindle.com/posts/a-quicker-study-on-tokenising/)
template<std::input_iterator InputIt, std::forward_iterator ForwardIt, typename BinaryFunc>
constexpr void
for_each_token(InputIt ib, InputIt ie, ForwardIt db, ForwardIt de, BinaryFunc function)
{
    while (ib != ie) {
        const auto x = std::find_first_of(ib, ie, db, de); // Find a token in the input text
        function(ib, x);                                   // Call the user supplied function on the token
        if (x == ie) break;                                // Stop if we hit the end of the input text
        ib = std::next(x);                                 // Otherwise go again past that token we just found
    }
}

/// Tokenize a string and put the tokens into the supplied output container.
///
/// - By default we ignore any empty tokens (e.g. two spaces in a row). Use the `skip` argument to change that.
/// - By default tokens are broken on white space, commas, semi-colons, and colons. Use the `delimiters` argument to
/// change that.
/// - You pass in this "STL" container which we fill with the tokens.
///
/// # Example
/// ```
/// std::string str = "Hello, World";
/// std::vector<std::string> tokens;
/// utilities::tokenize(str, tokens);
/// std::vector<std::string> expected = {"Hello", "World"};
/// attest_eq(tokens, expected);
/// ```
template<typename Container_t>
constexpr void
tokenize(std::string_view input, Container_t& output, std::string_view delimiters = "\t,;: ", bool skip = true)
{
    auto ib = cbegin(input);
    auto ie = cend(input);
    auto db = cbegin(delimiters);
    auto de = cend(delimiters);

    for_each_token(ib, ie, db, de, [&output, &skip](auto tb, auto te) {
        if (tb != te || !skip) { output.emplace_back(tb, te); }
    });
}

/// Tokenize a string and return the tokens as a vector of strings.
///
/// - By default tokens are broken on white space, commas, semi-colons, and colons. Use the `delimiters` argument to
/// change that.
/// - By default we ignore any empty tokens (e.g. two spaces in a row). Use the `skip` argument to change that.
///
/// # Example
/// ```
/// std::string str = "Hello, World";
/// auto tokens = utilities::split(str);
/// std::vector<std::string> expected = {"Hello", "World"};
/// attest_eq(tokens, expected);
/// ```
inline std::vector<std::string>
split(std::string_view input, std::string_view delimiters = "\t,;: ", bool skip = true)
{
    std::vector<std::string> output;
    output.reserve(input.size() / 2);
    tokenize(input, output, delimiters, skip);
    return output;
}

///  A version of @c regex_replace(...) where each match in turn is is run through a function you supply.
///
/// - `ib` e.g. `std::cbegin(a_string)`.
/// - `ie` e.g. `std::cend(a_string)`.
/// - `re` The regular expression that defines the match we are after.
/// - `f` A callback function that will be passed the match and should return the desired output string.
///
/// Credit to [stackoverflow article](https://stackoverflow.com/questions/57193450/c-regex-replace-one-by-one)
template<typename Iter, typename Traits, typename CharT, typename UnaryFunction>
std::basic_string<CharT>
regex_replace(Iter ib, Iter ie, const std::basic_regex<CharT, Traits>& re, UnaryFunction f)
{
    std::basic_string<CharT> s;

    using diff_t = typename std::match_results<Iter>::difference_type;
    diff_t match_pos_old = 0;
    auto   end_last_match = ib;

    auto callback = [&](const std::match_results<Iter>& match) {
        auto match_pos = match.position(0);
        auto diff = match_pos - match_pos_old;

        auto start_match = end_last_match;
        std::advance(start_match, diff);

        s.append(end_last_match, start_match);
        s.append(f(match));

        auto match_len = match.length(0);
        match_pos_old = match_pos + match_len;
        end_last_match = start_match;
        std::advance(end_last_match, match_len);
    };

    std::regex_iterator<Iter> begin(ib, ie, re), end;
    std::for_each(begin, end, callback);
    s.append(end_last_match, ie);
    return s;
}

/// A version of `regex_replace(...)` where each match in turn is is run through a function you supply.
///
/// - `s` The string to hunt for matches in.
/// - `re` The regular expression that defines the match we are after.
/// - `f` A callback function that will be passed the match and should return the desired output string.
///
/// Credit to [stackoverflow article](https://stackoverflow.com/questions/57193450/c-regex-replace-one-by-one)
template<typename Traits, typename CharT, typename UnaryFunction>
std::string
regex_replace(const std::string& s, const std::basic_regex<CharT, Traits>& re, UnaryFunction f)
{
    return regex_replace(s.cbegin(), s.cend(), re, f);
}

} // namespace utilities
