#include <catch2/catch_test_macros.hpp>

#include <vml/vml.hpp>

using namespace mtl::short_types;

TEST_CASE("vector ext", "[vector]") {
	int3 const v = { 1, 2, 3 };
	short3 const w = { 4, 2, -1 };
	CHECK(mtl::dot(v, w) == 4 + 4 - 3);
	CHECK(mtl::norm_squared(v) == 14);
	CHECK(mtl::norm(v) == std::sqrt(14));
	CHECK(mtl::length(mtl::normalize(v)) == 1);
	
	float3 const f = { 0.3f, 1.3f, -0.3f };
	CHECK(mtl::fract(f) == float3{ 0.3f, 1.3f - 1.0f, 1.0f - 0.3f });
	
	CHECK(mtl::concat(1, 2)                         == int2{ 1, 2       });
	CHECK(mtl::concat(1, 2, 3)                      == int3{ 1, 2, 3    });
	CHECK(mtl::concat(1, 2, 3, 4)                   == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(int2{ 1, 2 }, 3, 4)           == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(1, int2{ 2, 3 }, 4)           == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(1, 2, int2{ 3, 4 })           == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(int2{ 1, 2 }, float2{ 3, 4 }) == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(int3{ 1, 2, 3 }, 4)           == int4{ 1, 2, 3, 4 });
	CHECK(mtl::concat(1, int3{ 2, 3, 4 })           == int4{ 1, 2, 3, 4 });
	
	CHECK(mtl::reverse(int4{ 1, 2, 3, 4 }) == int4{ 4, 3, 2, 1 });
	
	CHECK(mtl::type_cast<float>(int4{ 1, 2, 3, 4 }) == int4{ 1, 2, 3, 4 });
	
	CHECK(mtl::dimension_cast<3>(int4{ 1, 2, 3, 4 }) == int3{ 1, 2, 3 });
	CHECK(mtl::dimension_cast<5>(int4{ 1, 2, 3, 4 }) == mtl::vector<int, 5>{ 1, 2, 3, 4, 0 });
	
	// norm without potential intermediate overflow
	CHECK(mtl::norm(int3{ 1, 2, 3 }) == std::sqrt(1 + 4 + 9));
	// norm with potential intermediate overflow
	{
		double x = std::numeric_limits<double>::max() / 2;
		CHECK(mtl::norm(double3{ x }) == std::sqrt(3) * x);
		
		CHECK(mtl::hypot(x, x / 2) == std::hypot(x, x / 2));
		// fails because libc++'s implementation of hypot(x, y, z) is the naive sqrt(x*x+y*y+z*z)
//		CHECK(mtl::hypot(x, x / 2, x / 3) == std::hypot(x, x / 2, x / 3));
	}
	
	// pnorm without potential intermediate overflow
	CHECK(mtl::pnorm(2, int3{ 1, 2, 3 }) == std::pow(1 + 4 + 9, 1.0 / 2.0));
	CHECK(mtl::pnorm(3, int3{ 1, 2, 3 }) == std::pow(1 + 8 + 27, 1.0 / 3.0));
	CHECK(mtl::pnorm(3, int3{ 3 })       == std::pow(3, 1.0 / 3.0) * 3);
	// pnorm with potential intermediate overflow
	{
		double x = std::numeric_limits<double>::max() / 2;
		CHECK(mtl::pnorm(3, double3{ x }) == std::pow(3, 1.0 / 3.0) * x);
		CHECK(mtl::pnorm(2, double3{ x }) == std::pow(3, 1.0 / 2.0) * x);
		
		CHECK(mtl::pnorm(1e100, double3{ x }) == std::pow(1e100, 1.0 / 1e100) * x);
	}
}

TEST_CASE("matrix ext", "[matrix]") {
	SECTION("transpose") {
		int4x3 const m = {
			 1,  2,  3,
			 4,  5,  6,
			 7,  8,  9,
			10, 11, 12
		};
		
		int3x4 const ref = {
			1, 4, 7, 10,
			2, 5, 8, 11,
			3, 6, 9, 12
		};
		
		CHECK(mtl::transpose(m) == ref);
	}
	
	SECTION("casts") {
		int4x3 const m = {
			 1,  2,  3,
			 4,  5,  6,
			 7,  8,  9,
			10, 11, 12
		};
		
		CHECK(mtl::type_cast<float>(m) == m);
		
		int4x4 const ref_a = {
			 1,  2,  3, 0,
			 4,  5,  6, 0,
			 7,  8,  9, 0,
			10, 11, 12, 0
		};
		CHECK(mtl::dimension_cast<4, 4>(m) == ref_a);
		
		int3x3 const ref_b = {
			 1,  2,  3,
			 4,  5,  6,
			 7,  8,  9
		};
		CHECK(mtl::dimension_cast<3, 3>(m) == ref_b);
		
		
		mtl::matrix<int, 5, 4> const ref_c = {
			1,  2,  3, 0,
			4,  5,  6, 0,
			7,  8,  9, 0,
		   10, 11, 12, 0,
			0,  0,  0, 0
		};
		CHECK(mtl::dimension_cast<5, 4>(m) == ref_c);
	}
}

TEST_CASE("matrix inverse") {
	SECTION("4x4") {
		ldouble4x4 const A = {
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 0, 1, 2,
			3, 0, 5, 6
		};
		ldouble4x4 const I = mtl::inverse(A);
		
		CHECK(A * I == mtl::approx(ldouble4x4(1)).epsilon(0.00000000001));
	}
	
	SECTION("3x3") {
		ldouble3x3 const A = {
			1, 2, 3,
			5, 6, 7,
			9, 0, 1
		};
		ldouble3x3 const I = mtl::inverse(A);
		
		CHECK(A * I == mtl::approx(ldouble3x3(1)).epsilon(0.00000000001));
	}
	
	SECTION("2x2") {
		ldouble2x2 const A = {
			1, 2,
			5, 6
		};
		ldouble2x2 const I = mtl::inverse(A);
		
		CHECK(A * I == mtl::approx(ldouble2x2(1)).epsilon(0.00000000001));
	}
}

TEST_CASE("submatrix") {
	int4x4 const A = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 0, 1, 2,
		4, 5, 6, 7
	};
	
	CHECK(mtl::submatrix(A, 2, 1) == int3x3{
		1, 3, 4,
		5, 7, 8,
		4, 6, 7
	});
}

TEST_CASE("matrix determinant", "[matrix]") {
	ldouble4x4 const A = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 0, 1, 2,
		3, 0, 5, 6
	};
	
	CHECK(mtl::det(A) == 160);
}

TEST_CASE("AABB") {

	mtl::AABB<float, 2> a = { { 0, 1 }, { 2, 3 } };
	
	CHECK(a.lower_bound() == mtl::float2{ 0, 1 });
	CHECK(a.size() == mtl::float2{ 2, 3 });
	
	mtl::AABB<float, 2> b = { { -2, -2 }, { 2, 3 } };
	
	CHECK(b.lower_bound() == mtl::float2{ -2, -2 });
	CHECK(b.size() == mtl::float2{ 2, 3 });

	auto c = mtl::enclosing(a, b);
	CHECK(c.lower_bound() == mtl::float2{ -2, -2 });
	CHECK(c.size() == mtl::float2{ 4, 6 });
	
}
