#include <stlab/enum_ops.hpp>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// Legacy-style unscoped enum with both bitmask and arithmetic enabled
enum number {
    num_neg_one = -1,
    num_0 = 0,
    num_1 = 1,
    num_2 = 2,
    num_3 = 3,
    num_4 = 4,
    num_5 = 5,
    num_6 = 6,
    num_7 = 7
};

// Bitmask enum classes
enum class views : int { none = 0, text = 1 << 0, icon = 1 << 1, preview = 1 << 2 };
enum class flags : unsigned int { none = 0, a = 1u << 0, b = 1u << 1 };

// Arithmetic enum classes
enum class num : int {
    num_0 = 0,
    num_1 = 1,
    num_2 = 2,
    num_3 = 3,
    num_4 = 4,
    num_5 = 5,
    num_6 = 6,
    num_7 = 7
};
enum class counter : int { zero = 0, one = 1, two = 2 };

// Enable operations
auto stlab_enable_bitmask_enum(views) -> std::true_type;
auto stlab_enable_bitmask_enum(flags) -> std::true_type;
auto stlab_enable_bitmask_enum(number) -> std::true_type;
auto stlab_enable_arithmetic_enum(number) -> std::true_type;
auto stlab_enable_arithmetic_enum(num) -> std::true_type;
auto stlab_enable_arithmetic_enum(counter) -> std::true_type;

TEST_CASE("bitmask ops: unscoped number") {
    number x{};
    x = num_1 | num_2;
    CHECK(x == 3);
    x = num_3 & num_1;
    CHECK(x == 1);
    x = num_2 ^ num_3;
    CHECK(x == 1);
    x = num_7 & (~num_5);
    CHECK(x == 2);

    // Shifts (value-returning and assignment variants)
    // Shifts: test only assignment forms to avoid ambiguity with built-ins
    x = num_1;
    x <<= 2;
    CHECK(x == 4);
    x >>= 1;
    CHECK(x == 2);

    // Subtraction with value convertible to underlying
    CHECK((num_7 - 1) == 6);
}

TEST_CASE("bitmask ops: enum class flags/views") {
    flags f = flags::a | flags::b;
    CHECK((f & flags::a) == flags::a);
    CHECK((f & flags::b) == flags::b);
    f &= flags::a;
    CHECK(f == flags::a);
    f |= flags::b;
    CHECK((f & flags::b) == flags::b);
    CHECK(~flags::none != flags::none);

    // Shifts on enum class
    views v = views::text;
    v <<= 1;
    CHECK(v == views::icon);
    v >>= 1;
    CHECK(v == views::text);
}

TEST_CASE("arithmetic ops: unscoped number") {
    number x{};
    x = num_1 + num_2;
    CHECK(x == 3);
    x = num_3 - num_1;
    CHECK(x == 2);
    x = -num_neg_one;
    CHECK(x == 1);
    x = +num_2;
    CHECK(x == 2);
    x = num_2 * 3;
    CHECK(x == 6);
    x = 3 * num_2; // scalar on LHS
    CHECK(x == 6);
    x = num_5 / 2;
    CHECK(x == 2);
    x = num_5 % 2;
    CHECK(x == 1);

    x = num_1;
    x += num_2;
    CHECK(x == 3);
    x = num_3;
    x -= num_1;
    CHECK(x == 2);
    x = num_2;
    x *= 3;
    CHECK(x == 6);
    x = num_5;
    x /= 2;
    CHECK(x == 2);
    x = num_5;
    x %= 2;
    CHECK(x == 1);

    x = num_1;
    CHECK(++x == 2);
    CHECK(x == 2);
    CHECK(x++ == 2);
    CHECK(x == 3);
    CHECK(--x == 2);
    CHECK(x-- == 2);
    CHECK(x == 1);
}

TEST_CASE("arithmetic ops: enum class num/counter") {
    num x = num::num_1;
    CHECK((x + num::num_3) == num::num_4);
    x += num::num_1;
    CHECK(x == num::num_2);
    x -= num::num_1;
    CHECK(x == num::num_1);

    counter c = counter::one;
    c += counter::one;
    CHECK(c == counter::two);
    c -= counter::one;
    CHECK(c == counter::one);
}

TEST_CASE("nullptr comparisons and logical not") {
    // Bitmask-enabled
    flags f = flags::none;
    // Double parenthesis to avoid ambiguity with doctest expression decomp
    CHECK((f == 0));
    f |= flags::a;
    CHECK((f != 0));
    CHECK(!flags::none);

    // Arithmetic-enabled
    number n = num_0;
    CHECK((n == 0));
    n = num_1;
    CHECK((n != 0));
    CHECK(!num_0);
}
