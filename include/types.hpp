#ifndef ALGEBRAIC_DATATYPES_TYPES_HPP
#define ALGEBRAIC_DATATYPES_TYPES_HPP

namespace algebraic_datatypes {
struct empty_t;
struct unit_t {};

template <class...> struct adapter_t;
template <class...> struct sum_type_t;
template <class...> struct product_type_t;

template <class T> struct type_t {
  using type = T;
  constexpr const adapter_t<T> *operator->() const noexcept;
};

template <class R, class... Ts> struct function_wrapper_t {
  using type = R(Ts...);
};

template <class R, class... Ts>
struct type_t<R(Ts...)> : type_t<function_wrapper_t<R, Ts...>> {};

using zero_t = type_t<empty_t>;
using one_t = type_t<unit_t>;

} // namespace algebraic_datatypes

#endif // ALGEBRAIC_DATATYPES_TYPES_HPP