/// Run though some formatting tests - make sure that strings come out as expected

#include "utilities/formatter.h"
#include <iostream>
#include <string>

#define CHECK_EQUAL(code, expect)                                                      \
    code;                                                                              \
    if (str != expect) {                                                               \
        std::cout << "Test failed, line " << __LINE__ << "\n";                         \
        std::cout << "str = '" << str << "' instead of expected '" << expect << "'\n"; \
        std::cout << "FAILURE FROM> " << #code << '\n';                                \
        ++n_failures;                                                                  \
    }                                                                                  \
    ++n_tests;

int
main()
{
    std::string str;
    std::size_t n_tests = 0;
    std::size_t n_failures = 0;

    // Test the strings we get by parsing various  basic formats
    CHECK_EQUAL(str = std::format("{:s}", "asdf"), "asdf")
    CHECK_EQUAL(str = std::format("{:d}", 1234), "1234")
    CHECK_EQUAL(str = std::format("{:d}", -5678), "-5678")
    CHECK_EQUAL(str = std::format("{:o}", 012), "12")
    CHECK_EQUAL(str = std::format("{:d}", 123456u), "123456")
    CHECK_EQUAL(str = std::format("{:x}", 0xdeadbeef), "deadbeef")
    CHECK_EQUAL(str = std::format("{:X}", 0xDEADBEEF), "DEADBEEF")
    CHECK_EQUAL(str = std::format("{:e}", 1.23456e10), "1.234560e+10")
    CHECK_EQUAL(str = std::format("{:E}", -1.23456E10), "-1.234560E+10")
    CHECK_EQUAL(str = std::format("{:f}", -9.8765), "-9.876500")
    CHECK_EQUAL(str = std::format("{:F}", 9.8765), "9.876500")
    CHECK_EQUAL(str = std::format("{:g}", 10.0), "10")
    CHECK_EQUAL(str = std::format("{:G}", 100.0), "100")
    CHECK_EQUAL(str = std::format("{:c}", 65), "A")
    CHECK_EQUAL(str = std::format("{:s}", "asdf_123098"), "asdf_123098")

    // Test the strings we get by parsing booleans as ints or strings
    CHECK_EQUAL(str = std::format("{:s}", true), "true")
    CHECK_EQUAL(str = std::format("{:d}", true), "1")

    // Test precision & width
    CHECK_EQUAL(str = std::format("{:10d}", -10), "       -10")
    CHECK_EQUAL(str = std::format("{:04d}", 10), "0010")
    CHECK_EQUAL(str = std::format("{:10.4f}", 1234.1234567890), " 1234.1235")
    CHECK_EQUAL(str = std::format("{:.0f}", 10.49), "10")
    CHECK_EQUAL(str = std::format("{:.0f}", 10.51), "11")
    CHECK_EQUAL(str = std::format("{:.2s}", "asdf"), "as")

    // Test "flags"
    CHECK_EQUAL(str = std::format("{:#x}", 0x271828), "0x271828")
    CHECK_EQUAL(str = std::format("{:#o}", 0x271828), "011614050")
    CHECK_EQUAL(str = std::format("{:#f}", 3.0), "3.000000")
    CHECK_EQUAL(str = std::format("{:010d}", 100), "0000000100")
    CHECK_EQUAL(str = std::format("{:010d}", -10), "-000000010")
    CHECK_EQUAL(str = std::format("{:#010X}", 0xBEEF), "0X0000BEEF")
    CHECK_EQUAL(str = std::format("{:+10d}", 10), "       +10")
    CHECK_EQUAL(str = std::format("{:-10d}", -10), "       -10")
    CHECK_EQUAL(str = std::format("{:10d}", -10), "       -10")
    CHECK_EQUAL(str = std::format("{:<10d}", 10), "10        ")
    CHECK_EQUAL(str = std::format("{:<10d}", -10), "-10       ")

    // Complicated string
    auto X = static_cast<int>('X');
    CHECK_EQUAL(str = std::format("{:0.10f} - {:04d} - {:+g} - {:s} - {:#X} - {:c}", 1.234, 42, 3.13,
                                  "some string or other", 0XDEAD, X),
                "1.2340000000 - 0042 - +3.13 - some string or other - 0XDEAD - X")

    std::cout << "Number of tests = " << n_tests << ", failures = " << n_failures << ".\n";

    return 0;
}
