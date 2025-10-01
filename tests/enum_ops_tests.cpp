#include <stlab/enum_ops.hpp>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <type_traits>

/**************************************************************************************************/
// Test enum definitions with different underlying types and capabilities

// Bitmask-only enums
enum class bitmask_flags : unsigned {
    none = 0,
    flag_a = 1u << 0,
    flag_b = 1u << 1,
    flag_c = 1u << 2,
    flag_d = 1u << 3,
    all = flag_a | flag_b | flag_c | flag_d
};

enum class permissions : unsigned {
    none = 0,
    read = 1,
    write = 2,
    execute = 4,
    all = read | write | execute
};

// Arithmetic-only enums
enum class arithmetic_int : int {
    neg_two = -2,
    neg_one = -1,
    zero = 0,
    one = 1,
    two = 2,
    three = 3
};

enum class arithmetic_char : char { zero = 0, one = 1, two = 2, max_value = 127 };

// Combined arithmetic and bitmask (unscoped for legacy compatibility)
enum mixed_enum : int {
    mixed_none = 0,
    mixed_one = 1,
    mixed_two = 2,
    mixed_four = 4,
    mixed_neg = -1
};

// Non-enabled enum for negative tests
enum class disabled_enum : int { value_a = 1, value_b = 2 };

// Enable operations for specific enums
auto stlab_enable_bitmask_enum(bitmask_flags) -> std::true_type;
auto stlab_enable_bitmask_enum(permissions) -> std::true_type;
auto stlab_enable_bitmask_enum(mixed_enum) -> std::true_type;

auto stlab_enable_arithmetic_enum(arithmetic_int) -> std::true_type;
auto stlab_enable_arithmetic_enum(arithmetic_char) -> std::true_type;
auto stlab_enable_arithmetic_enum(mixed_enum) -> std::true_type;

/**************************************************************************************************/
// BITMASK OPERATIONS TESTS

TEST_CASE("Bitmask: bitwise OR operator") {
    // Test enum class with unsigned underlying type
    auto result = bitmask_flags::flag_a | bitmask_flags::flag_b;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(result == bitmask_flags{3u});
    CHECK((result & bitmask_flags::flag_a) == bitmask_flags::flag_a);
    CHECK((result & bitmask_flags::flag_b) == bitmask_flags::flag_b);

    // Test enum class with signed underlying type
    auto perms = permissions::read | permissions::write;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(perms == permissions{3});

    // Test unscoped enum
    auto mixed = mixed_one | mixed_two;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(mixed == mixed_enum{3});

    // Verify result type is same as operands
    static_assert(
        std::is_same_v<decltype(bitmask_flags::flag_a | bitmask_flags::flag_b), bitmask_flags>);
}

TEST_CASE("Bitmask: bitwise AND operator") {
    auto flags = bitmask_flags::flag_a | bitmask_flags::flag_b | bitmask_flags::flag_c;

    CHECK((flags & bitmask_flags::flag_a) == bitmask_flags::flag_a);
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::flag_b);
    CHECK((flags & bitmask_flags::none) == bitmask_flags::none);

    // Test isolation of single bit
    CHECK((bitmask_flags::all & bitmask_flags::flag_c) == bitmask_flags::flag_c);

    static_assert(
        std::is_same_v<decltype(bitmask_flags::flag_a & bitmask_flags::flag_b), bitmask_flags>);
}

TEST_CASE("Bitmask: bitwise XOR operator") {
    auto result = bitmask_flags::flag_a ^ bitmask_flags::flag_b;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(result == bitmask_flags{3u});

    // XOR with self should give zero
    CHECK((bitmask_flags::flag_a ^ bitmask_flags::flag_a) == bitmask_flags::none);

    // Toggle operation
    auto flags = bitmask_flags::flag_a;
    flags = flags ^ bitmask_flags::flag_b; // Add flag_b
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::flag_b);
    flags = flags ^ bitmask_flags::flag_b; // Remove flag_b
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::none);

    static_assert(
        std::is_same_v<decltype(bitmask_flags::flag_a ^ bitmask_flags::flag_b), bitmask_flags>);
}

TEST_CASE("Bitmask: bitwise NOT operator") {
    auto result = ~bitmask_flags::none;
    CHECK(result != bitmask_flags::none);

    // Double negation should return to original (within meaningful bits)
    auto original = bitmask_flags::flag_a | bitmask_flags::flag_b;
    auto meaningful_bits = bitmask_flags::all;
    CHECK(((~(~original)) & meaningful_bits) == original);

    static_assert(std::is_same_v<decltype(~bitmask_flags::flag_a), bitmask_flags>);
}

