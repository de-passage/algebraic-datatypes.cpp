#ifndef ALGEBRAIC_DATATYPES_HPP
#define ALGEBRAIC_DATATYPES_HPP

#include "detail/utility.hpp"
#include "policies/unwrap.hpp"
#include "types.hpp"

namespace algebraic_datatypes {

////////////////
// Operations //
////////////////

/** Computes the sum type of T and U
 */
template <class T, class U, class Tag> struct add_t {
  using type = type_t<detail::merge<sum_type_t, T, U, empty_t>, Tag>;
};

template <class T, class U, class Tag>
using add = typename add_t<T, U, Tag>::type;

/** Computes the product type of T and U
 */
template <class T, class U, class Tag> struct mult_t {
  using type =
      type_t<detail::merge<product_type_t, T, U, unit_t, empty_t>, Tag>;
};

template <class T, class U, class Tag>
using mult = typename mult_t<T, U, Tag>::type;

/** Computes the power type of T and U
 */
template <class T, class U, class Tag> struct pow_t {
  using type = type_t<function_wrapper_t<T, U>, Tag>;
};

template <class Tag> struct pow_t<unit_t, unit_t, Tag> {
  using type = type_t<function_wrapper_t<void>, Tag>;
};

template <class R, class Tag, class... U>
struct pow_t<function_wrapper_t<R, U...>, unit_t, Tag> {
  using type = type_t<function_wrapper_t<R, U...>, Tag>;
};

template <class T, class Tag> struct pow_t<unit_t, T, Tag> {
  using type = type_t<function_wrapper_t<void, T>, Tag>;
};

template <class T, class R, class Tag, class... U>
struct pow_t<function_wrapper_t<R, U...>, T, Tag> {
  using type = type_t<function_wrapper_t<R, T, U...>, Tag>;
};

template <class T, class U, class Tag>
using pow = typename pow_t<T, U, Tag>::type;

///////////////
// Operators //
///////////////

template <class U, class V, class Tag>
constexpr add<U, V, Tag> operator+(type_t<U, Tag>, type_t<V, Tag>) noexcept {
  return {};
}

template <class U, class V, class Tag>
constexpr mult<U, V, Tag> operator*(type_t<U, Tag>, type_t<V, Tag>)noexcept {
  return {};
}

template <class U, class V, class Tag>
constexpr pow<U, V, Tag> operator^(type_t<U, Tag>, type_t<V, Tag>) noexcept {
  return {};
}

template <class U, class V, class Tag>
constexpr add<U, V, Tag> operator|(type_t<U, Tag>, type_t<V, Tag>) noexcept {
  return {};
}

template <class U, class V, class Tag>
constexpr mult<U, V, Tag> operator&(type_t<U, Tag>, type_t<V, Tag>)noexcept {
  return {};
}

template <class U, class V, class Tag>
constexpr bool operator==(type_t<U, Tag>, type_t<V, Tag>) noexcept {
  return std::is_same_v<U, V>;
}

template <class U, class V, class Tag>
constexpr bool operator!=(type_t<U, Tag> u, type_t<V, Tag> v) noexcept {
  return !(u == v);
}

/////////////////////
// Function stuffs //
/////////////////////

template <class Tag, class = void> struct aliases {};
template <class T>
struct aliases<T, std::void_t<typename T::template aliases<T>>>
    : T::template aliases<T> {};

template <class T, class TagContainer>
constexpr static inline type_t<
    detail::unpack_into<
        detail::bind_front_t<function_wrapper_t, T>::template type,
        typename TagContainer::arg_list>,
    typename TagContainer::tag>
    alias{};

namespace detail {
template <class Tag, class... Ts> struct tag_container : Tag {
  using arg_list = detail::list<Ts...>;
  using tag = Tag;
};
} // namespace detail

template <class Tag, class... Ts>
struct adapter_t : aliases<detail::tag_container<Tag, Ts...>> {
  template <class U>
  constexpr type_t<function_wrapper_t<U, Ts...>, Tag>
  return_(type_t<U, Tag>) const noexcept {
    return {};
  }

  template <class U>
  constexpr static inline type_t<function_wrapper_t<U, Ts...>, Tag> type{};
};

template <class... Ts>
[[maybe_unused]] constexpr static inline adapter_t<Ts...> adapter{};

template <class T, class Tag>
constexpr const adapter_t<Tag, T> *type_t<T, Tag>::operator->() const noexcept {
  return &adapter<Tag, T>;
}

template <class T, class Tag = default_tag>
constexpr inline static type_t<T, Tag> type{};
template <class Tag = default_tag> constexpr inline static zero_t<Tag> zero{};
template <class Tag = default_tag> constexpr inline static one_t<Tag> one{};

template <class Tag, class First, class... Ts>
constexpr const adapter_t<Tag, First, Ts...> *
    argument_pack_t<Tag, First, Ts...>::operator->() const noexcept {
  return &adapter<Tag, First, Ts...>;
}

template <class Left, class Right,
          class Config = detail::common_config_tag<Left, Right>,
          class R = detail::unpack_into<
              detail::bind_front_t<argument_pack_t, Config>::template type,
              Left, Right>>
constexpr R operator,(Left, Right) noexcept {
  return {};
}

template <class T, class P = policies::unwrap_t>
using eval = detail::apply_policy<P, T>;

/////////////
// Utility //
/////////////
template <class T, class Tag>
constexpr type_t<type_t<T, Tag>> wrap(type_t<T, Tag>) {
  return {};
}

template <class T, class Tag>
constexpr type_t<type_t<T, Tag>> operator~(type_t<T, Tag>) {
  return {};
}

template <class T, class Tag>
constexpr type_t<T, Tag> unwrap(type_t<type_t<T, Tag>>) {
  return {};
}

#define ALGEBRAIC_DATATYPES_CONFIGURE(Config)                                  \
  template <class T> using type_t = ::algebraic_datatypes::type_t<T, Config>;  \
  template <class T>                                                           \
  [[maybe_unused]] constexpr static inline auto type =                         \
      ::algebraic_datatypes::type<T, Config>;

} // namespace algebraic_datatypes

#endif