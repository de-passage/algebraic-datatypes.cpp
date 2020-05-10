#ifndef ALGEBRAIC_DATATYPES_POLICIES_STD_HPP
#define ALGEBRAIC_DATATYPES_POLICIES_STD_HPP

#include "../detail/utility.hpp"
#include "../types.hpp"
#include "unwrap.hpp"

#include <functional>
#include <tuple>
#include <variant>

namespace algebraic_datatypes::policies {

struct use_std_function_t {
  template <class, class T> struct policy { using type = T; };

  template <class P, class R, class... Ts>
  struct policy<P, function_wrapper_t<R, Ts...>> {
    using type = std::function<detail::apply_policy<P, R>(
        detail::apply_policy<P, Ts>...)>;
  };
};

struct use_std_tuple_t {
  template <class, class T> struct policy { using type = T; };

  template <class P, class... Ts> struct policy<P, product_type_t<Ts...>> {
    using type = std::tuple<detail::apply_policy<P, Ts>...>;
  };
};

struct use_std_variant_t {
  template <class, class T> struct policy { using type = T; };

  template <class P, class... Ts> struct policy<P, sum_type_t<Ts...>> {
    using type = std::variant<detail::apply_policy<P, Ts>...>;
  };
};

template <class F, class... Ps> struct combine_t {
  template <class P, class T> struct policy {
    using type = detail::apply_policy<P, detail::apply_policy<P, T, F>,
                                      combine_t<Ps...>>;
  };
};

template <class F> struct combine_t<F> {
  template <class P, class T> struct policy {
    using type = detail::apply_policy<P, T, F>;
  };
};

using use_std_types_t =
    combine_t<unwrap_t, use_std_function_t, use_std_tuple_t, use_std_variant_t>;
} // namespace algebraic_datatypes::policies

#endif // ALGEBRAIC_DATATYPES_POLICIES_STD_HPP