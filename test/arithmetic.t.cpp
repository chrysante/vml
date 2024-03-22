#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <vml/vml.hpp>

using namespace mtl::short_types;

TEST_CASE("vector basic arithmetic", "[vector]") {
    int3 a = { 1, 2, 3 };
    float3 b = { 0.1f, 0.2f, 0.3f };
    CHECK(a + b == float3{ 1.1f, 2.2f, 3.3f });
    CHECK(a + 1 == int3{ 2, 3, 4 });
    CHECK(1 + a == int3{ 2, 3, 4 });
    CHECK(a - b == float3{ 0.9f, 1.8f, 2.7f });
    CHECK(a - 1 == int3{ 0, 1, 2 });
    CHECK(1 - a == int3{ 0, -1, -2 });
    CHECK(a * b == float3{ 0.1f, 0.2f * 2, 0.3f * 3 });
    CHECK(a * 0.5 == float3{ 0.5, 1, 1.5 });
    CHECK(0.5 * a == float3{ 0.5, 1, 1.5 });
    CHECK(a / 2.0 == float3{ 0.5, 1, 1.5 });

    int3 c = { 2, 2, 2 };
    CHECK(a % c == int3{ 1, 0, 1 });
    CHECK(a % 2 == int3{ 1, 0, 1 });
    CHECK(2 % a == int3{ 0, 0, 2 });

    using namespace mtl::literals;
    CHECK(complex_int{ 3, -2 } * 1_i ==
          mtl::__mtl_mul_by_i(complex_int{ 3, -2 }));
}

TEST_CASE("matrix basic arithmetic", "[matrix]") {
    int2x2 const a = { 1, 2, 3, 4 };
    float2x2 const b = { 0.1f, 0.2f, 0.3f, 0.4f };
    CHECK(a + b == float2x2{ 1.1f, 2.2f, 3.3f, 4.4f });
    CHECK(a - b == float2x2{ 0.9f, 1.8f, 2.7f, 3.6f });
    CHECK(a * 0.5 == float2x2{ 0.5f, 1.0f, 1.5f, 2.0f });
    CHECK(0.5 * a == float2x2{ 0.5f, 1.0f, 1.5f, 2.0f });
    CHECK(a / 2.0 == float2x2{ 0.5f, 1.0f, 1.5f, 2.0f });
    CHECK(a % 2 == int2x2{ 1, 0, 1, 0 });
}

TEST_CASE("matrix multiplication", "[matrix]") {
    int3x3 const A = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    CHECK(A * A == int3x3{ 30, 36, 42, 66, 81, 96, 102, 126, 150 });
    int3x3 const B = { 5, 2, 3, 4, 1, 2, 5, 8, 7 };
    CHECK(A * B == int3x3{ 28, 28, 28, 70, 61, 64, 112, 94, 100 });
    int3 const b = { 1, 4, 7 };
    CHECK(A * b == int3{ 30, 66, 102 });
    CHECK(b * A == int3{ 66, 78, 90 });
}

#define TYPE_LIST(_Tp, _Up)                                                    \
    (mtl::quaternion<_Tp>, mtl::quaternion<_Up>, 0),                           \
        (mtl::quaternion<_Tp>, mtl::complex<_Up>, 0),                          \
        (mtl::quaternion<_Tp>, _Up, 0),                                        \
        (mtl::complex<_Tp>, mtl::quaternion<_Up>, 0),                          \
        (_Tp, mtl::quaternion<_Up>, 0),                                        \
        (mtl::complex<_Tp>, mtl::complex<_Up>, 0),                             \
        (_Tp, mtl::complex<_Up>, 0), (mtl::complex<_Tp>, _Up, 0),              \
                                                                               \
        (mtl::vector3<mtl::quaternion<_Tp>>, mtl::quaternion<_Up>, 0),         \
        (mtl::vector3<mtl::quaternion<_Tp>>, mtl::complex<_Up>, 0),            \
        (mtl::vector3<mtl::quaternion<_Tp>>, _Up, 0),                          \
        (mtl::vector3<mtl::complex<_Tp>>, mtl::quaternion<_Up>, 0),            \
        (mtl::vector3<_Tp>, mtl::quaternion<_Up>, 0),                          \
        (mtl::vector3<mtl::complex<_Tp>>, mtl::complex<_Up>, 0),               \
        (mtl::vector3<_Tp>, mtl::complex<_Up>, 0),                             \
        (mtl::vector3<mtl::complex<_Tp>>, _Up, 0),                             \
                                                                               \
        (mtl::quaternion<_Tp>, mtl::vector3<mtl::quaternion<_Up>>, 0),         \
        (mtl::quaternion<_Tp>, mtl::vector3<mtl::complex<_Up>>, 0),            \
        (mtl::quaternion<_Tp>, mtl::vector3<_Up>, 0),                          \
        (mtl::complex<_Tp>, mtl::vector3<mtl::quaternion<_Up>>, 0),            \
        (_Tp, mtl::vector3<mtl::quaternion<_Up>>, 0),                          \
        (mtl::complex<_Tp>, mtl::vector3<mtl::complex<_Up>>, 0),               \
        (_Tp, mtl::vector3<mtl::complex<_Up>>, 0),                             \
        (mtl::complex<_Tp>, mtl::vector3<_Up>, 0),                             \
                                                                               \
        (mtl::vector3<mtl::quaternion<_Tp>>,                                   \
         mtl::vector3<mtl::quaternion<_Up>>, 0),                               \
        (mtl::vector3<mtl::quaternion<_Tp>>, mtl::vector3<mtl::complex<_Up>>,  \
         0),                                                                   \
        (mtl::vector3<mtl::quaternion<_Tp>>, mtl::vector3<_Up>, 0),            \
        (mtl::vector3<mtl::complex<_Tp>>, mtl::vector3<mtl::quaternion<_Up>>,  \
         0),                                                                   \
        (mtl::vector3<_Tp>, mtl::vector3<mtl::quaternion<_Up>>, 0),            \
        (mtl::vector3<mtl::complex<_Tp>>, mtl::vector3<mtl::complex<_Up>>, 0), \
        (mtl::vector3<_Tp>, mtl::vector3<mtl::complex<_Up>>, 0),               \
        (mtl::vector3<mtl::complex<_Tp>>, mtl::vector3<_Up>, 0)

#define ARITHMETIC_INST_TEST(...)                                              \
    TEMPLATE_TEST_CASE_SIG(__VA_ARGS__,                                        \
                           ((typename T, typename U, int _ignore_this), T, U,  \
                            _ignore_this),                                     \
                           TYPE_LIST(int, int), TYPE_LIST(float, float),       \
                           TYPE_LIST(int, float))

ARITHMETIC_INST_TEST("arithmetic instantiation test",
                     "[vector][quaternion][complex]") {
    /// We don't want to execute these because we'd divide by zero, we just
    /// wan't to instantiate everything
    if ((false)) {
        (void)+T{};
        (void)-T{};
        (void)(T{} + U{});
        (void)(T{} - U{});
        (void)(T{} * U{});
        (void)(T{} / U{});
    }
}

TEST_CASE("arithmetic matrix instantiation test",
          "[matrix][vector][quaternion][complex]") {
    using namespace mtl;
    static_assert(requires {
        {
            matrix3x3<complex<float>>{} * quaternion<int> {}
        } -> std::same_as<matrix3x3<quaternion<float>>>;
    });
    static_assert(requires {
        {
            matrix3x3<complex<float>>{} * vector3<quaternion<int>> {}
        } -> std::same_as<vector3<quaternion<float>>>;
    });
}
