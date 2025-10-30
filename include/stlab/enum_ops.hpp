/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_ENUM_OPS_HPP
#define STLAB_ENUM_OPS_HPP

/**************************************************************************************************/

#include <cassert>
#include <type_traits>

/**************************************************************************************************/

/*!
    \file
    \brief Typesafe operators for enum types enabled via opt-in customization points.
*/

/*!
    \defgroup CustomizationPoints Customization Points
    \brief Functions to enable typesafe operations for enum types

    These functions must be overloaded in your enum's namespace to enable the corresponding
    operations for your enum type.
*/

/*!
    \defgroup Traits Traits
    \brief Traits for enum types

    These traits are used to determine if an enum type has enabled bitmask or arithmetic operations.
*/

/*!
    \defgroup BitmaskOperations Bitmask Operations
    \brief Bitwise operations for bitmask-enabled enums

    These operations are available when you enable bitmask operations by defining:
    ```cpp
    auto stlab_enable_bitmask_enum(YourEnum) -> std::true_type;
    ```
*/

/*!
    \defgroup ArithmeticOperations Arithmetic Operations
    \brief Arithmetic operations for arithmetic-enabled enums

    These operations are available when you enable arithmetic operations by defining:
    ```cpp
    auto stlab_enable_arithmetic_enum(YourEnum) -> std::true_type;
    ```
*/

/*!
    \defgroup CommonOperations Common Operations
    \brief Operations available for both bitmask and arithmetic-enabled enums

    These operations are available for enums that have either bitmask or arithmetic operations
   enabled.
*/

/*!
    \mainpage

    [![View on
   GitHub](https://img.shields.io/badge/GitHub-enum--ops-181717?logo=github&style=flat)](https://github.com/stlab/enum-ops)

    \section Description Description

    \c enum_ops provides optional typesafe bitset and arithmetic operations for enumeration
   types. Without these typesafe operations, the compiler will promote the operand(s) to the
   appropriate integral type, and the result will be an integral type. When the typesafe
   operations have been defined for an enumeration type, \c E, the result will be of type \c E
   exactly when the operand(s) are of type \c E.

    \section Operations Available Operations

    \subsection CustomizationPointsSection Customization Points
    \copydoc CustomizationPoints
    - \ref CustomizationPoints "View all customization functions"

    \subsection BitmaskOperationsSection Bitmask Operations
    \copydoc BitmaskOperations
    - \ref BitmaskOperations "View all bitmask operations"

    \subsection ArithmeticOperationsSection Arithmetic Operations
    \copydoc ArithmeticOperations
    - \ref ArithmeticOperations "View all arithmetic operations"

    \subsection CommonOperationsSection Common Operations
    \copydoc CommonOperations
    - \ref CommonOperations "View all common operations"

    \section Definition Definition

    Defined in \link enum_ops.hpp <code>stlab/enum_ops.hpp</code> \endlink

    \section Example Example

    The following is an example of code that will compile:

    \dontinclude enum_ops_example_test.cpp
    \skip EXAMPLE_START
    \until OMIT_START
    \skip OMIT_END
    \until EXAMPLE_END


    The following is contains an example of code that will not compile
    since the typesafe operators have not been defined.

    \include enum_ops_example_fail.cpp
*/

/**************************************************************************************************/

