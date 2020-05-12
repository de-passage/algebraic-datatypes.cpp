#ifndef ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP
#define ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP

#include <type_traits>

#include "../types.hpp"

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

template <template <class...> class S, class T, class One, class Zero>
struct merge_t<S, T, Zero, One, Zero> {
  using type = Zero;
};

template <template <class...> class S, class T, class One, class Zero>
struct merge_t<S, Zero, T, One, Zero> {
  using type = Zero;
};

template <template <class...> class S, class T, class One, class Zero>
struct merge_t<S, One, T, One, Zero> {
  using type = T;
};

template <template <class...> class S, class T, class One, class Zero>
struct merge_t<S, T, One, One, Zero> {
  using type = T;
};

template <template <class...> class S, class... Ts, class U, class One,
          class Zero>
struct merge_t<S, S<Ts...>, U, One, Zero> {
  using type = S<Ts..., U>;
};

template <template <class...> class S, class T, class... Us, class One,
          class Zero>
struct merge_t<S, T, S<Us...>, One, Zero> {
  using type = S<T, Us...>;
};

template <template <class...> class S, class... Ts, class... Us, class One,
          class Zero>
struct merge_t<S, S<Ts...>, S<Us...>, One, Zero> {
  using type = S<Ts..., Us...>;
};

template <template <class...> class S, class T, class U, class One,
          class Zero = detail::ignore>
using merge = typename merge_t<S, T, U, One, Zero>::type;

/// Apply a policy to a type
template <class P, class T, class S = P>
using apply_policy = typename S::template policy<P, T>::type;

// Extract the template arguments of a list of types and combine them into a new
// list
template <template <class...> class C, class C1, class... Ts>
struct unpack_into_t;

template <template <class...> class C, template <class...> class C1,
          template <class...> class C2, class... R, class... Ts, class... Us>
struct unpack_into_t<C, C1<Ts...>, C2<Us...>, R...> {
  using type = typename unpack_into_t<C, C1<Ts..., Us...>, R...>::type;
};

template <template <class...> class C, template <class...> class C1,
          class... Ts>
struct unpack_into_t<C, C1<Ts...>> {
  using type = C<Ts...>;
};

template <class... Ts> struct list;
template <class T> struct to_meta_list_t;
template <template <class...> class C, class... Ts>
struct to_meta_list_t<C<Ts...>> {
  using type = list<Ts...>;
};
template <class Tag, class... Ts>
struct to_meta_list_t<argument_pack_t<Tag, Ts...>> {
  using type = list<Ts...>;
};
template <class T, class Tag> struct to_meta_list_t<type_t<T, Tag>> {
  using type = list<T>;
};

template<class T>
using to_meta_list = typename to_meta_list_t<T>::type;

template <template <class...> class C, class... Ts>
using unpack_into = typename unpack_into_t<C, to_meta_list<Ts>...>::type;

template <class T> struct is_type_or_argument_pack_t : std::false_type {};
template <class T, class Tag>
struct is_type_or_argument_pack_t<type_t<T, Tag>> : std::true_type {};
template <class Tag, class... Ts>
struct is_type_or_argument_pack_t<argument_pack_t<Tag, Ts...>> : std::true_type {};

template <class T>
constexpr static inline bool is_type_or_argument_pack =
    is_type_or_argument_pack_t<T>::value;

template<template <class ...> class C, class ...Ts>
struct bind_front_t {
  template<class ...Us>
  using type = C<Ts..., Us...>;
};

template<class T>
struct config_tag_t;
template<class T, class Tag> 
struct config_tag_t<type_t<T, Tag>> {
  using type = Tag;
};
template<class Tag, class ...Ts>
struct config_tag_t<argument_pack_t<Tag, Ts...>> {
  using type = Tag;
};

template<class T> using config_tag = typename config_tag_t<T>::type;

template<class T, class U, class = std::enable_if_t<std::is_same_v<config_tag<T>, config_tag<U>>>>
struct common_config_tag_t {
  using type = config_tag<T>;
};
template<class T, class U>
using common_config_tag = typename common_config_tag_t<T, U>::type;

} // namespace algebraic_datatypes::detail

#endif // ALGEBRAIC_DATATYPES_DETAIL_UTILITY_HPP