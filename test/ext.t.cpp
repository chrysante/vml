#include <vml/vml.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace vml::short_types;

TEST_CASE("vector ext", "[vector]") {
    int3 const v = { 1, 2, 3 };
    short3 const w = { 4, 2, -1 };
    CHECK(vml::dot(v, w) == 4 + 4 - 3);
    CHECK(vml::norm_squared(v) == 14);
    CHECK(vml::norm(v) == std::sqrt(14));
    CHECK(vml::length(vml::normalize(v)) == 1);

    float3 const f = { 0.3f, 1.3f, -0.3f };
    CHECK(vml::fract(f) == float3{ 0.3f, 1.3f - 1.0f, 1.0f - 0.3f });

    CHECK(vml::concat(1, 2) == int2{ 1, 2 });
    CHECK(vml::concat(1, 2, 3) == int3{ 1, 2, 3 });
    CHECK(vml::concat(1, 2, 3, 4) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(int2{ 1, 2 }, 3, 4) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(1, int2{ 2, 3 }, 4) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(1, 2, int2{ 3, 4 }) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(int2{ 1, 2 }, float2{ 3, 4 }) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(int3{ 1, 2, 3 }, 4) == int4{ 1, 2, 3, 4 });
    CHECK(vml::concat(1, int3{ 2, 3, 4 }) == int4{ 1, 2, 3, 4 });

    CHECK(vml::reverse(int4{ 1, 2, 3, 4 }) == int4{ 4, 3, 2, 1 });

    CHECK(vml::type_cast<float>(int4{ 1, 2, 3, 4 }) == int4{ 1, 2, 3, 4 });

    CHECK(vml::dimension_cast<3>(int4{ 1, 2, 3, 4 }) == int3{ 1, 2, 3 });
    CHECK(vml::dimension_cast<5>(int4{ 1, 2, 3, 4 }) ==
          vml::vector<int, 5>{ 1, 2, 3, 4, 0 });

    // norm without potential intermediate overflow
    CHECK(vml::norm(int3{ 1, 2, 3 }) == std::sqrt(1 + 4 + 9));
    // norm with potential intermediate overflow
    {
        double x = std::numeric_limits<double>::max() / 2;
        CHECK(vml::norm(double3{ x }) == std::sqrt(3) * x);

        CHECK(vml::hypot(x, x / 2) == std::hypot(x, x / 2));
        // fails because libc++'s implementation of hypot(x, y, z) is the naive
        // sqrt(x*x+y*y+z*z)
        //		CHECK(vml::hypot(x, x / 2, x / 3) == std::hypot(x, x / 2, x /
        // 3));
    }

    // pnorm without potential intermediate overflow
    CHECK(vml::pnorm(2, int3{ 1, 2, 3 }) == std::pow(1 + 4 + 9, 1.0 / 2.0));
    CHECK(vml::pnorm(3, int3{ 1, 2, 3 }) == std::pow(1 + 8 + 27, 1.0 / 3.0));
    CHECK(vml::pnorm(3, int3{ 3 }) == std::pow(3, 1.0 / 3.0) * 3);
    // pnorm with potential intermediate overflow
    {
        double x = std::numeric_limits<double>::max() / 2;
        CHECK(vml::pnorm(3, double3{ x }) == std::pow(3, 1.0 / 3.0) * x);
        CHECK(vml::pnorm(2, double3{ x }) == std::pow(3, 1.0 / 2.0) * x);

        CHECK(vml::pnorm(1e100, double3{ x }) ==
              std::pow(1e100, 1.0 / 1e100) * x);
    }
}

TEST_CASE("matrix ext", "[matrix]") {
    SECTION("transpose") {
        int4x3 const m = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

        int3x4 const ref = { 1, 4, 7, 10, 2, 5, 8, 11, 3, 6, 9, 12 };

        CHECK(vml::transpose(m) == ref);
    }

    SECTION("casts") {
        int4x3 const m = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

        CHECK(vml::type_cast<float>(m) == m);

        int4x4 const ref_a = {
            1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 10, 11, 12, 0
        };
        CHECK(vml::dimension_cast<4, 4>(m) == ref_a);

        int3x3 const ref_b = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK(vml::dimension_cast<3, 3>(m) == ref_b);

        vml::matrix<int, 5, 4> const ref_c = {
            1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0
        };
        CHECK(vml::dimension_cast<5, 4>(m) == ref_c);
    }
}

