#ifndef ALGEBRAIC_DATATYPES_TYPES_HPP
#define ALGEBRAIC_DATATYPES_TYPES_HPP

namespace algebraic_datatypes {
struct empty_t;
struct unit_t {};
struct default_tag {};

template <class Tag, class...> struct adapter_t;
template <class...> struct sum_type_t;
template <class...> struct product_type_t;

template <class T, class Tag = default_tag> struct type_t : Tag {
  using type = T;
  constexpr const adapter_t<Tag, T> *operator->() const noexcept;
};

template <class R, class... Ts> struct function_wrapper_t {
  using type = R(Ts...);
};

template <class R, class Tag, class... Ts>
struct type_t<R(Ts...), Tag> : type_t<function_wrapper_t<R, Ts...>, Tag> {};

template <class Tag, class First, class... Ts> struct argument_pack_t {
  constexpr const adapter_t<Tag, First, Ts...> *operator->() const noexcept;
};

template<class Tag = unit_t>
using zero_t = type_t<empty_t, Tag>;
template<class Tag = unit_t>
using one_t = type_t<unit_t, Tag>;

} // namespace algebraic_datatypes

#endif // ALGEBRAIC_DATATYPES_TYPES_HPP