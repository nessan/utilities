# Formatter Specialisation

The `<utilities/formatter.h>` header speci# Logging Utilities

The header `<utilities/log.h>` provides a couple of macros for simple logging and debugging.

-   `LOG(...)` dispatches messages **unless** the `NO_LOGS` flag is set at compile time.
-   `DBG(...)` dispatches a message **only if** the `DEBUG` flag is set at compile time.

These macros create and dispatch `utilities::message` objects, which hold an arbitrary user-supplied message. The instances also include source code location information.
The `LOG` and `DBG` macros automatically pass the correct source code location information to the message.

The message payload can be anything that works for the facilities in [`std::format`].
The default message handler prints the source code location if there is no payload.

> [!WARNING]
Microsoft's old traditional preprocessor is unhappy with some of our macros, but its newer, cross-platform-compatible one is fine. <br>
> Add the `/Zc:preprocessor` flag to use that upgrade at compile time.
> Our `CMake` module `compiler_init` does that automatically for you.

## Message Handling

The macros create and dispatch messages to a message handler immediately.
The default handler prints the message and the location information to a default standard output stream.
You can change that default stream, for example:

```c++
log_file = std::ofstream("log.txt");
utilities::message::stream = &log_file;
```

For fancier usage, you can interpose your message handler.

```c++
void my_handler(const utilities::message& msg) { ... }
utilities::message::handler() = my_handler;
```

Your handler can do whatever it likes with the messages it receives.
You can revert to the default handler by calling the class method `utilities::message::use_default_handler()`.

> [!NOTE]
> Everything here is deliberately rudimentary, with no consideration for rotating log files, etc., or for streaming efficiency.
> The primary use case for the macros above and the underlying `utilities::message` class is easily printing useful messages during the development cycle.
> The emphasis is on ease of use rather than great generality or speed.
> During the development cycle, the typical output is going to be small anyway.

## Example

```cpp
#include <utilities/log.h>
int add(int x, int y)
{
    DBG("The DEBUG flag was set at compile time.");
    LOG("x = {}, y = {}", x, y);
    return x + y;
}

int main()
{
    LOG(); // <1>
    return add(10, 11);
}
```

1. A call to `LOG` without any message will print the source location.

If that source was in a file called `example.cpp`, then the program will output:

```sh
[LOG] function 'main' (example.cpp, line 11)
[DBG] function 'add' (example.cpp, line 4): The DEBUG flag was set at compile time.
[LOG] function 'add' (example.cpp, line 5): x = 10, y = 11
```

alises [`std::formatter`] for _any_ type that has a `to_string()` method.

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

We then specialised [`std::formatter`] to work with _any_ `has_to_string` compatible class.

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
