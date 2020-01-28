#include "algebraic_datatypes.hpp"
#include <string>
#include <iostream>

using namespace algebraic_datatypes;

using namespace std;

#define eval_m(x) ::algebraic_datatypes::eval<decltype(x)>
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

static_assert(eq<eval_m(Int + Float), variant<int, float>>);
static_assert(eq<eval_m(Int + Float + Char), variant<int, float, char>>);
static_assert(eq<eval_m(Int * Float * Char), tuple<int, float, char>>);
static_assert(eq<eval_m(Int ^ String ^ Char), function<int(char, string)>>);
static_assert(eq<eval_m(Int ^ Float * Char), function<int(tuple<float, char>)>>);

int main() {

    return 0;
}