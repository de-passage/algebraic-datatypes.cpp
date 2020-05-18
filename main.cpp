#include "algebraic_datatypes.hpp"

#include "policies.hpp"

#include <functional>
#include <string>
#include <tuple>
#include <variant>

namespace adt = ::algebraic_datatypes;
using adt::type;
using adt::type_t;

template <class T>
using ev1 = ::algebraic_datatypes::eval<T, adt::policies::use_std_types_t>;

using namespace std;

#define ev(...) ::ev1<decltype(__VA_ARGS__)>
#define P(...) type<ev(__VA_ARGS__)>

template <class T, class U> constexpr static inline bool eq = is_same_v<T, U>;

static constexpr inline auto Int = type<int>;
static constexpr inline auto Float = type<float>;
static constexpr inline auto Char = type<char>;
static constexpr inline auto String = type<string>;
static constexpr inline auto Double = type<double>;
static constexpr inline auto Zero = adt::zero<>;
static constexpr inline auto One = adt::one<>;

//*
static_assert(Int == Int);
static_assert(Int != Float);
static_assert(Char != String);

static_assert(((Int | Char) | String) == (Int | (Char | String)));
static_assert(((Int & Float) & Int) == (Int & (Float & Int)));
static_assert(((Int & Float) & Int) == (Int & (Float & Int)));

static_assert(eq<ev(Int | Float), variant<int, float>>);
static_assert(eq<ev(Int | Float | Char), variant<int, float, char>>);
static_assert(eq<ev(Int &Float &Char), tuple<int, float, char>>);
static_assert(eq<ev(Int &Float | Char), variant<tuple<int, float>, char>>);
static_assert(eq<ev(Int | Float & Char), variant<int, tuple<float, char>>>);

static_assert(unwrap(wrap(Int)) == Int);

int main() {

  return 0;
}
//*/

using adt::argument_pack_t;
using adt::function_wrapper_t;
using adt::product_type_t;
using adt::sum_type_t;

static_assert(eq<ev1<type_t<int>>, int>);
static_assert(eq<ev1<type_t<sum_type_t<int, double>>>, variant<int, double>>);
static_assert(eq<ev1<type_t<sum_type_t<type_t<variant<int, double>>, double>>>,
                 variant<variant<int, double>, double>>);

static_assert(eq<ev(argument_pack_t<adt::default_tag, char, double> {
                 }->return_(type<int>)),
                 function<int(char, double)>>);
static_assert(eq<ev(argument_pack_t<adt::default_tag, char, double, double> {
                 }->return_(type<int>)),
                 function<int(char, double, double)>>);
static_assert(
    eq<ev((Char, Double)->return_(Int)), function<int(char, double)>>);

static_assert(eq<decltype(operator,(Char, Double)),
                 argument_pack_t<adt::default_tag, char, double>>);
static_assert(eq<decltype(Char, Double, String),
                 argument_pack_t<adt::default_tag, char, double, string>>);
static_assert(eq<ev((Char, Char, Double)->return_(Int)),
                 function<int(char, char, double)>>);

static_assert(
    eq<adt::detail::common_config_tag<
           argument_pack_t<adt::default_tag, char, double>, type_t<int>>,
       adt::default_tag>);
static_assert(
    eq<adt::detail::bind_front_t<adt::argument_pack_t, int>::type<char>,
       adt::argument_pack_t<int, char>>);
static_assert(
    eq<adt::detail::unpack_into<adt::detail::list, type_t<int>, type_t<double>>,
       adt::detail::list<int, double>>);
static_assert(
    eq<adt::detail::unpack_into<
           adt::detail::bind_front_t<adt::detail::list, char>::template type,
           type_t<int>>,
       adt::detail::list<char, int>>);
static_assert(
    eq<adt::detail::unpack_into<
           adt::detail::bind_front_t<adt::detail::list, char>::template type,
           type_t<int>, type_t<double>>,
       adt::detail::list<char, int, double>>);

namespace a1 {
struct config {
  template <class T> struct aliases {
    constexpr static inline auto Int = adt::alias<int, T>;
    constexpr static inline auto Double = adt::alias<double, T>;
  };
};
ALGEBRAIC_DATATYPES_CONFIGURE(config)

constexpr static inline auto Int = type<int>;
constexpr static inline auto Double = type<double>;

static_assert(eq<ev(Int), int>);
static_assert(eq<ev(Int & Int), std::tuple<int, int>>);

static_assert(eq<ev(Int->Int), std::function<int(int)>>);
static_assert(eq<ev(type<int>->type<int>), std::function<int(int)>>);
static_assert(
    eq<ev(Int->Double->Int), std::function<int(std::function<double(int)>)>>);
static_assert(eq<ev(Int->return_(Double->Int)),
                 std::function<function<int(double)>(int)>>);
} // namespace a1

template<class T> using T2 = adt::type_t<T, a1::config>;
template<class T> constexpr static inline auto type2 = adt::type<T, a1::config>;
static_assert(eq<ev(type2<int> & type2<int> | type2<char> & type2<char>), variant<tuple<int, int>, tuple<char, char>>>);

constexpr static inline auto foo = [](auto a) {
  return a & Int & (Double | a);
};

static_assert(eq<ev(foo(Char)), tuple<char, int, variant<double, char>>>);
static_assert(eq<ev(foo(String)), tuple<string, int, variant<double, string>>>);

struct function_pseudo_operator_core {}
    constexpr static inline D{};
template<class C, class ...Ts>
struct partially_applied_function_pseudo_operator{};

template<class T, class C>
partially_applied_function_pseudo_operator<C, argument_pack_t<C, T>> 
    operator-(type_t<T, C>, function_pseudo_operator_core) noexcept {
    return {};
}

template<class ...Ts, class C>
partially_applied_function_pseudo_operator<C, argument_pack_t<C,  Ts...>> 
    operator-(argument_pack_t<C, Ts...>, function_pseudo_operator_core) noexcept {
    return {};
}

template<class C, class R, class... Ts>
constexpr type_t<function_wrapper_t<R, Ts...>, C>
    operator>(partially_applied_function_pseudo_operator<C, Ts...>, type_t<R, C>) noexcept {
    return {};
}

template<class C, class... Ts, class R>
constexpr partially_applied_function_pseudo_operator<C, R, Ts...>
    operator>(partially_applied_function_pseudo_operator<C, Ts...>,
              partially_applied_function_pseudo_operator<C, R>) noexcept {
    return {};
}

static_assert(eq<ev(Int -D> Float), function<float(int)>>);
static_assert(eq<ev(Int -D> Char -D> Float), function<function<float(char)>(int)>>);
static_assert(eq<ev(Int -D> (Char, Double) -D> Float), function<function<float(char, double)>(int)>>);
static_assert(eq<ev(Int -D> (Float & Char)), function<tuple<float, char>(int)>>);
static_assert(eq<ev((Float & Int) -D> Char), function<char(tuple<float, int>)>>);
static_assert(eq<ev((Float, Int) -D> Char), function<char(float, int)>>);
static_assert(eq<ev(Int -D> (Float | Char)), function<variant<float, char>(int)>>);
