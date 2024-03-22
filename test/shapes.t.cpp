#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <vml/vml.hpp>

TEST_CASE("mtl::rectangle") {
	mtl::rectangle<float> R = { { 1, 1 }, { 2, 2 } };
	using namespace Catch::literals;
	CHECK(area(R) == 4.0_a);
}

TEST_CASE("mtl::rectangle intersect") {
	{
		mtl::rectangle<float> const R = { { 0, 0 }, { 2, 2 } };
		mtl::rectangle<float> const S = { { 1, 1 }, { 0.5, 0.5 } };
		CHECK(do_intersect(R, S));
		CHECK(do_intersect(S, R));
	}
	{
		mtl::rectangle<float> const R = { { 0, 0 }, { 2, 2 } };
		mtl::rectangle<float> const S = { { -2, -2 }, { 1, 1 } };
		CHECK(!do_intersect(R, S));
		CHECK(!do_intersect(S, R));
	}
	{
		mtl::rectangle<float> const R = { { 0, 0 }, { 4, 1 } };
		mtl::rectangle<float> const S = { { 1, -1 }, { 1, 4 } };
		CHECK(do_intersect(R, S));
		CHECK(do_intersect(S, R));
	}
	{
		mtl::rectangle<float> const R = { { 0, 0 }, { 4, 1 } };
		mtl::rectangle<float> const S = { { 1, 4 }, { 1, 4 } };
		CHECK(!do_intersect(R, S));
		CHECK(!do_intersect(S, R));
	}
	{
		mtl::rectangle<float> const R = { { 0, 0 }, { 4, 4 } };
		mtl::float2 const p = { 1, 1 };
		CHECK(mtl::do_intersect(R, p));
		CHECK(mtl::do_intersect(p, R));
		
		mtl::float2 const q = { -1, 1 };
		CHECK(!mtl::do_intersect(R, q));
		CHECK(!mtl::do_intersect(q, R));
		
		mtl::float2 const r = { -1, -1 };
		CHECK(!mtl::do_intersect(R, r));
		CHECK(!mtl::do_intersect(r, R));
	}
}

TEST_CASE("mtl::sphere overlap") {
	{
		mtl::sphere<float, 3> const R = { { 0, 0, 0 }, 1 };
		mtl::sphere<float, 3> const S = { { 1, 0, 0 }, 0.5 };
		CHECK(mtl::do_intersect(R, S));
		CHECK(mtl::do_intersect(S, R));
	}
	{
		mtl::sphere<float, 3> const R = { { 0, 0, 0 }, 1 };
		mtl::sphere<float, 3> const S = { { -2, -2, -2 }, 1 };
		CHECK(!mtl::do_intersect(R, S));
		CHECK(!mtl::do_intersect(S, R));
	}
	{
		mtl::sphere<float, 3> const R = { { 0, 0, 0 }, 2 };
		mtl::float3 const p = { 1, 1, 1 };
		CHECK(mtl::do_intersect(R, p));
		CHECK(mtl::do_intersect(p, R));
	}
}

TEST_CASE("mtl::sphere mtl::AABB overlap") {
	{
		mtl::sphere<float, 2> const sphere = { { 0, 0 }, 1 };
		mtl::AABB<float, 2>   const box    = { { 0.5, 0 }, { 1, 1} };
		CHECK(mtl::do_intersect(box, sphere));
	}
	{
		mtl::sphere<float, 2> const sphere = { { 0, 0 }, 1 };
		mtl::AABB<float, 2>   const box    = { { 1.5, 0 }, { 1, 1} };
		CHECK(!mtl::do_intersect(box, sphere));
	}
}
