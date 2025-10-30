# String Utilities

The header `<utilities/string.h>` supplies many utility functions that work on strings.

Most of the functions come in two flavours.
One version alters the input string in place, while the other returns a new string that is a copy of the input appropriately converted, leaving the original untouched.

For example, `utilities::upper_case(str)` converts `str` to upper-case in place.
On the other hand, `utilities::upper_cased(str)` returns a fresh string that is a copy of `str` converted to upper-case.
The naming style is consistently used across many functions in the header.

There are other functions where this distinction is unnecessary, such as `utilities::starts_with(...)`.

## Case Conversions

```c++
void utilities::upper_case(std::string&); // <1>
void utilities::lower_case(std::string&); // <2>

std::string utilities::upper_cased(std::string_view); // <3>
std::string utilities::lower_cased(std::string_view); // <4>
```

1. Converts a string to uppercase.
2. Converts a string to lowercase.
3. Returns a new string, an uppercase copy of the input string.
4. Returns a new string, a lowercase copy of the input string.

> !CAUTION
> The case conversions rely on the [`std::tolower`] and [`std::toupper`] functions, which only work for simple character types.

## Trimming Spaces

```c++
void utilities::trim_left(std::string&); // <1>
void utilities::trim_right(std::string&); // <2>
void utilities::trim(std::string&); // <3>

std::string utilities::trimmed_left(std::string_view); // <4>
std::string utilities::trimmed_right(std::string_view); // <5>
std::string utilities::trim(std::string_view); // <6>
```

1. Remove any leading whitespace from the input string.
2. Remove any trailing whitespace from the input string.
3. Remove leading and trailing whitespace from the input string.
4. Returns a new string, a left-trimmed copy of the input string.
5. Returns a new string, a right-trimmed copy of the input string.
6. Returns a new string that is a trimmed copy of the input string on both sides.

> !CAUTION
> The case conversions rely on the [`std::isspace`] function to identify whitespace characters.

## Replacing Substrings

```c++
void
utilities::replace_left(std::string &str,
                        std::string_view target,
                        std::string_view replacement); // <1>
void
utilities::replace_right(std::string &str,
                         std::string_view target,
                         std::string_view replacement); // <2>
void
utilities::replace(std::string &str,
                   std::string_view target,
                   std::string_view replacement); // <3>
std::string
utilities::replaced_left(std::string_view str,
                         std::string_view target,
                         std::string_view replacement); // <4>
std::string
utilities::replaced_right(std::string_view str,
                          std::string_view target,
                          std::string_view replacement);// <5>
std::string
utilities::replaced(std::string_view str,
                    std::string_view target,
                    std::string_view replacement); // <6>
```

1. Replace the first occurrence of `target` in `str` with `replacement`.
2. Replace the final occurrence of `target` in `str` with `replacement`.
3. Replace all occurrences of `target` in `str` with `replacement`.
4. Returns a new string, a copy of `str` with the first occurrence of `target` changed to `replacement`.
5. Returns a new string, a copy of `str` with the final occurrence of `target` changed to `replacement`.
6. Returns a new string, a copy of `str` with all occurrences of `target` changed to `replacement`.

We also have functions to replace all contiguous white space sequences in a string:

```c++
void
utilities::replace_space(std::string &str,
                         const std::string &with = " ",
                         bool also_trim = true); // <1>
std::string
utilities::condense(std::string_view str,
                    bool also_trim = true); // <2>
std::string
utilities::replaced_space(std::string_view &str,
                          const std::string &with = " ",
                          bool also_trim = true); // <3>
std::string
utilities::condensed(std::string_view str,
                     bool also_trim = true); // <4>
```

1. Replaces all contiguous white space sequences in a string with a single white space character or, optionally, something else.
 By default, the string is also trimmed of white space on both the left and right.
1. Replaces all contiguous white space sequences in a string with a single white space character.
 By default, the string is also trimmed of white space on both the left and right.
1. Returns a new string, a copy of `str` with all contiguous white space sequences replaced with a single white space character or, optionally, something else.
 By default, the output string is also trimmed of white space on both the left and right.
1. Returns a new string, a copy of `str` with all contiguous white space sequences replaced with a single white space character.
 By default, the output string is also trimmed of white space on both the left and right.

## Erasing Substrings

```c++
void
utilities::erase_left(std::string &str,
                      std::string_view target); // <1>
void
utilities::erase_right(std::string &str,
                       std::string_view target); // <2>
void
utilities::erase(std::string &str,
                 std::string_view target); // <3>
std::string
utilities::erased_left(std::string_view str,
                       std::string_view target); // <4>
std::string
utilities::erased_right(std::string_view str,
                        std::string_view target); // <5>
std::string
utilities::erased(std::string_view str,
                  std::string_view target); // <6>
```

