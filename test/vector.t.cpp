#include <vml/quaternion.hpp>
#include <vml/vector.hpp>

#include <array>
#include <tuple>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace vml::short_types;

#define TYPE_LIST(O)                                                           \
    (int, 2, O), (int, 3, O), (int, 4, O), (unsigned, 2, O), (unsigned, 3, O), \
        (unsigned, 4, O), (float, 2, O), (float, 3, O), (float, 4, O),         \
        (double, 2, O), (double, 3, O), (double, 4, O), (double, 7, O),        \
        (double, 8, O)

#define VECTOR_TEST_CASE(...)                                                  \
    TEMPLATE_TEST_CASE_SIG(__VA_ARGS__,                                        \
                           ((typename T, size_t Size,                          \
                             vml::vector_options Options),                     \
                            T, Size, Options),                                 \
                           TYPE_LIST(vml::vector_options{}),                   \
                           TYPE_LIST(vml::vector_options{}.packed(true)))

namespace {

template <typename T>
struct OtherType {
    using type = int;
};
template <>
struct OtherType<int> {
    using type = float;
};
template <vml::vector_options O>
struct OtherOptions {
    auto constexpr static value = O.packed(!O.packed());
};

} // namespace

VECTOR_TEST_CASE("vector offsets", "[vector][vector_data]") {
    using V = vml::vector<T, Size, Options>;
    CHECK(std::is_trivial_v<V>);
    if constexpr (Options.packed()) {
        CHECK(sizeof(V) == sizeof(T) * Size);
        CHECK(alignof(V) == alignof(T));
    }
    else if constexpr (V::size() <= 4) {
        CHECK(sizeof(V) == sizeof(T) * (Size + Size % 2));
        CHECK(alignof(V) == sizeof(V));
    }
    else if constexpr (V::size() < 8) {
        CHECK(sizeof(V) == sizeof(T) * Size);
        CHECK(alignof(V) == alignof(T));
    }
    else {
        CHECK(sizeof(V) == sizeof(T) * Size);
        CHECK(alignof(V) == sizeof(V));
    }
    CHECK(offsetof(V, x) == 0 * sizeof(T));
    CHECK(offsetof(V, y) == 1 * sizeof(T));
    if constexpr (Size > 2) {
        CHECK(offsetof(V, z) == 2 * sizeof(T));
    }
    if constexpr (Size > 3) {
        CHECK(offsetof(V, w) == 3 * sizeof(T));
    }
}

VECTOR_TEST_CASE("vector single value constructor", "[vector]") {
    T const value = (T)GENERATE(0, 1, -1, 5321);
    vml::vector<T, Size, Options> const v = value;
    for (auto i: v) {
        CHECK(i == value);
    }
}

VECTOR_TEST_CASE("vector functional constructor", "[vector]") {
    using V = vml::vector<T, Size, Options>;
    static_assert(Size <= 8);
    T const values[8] = { 0, 1, (T)-1, 5321, 1, 2, 3, 4 };
    V v;
    SECTION("1") {
        v = V([&](int i) { return values[i]; });
    }
    SECTION("2") {
        v = V([&, i = 0]() mutable { return values[i++]; });
    }
    for (int i = 0; auto value: v) {
        CHECK(value == values[i++]);
    }
}

VECTOR_TEST_CASE("vector conversion", "[vector]") {
    T const values[8] = { 0, (T)1.5, (T)-1, 5321, 1, 2, 3, 4 };
    vml::vector<T, Size, Options> const v([&](int i) { return values[i]; });
    using U = typename OtherType<T>::type;
    constexpr auto P = OtherOptions<Options>::value;
    vml::vector<U, Size, P> const w = v;
    for (int i = 0; auto value: w) {
        CHECK(value == (U)values[i++]);
    }
}

template <typename, size_t>
struct Vec;

template <typename T>
struct Vec<T, 2> {
    T x, y;
    bool operator==(Vec const&) const = default;
};

template <typename T>
struct Vec<T, 3> {
    T x, y, z;
};

template <typename T>
struct Vec<T, 4> {
    T x, y, z, w;
};

