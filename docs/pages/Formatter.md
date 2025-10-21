# Formatter Specialisation

The `<utilities/formatter.h>` header specialises [`std::formatter`] for _any_ type that has a `to_string()` method.

C++20 introduced [`std::format`] to create strings with interpolated values; a core feature for the [`std::print`] standard output library.

Moreover, you can extend the standard library for your own types by _specialising_ the [`std::formatter`] class.

Many classes already implement a `to_string()` method that returns a string representation of an instance.
It seems like a good stopgap measure to connect all those types to `std::format` and its friends.

We have a [`concept`] that captures types with an accessible `to_string(...)` method that can be called without any arguments (of course, there can be lots of default arguments for the function):

```c++
template<typename T>
concept utilities::has_to_string = requires(const T& x) {
    { x.to_string() } -> std::convertible_to<std::string>;
};
```

We then specialised [`std::formatter`] to work with _any_ class that has a `to_string` method.

## Example

```cpp
#include <utilities/formatter.h>
#include <print>

struct Whatever {
    std::string to_string() const { return "Whatever!"; }
};

int main()
{
    Whatever w;
    std::println("Struct with a `to_string()` method: '{}'", w);
    return 0;
}
```

Running that outputs:

```txt
Struct with a to_string() method: 'Whatever!'
```

## Notes

1. The `std::formatter` specialisation only works with _default_ formatting.
   So `std::println("{}", w)` is fine, but something like `std::println("{:x}", w)` will cause a program fault. <br>
   Your `to_string` method may handle various output options, but we only ever call it with no passed arguments.

2. You can safely have a `to_string(...)` method **and** provide your own specialisation of `std::formatter`. <br>
   The compiler will see your specialisation as a better fit than the blanket one provided in `<utilities/formatter.h>`.

## See Also

[`std::format`] <br>
[`std::formatter`]

<!-- Reference Links -->

[`concept`]: https://en.cppreference.com/w/cpp/language/constraints
[`std::format`]: https://en.cppreference.com/w/cpp/utility/format/format
[`std::formatter`]: https://en.cppreference.com/w/cpp/utility/format/formatter
[`std::print`]: https://en.cppreference.com/w/cpp/io/print.html
