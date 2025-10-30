# Stream Utilities

The `<utilities/stream.h>` header provides utility functions for streams.

## Readers

The functions differ from [`std::getline`] in a few ways:

1. They ignore blank lines in the input stream.
2. They allow for long lines by assuming that lines that end with a "\\" continue to the next.
3. They strip out comment lines and trailing comments where comment lines begin with "#" by default.

```c++
std::string utilities::read_line(std::istream &s, std::string_view comment_begin = "#");
```

reads a "line" from a stream and returns that as a string.

```c++

std::size_t utilities::read_line(std::istream &s, std::string &line, std::string_view comment_begin = "#");
```

overwrites the 'line' argument with the contents of a stream and returns the number of characters read into 'line’.

## Other Functions

We supply a method that rewinds an input stream to the start.

```c++
std::istream & rewind(std::istream &is);
```

and another that returns the number of non-comment lines in the stream.

```c++
std::size_t line_count(std::istream &is, std::string_view comment_begin = "#");
```

If the `comment_begin` argument is empty, `line_count` uses [`std::getline`] to read the lines.
Otherwise, we use our `read_line(...)` function, so comment lines are excluded, etc.

> [!WARNING]
> These two functions only work with file streams, etc.

<!-- Reference Links -->

[`std::getline`]: https://en.cppreference.com/w/cpp/string/basic_string/getline
