#include "algebraic_datatypes.hpp"

#include "policies.hpp"

#include <functional>
#include <string>
#include <tuple>
#include <variant>

using namespace algebraic_datatypes;

template <class T>
using ev1 = ::algebraic_datatypes::eval<T, policies::use_std_types_t>;

using namespace std;

#define ev(...) ::ev1<decltype(__VA_ARGS__)>
#define P(...) type<ev(__VA_ARGS__)>

template <class T, class U> constexpr static inline bool eq = is_same_v<T, U>;

static constexpr inline auto Int = type<int>;
static constexpr inline auto Float = type<float>;
static constexpr inline auto Char = type<char>;
static constexpr inline auto String = type<string>;
static constexpr inline auto Double = type<double>;

//*
static_assert(Int == Int);
static_assert(Int != Float);
static_assert(Char != String);
static_assert((Int + Char) + String == Int + (Char + String));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert(Int + zero == Int);
static_assert(Int * zero == zero);
static_assert(Int * one == Int);
static_assert(zero + Int == Int);
static_assert(zero * Int == zero);
static_assert(one * Int == Int);

static_assert(eq<ev(Int + Float), variant<int, float>>);
static_assert(eq<ev(Int + Float + Char), variant<int, float, char>>);
static_assert(eq<ev(Int *Float *Char), tuple<int, float, char>>);
static_assert(eq<ev(Int *Float + Char), variant<tuple<int, float>, char>>);
static_assert(eq<ev(Int + Float * Char), variant<int, tuple<float, char>>>);
static_assert(eq<ev(Int ^ String ^ Char), function<int(char, string)>>);
static_assert(eq<ev(Int ^ Float * Char), function<int(tuple<float, char>)>>);
static_assert(eq<ev((Int ^ Float) * Char), tuple<function<int(float)>, char>>);
static_assert(eq<ev(one ^ String), function<void(string)>>);
static_assert(eq<ev(one ^ one), function<void()>>);
static_assert(eq<ev(one ^ String ^ one), function<void(string)>>);
static_assert(eq<ev(one ^ one ^ one ^ one), function<void()>>);

static_assert(((Int | Char) | String) == (Int | (Char | String)));
static_assert(((Int & Float) & Int) == (Int & (Float & Int)));
static_assert(((Int & Float) & Int) == (Int & (Float & Int)));

static_assert(eq<ev(Int | Float), variant<int, float>>);
static_assert(eq<ev(Int | Float | Char), variant<int, float, char>>);
static_assert(eq<ev(Int &Float &Char), tuple<int, float, char>>);
static_assert(eq<ev(Int &Float | Char), variant<tuple<int, float>, char>>);
static_assert(eq<ev(Int | Float & Char), variant<int, tuple<float, char>>>);

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

static_assert(eq<ev(argument_pack_t<char, double> {}->return_(type<int>)),
                 function<int(char, double)>>);
static_assert(eq<ev(argument_pack_t<char, double, double> {}->return_(type<int>)),
                 function<int(char, double, double)>>);
static_assert(
    eq<ev((Char, Double)->return_(Int)), function<int(char, double)>>);

static_assert(eq<decltype(operator,(Char, Double)), argument_pack_t<char, double>>);
static_assert(eq<decltype(Char, Double, String), argument_pack_t<char, double, string>>);
static_assert(eq<ev((Char, Char, Double)->return_(Int)),
                 function<int(char, char, double)>>);