TEST_CASE("Bitmask: shift operators") {
    auto result = bitmask_flags::flag_a << 1;
    CHECK(result == bitmask_flags::flag_b);

    result = bitmask_flags::flag_b << 1;
    CHECK(result == bitmask_flags::flag_c);

    // Right shift
    result = bitmask_flags::flag_b >> 1;
    CHECK(result == bitmask_flags::flag_a);

    result = bitmask_flags::flag_c >> 2;
    CHECK(result == bitmask_flags::flag_a);

    // Test with permissions enum
    auto perm_result = permissions::execute >> 1;
    CHECK(perm_result == permissions::write);

    static_assert(std::is_same_v<decltype(bitmask_flags::flag_a << 1), bitmask_flags>);
    static_assert(std::is_same_v<decltype(bitmask_flags::flag_a >> 1), bitmask_flags>);
}

TEST_CASE("Bitmask: assignment operators") {
    // OR assignment
    bitmask_flags flags = bitmask_flags::none;
    flags |= bitmask_flags::flag_a;
    CHECK((flags & bitmask_flags::flag_a) == bitmask_flags::flag_a);

    flags |= bitmask_flags::flag_b;
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::flag_b);
    CHECK((flags & bitmask_flags::flag_a) == bitmask_flags::flag_a);

    // AND assignment
    flags &= bitmask_flags::flag_a;
    CHECK(flags == bitmask_flags::flag_a);

    // XOR assignment
    flags ^= bitmask_flags::flag_b; // Add flag_b
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::flag_b);
    flags ^= bitmask_flags::flag_b; // Remove flag_b
    CHECK((flags & bitmask_flags::flag_b) == bitmask_flags::none);

    // Shift assignments
    flags = bitmask_flags::flag_a;
    flags <<= 1;
    CHECK(flags == bitmask_flags::flag_b);
    flags >>= 1;
    CHECK(flags == bitmask_flags::flag_a);
}

TEST_CASE("Bitmask: clear_least_set") {
    auto result = bitmask_flags::flag_c | bitmask_flags::flag_b;
    CHECK(bitmask_flags::flag_c == (result & (result - 1)));

    auto perm_result = permissions::read | permissions::write;
    CHECK(permissions::write == (perm_result & (perm_result - 1)));
}

TEST_CASE("Bitmask: least_set") {
    auto result = bitmask_flags::flag_c | bitmask_flags::flag_b;
    CHECK(bitmask_flags::flag_b == (result & -result));

    auto perm_result = permissions::read | permissions::write;
    CHECK(permissions::read == (perm_result & -perm_result));
}

TEST_CASE("Bitmask: clear_trailing_set") {
    auto result = bitmask_flags::flag_d | bitmask_flags::flag_b | bitmask_flags::flag_a;
    CHECK(bitmask_flags::flag_d == (result & (result + 1)));
}
/**************************************************************************************************/
// ARITHMETIC OPERATIONS TESTS

TEST_CASE("Arithmetic: unary operators") {
    auto plus_result = +arithmetic_int::two;
    CHECK(plus_result == arithmetic_int::two);

    auto minus_result = -arithmetic_int::one;
    CHECK(minus_result == arithmetic_int::neg_one);

    auto double_negative = -arithmetic_int::neg_one;
    CHECK(double_negative == arithmetic_int::one);

    static_assert(std::is_same_v<decltype(+arithmetic_int::one), arithmetic_int>);
    static_assert(std::is_same_v<decltype(-arithmetic_int::one), arithmetic_int>);
}

TEST_CASE("Arithmetic: binary operators") {
    // Addition
    auto add_result = arithmetic_int::one + arithmetic_int::two;
    CHECK(add_result == arithmetic_int::three);

    // Subtraction
    auto sub_result = arithmetic_int::three - arithmetic_int::one;
    CHECK(sub_result == arithmetic_int::two);

    // Multiplication (enum * scalar)
    auto mult_result = arithmetic_int::two * 3;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(mult_result == arithmetic_int{6});

    // Multiplication (scalar * enum)
    auto mult_result2 = 3 * arithmetic_int::two;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(mult_result2 == arithmetic_int{6});

    // Division
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    auto div_result = arithmetic_int{6} / 2;
    CHECK(div_result == arithmetic_int::three);

    // Modulo
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    auto mod_result = arithmetic_int{5} % 2;
    CHECK(mod_result == arithmetic_int::one);

    static_assert(
        std::is_same_v<decltype(arithmetic_int::one + arithmetic_int::two), arithmetic_int>);
    static_assert(std::is_same_v<decltype(arithmetic_int::two * 3), arithmetic_int>);
    static_assert(std::is_same_v<decltype(3 * arithmetic_int::two), arithmetic_int>);
}

