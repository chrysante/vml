#include <vml/__matrix.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#define TYPE_LIST(O)                                                           \
    (int, 2, 2, O), (int, 3, 2, O), (int, 4, 2, O), (int, 2, 3, O),            \
        (int, 3, 3, O), (int, 4, 3, O), (int, 2, 4, O), (int, 3, 4, O),        \
        (int, 4, 4, O), (unsigned, 2, 2, O), (unsigned, 3, 2, O),              \
        (unsigned, 4, 2, O), /*(unsigned, 2, 3, O), (unsigned, 3, 3, O),       \
                                (unsigned, 4, 3, O),*/                         \
        (unsigned, 2, 4, O), (unsigned, 3, 4, O), (unsigned, 4, 4, O),         \
        (float, 2, 2, O), (float, 3, 2, O), (float, 4, 2, O),                  \
        (float, 2, 3, O), (float, 3, 3, O), (float, 4, 3, O),                  \
        (float, 2, 4, O), (float, 3, 4, O), (float, 4, 4, O),                  \
        (double, 2, 2, O), (double, 3, 2, O),                                  \
        (double, 4, 2,                                                         \
         O), /*(double, 2, 3, O), (double, 3, 3, O), (double, 4, 3, O),*/      \
        (double, 2, 4, O), (double, 3, 4, O), (double, 4, 4, O)

#define MATRIX_TEST_CASE(...)                                                  \
    TEMPLATE_TEST_CASE_SIG(__VA_ARGS__,                                        \
                           ((typename T, std::size_t Rows,                     \
                             std::size_t Columns,                              \
                             mtl::vector_options Options),                     \
                            T, Rows, Columns, Options),                        \
                           TYPE_LIST(mtl::vector_options{}),                   \
                           TYPE_LIST(mtl::vector_options{}.packed(true)))

#define MATRIX_TYPE ::mtl::matrix<T, Rows, Columns, Options>

using namespace mtl::short_types;

namespace {
template <typename T>
struct OtherType {
    using type = int;
};
template <>
struct OtherType<int> {
    using type = float;
};
template <mtl::vector_options O>
struct OtherOptions {
    auto constexpr static value = O.packed(!O.packed());
};
} // namespace

MATRIX_TEST_CASE("matrix offsets", "[matrix][matrix_data]") {
    using M = mtl::matrix<T, Rows, Columns, Options>;
    CHECK(std::is_trivial_v<M>);
    if constexpr (Options.packed()) {
        CHECK(sizeof(M) == sizeof(T) * M::size());
        CHECK(alignof(M) == alignof(M));
    }
    else {
        CHECK(sizeof(M) == sizeof(T) * (Columns + Columns % 2) * Rows);
        CHECK(alignof(M) == sizeof(typename M::row_type));
    }
}

MATRIX_TEST_CASE("matrix single value constructor", "[matrix]") {
    T const value = (T)GENERATE(0, 1, -1, 5321);
    mtl::matrix<T, Rows, Columns, Options> const m = value;
    for (std::size_t i = 0; i < Rows; ++i) {
        for (std::size_t j = 0; j < Columns; ++j) {
            if (i == j) {
                CHECK(m(i, j) == value);
            }
            else {
                CHECK(m(i, j) == 0);
            }
        }
    }
}

MATRIX_TEST_CASE("matrix functional constructor", "[matrix]") {
    using M = mtl::matrix<T, Rows, Columns, Options>;
    T const values[16] = { 0,     1, (T)-1, 5321,  35,     (T)-23, (T)1.2,  3,
                           (T)-2, 0, 1,     (T)-3, (T)3.5, 23,     (T)-1.2, 3 };
    M m;
    SECTION("1") {
        m = M([&, i = 0]() mutable { return values[i++]; });
    }
    SECTION("2") {
        m = M([&](int i) { return values[i]; });
        INFO(m);

        for (int i = 0; auto v: m) {
            CHECK(v == values[i++]);
        }
    }
    SECTION("3") {
        m = M([&](auto i, auto j) { return values[i * 4 + j]; });

        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Columns; ++j) {
                CHECK(m(i, j) == values[i * 4 + j]);
            }
        }
    }
    SECTION("4") {
        using RowType = typename M::row_type;
        RowType rows[4] = { RowType([&](int i) { return values[0 + i]; }),
                            RowType([&](int i) { return values[4 + i]; }),
                            RowType([&](int i) { return values[8 + i]; }),
                            RowType([&](int i) { return values[12 + i]; }) };
        m = M(mtl::rows, [&](int i) { return rows[i]; });
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Columns; ++j) {
                CHECK(m(i, j) == values[i * 4 + j]);
            }
        }
    }
    SECTION("5") {
        using ColumnType = typename M::column_type;
        ColumnType columns[4] = {
            ColumnType([&](int i) { return values[i * 4 + 0]; }),
            ColumnType([&](int i) { return values[i * 4 + 1]; }),
            ColumnType([&](int i) { return values[i * 4 + 2]; }),
            ColumnType([&](int i) { return values[i * 4 + 3]; })
        };
        m = M(mtl::columns, [&](int i) { return columns[i]; });
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Columns; ++j) {
                CHECK(m(i, j) == values[i * 4 + j]);
            }
        }
    }
}

