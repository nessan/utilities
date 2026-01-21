#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// A stopwatch class to measure execution times. <br>
/// See the [Stopwatch](docs/pages/Stopwatch.md) page for all the details.

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <format>

namespace utilities {
/// See the [Stopwatch](docs/pages/Stopwatch.md) page for all the details.
template<typename Clock = std::chrono::high_resolution_clock>
class stopwatch {
public:
    /// The underlying clock type
    using clock_type = Clock;

    /// A stopwatch can have a name to distinguish it from others you may have running
    explicit stopwatch(const std::string& str = "") : m_name(str) { reset(); }

    /// Provides read-only access to the stopwatch's name
    std::string name() const { return m_name; }

    /// Provides read-write access to the stopwatch's name
    std::string& name() { return m_name; }

    /// Set/reset the stopwatch's 'zero' point & clear any measured splits.
    void reset()
    {
        m_zero = clock_type::now();
        m_split = 0;
        m_prior = 0;
    }

    /// Get the time that has passed from the zero point to now. Units are seconds.
    double elapsed() const
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto t = clock_type::now();
        std::atomic_thread_fence(std::memory_order_relaxed);
        return std::chrono::duration<double>(t - m_zero).count();
    }

    /// Clicks the stopwatch to create a new 'split' and returns the elapsed time in seconds.
    double click()
    {
        auto tau = elapsed();
        m_prior = m_split;
        m_split = tau;
        return m_split;
    }

    /// Returns the split as the time in seconds that elapsed from the zero point to the last click.
    double split() const { return m_split; }

    /// Returns the last 'lap' time in seconds (i.e. the time between prior 2 splits).
    double lap() const { return m_split - m_prior; }

    /// Class method that returns a "pretty" string for a time in seconds.
    ///
    /// - If the time is less than 1 second, it is formatted as a number of milliseconds.
    /// - If the time is greater than 1 second, it is formatted as a number of seconds.
    ///
    /// Numbers are formatted to 2 decimal places and output with a unit suffix (`ms` or `s`).
    ///
    /// # Examples
    /// ```
    /// assert_eq(stopwatch<>::format_seconds(0.0001), "0.10ms");
    /// assert_eq(stopwatch<>::format_seconds(0.011), "11.00ms");
    /// assert_eq(stopwatch<>::format_seconds(1.0), "1.00s");
    /// assert_eq(stopwatch<>::format_seconds(25.23456789), "25.23s");
    /// ```
    constexpr static std::string format_seconds(double seconds)
    {
        if (seconds < 1.0) { return std::format("{:.2f}ms", seconds * 1000.0); }
        return std::format("{:.2f}s", seconds);
    }

    /// Returns a string representation of the stopwatch's elapsed time in seconds.
    std::string to_string() const
    {
        auto tau = format_seconds(elapsed());
        if (m_name.empty()) return tau;
        return std::format("{}: {}", m_name, tau);
    }

private:
    using time_point = typename Clock::time_point;

    std::string m_name;  // Name of the stopwatch.
    time_point  m_zero;  // The stopwatch measures time (in seconds) from this time point.
    double      m_split; // The total seconds to when the stopwatch was most recently clicked.
    double      m_prior; // The prior split.
};

/// A convenience function that converts a `std::chrono::duration` to a `double` number of seconds.
template<class Rep, class Period>
inline double
to_seconds(const std::chrono::duration<Rep, Period>& d)
{
    return std::chrono::duration<double>(d).count();
}

/// Theoretically the most precise stopwatch -- it might get put off by system reboots etc.
using precise_stopwatch = stopwatch<std::chrono::high_resolution_clock>;

/// A stopwatch that is guaranteed to be monotonic.
using steady_stopwatch = stopwatch<std::chrono::steady_clock>;

/// A stopwatch that is uses the system clock.
using system_stopwatch = stopwatch<std::chrono::system_clock>;

/// The usual output stream operator.
///
/// Prints the name of the stopwatch if any followed by the elapsed time in seconds.
template<typename Clock>
inline std::ostream&
operator<<(std::ostream& os, const stopwatch<Clock>& rhs)
{
    return os << rhs.to_string();
}

} // namespace utilities

// --------------------------------------------------------------------------------------------------------------------
// Specialises `std::formatter` to handle our stopwatch class ...
// -------------------------------------------------------------------------------------------------------------------

/// Specialise `std::formatter` for our `utilities::stopwatch<Clock>` type.
template<typename Clock>
struct std::formatter<utilities::stopwatch<Clock>> {

    constexpr auto parse(const std::format_parse_context& ctx)
    {
        // Throw an error for anything that is not default formatted for now ...
        auto it = ctx.begin();
        assert(it == ctx.end() || *it == '}');
        return it;
    }

    template<class FormatContext>
    auto format(const utilities::stopwatch<Clock>& rhs, FormatContext& ctx) const
    {
        // Punt the formatting to the stopwatch's `to_string()` method.
        return std::format_to(ctx.out(), "{}", rhs.to_string());
    }
};