TEST_CASE("Arithmetic: assignment operators") {
    // Addition assignment
    arithmetic_int value = arithmetic_int::one;
    value += arithmetic_int::two;
    CHECK(value == arithmetic_int::three);

    // Subtraction assignment
    value = arithmetic_int::three;
    value -= arithmetic_int::one;
    CHECK(value == arithmetic_int::two);

    // Multiplication assignment
    value = arithmetic_int::two;
    value *= 3;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(value == arithmetic_int{6});

    // Division assignment
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    value = arithmetic_int{6};
    value /= 2;
    CHECK(value == arithmetic_int::three);

    // Modulo assignment
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    value = arithmetic_int{7};
    value %= 3;
    CHECK(value == arithmetic_int::one);

    static_assert(std::is_same_v<decltype(value += arithmetic_int::one), arithmetic_int&>);
    static_assert(std::is_same_v<decltype(value *= 2), arithmetic_int&>);
}

TEST_CASE("Arithmetic: increment and decrement operators") {
    arithmetic_int value = arithmetic_int::one;

    // Prefix increment
    auto pre_inc_result = ++value;
    CHECK(pre_inc_result == arithmetic_int::two);
    CHECK(value == arithmetic_int::two);

    // Postfix increment
    auto post_inc_result = value++;
    CHECK(post_inc_result == arithmetic_int::two);
    CHECK(value == arithmetic_int::three);

    // Prefix decrement
    auto pre_dec_result = --value;
    CHECK(pre_dec_result == arithmetic_int::two);
    CHECK(value == arithmetic_int::two);

    // Postfix decrement
    auto post_dec_result = value--;
    CHECK(post_dec_result == arithmetic_int::two);
    CHECK(value == arithmetic_int::one);

    static_assert(std::is_same_v<decltype(++value), arithmetic_int&>);
    static_assert(std::is_same_v<decltype(value++), arithmetic_int>);
    static_assert(std::is_same_v<decltype(--value), arithmetic_int&>);
    static_assert(std::is_same_v<decltype(value--), arithmetic_int>);
}

/**************************************************************************************************/
// COMPARISON AND LOGICAL OPERATIONS TESTS

TEST_CASE("Comparison: nullptr equality") {
    // Bitmask enums - use parentheses to avoid doctest expression decomposition conflicts
    CHECK((bitmask_flags::none == nullptr));
    CHECK((nullptr == bitmask_flags::none));
    CHECK_FALSE((bitmask_flags::flag_a == nullptr));
    CHECK_FALSE((nullptr == bitmask_flags::flag_a));

    CHECK((permissions::none == nullptr));
    CHECK_FALSE((permissions::read == nullptr));

    // Arithmetic enums
    CHECK((arithmetic_int::zero == nullptr));
    CHECK((nullptr == arithmetic_int::zero));
    CHECK_FALSE((arithmetic_int::one == nullptr));
    CHECK_FALSE((nullptr == arithmetic_int::one));

    CHECK((arithmetic_char::zero == nullptr));
    CHECK_FALSE((arithmetic_char::one == nullptr));
}

TEST_CASE("Comparison: nullptr inequality") {
    // Bitmask enums - use parentheses to avoid doctest expression decomposition conflicts
    CHECK_FALSE((bitmask_flags::none != nullptr));
    CHECK_FALSE((nullptr != bitmask_flags::none));
    CHECK((bitmask_flags::flag_a != nullptr));
    CHECK((nullptr != bitmask_flags::flag_a));

    // Arithmetic enums
    CHECK_FALSE((arithmetic_int::zero != nullptr));
    CHECK_FALSE((nullptr != arithmetic_int::zero));
    CHECK((arithmetic_int::one != nullptr));
    CHECK((nullptr != arithmetic_int::one));
}