template <typename T>
struct Vec4_2 {
    Vec4_2() = default;
    Vec4_2(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
    T x, y, z, w;
};

TEST_CASE("extern vector type conversion", "[vector]") {
    SECTION("Size = 2") {
        Vec<int, 2> const ex = { 1, 2 };
        vml::int2 const v = ex;
        CHECK(v == vml::int2{ 1, 2 });
        Vec<int, 2> const ex2 = v;
        CHECK(ex2.x == 1);
        CHECK(ex2.y == 2);
    }
    SECTION("Size = 3") {
        Vec<int, 3> const ex = { 1, 2, 3 };
        vml::int3 const v = ex;
        CHECK(v == vml::int3{ 1, 2, 3 });
        Vec<int, 3> const ex2 = v;
        CHECK(ex2.x == 1);
        CHECK(ex2.y == 2);
        CHECK(ex2.z == 3);
    }
    SECTION("Size = 4") {
        Vec<int, 4> const ex = { 1, 2, 3, 4 };
        vml::int4 const v = ex;
        CHECK(v == vml::int4{ 1, 2, 3, 4 });
        Vec<int, 4> const ex2 = v;
        CHECK(ex2.x == 1);
        CHECK(ex2.y == 2);
        CHECK(ex2.z == 3);
        CHECK(ex2.w == 4);
    }
}

TEST_CASE("extern vector type conversion 2", "[vector]") {
    Vec4_2<int> const ex = { 1, 2, 3, 4 };
    vml::int4 const v = ex;
    CHECK(v == vml::int4{ 1, 2, 3, 4 });
    Vec4_2<int> const ex2 = v;
    CHECK(ex2.x == 1);
    CHECK(ex2.y == 2);
    CHECK(ex2.z == 3);
    CHECK(ex2.w == 4);
}

TEST_CASE("vector tuple conversion", "[vector]") {
    std::tuple<int, int, int, int> const t = { 1, 2, 3, 4 };
    vml::int4 const v = t;
    CHECK(v == vml::int4{ 1, 2, 3, 4 });
    std::tuple<int, int, int, int> const s = v;
    CHECK(std::get<0>(s) == 1);
    CHECK(std::get<1>(s) == 2);
    CHECK(std::get<2>(s) == 3);
    CHECK(std::get<3>(s) == 4);
}

TEST_CASE("vector tuple conversion 2", "[vector]") {
    std::tuple<int, float, double, char> const t = { 1, 2, 3, 4 };
    vml::int4 const v = t;
    CHECK(v == vml::int4{ 1, 2, 3, 4 });
    std::tuple<int, float, double, char> const s = v;
    CHECK(std::get<0>(s) == 1);
    CHECK(std::get<1>(s) == 2);
    CHECK(std::get<2>(s) == 3);
    CHECK(std::get<3>(s) == 4);
}

TEST_CASE("vector array conversion", "[vector]") {
    std::array<int, 4> const t = { 1, 2, 3, 4 };
    vml::int4 const v = t;
    CHECK(v == vml::int4{ 1, 2, 3, 4 });
    std::array<int, 4> const s = v;
    CHECK(s[0] == 1);
    CHECK(s[1] == 2);
    CHECK(s[2] == 3);
    CHECK(s[3] == 4);
}

TEST_CASE("vector pair conversion", "[vector]") {
    std::pair<int, int> const t = { 1, 2 };
    vml::int2 const v = t;
    CHECK(v == vml::int2{ 1, 2 });
    std::pair<int, int> const s = v;
    CHECK(s.first == 1);
    CHECK(s.second == 2);
}

TEST_CASE("vector quaternion conversion", "[vector]") {
    vml::quaternion_int const q = { 1, 2, 3, 4 };
    vml::int4 const v = q;
    CHECK(v == vml::int4{ 1, 2, 3, 4 });
    vml::quaternion_int const r = v;
    CHECK(r.x == 1);
    CHECK(r.y == 2);
    CHECK(r.z == 3);
    CHECK(r.w == 4);
}

TEST_CASE("vector complex conversion", "[vector]") {
    vml::complex_int const z = { 1, 2 };
    vml::int2 const v = z;
    CHECK(v == vml::int2{ 1, 2 });
    vml::complex_int const r = z;
    CHECK(r.x == 1);
    CHECK(r.y == 2);
}

TEST_CASE("vector deduction guide", "[vector]") {
    vml::vector v = { 1, 1.4, 2u };
    static_assert(std::is_same_v<decltype(v)::value_type, double>);
    static_assert(decltype(v)::size() == 3);
    static_assert(!decltype(v)::options().packed());
}

static constexpr auto Plus = [](auto... a) { return (... + a); };

static constexpr auto Mul = [](auto... a) { return (... * a); };

TEST_CASE("map(vector)", "[vector]") {
    auto const m = vml::map(float3{ 0.1f, 0.2f, 0.3f }, int3{ 1, 2, 3 },
                            packed_int3{}, Plus);
    static_assert(std::is_same_v<decltype(m)::value_type, float>);
    static_assert(!decltype(m)::options().packed());
    CHECK(m == float3{ 1.1f, 2.2f, 3.3f });
}

TEST_CASE("fold(vector)", "[vector]") {
    double const x = 1e30, y = -1e30, z = 1;
    auto const lf = vml::left_fold(double3{ x, y, z }, Plus);
    auto const rf = vml::right_fold(double3{ x, y, z }, Plus);
    CHECK(lf == 1);
    CHECK(rf == 0);
    static_assert(std::is_same_v<decltype(lf), double const>);
    static_assert(std::is_same_v<decltype(rf), double const>);
}

TEST_CASE("vector comparisons", "[vector]") {
    int3 const i = { 1, 2, 3 };
    float3 const f = { .1, .2, .3 };
    CHECK((i > f).all());
    float3 const g = { 1.1, .2, .3 };
    CHECK((i < g).any());
}

TEST_CASE("fmod", "[vector]") {
    auto v = fmod(float3{ 1, 5.5, 3.3 }, 3.0);
    using namespace Catch::literals;
    CHECK(v.x == 1_a);
    CHECK(v.y == 2.5_a);
    CHECK(v.z == 0.3_a);
}

TEST_CASE("clamp", "[vector]") {
    float3 v = { 0.5, 1.5, -1.2 };
    auto w = vml::clamp(v, { 0, 0, 0 }, { 1, 1, 1 });
    CHECK(w == vml::float3{ 0.5, 1, 0 });
}

namespace {

struct MyVec2 {
    float x, y;
};

template <size_t I>
float get(MyVec2 v) {
    return ((float*)&v)[I];
}

} // namespace

template <>
struct std::tuple_size<MyVec2>: std::integral_constant<size_t, 2> {};

TEST_CASE("Conversion from foreign vector and tuple type") {
    float2 v = MyVec2{ 1, 2 };
    CHECK(v == float2{ 1, 2 });
    MyVec2 w = v;
    CHECK(w.x == 1);
    CHECK(w.y == 2);
}

namespace {

template <unsigned N>
struct int_mod {
    int_mod(unsigned v): value(v % N) {}

