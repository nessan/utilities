/// @brief Simple utility class that can time how long code blocks take to execute.
/// @link  https://nessan.github.io/utilities/
/// SPDX-FileCopyrightText:  2024 Nessan Fitzmaurice <nessan.fitzmaurice@me.com>
/// SPDX-License-Identifier: MIT
#pragma once

#include <atomic>
#include <chrono>
#include <iostream>

namespace utilities {

template<typename Clock = std::chrono::high_resolution_clock>
class stopwatch {
public:
    /// @brief The underlying clock type
    using clock_type = Clock;

    /// @brief A stopwatch can have a name to distinguish it from others you may have running
    explicit stopwatch(const std::string &str = "") : m_name(str) { reset(); }

    /// @brief Read-only access to the stopwatch's name
    std::string name() const { return m_name; }

    /// @brief Read-write access to the stopwatch's name
    std::string &name() { return m_name; }

    /// @brief Set/reset the stopwatch's 'zero' point & clear any measured splits.
    constexpr void reset()
    {
        m_zero = clock_type::now();
        m_split = 0;
        m_prior = 0;
    }

    /// @brief Get the time that has passed from the zero point to now. Units are seconds.
    constexpr double elapsed() const
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto t = clock_type::now();
        std::atomic_thread_fence(std::memory_order_relaxed);
        return std::chrono::duration<double>(t - m_zero).count();
    }

    /// @brief Clicks the stopwatch to create a new 'split'.
    /// @return The elapsed time to the click in seconds.
    constexpr double click()
    {
        auto tau = elapsed();
        m_prior = m_split;
        m_split = tau;
        return m_split;
    }

    /// @brief Returns the split as the time in seconds that elapsed from the zero point to the last click.
    constexpr double split() const { return m_split; }

    /// @brief Returns the last 'lap' time in seconds (i.e. the time between prior 2 splits).
    constexpr double lap() const { return m_split - m_prior; }

    /// @brief Get a string representation of the stopwatch's elapsed time.
    std::string to_string() const {
        auto tau = elapsed();
        if(m_name.empty()) return std::format("{}s", tau);
        return std::format("{}: {}s", m_name, tau);
    }

private:
    using time_point = typename Clock::time_point;

    std::string m_name;  // Name of the stopwatch.
    time_point  m_zero;  // The stopwatch measures time (in seconds) from this time point.
    double      m_split; // The total seconds to when the stopwatch was most recently clicked.
    double      m_prior; // The prior split.
};

/// @brief Usual output operator. Prints the name of the stopwatch if any followed by the elapsed time in seconds.
template<typename Clock>
inline std::ostream &
operator<<(std::ostream &os, const stopwatch<Clock> &rhs)
{
    return os << rhs.to_string();
}

/// @brief Small convenience function that converts a std::chrono::duration to seconds in a double.
template<class Rep, class Period>
constexpr double
to_seconds(const std::chrono::duration<Rep, Period> &d)
{
    return std::chrono::duration<double>(d).count();
}

/// @brief stopwatch specialization: The most precise stopwatch -- may get put off by system reboots etc.
using precise_stopwatch = stopwatch<std::chrono::high_resolution_clock>;

/// @brief stopwatch specialization: A stopwatch that is guaranteed to be monotonic.
using steady_stopwatch = stopwatch<std::chrono::steady_clock>;

/// @brief stopwatch specialization:  A stopwatch that is uses the system clock.
using system_stopwatch = stopwatch<std::chrono::system_clock>;

} // namespace utilities