TEST_CASE("Logical: NOT operator") {
    // Bitmask enums
    CHECK(!bitmask_flags::none);
    CHECK_FALSE(!bitmask_flags::flag_a);
    CHECK_FALSE(!(bitmask_flags::flag_a | bitmask_flags::flag_b));

    CHECK(!permissions::none);
    CHECK_FALSE(!permissions::read);

    // Arithmetic enums
    CHECK(!arithmetic_int::zero);
    CHECK_FALSE(!arithmetic_int::one);
    CHECK_FALSE(!arithmetic_int::neg_one);

    CHECK(!arithmetic_char::zero);
    CHECK_FALSE(!arithmetic_char::one);
}

/**************************************************************************************************/
// MIXED ENUM TESTS

TEST_CASE("Mixed enum: both bitmask and arithmetic operations") {
    // Bitmask operations
    auto bitmask_result = mixed_one | mixed_two;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(bitmask_result == mixed_enum{3});

    auto and_result = mixed_four & mixed_one;
    CHECK(and_result == mixed_none);

    // Arithmetic operations
    auto arith_result = mixed_one + mixed_two;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(arith_result == mixed_enum{3});

    auto mult_result = mixed_two * 2;
    CHECK(mult_result == mixed_four);

    // Assignment operations
    mixed_enum flags = mixed_one;
    flags |= mixed_two;
    CHECK((flags & mixed_two) == mixed_two);

    mixed_enum value = mixed_two;
    value += mixed_one;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(value == mixed_enum{3});
}

/**************************************************************************************************/
// CONSTEXPR TESTS

TEST_CASE("Constexpr: compile-time evaluation") {
    // Bitmask operations
    constexpr auto or_result = bitmask_flags::flag_a | bitmask_flags::flag_b;
    constexpr auto and_result = bitmask_flags::all & bitmask_flags::flag_a;
    constexpr auto xor_result = bitmask_flags::flag_a ^ bitmask_flags::flag_b;
    constexpr auto not_result = ~bitmask_flags::none;
    constexpr auto shift_left_result = bitmask_flags::flag_a << 1;
    constexpr auto shift_right_result = bitmask_flags::flag_b >> 1;

    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(or_result == bitmask_flags{3u});
    CHECK(and_result == bitmask_flags::flag_a);
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(xor_result == bitmask_flags{3u});
    CHECK(not_result != bitmask_flags::none);
    CHECK(shift_left_result == bitmask_flags::flag_b);
    CHECK(shift_right_result == bitmask_flags::flag_a);

    // Arithmetic operations
    constexpr auto add_result = arithmetic_int::one + arithmetic_int::two;
    constexpr auto sub_result = arithmetic_int::three - arithmetic_int::one;
    constexpr auto mult_result = arithmetic_int::two * 3;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    constexpr auto div_result = arithmetic_int{6} / 2;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    constexpr auto mod_result = arithmetic_int{5} % 2;
    constexpr auto unary_minus_result = -arithmetic_int::one;
    constexpr auto unary_plus_result = +arithmetic_int::two;

    CHECK(add_result == arithmetic_int::three);
    CHECK(sub_result == arithmetic_int::two);
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    CHECK(mult_result == arithmetic_int{6});
    CHECK(div_result == arithmetic_int::three);
    CHECK(mod_result == arithmetic_int::one);
    CHECK(unary_minus_result == arithmetic_int::neg_one);
    CHECK(unary_plus_result == arithmetic_int::two);
}

/**************************************************************************************************/
// DOCUMENTATION: NEGATIVE TESTS
//
// The following operations should NOT compile (SFINAE should prevent them):
//
// Disabled enum operations:
// - disabled_enum::value_a | disabled_enum::value_b  (no bitmask ops)
// - disabled_enum::value_a + disabled_enum::value_b  (no arithmetic ops)
// - ~disabled_enum::value_a                          (no bitwise NOT)
// - disabled_enum::value_a * 2                       (no multiplication)
// - disabled_enum::value_a << 1                      (no shifts)
// - disabled_enum::value_a == nullptr                (no nullptr comparison)
//
// Wrong operation type for enabled enums:
// - bitmask_flags::flag_a + bitmask_flags::flag_b    (bitmask-only, no arithmetic)
// - arithmetic_int::one | arithmetic_int::two        (arithmetic-only, no bitmask)
// - arithmetic_int::one << 1                         (arithmetic-only, no shifts)
/**************************************************************************************************/
