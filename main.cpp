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
static_assert((Int + Char) + String == Int + (Char + String));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert(Int + Zero == Int);
static_assert(Int * Zero == Zero);
static_assert(Int * One == Int);
static_assert(Zero + Int == Int);
static_assert(Zero * Int == Zero);
static_assert(One * Int == Int);

static_assert(eq<ev(Int + Float), variant<int, float>>);
static_assert(eq<ev(Int + Float + Char), variant<int, float, char>>);
static_assert(eq<ev(Int *Float *Char), tuple<int, float, char>>);
static_assert(eq<ev(Int *Float + Char), variant<tuple<int, float>, char>>);
static_assert(eq<ev(Int + Float * Char), variant<int, tuple<float, char>>>);
static_assert(eq<ev(Int ^ String ^ Char), function<int(char, string)>>);
static_assert(eq<ev(Int ^ Float * Char), function<int(tuple<float, char>)>>);
static_assert(eq<ev((Int ^ Float) * Char), tuple<function<int(float)>, char>>);
static_assert(eq<ev(One ^ String), function<void(string)>>);
static_assert(eq<ev(One ^ One), function<void()>>);
static_assert(eq<ev(One ^ String ^ One), function<void(string)>>);
static_assert(eq<ev(One ^ One ^ One ^ One), function<void()>>);

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

  auto Int3 = Int * Int * Int;
  auto F = String ^ Char ^ Int;
  auto test = String->return_(Char->return_(Int));
  ev(test) f = [](string) { return [](char) { return 0; }; };

  ev(Int3) int3 = {0, 1, 2};
  ev(Int3 + F) was_it_really_necessary(in_place_index<0>, int3);
  was_it_really_necessary = [](char c, int i) { return string(c, i); };

  return 0;
}
//*/

using adt::argument_pack_t;
using adt::function_wrapper_t;
using adt::product_type_t;
using adt::sum_type_t;
static_assert(eq<ev(Int ^ Double), function<int(double)>>);
static_assert(eq<ev1<decltype(Int ^ Double * Int)>,
                 function<int(std::tuple<double, int>)>>);
static_assert(
    eq<ev1<decltype(Int ^ Double ^ Char)>, function<int(char, double)>>);
static_assert(eq<ev1<type_t<function_wrapper_t<function_wrapper_t<int>,
                                               function_wrapper_t<int>>>>,
                 function<function<int()>(function<int()>)>>);

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
static_assert(eq<ev(Int ^ Int), std::function<int(int)>>);

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
