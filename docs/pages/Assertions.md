# Assertions

The `<utilities/assert.h>` header includes macros for checking a boolean condition or comparing two values.

On failures, the macros print a customisable message to the standard error stream and then exit the program with a non-zero status code.

Failed assertions show the source code location where the assertion was called, along with a stringified version of the condition that caused the failure.

You can also add extra content by passing trailing arguments to the macros.
Those arguments are sent to [`std::format`] to produce a string.

## Boolean Assertions

The following macros are available to check a Boolean condition:

```cpp
always_assert(condition, ...) // <1>
debug_assert(condition, ...) // <2>
assert(condition, ...) // <3>
```

1. The `condition` is checked, and if it evaluates to `false`, an error message is printed to the standard error stream. <br> That can include a custom payload synthesised from the trailing argument list.
2. The `debug_assert` macro expands to `always_assert` **if** the `DEBUG` flag is set; otherwise, it's a no-op.
3. The `assert` macro expands to `always_assert` **unless** the `NDEBUG` flag is set, which is a no-op. <br>This is similar to the [standard assert] macro.

## Equality Assertions

The following macros are available to check for the equality of two objects:

```cpp
always_assert_eq(lhs, rhs, ...) // <1>
debug_assert_eq(lhs, rhs, ...) // <2>
assert_eq(lhs, rhs, ...) // <3>
```

1. The two values `lhs` and `rhs` are checked for equality, and if they are not equal, the macro prints a customisable message and exits the program.
2. The `debug_assert_eq` macro expands to `always_assert_eq` **if** the `DEBUG` flag is set; otherwise, it's a no-op.
3. The `assert_eq` macro expands to `always_assert_eq` **unless** the `NDEBUG` flag is set, which is a no-op similar to the [standard assert] macro.

The `lhs` and `rhs` arguments must be comparable using `operator==`.
They must also be printable using [`std::print`], so we will need an appropriate specialisation for [`std::formatter`].

## Example

For the sake of the example, we will use the `always_assert_eq` macro to check for equality.
More typically, you use either the `debug_assert_eq` or the `assert_eq` macro.

```cpp
#include <utilities/assert.h>
int subtract(int x, int y)
{
    always_assert_eq(x, y, "well that didn't work but x - y = {}", x - y);
    return y - x;
}
int main()
{
    return subtract(10, 11);
}
```

That program exits with the message:

```text
FAILED `assert_eq(x, y)` [assert02.cpp:7]
well that didn't work but x - y = -1
lhs = 10
rhs = 11
```

## Design Rationale

### The "Always" Versions

The `always_assert` and `always_assert_eq` macros are used to verify that a condition is true and to exit with an informative message if it is not. <br>
These macros are unaffected by compiler flags.

### The "Debug" Versions

In the development cycle, it can be helpful to range-check indices and so on.

However, those checks are expensive and can slow down numerical code by orders of magnitude.
Therefore, we don't want there to be any chance that those verifications are accidentally left "on" in the production code.
The `debug_assert` and `debug_assert_eq` macros cover this type of verification.
Enabling these checks requires the programmer to take a specific action: she must set the `DEBUG` flag at compile time.

For example, here is a precondition from a hypothetical `dot(Vector u, Vector v)` function:

```cpp
debug_assert_eq(u.size(), v.size(), "Incompatible vector sizes!");
```

This code checks that the two vector arguments have equal length --- a necessary constraint for the dot product operation to make sense.
If the requirement is not satisfied, the program exits with an informative message that includes the size of the two vectors.

The check here is **off** by default, and you need to do something special (i.e., define the `DEBUG` flag at compile time) to enable it.
Production code may do many of these dot products; we do not generally want to pay for the check.
However, enabling these sorts of checks may be very useful during development.

These "debug" macros expand to nothing **unless** you set the `DEBUG` flag at compile time.

### The "Standard" Versions

On the other hand, there are some checks where the assertion cost is slight compared to the function's overall work.
Leaving these checks on in production will not likely impose much of a performance penalty.

For example, a precondition for a matrix inversion method is that the input matrix must be square.
Here is how you might do that check in an `invert(const Matrix& M)` function:

```cpp
assert(M.is_square(), "Cannot invert a {} x {} NON-square matrix!", M.rows(), M.cols());
```

We can only invert square matrices.
The `M.is_square()` call checks that condition and, on failure, exits the program with a helpful message.
The check cost is very slight compared to the work done by the `invert(...)` method, so leaving it on even in production code is not a problem.

The check here is **on** by default, and you need to do something special (i.e., define the `NDEBUG` flag at compile time) to turn it off.

These macros expand to nothing **only if** you set the `NDEBUG` flag at compile time.

The decision to use one form vs. the other is predicated on the cost of doing the check versus the work done by the method in question.

A primary use case for `debug_assert` and `debug_assert_eq` is checking index bounds.
From experience, this is vital during development.
However, bounds-checking every index operation incurs a considerable performance penalty and can slow down numerical code by orders of magnitude.
It makes sense to have the checks in place for development, but ensure they are never present in release builds.

## Why Macros?

The main reason we use macros instead of a function is that we need to stringify the assertion condition and assertion arguments to produce a helpful message if the assertion fails.

WARNING: Macros can depend on comma placement to separate arguments, which can be a gotcha in some cases.
For example, if you are equality testing between two `std::pair` variables.
You can often get around any issues by the judicious use of parentheses, but it is something to be aware of if you start to see weird compiler errors.

> [!WARNING]
> Microsoft's old traditional preprocessor is unhappy with some of our macros, but its newer, cross-platform-compatible one is fine. <br>
> Add the `/Zc:preprocessor` flag to use that upgrade at compile time.
> Our `CMake` module `compiler_init` does that automatically for you.

## See Also

[standard assert]

<!-- Reference Links -->

[standard assert]: https://en.cppreference.com/w/cpp/error/assert.html
[`std::format`]: https://en.cppreference.com/w/cpp/utility/format/format
[`std::formatter`]: https://en.cppreference.com/w/cpp/utility/format/formatter
[`std::print`]: https://en.cppreference.com/w/cpp/io/print.html
