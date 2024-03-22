#include <vml/vml.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("rgb_to_hsv") {
    CHECK(vml::rgb_to_hsv(vml::float3(1, 0, 0)) == vml::float3(0, 1, 1));
    CHECK(vml::rgb_to_hsv(vml::float3(1, 1, 0)) ==
          vml::float3(1.0 / 6.0, 1, 1));
    CHECK(vml::rgb_to_hsv(vml::float3(1, 1, 1)) == vml::float3(0, 0, 1));
    CHECK(vml::rgb_to_hsv(vml::float3(0, 0, 0)) == vml::float3(0, 0, 0));
    /// We need to tear this one test apart because of floating point
    /// inaccuracies
    auto c = vml::rgb_to_hsv(vml::float3(0, 0, 1));
    using namespace Catch::literals;
    CHECK(c.r == 0.66666_a);
    CHECK(c.g == 1.0);
    CHECK(c.b == 1.0);
    CHECK(vml::rgb_to_hsv(vml::float3(0.5)) == vml::float3(0, 0, 0.5));
}

TEST_CASE("hsv_to_rgb") {
    CHECK(vml::hsv_to_rgb(vml::float3(0, 1, 1)) == vml::float3(1, 0, 0));
    CHECK(vml::hsv_to_rgb(vml::float3(1.0 / 6.0, 1, 1)) ==
          vml::float3(1, 1, 0));
    CHECK(vml::hsv_to_rgb(vml::float3(0, 0, 1)) == vml::float3(1, 1, 1));
    CHECK(vml::hsv_to_rgb(vml::float3(0, 0, 0)) == vml::float3(0, 0, 0));
    CHECK(vml::hsv_to_rgb(vml::float3(4.0 / 6.0, 1, 1)) ==
          vml::float3(0, 0, 1));
    CHECK(vml::hsv_to_rgb(vml::float3(0, 0, 0.5)) == vml::float3(0.5));
}