1. Erases the first occurrence of the `target` substring in `str`.
2. Erases the final occurrence of the `target` substring in `str`.
3. Erases all occurrences of the `target` substring in `str`.
4. Returns a new string, a copy of `str` with the first occurrence of `target` erased.
5. Returns a new string, a copy of `str` with the final occurrence of `target` erased.
6. Returns a new string, a copy of `str` with all occurrences of `target` erased.

## "Standardising" Strings

We often need to parse free-form input to find a keyword or phrase.
Having a facility that converts strings to some standard form is helpful.

```c++
void utilities::remove_surrounds(std::string&); // <1>
void utilities::standardize(std::string&); // <2>

std::string utilities::removed_surrounds(std::string_view); // <3>
std::string utilities::standardized(std::string_view); // <4>
```

1. Strips any "surrounds" from the input string. <br>
 For example, the string "(text)" becomes "text".
 Multiples also work, so "[[[text]]]" becomes "text".
 Only correctly balanced surrounds are ever removed.
2. Standardise the input string --- see below
3. Returns a new string, a copy of the input with any "surrounds" removed.
4. Returns a new string, a standardised copy of the input.

The `standardise` functions give you a string stripped of extraneous brackets, etc.
Moreover, the single space character will replace all interior white space. The function will also remove any leading or trailing whitespace.
So a string like "< Ace of Clubs >" will become "ACE OF CLUBS".

It is a lot easier to parse standardised strings.

## Searching

```c++
bool utilities::starts_with(std::string_view str, std::string_view prefix); // <1>
bool utilities::ends_with(std::string_view str, std::string_view prefix); // <2>
```

1. Returns `true` if `str` starts with `prefix`.
2. Returns `true` if `str` ends with `suffix`.

## Tokenizing

We often want to convert a stream of text into tokens and have functions to help with that.

```c++
template<std::input_iterator InIter, std::forward_iterator FwdIter, typename Func>
constexpr void
for_each_token(InIter  input_begin, InIter  input_end,
               FwdIter delims_begin, FwdIter delims_end, Func token_func); // <1>

template<typename Container_t>
constexpr void
tokenize(std::string_view input, Container_t &output_container,
         std::string_view delimiters = "\t,;: ", bool skip = true); // <2>

std::vector<std::string>
split(std::string_view input,
      std::string_view delimiters = "\t,;: ", bool skip = true); // <3>
```

1. Given iterators that bracket the input text and others that bracket the possible token delimiters, this method processes the text and passes each token to a user-supplied function.
2. Tokenises the input text string and places the tokens into `output_container`.
3. Tokenises the input text string and returns the tokens as a `std::vector` of strings.

We have based the `for_each_token` function on the excellent discussion [in this post](https://tristanbrindle.com/posts/a-quicker-study-on-tokenising/).

### Function Arguments

| Argument           | Description                                                                                                                                           |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| `input_begin`      | To tokenize the string stored in `text` `input_begin` should be `std::cbegin(text)`.                                                                  |
| `input_end`        | To tokenize the string stored in `text` `input_end` should be `std::cend(text)`.                                                                      |
| `delims_begin`     | If the possible delimiters for the tokens are in the string `delims`, which might be `"\t,;: "`, then `delims_begin` should be `std::cbegin(delims)`. |
| `delims_end`       | If the possible delimiters for the tokens are in the string `delims`, which might be `"\t,;: "`, then `delims_end` should be `std::cend(delims)`.     |
| `token_func`       | This will be called for each token: `token_func(token.cbegin(), token.cend())`.                                                                       |
| `output_container` | This container needs to be dynamically resizable and support the `emplace_back(token.cbegin(), token.cend())`.                                        |
| `skip`             | If true, we ignore empty tokens (e.g., two spaces in a row).                                                                                          |
| `delimiters`       | These are the characters that should delimit our tokens. Tokens break on white space, commas, semi-colons, and colons by default.                     |

## Extracting Values

We also have a function that attempts to parse a value or a particular type from a string.

```c++
template<typename T>
constexpr std::optional<T> possible(std::string_view str); // <1>
```

This function uses the [`std::from_chars`] function to retrieve a possible simple type from a string.
It returns a [`std::nullopt`] if it fails to parse the input.

### Example

```c++
auto x = possible<double>(str);
if(x) std::cout << str << ": parsed as the double value " << x << '\n';
```

If successful, this function tries to fill x with a double value read from a string and print it on `std::cout`.

<!-- Reference Links -->

[`std::toupper`]: https://en.cppreference.com/w/cpp/string/byte/toupper.html
[`std::tolower`]: https://en.cppreference.com/w/cpp/string/byte/tolower.html
[`std::isspace`]: https://en.cppreference.com/w/cpp/string/byte/isspace.html
[`std::from_chars`]: https://en.cppreference.com/w/cpp/utility/from_chars.html
[`std::nullopt`]: https://en.cppreference.com/w/cpp/utility/optional/nullopt.html
