#ifndef ALGEBRAIC_DATATYPES_POLICIES_STD
#define ALGEBRAIC_DATATYPES_POLICIES_STD

#include "../detail/utility.hpp"
#include "../types.hpp"

namespace algebraic_datatypes::policies {

struct unwrap_t {
  template <class, class T> struct policy { using type = T; };
  template <class P, class T, class Tag> struct policy<P, type_t<T, Tag>> {
    using type = detail::apply_policy<P, T>;
  };
  template <class P, class T, class Tag> struct policy<P, const type_t<T, Tag>> {
    using type = detail::apply_policy<P, T>;
  };
};

} // namespace policies
#endif