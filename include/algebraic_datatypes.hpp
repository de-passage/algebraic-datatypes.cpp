#ifndef ALGEBRAIC_DATATYPES_HPP
#define ALGEBRAIC_DATATYPES_HPP

#include "detail/utility.hpp"
#include "types.hpp"

#include <functional>
#include <tuple>
#include <variant>

namespace algebraic_datatypes {

/** Computes the sum type of T and U
 */
template <class T, class U> struct add_t {
  using type = type_t<detail::merge<std::variant, T, U, empty>>;
};

template <class T, class U> using add = typename add_t<T, U>::type;

/** Computes the product type of T and U
 */
template <class T, class U> struct mult_t {
  using type = type_t<detail::merge<std::tuple, T, U, unit, empty>>;
};

template <class T, class U> using mult = typename mult_t<T, U>::type;

/** Computes the power type of T and U
 */
template <class T, class U> struct pow_t {
  using type = type_t<std::function<T(U)>>;
};

template <> struct pow_t<unit, unit> {
  using type = type_t<std::function<void()>>;
};

template <class R, class... U> struct pow_t<std::function<R(U...)>, unit> {
  using type = type_t<std::function<R(U...)>>;
};

template <class T> struct pow_t<unit, T> {
  using type = type_t<std::function<void(T)>>;
};

template <class T, class R, class... U>
struct pow_t<std::function<R(U...)>, T> {
  using type = type_t<std::function<R(T, U...)>>;
};

template <class T, class U> using pow = typename pow_t<T, U>::type;

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

template <class T> struct adapter_t {
  template <class U> constexpr static inline pow<U, T> type;
};

template <class T> constexpr static inline adapter_t<T> adapter{};

template <class T>
constexpr const adapter_t<T> *type_t<T>::operator->() const noexcept {
  return &adapter<T>;
}

template <class T> constexpr inline static type_t<T> type{};
constexpr inline static zero_t zero{};
constexpr inline static one_t one{};

template <class T> struct unoptimized_type_t {
  using type = T;
  constexpr const adapter_t<T> *operator->() const noexcept {
    return &adapter<T>;
  }
};

template <class T> using eval = typename T::type;

} // namespace algebraic_datatypes

#endif