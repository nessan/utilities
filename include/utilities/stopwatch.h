#pragma once
// SPDX-FileCopyrightText: 2025 Nessan Fitzmaurice <nzznfitz+gh@icloud.com>
// SPDX-License-Identifier: MIT

/// @file
/// A stopwatch class to measure execution times. <br>
/// See the [Stopwatch](docs/pages/Stopwatch.md) page for all the details.

#include <atomic>
#include <chrono>
#include <iostream>

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

    /// Returns a string representation of the stopwatch's elapsed time in seconds.
    std::string to_string() const
    {
        auto tau = elapsed();
        if (m_name.empty()) return std::format("{}s", tau);
        return std::format("{}: {}s", m_name, tau);
    }

private:
    using time_point = typename Clock::time_point;

    std::string m_name;  // Name of the stopwatch.
    time_point  m_zero;  // The stopwatch measures time (in seconds) from this time point.
    double      m_split; // The total seconds to when the stopwatch was most recently clicked.
    double      m_prior; // The prior split.
};

/// The usual output operator.
///
/// Prints the name of the stopwatch if any followed by the elapsed time in seconds.
template<typename Clock>
inline std::ostream&
operator<<(std::ostream& os, const stopwatch<Clock>& rhs)
{
    return os << rhs.to_string();
}

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

} // namespace utilities
