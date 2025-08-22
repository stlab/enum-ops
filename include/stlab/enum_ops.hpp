/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_ENUM_OPS_HPP
#define STLAB_ENUM_OPS_HPP

/**************************************************************************************************/

#include <type_traits>

/**************************************************************************************************/

/*!
    \file
    \brief Typesafe operators for enum types enabled via opt-in customization points.
*/

/*!
    \mainpage Typesafe Integers and Bit Fields (enums)

    [![View on
   GitHub](https://img.shields.io/badge/GitHub-enum--ops-181717?logo=github&style=flat)](https://github.com/stlab/enum-ops)

    \section Description Description

    \c enum_ops provides optional typesafe bitset and arithmetic operations for enumeration
   types. Without these typesafe operations, the compiler will promote the operand(s) to the
   appropriate integral type, and the result will be an integral type. When the typesafe
   operations have been defined for an enumeration type, \c E, the result will be of type \c E
   exactly when the operand(s) are of type \c E.

    ```cpp
    auto stlab_enable_bitmask_enum(E) -> std::true_type;
    ```
    Enables the bitset operations, `~`, `|`, `&`, `^`, `|=`, `&=`, and `^=`,
    for enumeration type `E`.

    ```cpp
    auto stlab_enable_arithmetic_enum(E) -> std::true_type;
    ```
    Enables the typesafe arithmetic operations `+`, `-`, `*`, `/`, `%`, `+=`, `*=`, `-=`, `/=`, and
    `%=`, for enumeration type `E`.

    \section Definition Definition

    Defined in \link enum_ops.hpp <code>stlab/enum_ops.hpp</code> \endlink

    \section Example Example

    The following is an example of code that will compile:
    \include enum_ops_example.cpp

    The following is contains an example of code that will not compile
    since the typesafe operators have not been defined.

    \include enum_ops_example_fail.cpp
*/

/**************************************************************************************************/

/// The stlab namespace.
namespace stlab {

/**************************************************************************************************/

/// Overload this for your enum in the enum namespace to return std::true_type and enable bitwise
/// operators.
auto stlab_enable_bitmask_enum(...) -> std::false_type;
/// Overload this for your enum in the enum namespace to return std::true_type and enable arithmetic
/// operators.
auto stlab_enable_arithmetic_enum(...) -> std::false_type;

/**************************************************************************************************/

/// The implementation namespace.
namespace implementation {

/**************************************************************************************************/

template <class T>
using has_enabled_bitmask_t = decltype(stlab_enable_bitmask_enum(std::declval<T>()));

template <class T>
constexpr bool has_enabled_bitmask = has_enabled_bitmask_t<T>::value;

template <class T>
using has_enabled_arithmetic_t = decltype(stlab_enable_arithmetic_enum(std::declval<T>()));

template <class T>
constexpr bool has_enabled_arithmetic = has_enabled_arithmetic_t<T>::value;

template <class T, class U>
using enable_if_bitmask_or_arithmetic =
    std::enable_if_t<std::disjunction_v<stlab::implementation::has_enabled_bitmask_t<T>,
                                        stlab::implementation::has_enabled_arithmetic_t<T>>,
                     U>;

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

} // namespace stlab

/**************************************************************************************************/

template <class T>
/// Bitwise AND for bitmask-enabled enums; returns the same enum type.
constexpr auto operator&(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <class T>
/// Bitwise NOT for bitmask-enabled enums; returns the same enum type.
constexpr auto operator~(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(~static_cast<underlying>(a));
}

template <class T>
/// Bitwise OR for bitmask-enabled enums.
constexpr auto operator|(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <class T>
/// Bitwise XOR for bitmask-enabled enums.
constexpr auto operator^(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <class T>
/// Left shift for bitmask-enabled enums.
constexpr auto operator<<(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned_t<std::underlying_type_t<T>>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) << static_cast<underlying>(rhs));
}

template <class T>
/// Right shift for bitmask-enabled enums.
constexpr auto operator>>(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned_t<std::underlying_type_t<T>>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) >> static_cast<underlying>(rhs));
}

template <class T>
/// XOR-assign for bitmask-enabled enums.
constexpr auto operator^=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs ^ rhs;
}

template <class T>
/// AND-assign for bitmask-enabled enums.
constexpr auto operator&=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs & rhs;
}

template <class T>
/// OR-assign for bitmask-enabled enums.
constexpr auto operator|=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs | rhs;
}

template <class T>
/// Left shift-assign for bitmask-enabled enums.
constexpr auto operator<<=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs << rhs;
}

template <class T>
/// Right shift-assign for bitmask-enabled enums.
constexpr auto operator>>=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs >> rhs;
}

