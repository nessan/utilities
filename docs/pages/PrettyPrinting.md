# Pretty Printing Utilities

The `<utilities/thousands.h>` header provides functions that help you print large numbers in a readable format.
They force a stream or locale to insert appropriate commas, e.g., having 23456.7 print as 23,456.7.

```c++
utilities::imbue_stream_with_commas(std::ios_base &s = std::cout, bool on = true); // <1>
utilities::imbue_global_with_commas(bool on = true); // <2>
utilities::pretty_print_thousands(bool on = true); // <3>
```

1. Turns commas on or off for a particular stream.
2. Turns commas on or off for the global locale.
3. Turns commas on or off for the standard streams `std::cout`, `std::cerr`, `std::clog`, and the global locale.

Ideally, one should be able to rely on facilities in [`std::locale`] to format large numbers according to local conventions.
For example, using {std.format}, you might print a large number like this:

```c++
std::println("x = {:L}", 23456.7);
```

The `L` specifier tells the text formatting library to invoke an appropriate facet from the default locale.
Then you should see `23,456.7` displayed at many locations on the screen.

In practice, the various C++ standard libraries have poor support for resolving locale information.

Your computer correctly supports locales at the operating system level.
After all, computer manufacturers sell their machines to customers worldwide, and those customers expect dates and other time-related information to be displayed locally.

However, the default locale in many C++ implementations differs from the operating system's default locale.
Instead, it is often a complete blank, so rather than seeing `23,456.7` you will get the less readable `23456.7`

The various `utilities::imbue_xxx_with_commas` functions are a little hack that corrects the deficit.

If you call the final version above `pretty_print_thousands()`, we inject a comma-producing facet into the default locale, and printing large numbers will work as expected.

> [!NOTE]
> For now, the only punctuation we support with these functions is the comma.
> You should know that many countries treat the comma as a decimal point.

## Example

```cpp
#include <utilities/thousands.h>
#include <format>

int main()
{
    double x = 123456789.123456789;

    std::println("Using the default locale:");
    std::println("No locale specifier:   {:12.5f}",    x);
    std::println("With locale specifier: {:12.5Lf}\n", x); // <1>

    utilities::pretty_print_thousands();
    std::prinln("After adding a commas facet:");
    std::println("No locale specifier:   {:12.5f}",   x);
    std::println("With locale specifier: {:12.5Lf}",  x); // <2>
}
```

1. We might expect to see a readable `x` here, but typically will not.
2. We will see a readable `x` here for sure.

Typical output:

```txt
Using the default locale:
No locale specifier:   123456789.12346
With locale specifier: 123456789.12346

After adding a commas facet:
No locale specifier:   123456789.12346
With locale specifier: 123,456,789.12346
```

<!-- Reference Links -->

[`std::locale`]: https://en.cppreference.com/w/cpp/locale/locale.html
