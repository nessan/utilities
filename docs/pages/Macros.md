# Macros

The header `<utilities/macros.h>` defines a set of generally useful macros.

These include the `OVERLOAD` macro, which implements a well-known trick to "overload" any macro based on the number of passed arguments.
See [overloading] below.

> [!WARNING]
> Microsoft's old traditional preprocessor is unhappy with some of our macros, but its newer, cross-platform-compatible one is fine.
> Add the `/Zc:preprocessor` flag to use that upgrade at compile time.
> Our `CMake` module `compiler_init` does that automatically for you.

## Compiler String

The macro

```c++
COMPILER_NAME
```

expands to a string that encodes the compiler's name and version.

Test code can use the macro to annotate results.
We only support the usual three compiler suspects —`gcc`, `clang`, and `MSVC` —but adding more [compilers] is not difficult.

One of the examples below uses this macro.

## Macro Expansion

```c++
STRINGISE(foo) // <1>
CONCAT(foo, bar) // <2>
```

1. Turns the argument into a string.
2. Concatenates the two arguments.

These two classic macros work even if the arguments you pass in are themselves macros by fully expanding all arguments.

## Macro "Overloading"

The macro

```c++
OVERLOAD(macro, ...)
```

makes it appear to the consumer that `macro` is overloaded with multiple versions based on the number of arguments passed.

For example, if you have an overloaded `FOO` macro:

```c++
#define FOO(...) OVERLOAD(FOO, __VA_ARGS__)
```

The consumer of the macro can call it `FOO()`, `FOO(a)`, or `FOO(a,b)`.

All calls get automatically forwarded to the correct concrete macro.
For `FOO`, these must be named `FOO0`, `FOO1`, `FOO2`, etc.
The macro writer provides whichever of these makes sense, but the macro's _consumer_ can use them through the more straightforward call.

### Example

```cpp
#include <utilities/macros.h>
#include <iostream>

#define COUT(...) OVERLOAD(COUT, __VA_ARGS__) // <1>

#define COUT0(x) std::cout << "Zero argument version:   " << '\n' // <2>
#define COUT1(x) std::cout << "One argument version:    " << x << '\n'
#define COUT2(x, y) std::cout << "Two arguments version:   " << x << ", " << y << '\n'
#define COUT3(x, y, z) std::cout << "Three arguments version: " << x << ", " << y << ", " << z << '\n'

int main()
{
    std::cout << "Compiler: " << COMPILER_NAME << '\n';

 // NOTE: We only ever call COUT, but we expect to get the correct concrete version.
    COUT();
    COUT("x");
    COUT("x", 2);
    COUT("x", 2, 'z');
    return 0;
}
```

1. We overload a trivial macro based on the number of arguments passed.
2. The one-, two-, and three-argument versions of `COUT`.

The program will output something along the lines:

```txt
Compiler: clang 17.0.6
Zero argument version:
One argument version:    x
Two arguments version:   x, 2
Three arguments version: x, 2, z
```

## Semantic Version Strings

```c++
VERSION_STRING(major)                   <1>
VERSION_STRING(major, minor)            <2>
VERSION_STRING(major, minor, patch)     <3>
```

1. Expands to "major".
2. Expands to "major.minor".
3. Expands to "major.minor.patch".

`VERSION_STRING` is an _overloaded_ macro.
For example, `VERSION_STRING(3, 1, 0)` expands to the string `"3.1.0"`.

## Print Code & Execute

The `RUN` macro is always passed a line of code to execute and optionally one or two variables to print after the code is run:

```c++
RUN(code)               <1>
RUN(code, out)          <2>
RUN(code, out1, out2)   <3>
```

1. Prints a stringified version of `code` to the screen and then runs it.
2. Does the same thing, and then prints the value from `out`.
3. Does the same thing, and then prints the values `out1` and `out2`.

This _overloaded_ macro prints what will be executed in a test program, optionally followed by some results.
Best illustrated with an example.

### Example

```cpp
#include <utilities/macros.h>
#include <format>
#include <iostream>
#include <iterator>
#include <string>

int main()
{
    RUN(std::string s1);
    RUN(double x = 123456789.123456789);
    RUN(std::format_to(std::back_inserter(s1), "x = {:12.5f}", x), s1);
}
```

The program will output:

```txt
[CODE]   std::string s1
[CODE]   double x = 123456789.123456789
[CODE]   std::format_to(std::back_inserter(s1), "x = {:12.5f}", x)
[RESULT] s1: x = 123456789.12346
```

<!-- Reference Links -->

[compilers]: https://github.com/cpredef/predef/blob/master/Compilers.md
