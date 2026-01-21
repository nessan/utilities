# The Stopwatch Class

The `<utilities/stopwatch.h>` header defines `utilities::stopwatch`, a simple stopwatch class for measuring execution time.

You might use it like this:

```c++
utilities::stopwatch sw;
do_work();
std::cout << sw << '\n';
```

The output will be the time taken for the `do_work()` call.
The units are milliseconds if the elapsed time is less than a second, or seconds otherwise.
The time is always shown to two decimal places and suffixed with "ms" or "s" as appropriate.

## Declaration

The header-only class is a template over a specific clock choice, likely one of the clocks from [`std::chrono`].

```c++
template<typename Clock> class utilities::stopwatch;
```

## Notes

The default clock is [`std::chrono::high_resolution_clock`].

Discussions on sites like [Stack Overflow] instead favour [`std::chrono::steady_clock`].
That clock is guaranteed to be monotonic and unaffected by changes to the system time, making it suitable for long-running processes.

However, in practice, we primarily examine much shorter code blocks and value accuracy over "steadiness."
We also note that, in any case, the two clocks are often identical!

If the clock choice matters, you can use one of the following type aliases:

```c++
utilities::precise_stopwatch = utilities::stopwatch<std::chrono::high_resolution_clock>;
utilities::steady_stopwatch  = utilities::stopwatch<std::chrono::steady_clock>;
utilities::system_stopwatch  = utilities::stopwatch<std::chrono::system_clock>;
```

We always store elapsed times as a `double` number of seconds --- this is also contrary to the advice to use [`std::chrono::duration`].

However, the primary goal for `utilities::stopwatch` is ease of use.
Sticking to seconds as the standard unit makes everything consistent.
Besides, a double number of seconds gives you 15 or 16 places of accuracy, which is enough for any conceivable application.

> [!TIP]
> Use the `utilities::stopwatch` class for cheap-and-cheerful performance measurement.
> It is not a replacement for the many more complete, but more complex, code profiling tools.

## Construction

```c++
utilities::stopwatch(const std::string& name = "");
```

Creates a stopwatch and sets its _zero time_ to the current time.
You can give it a name, which is helpful if multiple stopwatches run in one executable.

## Timing Methods

The stopwatch class is kept deliberately simple.

At its core, it measures the elapsed time in seconds from a _zero time_ set on creation or by calling the stopwatch's `reset()` method.
It also supports the idea of _clicking_ a stopwatch to record a _split time — the time, in seconds, from the zero point to the click event.
Finally, it records a \_lap_ time, which is the time between the last two stopwatch clicks.
However, it has no memory beyond that.

```c++
constexpr void reset();             // <1>
constexpr double elapsed() const;   // <2>
constexpr double click();           // <3>
constexpr double split() const;     // <4>
constexpr double lap() const;       // <5>
```

1. Clears out any recorded split time and resets the _zero time_ to now.
2. Returns the number of seconds from the _zero_time_ to now.
3. Creates a _split_ by recording the elapsed time in seconds from the _zero time_ to the `click()` call and returns that time.
4. Returns the last recorded split time in seconds.
5. Returns the time in seconds between the last two splits --- i.e., between the previous two click events.

## Stringification

We have a method to get the elapsed time as a string:

```c++
constexpr std::string to_string() const;
```

This outputs the stopwatch's elapsed time either in milliseconds if the elapsed time is less than a second, or otherwise in seconds. The time is always shown to two decimal places and suffixed with "ms" or "s" as appropriate. If the stopwatch has a name, it is prepended to the output followed by a colon and a space.

## Other Methods

You read and set the name of the stopwatch:

```c++
const std::string& name() const;
std::string& name();
```

## Output Functions

There is the usual stopwatch.

```c++
constexpr std::string to_string() const;
```

We have the usual stream output operator and also a specialisation for [`std::formatter`]:

```c++
template<typename Clock>
std::ostream &
operator<<(std::ostream&, const utilities::stopwatch<Clock>&);

template<typename Clock>
struct std::formatter<utilities::stopwatch<Clock>>;
```

These functions delegate to the stopwatch's `to_string()` method.

We also have a little utility to convert a [`std::chrono::duration`] to a `double` number of seconds.

```c++
template<class Rep, class Period>
constexpr double
to_seconds(const std::chrono::duration<Rep, Period>);
```

## Example

How efficient is it to put a thread to sleep?

```c++
#include <utilities/stopwatch.h>
#include <format>
#include <thread>

int main()
{
    using namespace std::literals;
    utilities::stopwatch sw;

    for (auto sleep_duration = 0ms; sleep_duration <= 2s; sleep_duration += 200ms) {

        sw.click();                                                         // <1>
        std::this_thread::sleep_for(sleep_duration);                        // <2>
        sw.click();                                                         // <3>

        double sleep_ms = 1000 * utilities::to_seconds(sleep_duration);     // <4>
        double actual_ms = 1000 * sw.lap();                                 // <5>
        double diff = actual_ms - sleep_ms;
        double percent = sleep_ms != 0 ? 100 * diff / sleep_ms : 0;

        std::println("Requested sleep for {:8.2f}ms, measured wait was {:8.2f}ms => overhead {:.2f}ms ({:.2f}%)",
                      sleep_ms, actual_ms, diff, percent);
    }
    std::println("Total elapsed time: {}", sw);
    return 0;
}
```

1. Create a stopwatch split to mark the start time.
2. Sleep the current thread for a set number of milliseconds.
3. Create a second split and, hence, a lap that measures the actual sleep time.
4. Convert the requested sleep duration to a double number of seconds.
5. Get the actual sleep time as a double number of seconds from the stopwatch lap.

The program outputs something along the lines:

```txt
Requested sleep for     0.00ms, measured wait was     0.00ms => overhead 0.00ms (0.00%)
Requested sleep for   200.00ms, measured wait was   202.96ms => overhead 2.96ms (1.48%)
Requested sleep for   400.00ms, measured wait was   401.59ms => overhead 1.59ms (0.40%)
Requested sleep for   600.00ms, measured wait was   604.82ms => overhead 4.82ms (0.80%)
Requested sleep for   800.00ms, measured wait was   804.06ms => overhead 4.06ms (0.51%)
Requested sleep for  1000.00ms, measured wait was  1001.97ms => overhead 1.97ms (0.20%)
Requested sleep for  1200.00ms, measured wait was  1202.99ms => overhead 2.99ms (0.25%)
Requested sleep for  1400.00ms, measured wait was  1405.10ms => overhead 5.10ms (0.36%)
Requested sleep for  1600.00ms, measured wait was  1603.67ms => overhead 3.67ms (0.23%)
Requested sleep for  1800.00ms, measured wait was  1803.70ms => overhead 3.70ms (0.21%)
Requested sleep for  2000.00ms, measured wait was  2004.82ms => overhead 4.82ms (0.24%)
Total elapsed time: 11.036255763s
```

<!-- Reference links -->

[`std::chrono`]: https://en.cppreference.com/w/cpp/header/chrono
[`std::chrono::high_resolution_clock`]: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
[`std::chrono::steady_clock`]: https://en.cppreference.com/w/cpp/chrono/steady_clock
[`std::chrono::duration`]: https://en.cppreference.com/w/cpp/chrono/duration
[`std::formatter`]: https://en.cppreference.com/w/cpp/utility/format/formatter
[Stack Overflow]: https://stackoverflow.com