TEST_CASE("matrix inverse") {
    SECTION("4x4") {
        ldouble4x4 const A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0, 5, 6 };
        ldouble4x4 const I = vml::inverse(A);

        CHECK(A * I == vml::approx(ldouble4x4(1)).epsilon(0.00000000001));
    }

    SECTION("3x3") {
        ldouble3x3 const A = { 1, 2, 3, 5, 6, 7, 9, 0, 1 };
        ldouble3x3 const I = vml::inverse(A);

        CHECK(A * I == vml::approx(ldouble3x3(1)).epsilon(0.00000000001));
    }

    SECTION("2x2") {
        ldouble2x2 const A = { 1, 2, 5, 6 };
        ldouble2x2 const I = vml::inverse(A);

        CHECK(A * I == vml::approx(ldouble2x2(1)).epsilon(0.00000000001));
    }
}

TEST_CASE("submatrix") {
    int4x4 const A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 4, 5, 6, 7 };
    CHECK(vml::submatrix(A, 2, 1) == int3x3{ 1, 3, 4, 5, 7, 8, 4, 6, 7 });
}

TEST_CASE("matrix determinant", "[matrix]") {
    ldouble4x4 const A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0, 5, 6 };
    CHECK(vml::det(A) == 160);
}

TEST_CASE("AABB") {
    vml::AABB<float, 2> a = { float2{ 0, 1 }, float2{ 2, 4 } };

    CHECK(a.lower_bound() == vml::float2{ 0, 1 });
    CHECK(a.size() == vml::float2{ 2, 3 });

    vml::AABB<float, 2> b = { float2{ -2, -2 }, float2{ 0, 1 } };

    CHECK(b.lower_bound() == vml::float2{ -2, -2 });
    CHECK(b.size() == vml::float2{ 2, 3 });

    auto c = vml::enclosing(a, b);
    CHECK(c.lower_bound() == vml::float2{ -2, -2 });
    CHECK(c.size() == vml::float2{ 4, 6 });
}

TEST_CASE("AABB from size") {
    auto rect = vml::AABB<float, 2>::from_size({ 0, 1 }, { 2, 4 });
    CHECK(rect.lower_bound() == vml::float2{ 0, 1 });
    CHECK(rect.size() == vml::float2{ 2, 4 });
}

TEST_CASE("lerp", "[vector]") {
    float3 v = { 0.9, 0.5, 0.1 };
    auto w = vml::lerp(float3{ 0, 0, 0 }, float3{ 1, 1, 1 }, v);
    CHECK(w == v);
}

TEST_CASE("slerp", "[quaternion]") {
    using namespace Catch::literals;
    quaternion_double a = vml::make_rotation(0.5, double3{ 1, 0, 0 });
    quaternion_double b = vml::make_rotation(1.5, double3{ 0, 1, 0 });
    REQUIRE(vml::norm(a) == 1.0_a);
    REQUIRE(vml::norm(b) == 1.0_a);
    for (double t = 0.0; t <= 1.0; t += 0.125) {
        CHECK(vml::norm(vml::slerp(a, b, t)) == 1.0_a);
    }
    CHECK(vml::slerp(a, b, 0.0) == vml::approx{ a });
    CHECK(vml::slerp(a, b, 1.0) == vml::approx{ b });
}

TEST_CASE("slerp", "[vector]") {
    using namespace Catch::literals;
    double3 a = { 1, 0, 0 };
    double3 b = vml::normalize(double3{ 0, 1, 1 });
    double angle = std::acos(vml::dot(a, b));
    for (double t = 0.0; t <= 1.0; t += 0.125) {
        double3 s = vml::slerp(a, b, t);
        CHECK(vml::norm(s) == 1.0_a);
        double sAngle = std::acos(vml::dot(a, s));
        CHECK(sAngle == Catch::Approx(t * angle));
    }
    CHECK(vml::slerp(a, b, 0.0) == vml::approx{ a });
    CHECK(vml::slerp(a, b, 1.0) == vml::approx{ b });
}

TEST_CASE("quaternion to euler", "[quaternion]") {
    auto q = vml::make_rotation(vml::constants<>::pi, { 0, 0, 1 });
    auto euler = vml::to_euler(q);
    CHECK(euler.x == 0);
    CHECK(euler.y == 0);
    CHECK(euler.z == vml::constants<>::pi);
}

TEST_CASE("2D Transforms", "[vector][matrix]") {
    auto m = vml::make_rotation2x2(vml::constants<>::pi / 2);
    CHECK(m == vml::approx(float2x2{ 0, 1, -1, 0 }));
    CHECK(vml::rotate(float2{ 2, 1 }, vml::constants<>::pi) ==
          vml::approx(vml::float2{ -2, -1 }));
}