template <class T, class U>
/// Subtracts a value convertible to the underlying type from a bitmask-enabled enum.
constexpr auto operator-(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

/**************************************************************************************************/

template <class T>
/// Unary plus for arithmetic-enabled enums.
constexpr auto operator+(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(+static_cast<underlying>(a));
}

template <class T>
/// Unary minus for arithmetic-enabled enums.
constexpr auto operator-(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(-static_cast<underlying>(a));
}

template <class T>
/// Addition for arithmetic-enabled enums.
constexpr auto operator+(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) + static_cast<underlying>(rhs));
}

template <class T>
/// Subtraction for arithmetic-enabled enums.
constexpr auto operator-(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

template <class T, class U>
/// Multiplication by a value convertible to the enum's underlying type.
constexpr auto operator*(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) * rhs);
}

template <class U, class T>
/// Multiplication with the scalar on the left-hand side.
constexpr auto operator*(U lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(lhs * static_cast<underlying>(rhs));
}

template <class T, class U>
/// Division by a value convertible to the enum's underlying type.
constexpr auto operator/(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) / rhs);
}

template <class T, class U>
/// Modulo by a value convertible to the enum's underlying type.
constexpr auto operator%(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<T>(static_cast<underlying>(lhs) % rhs);
}

template <class T>
/// Addition assignment for arithmetic-enabled enums.
constexpr auto operator+=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs + rhs;
}

template <class T>
/// Subtraction assignment for arithmetic-enabled enums.
constexpr auto operator-=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs - rhs;
}

template <class T, class U>
/// Multiplication assignment by a value convertible to the enum's underlying type.
constexpr auto operator*=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs * rhs;
}

template <class T, class U>
/// Division assignment by a value convertible to the enum's underlying type.
constexpr auto operator/=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs / rhs;
}

template <class T, class U>
/// Modulo assignment by a value convertible to the enum's underlying type.
constexpr auto operator%=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs % rhs;
}

template <class T>
/// Prefix increment for arithmetic-enabled enums.
constexpr auto operator++(T& lhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs += static_cast<T>(1);
}

template <class T>
/// Postfix increment for arithmetic-enabled enums.
constexpr auto operator++(T& lhs, int)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs += static_cast<T>(1);
    return result;
}

template <class T>
/// Prefix decrement for arithmetic-enabled enums.
constexpr auto operator--(T& lhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs -= static_cast<T>(1);
}

template <class T>
/// Postfix decrement for arithmetic-enabled enums.
constexpr auto operator--(T& lhs, int)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs -= static_cast<T>(1);
    return result;
}

/**************************************************************************************************/

template <class T>
/// Equality with nullptr for bitmask or arithmetic scoped enums; true when the value is zero.
constexpr auto operator==(T lhs, std::nullptr_t)
    -> std::enable_if_t<(stlab::implementation::has_enabled_bitmask<T> ||
                         stlab::implementation::has_enabled_arithmetic<T>) &&
                            !stlab::implementation::is_convertible_to_underlying<T, T>::value,
                        bool> {
    return !lhs;
}

template <class T>
/// Equality with nullptr for bitmask or arithmetic scoped enums; true when the value is zero.
constexpr auto operator==(std::nullptr_t, T rhs)
    -> std::enable_if_t<(stlab::implementation::has_enabled_bitmask<T> ||
                         stlab::implementation::has_enabled_arithmetic<T>) &&
                            !stlab::implementation::is_convertible_to_underlying<T, T>::value,
                        bool> {
    return !rhs;
}

template <class T>
/// Inequality with nullptr for bitmask or arithmetic scoped enums.
constexpr auto operator!=(T lhs, std::nullptr_t rhs)
    -> std::enable_if_t<(stlab::implementation::has_enabled_bitmask<T> ||
                         stlab::implementation::has_enabled_arithmetic<T>) &&
                            !stlab::implementation::is_convertible_to_underlying<T, T>::value,
                        bool> {
    return !(lhs == rhs);
}

template <class T>
/// Inequality with nullptr for bitmask or arithmetic scoped enums.
constexpr auto operator!=(std::nullptr_t lhs, T rhs)
    -> std::enable_if_t<(stlab::implementation::has_enabled_bitmask<T> ||
                         stlab::implementation::has_enabled_arithmetic<T>) &&
                            !stlab::implementation::is_convertible_to_underlying<T, T>::value,
                        bool> {
    return !(lhs == rhs);
}

template <class T>
/// Logical NOT for bitmask or arithmetic enums; true when the value converts to false.
constexpr auto operator!(T lhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !static_cast<bool>(lhs);
}

/**************************************************************************************************/

#endif

/**************************************************************************************************/