    int_mod& operator=(unsigned rhs) { return set_value(rhs); }

    int_mod& operator+=(int_mod rhs) { return set_value(get() + rhs.get()); }

    int_mod& operator-=(int_mod rhs) { return set_value(get() - rhs.get()); }

    int_mod& operator*=(int_mod rhs) { return set_value(get() * rhs.get()); }

    int_mod& operator/=(int_mod rhs) {
        assert(rhs != 0);
        return set_value(get() * mod_inverse(rhs.get()));
    }

    bool operator==(int_mod const&) const = default;

    bool operator==(unsigned rhs) const { return *this == int_mod(rhs); }

    unsigned get() const { return value; }

private:
    int_mod& set_value(unsigned v) {
        value = v % N;
        return *this;
    }

    std::array<int, 3> gcd_extended(int val, int n) {
        if (val == 0) return { n, 0, 1 };
        auto [gcd, x1, y1] = gcd_extended(n % val, val);
        int x = y1 - (n / val) * x1;
        int y = x1;
        return { gcd, x, y };
    }

    unsigned mod_inverse(unsigned val) {
        auto [gcd, x, _] = gcd_extended((int)val, N);
        assert(gcd == 1);
        int result = (x % (int)N + (int)N) % (int)N;
        assert(result >= 0);
        return (unsigned)result;
    }

    friend std::ostream& operator<<(std::ostream& str, int_mod i) {
        return str << i.get() << " + " << N << "Z";
    }

    unsigned value;
};

template <unsigned N>
int_mod<N> operator+(int_mod<N> lhs, int_mod<N> rhs) {
    return lhs += rhs;
}

template <unsigned N>
int_mod<N> operator-(int_mod<N> lhs, int_mod<N> rhs) {
    return lhs -= rhs;
}

template <unsigned N>
int_mod<N> operator*(int_mod<N> lhs, int_mod<N> rhs) {
    return lhs *= rhs;
}

template <unsigned N>
int_mod<N> operator/(int_mod<N> lhs, int_mod<N> rhs) {
    return lhs /= rhs;
}

} // namespace

template <unsigned N>
struct vml::is_scalar<int_mod<N>>: std::true_type {};

TEST_CASE("custom scalar type", "[vector]") {
    using V = vml::vector<int_mod<7>, 3>;
    CHECK(V{ 3, 4, 5 } * V{ 4, 5, 2 } == V{ 5, 6, 3 });
    CHECK(V{ 1, 1, 1 } / V{ 4, 5, 2 } == V{ 2, 3, 4 });
}
