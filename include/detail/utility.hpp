#ifndef ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP
#define ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP

#include <type_traits>

namespace algebraic_datatypes::detail {

template <class C, class T, class U> struct get_not_t;

template <class C, class T> struct get_not_t<C, C, T> { using type = T; };

template <class C, class T> struct get_not_t<C, T, C> { using type = T; };

template <class C, class T, class U>
using get_not = typename get_not_t<C, T, U>::type;

template <class C, class T, class U>
struct either_is_t
    : std::bool_constant<std::is_same_v<C, T> || std::is_same_v<C, U>> {};

template <class C, class T, class U>
constexpr static inline bool either_is = either_is_t<C, T, U>::value;

struct ignore;

/** Merge two meta containers
 */
template <template <class...> class S, class T, class U, class One, class Zero>
struct merge_t {
  using type = S<T, U>;
};

template<template <class...> class S, class T, class One, class Zero>
struct merge_t<S, T, Zero, One, Zero> {
    using type = Zero;
};

template<template <class...> class S, class T, class One, class Zero>
struct merge_t<S, Zero, T, One, Zero> {
    using type = Zero;
};

template<template <class...> class S, class T, class One, class Zero>
struct merge_t<S, One, T, One, Zero> {
    using type = T;
};

template<template <class...> class S, class T, class One, class Zero>
struct merge_t<S, T, One, One, Zero> {
    using type = T;
};

template <template <class...> class S, class... Ts, class U, class One, class Zero>
struct merge_t<S, S<Ts...>, U, One, Zero> {
  using type = S<Ts..., U>;
};

template <template <class...> class S, class T, class... Us, class One, class Zero>
struct merge_t<S, T, S<Us...>, One, Zero> {
  using type = S<T, Us...>;
};

template <template <class...> class S, class... Ts, class... Us, class One, class Zero>
struct merge_t<S, S<Ts...>, S<Us...>, One, Zero> {
  using type = S<Ts..., Us...>;
};

template <template <class...> class S, class T, class U, class One, class Zero = detail::ignore>
using merge = typename merge_t<S, T, U, One, Zero>::type;

} // namespace algebraic_datatypes::detail

#endif // ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP