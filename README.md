# Project Overview

<p align="center"><img src="banner.svg"></p>

The `utilities` library is a small collection of C++ classes, functions, and macros that have proven useful across several projects.
It is header-only, so there is nothing to compile or link.

All non-macro functionality is in the `utilities` namespace.

> [!TIP]
> You can use any header file in this library on its own — there are no interdependencies.

## Available Facilities

| Focus          | Header File   | Brief Description                                                                                                                      |
| -------------- | ------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| [Assertions][] | `assert.h`    | Provides several [standard assert][] type macros that improve on the standard one in various ways.                                     |
| [Logging][]    | `log.h`       | Provided some very simple logging macros.                                                                                              |
| [Macros][]     | `macros.h`    | Provides macros commonly used in test programs. <br> It also defines a mechanism to overload a macro based on the number of arguments. |
| [Stopwatch][]  | `stopwatch.h` | Provides the `utilities::stopwatch` class, which you can use to time blocks of code.                                                   |
| [Streams][]    | `stream.h`    | Provides some functions to read lines from a file, ignoring comments and allowing for continuation lines.                              |
| [Strings][]    | `string.h`    | Provides several useful string functions (e.g., turning strings to uppercase, trimming white space, etc.).                             |
| [Formatter][]  | `formatter.h` | Provides a specialisation of [`std::formatter`][] for _any_ type that has a `to_string()` method.                                      |
| [Output][]     | `thousands.h` | Provides functions to imbue output streams and locales with commas that make it easier to read large numbers.                          |
| [Types][]      | `type.h`      | Provides the function `utilities::type`, which returns a string for a type.                                                            |

> [!WARNING]
> Microsoft's old traditional preprocessor is not happy with some of our macros, but its newer, cross-platform-compatible one is fine.
> Add the `/Zc:preprocessor` flag to use that upgrade at compile time.
> Our `CMake` module `compiler_init` does that automatically for you.

## Convenience Headers

There are a couple of extra convenience "include-the-lot" headers:

-   `utilities.h`: Pulls in all the other header files in the library.
-   `namespace.h`: Pulls in all the other header files in the library, and follows that with a `using namespace utilities` directive.

## Installation

This library is header-only, so there is nothing to compile & link.
Drop the small `utilities` header directory into a convenient location.
You can even use any single header file on its own.

Alternatively, if you are using `CMake`, you can use the standard `FetchContent` module by adding a few lines to your project's `CMakeLists.txt` file:

```cmake
include(FetchContent)
FetchContent_Declare(utilities URL https://github.com/nessan/utilities/releases/download/current/utilities.zip)
FetchContent_MakeAvailable(utilities)
```

This command downloads and unpacks the current version of `utilities` into your project's build folder.
You can then add a dependency on `utilities::utilities`, a `CMake` alias for `utilities`.
`FetchContent` will automatically ensure the build system knows where to find the downloaded header files and any needed compiler flags.

> [!NOTE]
> That URL downloads the library's current version—whatever is in the main branch when you run the download.
> That content may change from time to time.
> Use a URL like `https://github.com/nessan/utilities/releases/download/2.0.0/utilities.zip` to get a fixed library version.

## Links

Here are links to project's [repository][] and [documentation site][]

The project uses [Doxytest][], a tool for generating C++ test programs from code embedded in header file comments.

You can contact me by [email][].

## Copyright and License

Copyright (c) 2022-present Nessan Fitzmaurice. <br>
You can use this software under the [MIT License]

<!-- Reference Links -->

[MIT License]: https://opensource.org/license/mit
[Doxytest]: https://nessan.github.io/doxytest/
[repository]: https://github.com/nessan/utilities
[documentation site]: https://nessan.github.io/utilities
[email]: mailto:nzznfitz+gh@icloud.com
[`std::formatter`]: https://en.cppreference.com/w/cpp/utility/format/formatter
[standard assert]: https://en.cppreference.com/w/cpp/error/assert.html
[Assertions]: docs/pages/Assertions.md
[Formatter]: docs/pages/Formatter.md
[Logging]: docs/pages/Logging.md
[Macros]: docs/pages/Macros.md
[Output]: docs/pages/PrettyPrinting.md
[Stopwatch]: docs/pages/Stopwatch.md
[Streams]: docs/pages/StreamFunctions.md
[Strings]: docs/pages/StringFunctions.md
[Types]: docs/pages/TypeStrings.md