/// The stlab namespace.
namespace stlab {

/**************************************************************************************************/

/// \addtogroup CustomizationPoints
/// \{

/// Overload this for your enum in the enum namespace to return std::true_type and enable
/// bitwise operators.
auto stlab_enable_bitmask_enum(...) -> std::false_type;

/// Overload this for your enum in the enum namespace to return std::true_type and enable
/// arithmetic operators.
auto stlab_enable_arithmetic_enum(...) -> std::false_type;

// Don't use the `\deprecated` Doxygen tag here because clang will warn that the
// documentation marks the operations deprecated but the deprecated attribute is missing.

/// \note Use is **deprecated**. Use stlab_enable_bitmask_enum instead.
auto adobe_enable_bitmask_enum(...) -> std::false_type;

/// \note Use is **deprecated**. Use stlab_enable_arithmetic_enum instead.
auto adobe_enable_arithmetic_enum(...) -> std::false_type;

/// \}

/**************************************************************************************************/

/// The implementation namespace.
namespace implementation {

/**************************************************************************************************/

template <class T>
using has_enabled_bitmask_t = decltype(stlab_enable_bitmask_enum(std::declval<T>()));

template <class T>
using has_deprecated_bitmask_t = decltype(adobe_enable_bitmask_enum(std::declval<T>()));

template <class T>
constexpr bool has_enabled_bitmask =
    has_enabled_bitmask_t<T>::value || has_deprecated_bitmask_t<T>::value;

template <class T>
using has_enabled_arithmetic_t = decltype(stlab_enable_arithmetic_enum(std::declval<T>()));

template <class T>
using has_deprecated_arithmetic_t = decltype(adobe_enable_arithmetic_enum(std::declval<T>()));

template <class, bool>
struct safe_underlying_type;

template <class T>
struct safe_underlying_type<T, true> {
    using type = std::underlying_type_t<T>;
};

template <class T>
struct safe_underlying_type<T, false> {
    using type = void;
};

template <class T>
using safe_underlying_type_t = typename safe_underlying_type<T, std::is_enum<T>::value>::type;

template <class U, class T>
using is_convertible_to_underlying =
    std::is_convertible<U, stlab::implementation::safe_underlying_type_t<T>>;

/**************************************************************************************************/

} // namespace implementation

/**************************************************************************************************/

/// \addtogroup Traits
/// \{

/// Whether the enum type has enabled bitmask operations.
template <class T>
constexpr bool has_enabled_bitmask = implementation::has_enabled_bitmask_t<T>::value ||
                                     implementation::has_deprecated_bitmask_t<T>::value;

/// Whether the enum type has enabled arithmetic operations.
template <class T>
constexpr bool has_enabled_arithmetic = implementation::has_enabled_arithmetic_t<T>::value ||
                                        implementation::has_deprecated_arithmetic_t<T>::value;

/// Whether the scalar type `U` is compatible with the enum type `T`.
template <class U, class T>
constexpr bool is_compatible_scalar = implementation::is_convertible_to_underlying<U, T>::value;

/// \}

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

/// \addtogroup BitmaskOperations
/// \{

template <class T>
/// Bitwise AND for bitmask-enabled enums; returns the same enum type.
constexpr auto operator&(T lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <class T>
/// Bitwise NOT for bitmask-enabled enums; returns the same enum type.
constexpr auto operator~(T a) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(~static_cast<underlying>(a));
}

template <class T>
/// Bitwise OR for bitmask-enabled enums.
constexpr auto operator|(T lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <class T>
/// Bitwise XOR for bitmask-enabled enums.
constexpr auto operator^(T lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <class T>
/// Left shift for bitmask-enabled enums.
constexpr auto operator<<(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned_t<std::underlying_type_t<T>>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) << static_cast<underlying>(rhs));
}

template <class T>
/// Right shift for bitmask-enabled enums.
constexpr auto operator>>(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned_t<std::underlying_type_t<T>>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) >> static_cast<underlying>(rhs));
}

template <class T>
/// XOR-assign for bitmask-enabled enums.
constexpr auto operator^=(T& lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T&> {
    return lhs = lhs ^ rhs;
}

template <class T>
/// AND-assign for bitmask-enabled enums.
constexpr auto operator&=(T& lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T&> {
    return lhs = lhs & rhs;
}

template <class T>
/// OR-assign for bitmask-enabled enums.
constexpr auto operator|=(T& lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T&> {
    return lhs = lhs | rhs;
}

template <class T>
/// Left shift-assign for bitmask-enabled enums.
constexpr auto operator<<=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T&> {
    return lhs = lhs << rhs;
}

template <class T>
/// Right shift-assign for bitmask-enabled enums.
constexpr auto operator>>=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T&> {
    return lhs = lhs >> rhs;
}

template <class T, class U>
/// Subtracts a 0 or 1 scalar value from a bitmask-enabled enum.
/// Allows expressions like `e & (e - 1)` to clear the least set bit.
constexpr auto operator-(T lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T> && stlab::is_compatible_scalar<U, T>, T> {
    assert(rhs == 0 || rhs == 1);
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

template <class T>
/// Subtracts a bitmask-enabled enum from `0`
/// `0 - rhs` is equivalent to `-rhs`.
constexpr auto operator-(std::nullptr_t lhs, T rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T>, T> {
    return -rhs;
}

template <class T, class U>
/// Adds a 0 or 1 scalar value to a bitmask-enabled enum.
/// Allows expressions like `e & (e + 1)` to clear trailing set bits.
constexpr auto operator+(T lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T> && stlab::is_compatible_scalar<U, T>, T> {
    assert(rhs == 0 || rhs == 1);
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) + static_cast<underlying>(rhs));
}

template <class U, class T>
/// Adds a bitmask-enabled enum to `0` or `1`.
/// Allows expressions like `e & (1 + e)` to clear trailing set bits.
constexpr auto operator+(U lhs, T rhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T> && stlab::is_compatible_scalar<U, T>, T> {
    assert(lhs == 0 || lhs == 1);
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) + static_cast<underlying>(rhs));
}

/// \}

/**************************************************************************************************/

/// \addtogroup ArithmeticOperations
/// \{

template <class T>
/// Unary plus for arithmetic-enabled enums.
constexpr auto operator+(T a) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(+static_cast<underlying>(a));
}

template <class T>
/// Addition for arithmetic-enabled enums.
constexpr auto operator+(T lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) + static_cast<underlying>(rhs));
}

template <class T>
/// Subtraction for arithmetic-enabled enums.
constexpr auto operator-(T lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

template <class T, class U>
/// Multiplication by a scalar value.
constexpr auto operator*(T lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) * rhs);
}