MATRIX_TEST_CASE("matrix conversion", "[matrix]") {
    T const values[16] = { 0,     1, (T)-1, 5321,  35,     (T)-23, (T)1.2,  3,
                           (T)-2, 0, 1,     (T)-3, (T)3.5, 23,     (T)-1.2, 3 };
    mtl::matrix<T, Rows, Columns, Options> const m(
        [&](int i) { return values[i]; });
    using U = typename OtherType<T>::type;
    constexpr auto P = OtherOptions<Options>::value;
    mtl::matrix<U, Rows, Columns, P> const n = m;
    INFO(n);
    INFO(m);
    for (int i = 0; auto value: n) {
        CHECK(value == (U)values[i]);
        ++i;
    }
}

MATRIX_TEST_CASE("matrix::unit", "[matrix]") {
    using M = MATRIX_TYPE;
    auto m = M::unit(0, 1, 2);
    CHECK(m(0, 1) == 2);
    m(0, 1) = 0;
    CHECK(m == 0);
}

TEST_CASE("matrix::diag", "[matrix]") {
    auto m = int3x4::diag(-1, 1, 2);
    CHECK(m(0, 0) == -1);
    CHECK(m(1, 1) == 1);
    CHECK(m(2, 2) == 2);
    m(0, 0) = m(1, 1) = m(2, 2) = 0;
    CHECK(m == 0);
}

/// Disables because we don't have function objects
#if 0

TEST_CASE("map(matrix)", "[matrix]") {
	auto const m = mtl::map(float3x4::diag(0.1f, 0.2f, 0.3f), int3x4::diag(1, 2, 3), packed_int3x4{}, utl::plus);
	static_assert(std::is_same_v<decltype(m)::value_type, float>);
	static_assert(!decltype(m)::options().packed());
	CHECK(m == float3x4::diag(1.1f, 2.2f, 3.3f));
}

TEST_CASE("fold(matrix)", "[matrix]") {
	SECTION("1 / associativity") {
		double const x = 1e30, y = -1e30, z = 1;
		auto const A = double3x3::diag(x, y, z);
		auto const lf = mtl::left_fold(A, utl::plus);
		auto const rf = mtl::right_fold(A, utl::plus);
		static_assert(std::is_same_v<decltype(lf), double const>);
		static_assert(std::is_same_v<decltype(rf), double const>);
		CHECK(lf == (x + y) + z);
		CHECK(rf == x + (y + z));
	}
	SECTION("2") {
		int2x3 const A = { 1, 1, 1, 1, 1, 1 };
		auto const lf = mtl::left_fold(A, utl::multiplies);
		auto const rf = mtl::right_fold(A, utl::multiplies);
		static_assert(std::is_same_v<decltype(lf), int const>);
		static_assert(std::is_same_v<decltype(rf), int const>);
		CHECK(lf == 1);
		CHECK(rf == 1);
	}
}

#endif

TEST_CASE("matrix::swizzle", "[matrix]") {
    int3x4 const A = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 };
    SECTION("row_swizzle") {
        CHECK(A.row_swizzle(2, 1, 0, 2) ==
              int4x4{ 9, 0, 1, 2, 5, 6, 7, 8, 1, 2, 3, 4, 9, 0, 1, 2 });
        CHECK(A.row_swizzle(2, 1) == int2x4{ 9, 0, 1, 2, 5, 6, 7, 8 });
    }
    SECTION("column_swizzle") {
        CHECK(A.column_swizzle(2, 3, 1) == int3x3{ 3, 4, 2, 7, 8, 6, 1, 2, 0 });
        CHECK(A.column_swizzle(2, 3) == int3x2{ 3, 4, 7, 8, 1, 2 });
    }
}
