/// @brief Simple log & debug messages
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <iostream>
#include <string>
#include <format>

/// @brief Create and dispatch a debug messages -- only ever printed if the DEBUG flag is set at compile time.
#ifdef DEBUG
    #define DBG(...) MAKE_MESSAGE("DBG", __VA_ARGS__).dispatch()
#else
    #define DBG(...) void(0)
#endif

/// @brief Create and dispatch a log message. Can be turned off by setting NO_LOGS at compile time.
#ifndef NO_LOGS
    #define LOG(...) MAKE_MESSAGE("LOG", __VA_ARGS__).dispatch()
#else
    #define LOG(...) void(0)
#endif

/// @brief Messages (instances of the `utilities::message` class below) are constructed using the MAKE_MESSAGE macro.
#define MAKE_MESSAGE(type, ...) \
    utilities::message { __func__, __FILE__, __LINE__, type __VA_OPT__(, std::format(__VA_ARGS__)) }

namespace utilities {

/// @brief A utilities::message captures a location where the message was created and optionally a payload string.
/// @note  These are created by the `MAKE_MESSAGE(...)` macro above that inserts the needed location information.
class message {
public:
    /// @brief Messages are handled by a function with this signature -- there is just one handler at a time.
    /// @note  The default handler just prints the message to a default stream.
    using handler_type = void(const message&);

    /// @brief Class method that lets you set a custom message handling function.
    static handler_type*& handler() { return c_handler; }

    /// @brief Class method that sets the message handler back to the default value.
    /// @note  The default handler just prints the messages to a default stream
    static void use_default_handler() { c_handler = default_handler; }

    /// @brief The stream used by the default message handler (you can set it to say a file stream instead).
    inline static std::ostream* stream = &std::cout;

    /// @brief These objects are really created using a macro that inserts the needed source code location information.
    message(std::string_view func, std::string_view path, std::size_t line, std::string_view type,
            std::string_view payload = "") :
        m_function{func}, m_filename{filename(path)}, m_line{line}, m_type{type}, m_payload{payload}
    {
        // Empty body.
    }

    /// @brief Returns the whole message as a string e.g. "[DEBUG] 'foobar' foo.cpp line 25: x = 10, y = 11".
    std::string to_string() const
    {
        auto retval = std::format("[{}] function '{}' ({}, line {})", m_type, m_function, m_filename, m_line);
        if (!m_payload.empty()) {
            retval += ": ";
            retval += m_payload;
        }
        return retval;
    }

    /// @brief Dispatch this message to the message handler.
    void dispatch() const { c_handler(*this); }

    /// @brief The default message handler just prints the message to the default stream.
    static void default_handler(const message& message) { *stream << message.to_string() << std::endl; }

    /// @brief Reduce a full path to just the filename.
    static std::string filename(std::string_view path)
    {
        char sep = '/';
#ifdef _WIN32
        sep = '\\';
#endif
        auto i = path.rfind(sep, path.length());
        if (i != std::string::npos) return std::string{path.substr(i + 1, path.length() - i)};
        return "";
    }

private:
    std::string m_function; // Function/method where the message originated from.
    std::string m_filename; // Filename where the message originated from (just the filename not the path).
    std::size_t m_line;     // Line in the file where the message originated.
    std::string m_type;     // The type of this message e.g. "DEBUG".
    std::string m_payload;  // Any user supplied string that goes with this message.

    // User can set the a handler for all messages -- the default just prints the message to the default stream.
    inline static handler_type* c_handler = default_handler;
};

} // namespace utilities
