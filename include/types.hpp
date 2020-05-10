#ifndef ALGEBRAIC_DATATYPES_TYPES_HPP
#define ALGEBRAIC_DATATYPES_TYPES_HPP

namespace algebraic_datatypes {
struct empty;
struct unit {};

template <class T> struct adapter_t;

template <class T> struct type_t {
  using type = T;
  constexpr const adapter_t<T> *operator->() const noexcept;
};

using zero_t = type_t<empty>;
using one_t = type_t<unit>;

} // namespace algebraic_datatypes

#endif // ALGEBRAIC_DATATYPES_TYPES_HPP