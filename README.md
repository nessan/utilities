# README

The `utilities` library is a small collection of C++ classes, functions, and macros.

It is header-only, so there is nothing to compile or link.
Moreover, you can use any header file in this library on a standalone basis, as there are no interdependencies.

## Available Facilities

| Header File   | Purpose                                                      |
| ------------- | ------------------------------------------------------------ |
| `verify.h`    | Defines some `assert`-type macros that improve on the standard one in various ways. In particular, you can add a message explaining why a check failed. |
| `format.h`    | Functionality that connects any class with a `to_string()` method to `std::format`. |
| `print.h`     | Workaround for compilers that haven't yet implemented `std::print`. |
| `macros.h`    | Defines macros often used in test and example programs. <br/>It also defines a mechanism that lets you overload a macro based on the number of passed arguments. |
| `log.h`       | Some very simple logging macros.                             |
| `stopwatch.h` | Defines the `utilities::stopwatch` class you can use to time blocks of code. |
| `stream.h`    | Defines some functions to read lines from a file, ignoring comments and allowing for continuation lines. |
| `string.h`    | Defines several useful string functions (turn them to upper-case, trim white space, etc). |
| `thousands.h` | Defines functions to imbue output streams and locales with commas. This makes it easier to read large numbers–for example, printing 23000.56 as 23,000.56. |
| `type.h`      | Defines the function `utilities::type`,  which produces a string for a type. |
| `utilities.h` | This “include-the-lot” header pulls in all the other files above. |

## Installation

This library is header-only, so there is nothing to compile & link. Drop the small `utilities` header directory somewhere convenient. You can even use any single header file on a stand-alone basis.

Alternatively, if you are using `CMake`, you can use the standard `FetchContent` module by adding a few lines to your project's `CMakeLists.txt` file:

```cmake
include(FetchContent)
FetchContent_Declare(utilities URL https://github.com/nessan/utilities/releases/download/current/utilities.zip)
FetchContent_MakeAvailable(utilities)
```

This command downloads and unpacks an archive of the current version of `utilities` to your project's build folder. You can then add a dependency on `utilities::utilities`, a `CMake` alias for `utilities`. `FetchContent` will automatically ensure the build system knows where to find the downloaded header files and any needed compiler flags.

## Documentation

You can read the project's documentation [here](https://nessan.github.io/utilities/). \
We used the static website generator [Quarto](https://quarto.org) to construct the documentation site.

### Contact

You can contact me by email [here](mailto:nzznfitz+gh@icloud.com).

### Copyright and License

Copyright (c) 2022-present Nessan Fitzmaurice. \
You can use this software under the [MIT license](https://opensource.org/license/mit).
