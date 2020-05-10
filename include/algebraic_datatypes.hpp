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
template <class T, class U> struct add_t {
  using type = type_t<detail::merge<sum_type_t, T, U, empty_t>>;
};

template <class T, class U> using add = typename add_t<T, U>::type;

/** Computes the product type of T and U
 */
template <class T, class U> struct mult_t {
  using type = type_t<detail::merge<product_type_t, T, U, unit_t, empty_t>>;
};

template <class T, class U> using mult = typename mult_t<T, U>::type;

/** Computes the power type of T and U
 */
template <class T, class U> struct pow_t {
  using type = type_t<function_wrapper_t<T, U>>;
};

template <> struct pow_t<unit_t, unit_t> {
  using type = type_t<function_wrapper_t<void>>;
};

template <class R, class... U>
struct pow_t<function_wrapper_t<R, U...>, unit_t> {
  using type = type_t<function_wrapper_t<R, U...>>;
};

template <class T> struct pow_t<unit_t, T> {
  using type = type_t<function_wrapper_t<void, T>>;
};

template <class T, class R, class... U>
struct pow_t<function_wrapper_t<R, U...>, T> {
  using type = type_t<function_wrapper_t<R, T, U...>>;
};

template <class T, class U> using pow = typename pow_t<T, U>::type;

///////////////
// Operators //
///////////////

template <class U, class V>
constexpr add<U, V> operator+(type_t<U>, type_t<V>) noexcept {
  return {};
}

template <class U, class V>
constexpr mult<U, V> operator*(type_t<U>, type_t<V>)noexcept {
  return {};
}

template <class U, class V>
constexpr pow<U, V> operator^(type_t<U>, type_t<V>) noexcept {
  return {};
}

template <class U, class V>
constexpr add<U, V> operator|(type_t<U>, type_t<V>) noexcept {
  return {};
}

template <class U, class V>
constexpr mult<U, V> operator&(type_t<U>, type_t<V>)noexcept {
  return {};
}

template <class U, class V>
constexpr bool operator==(type_t<U>, type_t<V>) noexcept {
  return std::is_same_v<U, V>;
}

template <class U, class V>
constexpr bool operator!=(type_t<U> u, type_t<V> v) noexcept {
  return !(u == v);
}

/////////////////////
// Function stuffs //
/////////////////////

template <class... Ts> struct adapter_t {
  template <class U>
  constexpr type_t<function_wrapper_t<U, Ts...>> return_(type_t<U>) const
      noexcept {
    return {};
  }
};

template <class... Ts> constexpr static inline adapter_t<Ts...> adapter{};

template <class T>
constexpr const adapter_t<T> *type_t<T>::operator->() const noexcept {
  return &adapter<T>;
}

template <class T> constexpr inline static type_t<T> type{};
constexpr inline static zero_t zero{};
constexpr inline static one_t one{};

template<class First, class ...Ts>
  constexpr const adapter_t<First, Ts...>* argument_pack_t<First, Ts...>::operator->() const noexcept {
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
template <class T> type_t<type_t<T>> wrap(type_t<T>) { return {}; }

template <class T> type_t<T> unwrap(type_t<type_t<T>>) { return {}; }

} // namespace algebraic_datatypes

#endif