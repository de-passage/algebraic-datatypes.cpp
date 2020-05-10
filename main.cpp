#include "algebraic_datatypes.hpp"
#include <string>
#include <iostream>

using namespace algebraic_datatypes;

using namespace std;

#define ev(...) ::algebraic_datatypes::eval<decltype(__VA_ARGS__)>
#define P(...) type<ev(__VA_ARGS__)>

template<class T, class U>
constexpr static inline bool eq = is_same_v<T, U>;

static constexpr inline auto Int = type<int>;
static constexpr inline auto Float = type<float>;
static constexpr inline auto Char = type<char>;
static constexpr inline auto String = type<string>;

static_assert(Int == Int);
static_assert(Int != Float);
static_assert(Char != String);
static_assert((Int + Char) + String == Int + (Char + String));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert((Int * Float) * Int == Int * (Float * Int));
static_assert(Int + zero == Int);
//static_assert(Int * zero == zero);
static_assert(Int * one == Int);

static_assert(eq<ev(Int + Float), variant<int, float>>);
static_assert(eq<ev(Int + Float + Char), variant<int, float, char>>);
static_assert(eq<ev(Int * Float * Char), tuple<int, float, char>>);
static_assert(eq<ev(Int * Float + Char), variant<tuple<int, float>, char>>);
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
static_assert(eq<ev(Int & Float & Char), tuple<int, float, char>>);
static_assert(eq<ev(Int & Float | Char), variant<tuple<int, float>, char>>);
static_assert(eq<ev(Int | Float & Char), variant<int, tuple<float, char>>>);
static_assert(eq<ev(Char >> (String >> Int)), function<int(char, string)>>);
static_assert(eq<ev((Float & Char) >> Int), function<int(tuple<float, char>)>>);
static_assert(eq<ev(Float >> Int & Char), tuple<function<int(float)>, char>>);
static_assert(eq<ev(String >> one), function<void(string)>>);
static_assert(eq<ev(one >> one), function<void()>>);
static_assert(eq<ev(one >> (String >> one)), function<void(string)>>);
static_assert(eq<ev(one >> (one >> (one >> one))), function<void()>>);

int main() {

    auto Int3 = Int * Int * Int;
    auto F = String ^ Char ^ Int;
    auto test = String -> P(Char -> type<int>);
    ev(test) f = [](string, char) { return 0; };

    ev(Int3) int3 = {0, 1, 2};
    ev(Int3 + F) was_it_really_necessary(in_place_index<0>, int3);
    was_it_really_necessary = [] (char c, int i) { return string(c, i); };

    return 0;
}