template <class U, class T>
/// Multiplication by a scalar value.
constexpr auto operator*(U lhs, T rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(lhs * static_cast<underlying>(rhs));
}

template <class T, class U>
/// Division by a scalar value.
constexpr auto operator/(T lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) / rhs);
}

template <class T, class U>
/// Modulo by a scalar value.
constexpr auto operator%(T lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) % rhs);
}

template <class T>
/// Addition assignment for arithmetic-enabled enums.
constexpr auto operator+=(T& lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs + rhs;
}

template <class T>
/// Subtraction assignment for arithmetic-enabled enums.
constexpr auto operator-=(T& lhs, T rhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs - rhs;
}

template <class T, class U>
/// Multiplication assignment by a scalar value.
constexpr auto operator*=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T&> {
    return lhs = lhs * rhs;
}

template <class T, class U>
/// Division assignment by a scalar value.
constexpr auto operator/=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T&> {
    return lhs = lhs / rhs;
}

template <class T, class U>
/// Modulo assignment by a scalar value.
constexpr auto operator%=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> && stlab::is_compatible_scalar<U, T>, T&> {
    return lhs = lhs % rhs;
}

template <class T>
/// Prefix increment for arithmetic-enabled enums.
constexpr auto operator++(T& lhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T&> {
    return lhs += static_cast<T>(1);
}

template <class T>
/// Postfix increment for arithmetic-enabled enums.
constexpr auto operator++(T& lhs, int) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs += static_cast<T>(1);
    return result;
}

template <class T>
/// Prefix decrement for arithmetic-enabled enums.
constexpr auto operator--(T& lhs) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T&> {
    return lhs -= static_cast<T>(1);
}

template <class T>
/// Postfix decrement for arithmetic-enabled enums.
constexpr auto operator--(T& lhs, int) -> std::enable_if_t<stlab::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs -= static_cast<T>(1);
    return result;
}

/// \}

/**************************************************************************************************/

/// \addtogroup CommonOperations
/// \{

template <class T>
/// Unary minus enums.
/// For bitmask-enabled enums, this allows expressions like `e & -e` to return the least set bit.
constexpr auto operator-(T a)
    -> std::enable_if_t<stlab::has_enabled_arithmetic<T> || stlab::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(-static_cast<underlying>(a));
}

template <class T>
/// Equality with nullptr for bitmask or arithmetic scoped enums; true when the value is
/// zero.
constexpr auto operator==(T lhs, std::nullptr_t)
    -> std::enable_if_t<(stlab::has_enabled_bitmask<T> || stlab::has_enabled_arithmetic<T>) &&
                            !stlab::is_compatible_scalar<T, T>,
                        bool> {
    return !lhs;
}

template <class T>
/// Equality with nullptr for bitmask or arithmetic scoped enums; true when the value is
/// zero.
constexpr auto operator==(std::nullptr_t, T rhs)
    -> std::enable_if_t<(stlab::has_enabled_bitmask<T> || stlab::has_enabled_arithmetic<T>) &&
                            !stlab::is_compatible_scalar<T, T>,
                        bool> {
    return !rhs;
}

template <class T>
/// Inequality with nullptr for bitmask or arithmetic scoped enums.
constexpr auto operator!=(T lhs, std::nullptr_t rhs)
    -> std::enable_if_t<(stlab::has_enabled_bitmask<T> || stlab::has_enabled_arithmetic<T>) &&
                            !stlab::is_compatible_scalar<T, T>,
                        bool> {
    return !(lhs == rhs);
}

template <class T>
/// Inequality with nullptr for bitmask or arithmetic scoped enums.
constexpr auto operator!=(std::nullptr_t lhs, T rhs)
    -> std::enable_if_t<(stlab::has_enabled_bitmask<T> || stlab::has_enabled_arithmetic<T>) &&
                            !stlab::is_compatible_scalar<T, T>,
                        bool> {
    return !(lhs == rhs);
}

template <class T>
/// Logical NOT for bitmask or arithmetic enums; true when the value converts to false.
constexpr auto operator!(T lhs)
    -> std::enable_if_t<stlab::has_enabled_bitmask<T> || stlab::has_enabled_arithmetic<T>, bool> {
    return !static_cast<bool>(lhs);
}

/// \}

/**************************************************************************************************/

#endif

/**************************************************************************************************/
