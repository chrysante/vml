#include <vml/shapes.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace vml::short_types;

TEST_CASE("vml::rectangle") {
    vml::rectangle<float> R = { float2{ 1, 1 }, float2{ 3, 3 } };
    using namespace Catch::literals;
    CHECK(area(R) == 4.0_a);
}

TEST_CASE("vml::rectangle intersect") {
    {
        vml::rectangle<float> const R = { float2{ 0, 0 }, float2{ 2, 2 } };
        vml::rectangle<float> const S = { float2{ 1, 1 }, float2{ 1.5, 1.5 } };
        CHECK(do_intersect(R, S));
        CHECK(do_intersect(S, R));
    }
    {
        vml::rectangle<float> const R = { float2{ 0, 0 }, float2{ 2, 2 } };
        vml::rectangle<float> const S = { float2{ -2, -2 }, float2{ -1, -1 } };
        CHECK(!do_intersect(R, S));
        CHECK(!do_intersect(S, R));
    }
    {
        vml::rectangle<float> const R = { float2{ 0, 0 }, float2{ 4, 1 } };
        vml::rectangle<float> const S = { float2{ 1, -1 }, float2{ 2, 3 } };
        CHECK(do_intersect(R, S));
        CHECK(do_intersect(S, R));
    }
    {
        vml::rectangle<float> const R = { float2{ 0, 0 }, float2{ 4, 1 } };
        vml::rectangle<float> const S = { float2{ 1, 4 }, float2{ 2, 8 } };
        CHECK(!do_intersect(R, S));
        CHECK(!do_intersect(S, R));
    }
    {
        vml::rectangle<float> const R = { float2{ 0, 0 }, float2{ 4, 4 } };
        vml::float2 const p = { 1, 1 };
        CHECK(vml::do_intersect(R, p));
        CHECK(vml::do_intersect(p, R));

        vml::float2 const q = { -1, 1 };
        CHECK(!vml::do_intersect(R, q));
        CHECK(!vml::do_intersect(q, R));

        vml::float2 const r = { -1, -1 };
        CHECK(!vml::do_intersect(R, r));
        CHECK(!vml::do_intersect(r, R));
    }
}

TEST_CASE("vml::sphere overlap") {
    {
        vml::sphere<float, 3> const R = { { 0, 0, 0 }, 1 };
        vml::sphere<float, 3> const S = { { 1, 0, 0 }, 0.5 };
        CHECK(vml::do_intersect(R, S));
        CHECK(vml::do_intersect(S, R));
    }
    {
        vml::sphere<float, 3> const R = { { 0, 0, 0 }, 1 };
        vml::sphere<float, 3> const S = { { -2, -2, -2 }, 1 };
        CHECK(!vml::do_intersect(R, S));
        CHECK(!vml::do_intersect(S, R));
    }
    {
        vml::sphere<float, 3> const R = { { 0, 0, 0 }, 2 };
        vml::float3 const p = { 1, 1, 1 };
        CHECK(vml::do_intersect(R, p));
        CHECK(vml::do_intersect(p, R));
    }
}

TEST_CASE("vml::sphere vml::AABB overlap") {
    {
        vml::sphere<float, 2> const sphere = { { 0, 0 }, 1 };
        vml::AABB<float, 2> const box = { float2{ 0.5, 0 }, float2{ 1.5, 1 } };
        CHECK(vml::do_intersect(box, sphere));
    }
    {
        vml::sphere<float, 2> const sphere = { { 0, 0 }, 1 };
        vml::AABB<float, 2> const box = { float2{ 1.5, 0 }, float2{ 2.5, 1 } };
        CHECK(!vml::do_intersect(box, sphere));
    }
}
