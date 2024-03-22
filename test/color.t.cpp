#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <vml/vml.hpp>

TEST_CASE("rgb_to_hsv") {
    CHECK(mtl::rgb_to_hsv(mtl::float3(1, 0, 0)) == mtl::float3(0, 1, 1));
    CHECK(mtl::rgb_to_hsv(mtl::float3(1, 1, 0)) ==
          mtl::float3(1.0 / 6.0, 1, 1));
    CHECK(mtl::rgb_to_hsv(mtl::float3(1, 1, 1)) == mtl::float3(0, 0, 1));
    CHECK(mtl::rgb_to_hsv(mtl::float3(0, 0, 0)) == mtl::float3(0, 0, 0));
    /// We need to tear this one test apart because of floating point
    /// inaccuracies
    auto c = mtl::rgb_to_hsv(mtl::float3(0, 0, 1));
    using namespace Catch::literals;
    CHECK(c.r == 0.66666_a);
    CHECK(c.g == 1.0);
    CHECK(c.b == 1.0);
    CHECK(mtl::rgb_to_hsv(mtl::float3(0.5)) == mtl::float3(0, 0, 0.5));
}

TEST_CASE("hsv_to_rgb") {
    CHECK(mtl::hsv_to_rgb(mtl::float3(0, 1, 1)) == mtl::float3(1, 0, 0));
    CHECK(mtl::hsv_to_rgb(mtl::float3(1.0 / 6.0, 1, 1)) ==
          mtl::float3(1, 1, 0));
    CHECK(mtl::hsv_to_rgb(mtl::float3(0, 0, 1)) == mtl::float3(1, 1, 1));
    CHECK(mtl::hsv_to_rgb(mtl::float3(0, 0, 0)) == mtl::float3(0, 0, 0));
    CHECK(mtl::hsv_to_rgb(mtl::float3(4.0 / 6.0, 1, 1)) ==
          mtl::float3(0, 0, 1));
    CHECK(mtl::hsv_to_rgb(mtl::float3(0, 0, 0.5)) == mtl::float3(0.5));
}
