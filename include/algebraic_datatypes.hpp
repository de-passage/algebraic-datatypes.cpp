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

template <class... Ts> struct adapter_t {
  template <class U, class Tag>
  constexpr type_t<function_wrapper_t<U, Ts...>, Tag>
  return_(type_t<U, Tag>) const noexcept {
    return {};
  }
};

template <class... Ts> constexpr static inline adapter_t<Ts...> adapter{};

template <class T, class Tag>
constexpr const adapter_t<T> *type_t<T, Tag>::operator->() const noexcept {
  return &adapter<T>;
}

template <class T, class Tag = default_tag>
constexpr inline static type_t<T, Tag> type{};
template <class Tag = default_tag> constexpr inline static zero_t<Tag> zero{};
template <class Tag = default_tag> constexpr inline static one_t<Tag> one{};

template <class First, class... Ts>
constexpr const adapter_t<First, Ts...> *
    argument_pack_t<First, Ts...>::operator->() const noexcept {
  return &adapter<First, Ts...>;
}

template <class Left, class Right,
          class = std::enable_if_t<detail::is_type_or_argument_pack<Left> &&
                                   detail::is_type_or_argument_pack<Right>>>
detail::unpack_into<argument_pack_t, Left, Right> operator,(Left, Right) {
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
constexpr type_t<T, Tag> unwrap(type_t<type_t<T, Tag>>) {
  return {};
}

#define ALGEBRAIC_DATATYPES_CONFIGURE(Config)                                  \
  template <class T> using type_t = ::algebraic_datatypes::type_t<T, Config>;  \
  template <class T>                                                           \
  constexpr static inline auto type = ::algebraic_datatypes::type<T, Config>;

} // namespace algebraic_datatypes

#endif