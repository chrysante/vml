#ifndef __MTL_MATRIX_HPP_INCLUDED__
#define __MTL_MATRIX_HPP_INCLUDED__

#include <array>
#include <iomanip>
#include <iosfwd>
#include <sstream>

#include "__arithmetic.hpp"
#include "__base.hpp"
#include "__common.hpp"
#include "__vector.hpp"

namespace _VMTL {

/// Tag type to select matrix row constructor
struct rows_tag {};
/// Tag to select matrix row constructor
inline constexpr rows_tag rows{};
/// Tag type to select matrix column constructor
struct columns_tag {};
/// Tag to select matrix column constructor
inline constexpr columns_tag columns{};

/// MARK: Map
template <typename... T, std::size_t R, std::size_t C, vector_options... O,
          std::invocable<T...> F>
__mtl_mathfunction __mtl_always_inline constexpr auto __map_impl(
    F&& f, matrix<T, R, C, O> const&... m) {
    using U = std::invoke_result_t<F, T...>;
    if constexpr (std::same_as<U, void>) {
        for (std::size_t i = 0; i < R * C; ++i) {
            std::invoke(__mtl_forward(f), m.__mtl_vec_at(i)...);
        }
    }
    else {
        constexpr auto P = combine(O...);
        return matrix<U, R, C, P>([&](std::size_t i) {
            return std::invoke(__mtl_forward(f), m.__mtl_vec_at(i)...);
        });
    }
}

/// Map the unary function object f onto the matrix m0
template <typename T0, std::size_t R, std::size_t C, vector_options O0,
          std::invocable<T0> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, F&& f) {
    return __map_impl(__mtl_forward(f), m0);
}

/// Map the binary function object f onto the matrices m0 and m1
template <typename T0, typename T1, std::size_t R, std::size_t C,
          vector_options O0, vector_options O1, std::invocable<T0, T1> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, matrix<T1, R, C, O1> const& m1, F&& f) {
    return __map_impl(__mtl_forward(f), m0, m1);
}

/// Map the ternary function object f onto the matrices m0, m1, m2
template <typename T0, typename T1, typename T2, std::size_t R, std::size_t C,
          vector_options O0, vector_options O1, vector_options O2,
          std::invocable<T0, T1, T2> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, matrix<T1, R, C, O1> const& m1,
        matrix<T2, R, C, O2> const& m2, F&& f) {
    return __map_impl(__mtl_forward(f), m0, m1, m2);
}

template <typename T0, typename T1, typename T2, typename T3, std::size_t R,
          std::size_t C, vector_options O0, vector_options O1,
          vector_options O2, vector_options O3,
          std::invocable<T0, T1, T2, T3> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, matrix<T1, R, C, O1> const& m1,
        matrix<T2, R, C, O2> const& m2, matrix<T3, R, C, O3> const& m3, F&& f) {
    return __map_impl(__mtl_forward(f), m0, m1, m2, m3);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          std::size_t R, std::size_t C, vector_options O0, vector_options O1,
          vector_options O2, vector_options O3, vector_options O4,
          std::invocable<T0, T1, T2, T3, T4> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, matrix<T1, R, C, O1> const& m1,
        matrix<T2, R, C, O2> const& m2, matrix<T3, R, C, O3> const& m3,
        matrix<T4, R, C, O4> const& m4, F&& f) {
    return __map_impl(__mtl_forward(f), m0, m1, m2, m3, m4);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, std::size_t R, std::size_t C, vector_options O0,
          vector_options O1, vector_options O2, vector_options O3,
          vector_options O4, vector_options O5,
          std::invocable<T0, T1, T2, T3, T4, T5> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(matrix<T0, R, C, O0> const& m0, matrix<T1, R, C, O1> const& m1,
        matrix<T2, R, C, O2> const& m2, matrix<T3, R, C, O3> const& m3,
        matrix<T4, R, C, O4> const& m4, matrix<T5, R, C, O5> const& m5, F&& f) {
    return __map_impl(__mtl_forward(f), m0, m1, m2, m3, m4, m5);
}

/// MARK: Fold
template <typename T, std::size_t R, std::size_t C, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T
    left_fold(matrix<T, R, C, O> const& m, F&& f) {
    return __mtl_with_index_sequence((I, R * C), {
        return __left_fold_impl(f, m.__mtl_at(I)...);
    });
}

template <typename T, std::size_t R, std::size_t C, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T
    right_fold(matrix<T, R, C, O> const& m, F&& f) {
    return __mtl_with_index_sequence((I, R * C), {
        return __left_fold_impl(f, m.__mtl_at(R * C - 1 - I)...);
    });
}

template <typename T, std::size_t R, std::size_t C, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T fold(
    matrix<T, R, C, O> const& m, F&& f) {
    return _VMTL::left_fold(m, __mtl_forward(f));
}

/// MARK: - struct __matrix_data
template <typename T, std::size_t Rows, std::size_t Columns, bool Packed>
struct __matrix_data;

constexpr std::size_t __calculate_aligned_size(std::size_t S) {
    return S == 3 ? 4 : S;
}

constexpr std::size_t __calculate_columns_in_data(std::size_t C,
                                                  vector_options O) {
    return O.packed() ? C : __calculate_aligned_size(C);
}

/// MARK: Size = N x N
template <typename T, std::size_t Rows, std::size_t Columns, bool Packed>
struct __matrix_data {
    static constexpr std::size_t __columns_in_data = Columns;
    static constexpr std::size_t __size_in_data = Rows * Columns;
    union {
        T __data[__size_in_data];
        __simd_type_t<T, Columns, Packed> __vec[Rows];
    };

    __mtl_always_inline constexpr T& __mtl_at(std::size_t i) & {
        __mtl_assert_audit(i < Rows * Columns);
        return __data[i];
    }
    __mtl_always_inline constexpr T const& __mtl_at(std::size_t i) const& {
        __mtl_assert_audit(i < Rows * Columns);
        return __data[i];
    }
    __mtl_always_inline constexpr T&& __mtl_at(std::size_t i) && {
        __mtl_assert_audit(i < Rows * Columns);
        return (T&&)(__data[i]);
    }
    __mtl_always_inline constexpr T const&& __mtl_at(std::size_t i) const&& {
        __mtl_assert_audit(i < Rows * Columns);
        return (T const&&)(__data[i]);
    }

    __mtl_always_inline constexpr T __mtl_vec_at(std::size_t i) const {
        __mtl_assert_audit(i < Rows * Columns);
        return __vec[i / Columns][i % Columns];
    }

    __mtl_always_inline constexpr T& __mtl_at(std::size_t i, std::size_t j) & {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < Columns);
        return __data[i * Columns + j];
    }
    __mtl_always_inline constexpr T const& __mtl_at(std::size_t i,
                                                    std::size_t j) const& {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < Columns);
        return __data[i * Columns + j];
    }
    __mtl_always_inline constexpr T&& __mtl_at(std::size_t i,
                                               std::size_t j) && {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < Columns);
        return (T&&)(__data[i * Columns + j]);
    }
    __mtl_always_inline constexpr T const&& __mtl_at(std::size_t i,
                                                     std::size_t j) const&& {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < Columns);
        return (T const&&)(__data[i * Columns + j]);
    }

    __mtl_always_inline constexpr T __mtl_vec_at(std::size_t i,
                                                 std::size_t j) const {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < Columns);
        return __vec[i][j];
    }
};

/// MARK: Size = N x 3, Packed = false
/// Only this case is special, since we only properly support up to 4x4
/// matrices.
template <typename T, std::size_t Rows>
struct __matrix_data<T, Rows, 3, false> {
    static constexpr std::size_t __columns_in_data = 4;
    static constexpr std::size_t __size_in_data = __columns_in_data * Rows;
    union {
        T __data[__size_in_data];
        __simd_type_t<T, 4, false> __vec[Rows];
    };
    __mtl_pure __mtl_always_inline static constexpr std::size_t __index(
        std::size_t i, std::size_t j) {
        return i * 4 + j;
    }

    __mtl_always_inline constexpr T& __mtl_at(std::size_t index) & {
        __mtl_assert_audit(index < Rows * 3);
        return __data[__index(index / 3, index % 3)];
    }
    __mtl_always_inline constexpr T const& __mtl_at(std::size_t index) const& {
        __mtl_assert_audit(index < Rows * 3);
        return __data[__index(index / 3, index % 3)];
    }
    __mtl_always_inline constexpr T&& __mtl_at(std::size_t index) && {
        __mtl_assert_audit(index < Rows * 3);
        return (T&&)(__data[__index(index / 3, index % 3)]);
    }
    __mtl_always_inline constexpr T const&& __mtl_at(
        std::size_t index) const&& {
        __mtl_assert_audit(index < Rows * 3);
        return (T const&&)(__data[__index(index / 3, index % 3)]);
    }

    __mtl_always_inline constexpr T __mtl_vec_at(std::size_t index) const {
        __mtl_assert_audit(index < Rows * 3);
        return __vec[index / 3][index % 3];
    }

    __mtl_always_inline constexpr T& __mtl_at(std::size_t i, std::size_t j) & {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < 3);
        return __data[__index(i, j)];
    }
    __mtl_always_inline constexpr T const& __mtl_at(std::size_t i,
                                                    std::size_t j) const& {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < 3);
        return __data[__index(i, j)];
    }
    __mtl_always_inline constexpr T&& __mtl_at(std::size_t i,
                                               std::size_t j) && {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < 3);
        return (T&&)(__data[__index(i, j)]);
    }
    __mtl_always_inline constexpr T const&& __mtl_at(std::size_t i,
                                                     std::size_t j) const&& {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < 3);
        return (T const&&)(__data[__index(i, j)]);
    }

    __mtl_always_inline constexpr T __mtl_vec_at(std::size_t i,
                                                 std::size_t j) const {
        __mtl_assert_audit(i < Rows);
        __mtl_assert_audit(j < 3);
        return __vec[i][j];
    }

    __matrix_data() = default;
    __mtl_always_inline constexpr __matrix_data(T const& _00, T const& _01,
                                                T const& _02, T const& _10,
                                                T const& _11, T const& _12)
        requires(Rows == 2)
        : __data{ _00, _01, _02, 0, _10, _11, _12, 0 } {}
    __mtl_always_inline constexpr __matrix_data(T const& _00, T const& _01,
                                                T const& _02, T const& _10,
                                                T const& _11, T const& _12,
                                                T const& _20, T const& _21,
                                                T const& _22)
        requires(Rows == 3)
        : __data{ _00, _01, _02, 0, _10, _11, _12, 0, _20, _21, _22, 0 } {}
    __mtl_always_inline constexpr __matrix_data(T const& _00, T const& _01,
                                                T const& _02, T const& _10,
                                                T const& _11, T const& _12,
                                                T const& _20, T const& _21,
                                                T const& _22, T const& _30,
                                                T const& _31, T const& _32)
        requires(Rows == 4)
        :
        __data{ _00, _01, _02, 0, _10, _11, _12, 0,
                _20, _21, _22, 0, _30, _31, _32, 0 } {}
    __mtl_always_inline constexpr __matrix_data(
        T const& _00, T const& _01, T const& _02, T const& _10, T const& _11,
        T const& _12, T const& _20, T const& _21, T const& _22, T const& _30,
        T const& _31, T const& _32, T const& _40, T const& _41, T const& _42)
        requires(Rows == 5)
        :
        __data{ _00, _01, _02, 0,   _10, _11, _12, 0,   _20, _21,
                _22, 0,   _30, _31, _32, 0,   _40, _41, _42, 0 } {}
    __mtl_always_inline constexpr __matrix_data(
        T const& _00, T const& _01, T const& _02, T const& _10, T const& _11,
        T const& _12, T const& _20, T const& _21, T const& _22, T const& _30,
        T const& _31, T const& _32, T const& _40, T const& _41, T const& _42,
        T const& _50, T const& _51, T const& _52)
        requires(Rows == 6)
        :
        __data{ _00, _01, _02, 0, _10, _11, _12, 0, _20, _21, _22, 0,
                _30, _31, _32, 0, _40, _41, _42, 0, _50, _51, _52, 0 } {}
    __mtl_always_inline constexpr __matrix_data(
        T const& _00, T const& _01, T const& _02, T const& _10, T const& _11,
        T const& _12, T const& _20, T const& _21, T const& _22, T const& _30,
        T const& _31, T const& _32, T const& _40, T const& _41, T const& _42,
        T const& _50, T const& _51, T const& _52, T const& _60, T const& _61,
        T const& _62)
        requires(Rows == 7)
        :
        __data{ _00, _01, _02, 0,   _10, _11, _12, 0,   _20, _21,
                _22, 0,   _30, _31, _32, 0,   _40, _41, _42, 0,
                _50, _51, _52, 0,   _60, _61, _62, 0 } {}
    __mtl_always_inline constexpr __matrix_data(
        T const& _00, T const& _01, T const& _02, T const& _10, T const& _11,
        T const& _12, T const& _20, T const& _21, T const& _22, T const& _30,
        T const& _31, T const& _32, T const& _40, T const& _41, T const& _42,
        T const& _50, T const& _51, T const& _52, T const& _60, T const& _61,
        T const& _62, T const& _70, T const& _71, T const& _72)
        requires(Rows == 8)
        :
        __data{ _00, _01, _02, 0,   _10, _11, _12, 0,   _20, _21, _22,
                0,   _30, _31, _32, 0,   _40, _41, _42, 0,   _50, _51,
                _52, 0,   _60, _61, _62, 0,   _70, _71, _72, 0 } {}
};

/// MARK: - struct __matrix_base
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O,
          typename AllT = __mtl_make_type_sequence<T, Rows * Columns>,
          typename AllI = __mtl_make_index_sequence<Rows * Columns>,
          typename RowT = __mtl_make_type_sequence<T, Rows>,
          typename RowI = __mtl_make_index_sequence<Rows>,
          typename ColumnT = __mtl_make_type_sequence<T, Columns>,
          typename ColumnI = __mtl_make_index_sequence<Columns>,
          typename DiagT =
              __mtl_make_type_sequence<T, _VMTL::min(Rows, Columns)>,
          typename DiagI = __mtl_make_index_sequence<_VMTL::min(Rows, Columns)>>
struct __matrix_base;

template <typename T, std::size_t Rows, std::size_t Columns, vector_options O,
          typename... AllT, std::size_t... AllI, typename... RowT,
          std::size_t... RowI, typename... ColumnT, std::size_t... ColumnI,
          typename... DiagT, std::size_t... DiagI>
struct alignas(__calculate_alignment(alignof(T), Columns, O.packed()))
    __matrix_base<
        T, Rows, Columns, O, __mtl_type_sequence<AllT...>,
        __mtl_index_sequence<AllI...>, __mtl_type_sequence<RowT...>,
        __mtl_index_sequence<RowI...>, __mtl_type_sequence<ColumnT...>,
        __mtl_index_sequence<ColumnI...>, __mtl_type_sequence<DiagT...>,
        __mtl_index_sequence<DiagI...>>:
    public __matrix_data<T, Rows, Columns, O.packed()> {
public:
    using __mtl_base = __matrix_data<T, Rows, Columns, O.packed()>;
    using __mtl_base::__mtl_at;

    using row_type = vector<T, Columns, O>;
    using column_type = vector<T, Rows, O>;

public:
    /// MARK: Public Interface
    /// Value Constructors
    __mtl_interface_export __matrix_base() = default;
    __mtl_always_inline __mtl_interface_export constexpr __matrix_base(T x):
        __mtl_base{ (AllI / Columns == AllI % Columns ? x : T(0))... } {}
    __mtl_always_inline __mtl_interface_export constexpr __matrix_base(
        AllT... x):
        __mtl_base{ x... } {}
    __mtl_always_inline __mtl_interface_export constexpr __matrix_base(
        rows_tag, vector<RowT, Columns, O> const&... rows):
        __mtl_base{} {
        ([&](std::size_t j) { ((__mtl_at(RowI, j) = rows[j]), ...); }(ColumnI),
         ...);
    }
    __mtl_always_inline __mtl_interface_export constexpr __matrix_base(
        columns_tag, vector<ColumnT, Rows, O> const&... columns):
        __mtl_base{} {
        ([&](std::size_t j) { ((__mtl_at(j, ColumnI) = columns[j]), ...); }(
             RowI),
         ...);
    }

    /// Functional Constructors
    __mtl_always_inline __mtl_interface_export explicit constexpr __matrix_base(
        _VMTL::invocable_r<T> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f)>):
        __mtl_base{ ((void)AllI, (T)std::invoke(f))... } {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __matrix_base(
        _VMTL::invocable_r<T, std::size_t> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f), std::size_t>):
        __mtl_base{ (T)std::invoke(f, AllI)... } {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __matrix_base(
        _VMTL::invocable_r<T, std::size_t, std::size_t> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f), std::size_t,
                                                    std::size_t>):
        __mtl_base{ (T)std::invoke(f, AllI / Columns, AllI % Columns)... } {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __matrix_base(
        rows_tag,
        _VMTL::invocable_r<row_type, std::size_t> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f), std::size_t>):
        __matrix_base(rows, std::invoke(f, RowI)...) {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __matrix_base(
        columns_tag,
        _VMTL::invocable_r<column_type, std::size_t> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f), std::size_t>):
        __matrix_base(columns, std::invoke(f, ColumnI)...) {}

    /// Conversion Constructor
    template <std::convertible_to<T> U, vector_options P>
    __mtl_always_inline __mtl_interface_export constexpr __matrix_base(
        matrix<U, Rows, Columns, P> const& rhs):
        __mtl_base{ static_cast<T>(rhs.__mtl_at(AllI))... } {}

    /// set_row, set_column
    __mtl_always_inline __mtl_interface_export constexpr matrix<T, Rows,
                                                                Columns, O>&
        set_row(std::size_t i, RowT const&... row) {
        __mtl_bounds_check(i, 0, Rows);
        ((this->__mtl_at(i, RowI) = row), ...);
        return (matrix<T, Rows, Columns, O>&)*this;
    }
    __mtl_always_inline __mtl_interface_export constexpr matrix<T, Rows,
                                                                Columns, O>&
        set_column(std::size_t j, ColumnT const&... column) {
        __mtl_bounds_check(j, 0, Columns);
        ((this->__mtl_at(ColumnI, j) = column), ...);
        return (matrix<T, Rows, Columns, O>&)*this;
    }

    /// Static Functions
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr matrix<T, Rows, Columns, O>
        diag(T value) {
        return { value };
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr matrix<T, Rows, Columns, O>
        diag(vector<T, _VMTL::min(Rows, Columns), O> const& values) {
        return diag(values.__mtl_at(DiagI)...);
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr matrix<T, Rows, Columns, O>
        diag(DiagT... value) {
        matrix<T, Rows, Columns, O> result{};
        std::size_t i = 0;
        (((void)(result.__mtl_at(i, i) = value), ++i), ...);
        return result;
    }
};

/// MARK: - struct matrix
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
struct matrix: public __matrix_base<T, Rows, Columns, O> {
    using __mtl_base = __matrix_base<T, Rows, Columns, O>;
    using __mtl_base::__columns_in_data;
    using __mtl_base::__data;
    using __mtl_base::__size_in_data;

    template <typename U>
    struct __iterator_base_special {
        U* __ptr_;
        std::size_t __row_index_ = 0;
    };
    template <typename U>
    struct __iterator_base {
        U* __ptr_;
    };

    template <typename U, bool IsSpecial = !O.packed() && Columns == 3>
    struct __iterator:
        std::conditional_t<IsSpecial, __iterator_base_special<U>,
                           __iterator_base<U>> {
        using __mtl_base =
            std::conditional_t<IsSpecial, __iterator_base_special<U>,
                               __iterator_base<U>>;
        using __mtl_base::__ptr_;
        __mtl_always_inline __mtl_interface_export constexpr __iterator(
            U* __ptr):
            __mtl_base{ __ptr } {}
        __mtl_always_inline __mtl_interface_export constexpr __iterator&
            operator++() & {
            if constexpr (IsSpecial) {
                if (++this->__row_index_ == 3) {
                    this->__row_index_ = 0;
                    ++__ptr_;
                }
            }
            ++__ptr_;
            return *this;
        }

        __mtl_always_inline __mtl_interface_export constexpr U& operator*()
            const {
            return *__ptr_;
        }

        __mtl_always_inline __mtl_interface_export constexpr U* operator->()
            const {
            return __ptr_;
        }

        template <typename U_>
            requires(std::is_same_v<std::remove_cv_t<U>, std::remove_cv_t<U_>>)
        __mtl_always_inline __mtl_interface_export constexpr bool operator==(
            __iterator<U_> const& __that) const {
            return this->__ptr_ == __that.__ptr_;
        }
    };

    using iterator = __iterator<T>;
    using const_iterator = __iterator<T const>;

    /// MARK: Public Interface
    using value_type = T;
    using typename __mtl_base::column_type;
    using typename __mtl_base::row_type;
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        size() {
        return Rows * Columns;
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        data_size() {
        return __size_in_data;
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        rows() {
        return Rows;
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        columns() {
        return Columns;
    }
    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr vector_options
        options() {
        return O;
    }

    __mtl_mathfunction __mtl_always_inline
        __mtl_interface_export static constexpr matrix<T, Rows, Columns, O>
        unit(std::size_t i, std::size_t j, T const& value = 1) {
        __mtl_bounds_check(i, 0, Rows);
        __mtl_bounds_check(j, 0, Columns);
        matrix<T, Rows, Columns, O> result{};
        result.__mtl_at(i, j) = value;
        return result;
    }

    /// Inheriting interface from __matrix_base:
    using __mtl_base::__mtl_base;
    using __mtl_base::diag;

    matrix& operator=(matrix const&) & = default;

    /// operator[](std::size_t):
    __mtl_always_inline __mtl_interface_export constexpr T& operator[](
        std::size_t index) & {
        return const_cast<T&>(const_cast<matrix const&>(*this)[index]);
    }
    __mtl_always_inline __mtl_interface_export constexpr T const& operator[](
        std::size_t index) const& {
        __mtl_bounds_check(index, 0, Rows * Columns);
        return this->__mtl_at(index);
    }
    __mtl_always_inline __mtl_interface_export constexpr T&& operator[](
        std::size_t index) && {
        return std::move(this->operator[](index));
    }
    __mtl_always_inline __mtl_interface_export constexpr T const&& operator[](
        std::size_t index) const&& {
        return std::move(this->operator[](index));
    }
    /// operator[](usize2):
    __mtl_always_inline __mtl_interface_export constexpr T& operator[](
        usize2 index) & {
        return const_cast<T&>(const_cast<matrix const&>(*this)[index]);
    }
    __mtl_always_inline __mtl_interface_export constexpr T const& operator[](
        usize2 index) const& {
        __mtl_bounds_check(index.__mtl_at(0), 0, Rows);
        __mtl_bounds_check(index.__mtl_at(1), 0, Columns);
        return this->__mtl_at(index.__mtl_at(0), index.__mtl_at(1));
    }
    __mtl_always_inline __mtl_interface_export constexpr T&& operator[](
        size2 index) && {
        return std::move(this->operator[](index));
    }
    __mtl_always_inline __mtl_interface_export constexpr T const&& operator[](
        size2 index) const&& {
        return std::move(this->operator[](index));
    }

    /// operator(): [As a substitute for multi argument subscript]
    __mtl_always_inline __mtl_interface_export constexpr T& operator()(
        std::size_t i, std::size_t j) & {
        return const_cast<T&>(const_cast<matrix const&>(*this)(i, j));
    }
    __mtl_always_inline __mtl_interface_export constexpr T const& operator()(
        std::size_t i, std::size_t j) const& {
        __mtl_bounds_check(i, 0, Rows);
        __mtl_bounds_check(j, 0, Columns);
        return this->__mtl_at(i, j);
    }
    __mtl_always_inline __mtl_interface_export constexpr T&& operator()(
        std::size_t i, std::size_t j) && {
        return std::move(this->operator()(i, j));
    }
    __mtl_always_inline __mtl_interface_export constexpr T const&& operator()(
        std::size_t i, std::size_t j) const&& {
        return std::move(this->operator()(i, j));
    }

    /// row, column:
    __mtl_always_inline __mtl_interface_export constexpr row_type row(
        std::size_t i) const {
        __mtl_bounds_check(i, 0, Rows);
        return row_type(
            [this, i](std::size_t j) { return this->__mtl_at(i, j); });
    }
    __mtl_always_inline __mtl_interface_export constexpr column_type column(
        std::size_t j) const {
        __mtl_bounds_check(j, 0, Columns);
        return column_type(
            [this, j](std::size_t i) { return this->__mtl_at(i, j); });
    }
    __mtl_always_inline __mtl_interface_export constexpr matrix& set_row(
        std::size_t i, row_type const& row) {
        __mtl_bounds_check(i, 0, Rows);
        __mtl_with_index_sequence((J, Columns), {
            ((this->__mtl_at(i, J) = row.__mtl_at(J)), ...);
        });
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr matrix& set_column(
        std::size_t j, column_type const& column) {
        __mtl_bounds_check(j, 0, Columns);
        __mtl_with_index_sequence((I, Rows), {
            ((this->__mtl_at(I, j) = column.__mtl_at(I)), ...);
        });
        return *this;
    }
    using __mtl_base::set_column;
    using __mtl_base::set_row;

    /// begin, end:
    __mtl_always_inline __mtl_interface_export constexpr iterator begin() {
        return __data;
    }
    __mtl_always_inline __mtl_interface_export constexpr const_iterator begin()
        const {
        return __data;
    }
    __mtl_always_inline __mtl_interface_export constexpr const_iterator cbegin()
        const {
        return __data;
    }
    __mtl_always_inline __mtl_interface_export constexpr iterator end() {
        return __data + __size_in_data;
    }
    __mtl_always_inline __mtl_interface_export constexpr const_iterator end()
        const {
        return __data + __size_in_data;
    }
    __mtl_always_inline __mtl_interface_export constexpr const_iterator cend()
        const {
        return __data + __size_in_data;
    }

    /// data
    __mtl_always_inline __mtl_interface_export constexpr T* data() {
        return this->__data;
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* data() const {
        return this->__data;
    }

    /// swizzle:
    template <std::convertible_to<std::size_t>... I>
    __mtl_always_inline __mtl_interface_export constexpr matrix<T, sizeof...(I),
                                                                Columns, O>
        row_swizzle(I... i) const
        requires(sizeof...(I) > 0)
    {
        return { _VMTL::rows, row(i)... };
    }
    template <std::convertible_to<std::size_t>... I>
    __mtl_always_inline __mtl_interface_export constexpr matrix<T, Rows,
                                                                sizeof...(I), O>
        column_swizzle(I... i) const
        requires(sizeof...(I) > 0)
    {
        return { _VMTL::columns, column(i)... };
    }

    /// Map
    __mtl_always_inline __mtl_interface_export constexpr auto map(
        std::invocable<T> auto&& f) const {
        return _VMTL::map(*this, __mtl_forward(f));
    }

    /// Fold
    __mtl_always_inline __mtl_interface_export constexpr T fold(
        _VMTL::invocable_r<T, T, T> auto&& f) const {
        return _VMTL::fold(*this, __mtl_forward(f));
    }

    /// MARK: Arithmetic Assignment
    /// this + Matrix
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator+=(
        matrix const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    /// this - Matrix
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator-=(
        matrix const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    /// this * Matrix
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator*=(
        matrix<T, Columns, Columns, O> const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    /// this * Scalar
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator*=(
        T const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    /// this / Scalar
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator/=(
        T const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
    /// this % Scalar
    __mtl_always_inline __mtl_interface_export constexpr matrix& operator%=(
        T const& rhs) &
        requires(std::is_integral_v<T>)
    {
        *this = *this % rhs;
        return *this;
    }

    /// MARK: Matrix<bool> Members
    __mtl_always_inline __mtl_interface_export constexpr bool all() const
        requires std::convertible_to<T, bool>
    {
        return fold(__mtl_logical_and);
    }
    __mtl_always_inline __mtl_interface_export constexpr bool any() const
        requires std::convertible_to<T, bool>
    {
        return fold(__mtl_logical_or);
    }
    __mtl_always_inline __mtl_interface_export constexpr auto operator!() const
        requires requires(T&& t) { !t; }
    {
        return map(__mtl_logical_not);
    }

    /// MARK: Min, Max
    __mtl_always_inline __mtl_interface_export constexpr T min() const
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Rows * Columns),
                                         { return min(this->__mtl_at(I)...); });
    }

    __mtl_always_inline __mtl_interface_export constexpr T max() const
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Rows * Columns),
                                         { return max(this->__mtl_at(I)...); });
    }
};

/// MARK: - Operators
template <typename T, typename U = T, std::size_t Rows, std::size_t Columns,
          vector_options O, vector_options P = O>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(matrix<T, Rows, Columns, O> const& m,
               matrix<U, Rows, Columns, P> const& n) {
    return _VMTL::fold(_VMTL::map(m, n, _VMTL::__mtl_equals),
                       _VMTL::__mtl_logical_and);
}

template <typename T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(matrix<T, Rows, Columns, O> const& m, U const& x) {
    return m == _VMTL::matrix<U, Rows, Columns, O>(x);
}

template <scalar T, typename U, std::size_t Rows, std::size_t Columns,
          vector_options O>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(T const& x, matrix<U, Rows, Columns, O> const& m) {
    return m == x;
}

template <typename CharT, typename T, std::size_t Rows, std::size_t Columns,
          vector_options O>
    requires std::is_arithmetic_v<T> && requires(T&& t, std::ostream& str) {
        {
            str << t
        } -> std::convertible_to<std::ostream&>;
    }
__mtl_interface_export std::basic_ostream<CharT>& operator<<(
    std::basic_ostream<CharT>& str, matrix<T, Rows, Columns, O> const& m) {
    std::basic_stringstream<CharT> s;
    std::size_t len[Columns]{};
    // hacky way to iterate over a matrix
    (void)matrix<char, Rows, Columns, O>([&](std::size_t i, std::size_t j) {
        s << m(i, j);
        len[j] = max(s.str().size(), len[j]);
        s.str({});
        return 0;
    });
    constexpr CharT const* brackets[6] = {
#if MTL_UNICODE_MATH_PARANTHESES
        "⎛",
        "⎜",
        "⎝",
        "⎞",
        "⎥",
        "⎠"
#else  // MTL_UNICODE_MATH_PARANTHESES
        "|",
        "|",
        "|",
        "|",
        "|",
        "|"
#endif // MTL_UNICODE_MATH_PARANTHESES
    };
    vector<CharT const*, Rows> left_bracket = brackets[1];
    left_bracket[0] = brackets[0];
    left_bracket[Rows - 1] = brackets[2];
    vector<CharT const*, Rows> right_bracket = brackets[4];
    right_bracket[0] = brackets[3];
    right_bracket[Rows - 1] = brackets[5];

    for (std::size_t i = 0; i < Rows; ++i) {
        str << left_bracket[i];
        bool first = true;
        for (std::size_t j = 0; j < Columns; ++j) {
            str << (first ? ((void)(first = false), "") : "  ")
                << std::setw((int)len[j]) << m(i, j);
        }
        str << right_bracket[i] << (i == Rows - 1 ? "" : "\n");
    }
    return str;
}

/// MARK: - Generic Matrix Functions
/// Transpose
template <typename T, std::size_t Rows, std::size_t Columns,
          vector_options Options>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<T, Columns, Rows, Options>
    transpose(matrix<T, Rows, Columns, Options> const& m) {
    return matrix<T, Columns, Rows, Options>(
        [&](std::size_t i, std::size_t j) { return m.__mtl_at(j, i); });
}

/// Conjugate Transpose
template <typename T, std::size_t Rows, std::size_t Columns,
          vector_options Options>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<T, Columns, Rows, Options>
    conj_transpose(matrix<T, Rows, Columns, Options> const& m) {
    return matrix<T, Columns, Rows, Options>(
        [&](std::size_t i, std::size_t j) { return conj(m.__mtl_at(j, i)); });
}

/// Submatrix
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
__mtl_mathfunction __mtl_interface_export matrix<T, Rows - 1, Columns - 1, O>
    submatrix(matrix<T, Rows, Columns, O> const& m, std::size_t x,
              std::size_t y) {
    __mtl_bounds_check(x, 0, Rows);
    __mtl_bounds_check(y, 0, Columns);
    return matrix<T, Rows - 1, Columns - 1, O>(
        [&](std::size_t i, std::size_t j) {
        return m.__mtl_at(i + (i >= x), j + (j >= y));
    });
};

/// Type Cast
template <typename To, typename T, std::size_t Rows, std::size_t Columns,
          vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<To, Rows, Columns, O>
    type_cast(matrix<T, Rows, Columns, O> const& v) {
    return matrix<To, Rows, Columns, O>([&](std::size_t i, std::size_t j) {
        return static_cast<To>(v.__mtl_at(i, j));
    });
}

/// Dimension Cast
template <std::size_t ToRows, std::size_t ToColumns, typename T,
          std::size_t Rows, std::size_t Columns, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<T, ToRows, ToColumns, O>
    dimension_cast(matrix<T, Rows, Columns, O> const& v) {
    return matrix<T, ToRows, ToColumns, O>([&](std::size_t i, std::size_t j) {
        return i < Rows && j < Columns ? v.__mtl_at(i, j) : T(0);
    });
}

/// isnan
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr matrix<bool, Rows, Columns, O> isnan(
    matrix<T, Rows, Columns, O> const& m) {
    return matrix<bool, Rows, Columns, O>([&](std::size_t i) {
        using std::isnan;
        return isnan(m.__mtl_at(i));
    });
}

/// isinf
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr matrix<bool, Rows, Columns, O> isinf(
    matrix<T, Rows, Columns, O> const& m) {
    return matrix<bool, Rows, Columns, O>([&](std::size_t i) {
        using std::isinf;
        return isinf(m.__mtl_at(i));
    });
}

/// isfinite
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr matrix<bool, Rows, Columns, O> isfinite(
    matrix<T, Rows, Columns, O> const& m) {
    return matrix<bool, Rows, Columns, O>([&](std::size_t i) {
        using std::isfinite;
        return isfinite(m.__mtl_at(i));
    });
}

/// isnormal
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr matrix<bool, Rows, Columns, O> isnormal(
    matrix<T, Rows, Columns, O> const& m) {
    return matrix<bool, Rows, Columns, O>([&](std::size_t i) {
        using std::isnormal;
        return isnormal(m.__mtl_at(i));
    });
}

/// MARK: - Matrix Math Functions
/// Row Sum Norm
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr auto row_sum_norm(matrix<T, Rows, Columns, O> const& m) {
    return __mtl_with_index_sequence((I, Columns),
                                     { return max(sum_norm(m.row(I))...); });
}

/// Column Sum Norm
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr auto column_sum_norm(matrix<T, Rows, Columns, O> const& m) {
    return __mtl_with_index_sequence((I, Columns),
                                     { return max(sum_norm(m.column(I))...); });
}

/// Maximum Norm
template <typename T, std::size_t Rows, std::size_t Columns, vector_options O>
constexpr auto max_norm(matrix<T, Rows, Columns, O> const& m) {
    return __mtl_with_index_sequence((I, Rows * Columns), {
        using std::abs;
        return max(abs(m.__mtl_at(I))...);
    });
}

/// Determinant
template <scalar T, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T det(
    matrix2x2<T, O> const& A) {
    return A.__mtl_at(0) * A.__mtl_at(3) - A.__mtl_at(2) * A.__mtl_at(1);
}
template <scalar T, vector_options O>
__mtl_mathfunction __mtl_interface_export constexpr T det(
    matrix3x3<T, O> const& A) {
    return A.__mtl_at(0, 0) * (A.__mtl_at(1, 1) * A.__mtl_at(2, 2) -
                               A.__mtl_at(2, 1) * A.__mtl_at(1, 2)) -
           A.__mtl_at(0, 1) * (A.__mtl_at(1, 0) * A.__mtl_at(2, 2) -
                               A.__mtl_at(1, 2) * A.__mtl_at(2, 0)) +
           A.__mtl_at(0, 2) * (A.__mtl_at(1, 0) * A.__mtl_at(2, 1) -
                               A.__mtl_at(1, 1) * A.__mtl_at(2, 0));
}
template <scalar T, std::size_t N, vector_options O>
__mtl_mathfunction __mtl_interface_export constexpr T det(
    matrix<T, N, N, O> const& m) {
    auto constexpr sign = [](std::size_t i, std::size_t j) {
        return (int)((i + j + 1) % 2) * 2 - 1;
    };
    return __mtl_with_index_sequence((I, N), {
        return (
            (m.__mtl_at(I, 0) * sign(I, 0) * _VMTL::det(submatrix(m, I, 0))) +
            ...);
    });
}

/// Trace
template <scalar T, std::size_t N, vector_options O>
constexpr T trace(matrix<T, N, N, O> const& m) {
    return __mtl_with_index_sequence((I, N),
                                     { return (m.__mtl_at(I, I) + ...); });
}

/// Inverse
template <scalar T, vector_options O>
constexpr matrix2x2<T, O> __mtl_inverse(matrix2x2<T, O> const& m) {
    T const d = det(m);
    __mtl_expect(__mtl_is_unit(d));
    return matrix2x2<T, O>{ m.__mtl_at(3), -m.__mtl_at(1), -m.__mtl_at(2),
                            m.__mtl_at(0) } /
           d;
}
template <scalar T, vector_options O>
__mtl_mathfunction __mtl_interface_export constexpr matrix2x2<__mtl_floatify(T),
                                                              O>
    inverse(matrix2x2<T, O> const& m) {
    if constexpr (std::is_floating_point_v<T>) {
        return __mtl_inverse(m);
    }
    else {
        return __mtl_inverse(type_cast<__mtl_floatify(T)>(m));
    }
}
template <typename T, vector_options O>
    requires(std::is_integral_v<T>)
__mtl_mathfunction __mtl_interface_export constexpr matrix2x2<__mtl_floatify(T),
                                                              O>
    integral_inverse(matrix2x2<T, O> const& m) {
    return __mtl_inverse(m);
}
template <scalar T, vector_options O>
constexpr matrix3x3<T, O> __mtl_inverse(matrix3x3<T, O> const& m) {
    T const d = det(m);
    __mtl_expect(__mtl_is_unit(d));
    return matrix3x3<T, O>{ m.__mtl_at(1, 1) * m.__mtl_at(2, 2) -
                                m.__mtl_at(2, 1) * m.__mtl_at(1, 2),
                            m.__mtl_at(0, 2) * m.__mtl_at(2, 1) -
                                m.__mtl_at(0, 1) * m.__mtl_at(2, 2),
                            m.__mtl_at(0, 1) * m.__mtl_at(1, 2) -
                                m.__mtl_at(0, 2) * m.__mtl_at(1, 1),
                            m.__mtl_at(1, 2) * m.__mtl_at(2, 0) -
                                m.__mtl_at(1, 0) * m.__mtl_at(2, 2),
                            m.__mtl_at(0, 0) * m.__mtl_at(2, 2) -
                                m.__mtl_at(0, 2) * m.__mtl_at(2, 0),
                            m.__mtl_at(1, 0) * m.__mtl_at(0, 2) -
                                m.__mtl_at(0, 0) * m.__mtl_at(1, 2),
                            m.__mtl_at(1, 0) * m.__mtl_at(2, 1) -
                                m.__mtl_at(2, 0) * m.__mtl_at(1, 1),
                            m.__mtl_at(2, 0) * m.__mtl_at(0, 1) -
                                m.__mtl_at(0, 0) * m.__mtl_at(2, 1),
                            m.__mtl_at(0, 0) * m.__mtl_at(1, 1) -
                                m.__mtl_at(1, 0) * m.__mtl_at(0, 1) } /
           d;
}
template <scalar T, vector_options O>
__mtl_mathfunction __mtl_interface_export constexpr matrix3x3<__mtl_floatify(T),
                                                              O>
    inverse(matrix3x3<T, O> const& m) {
    if constexpr (std::is_floating_point_v<T>) {
        return __mtl_inverse(m);
    }
    else {
        return __mtl_inverse(type_cast<__mtl_floatify(T)>(m));
    }
}
template <typename T, vector_options O>
    requires(std::is_integral_v<T>)
__mtl_mathfunction __mtl_interface_export constexpr matrix3x3<__mtl_floatify(T),
                                                              O>
    integral_inverse(matrix3x3<T, O> const& m) {
    return __mtl_inverse(m);
}
template <scalar T, vector_options O>
constexpr matrix4x4<T, O> __mtl_inverse(matrix4x4<T, O> const& m) {
    matrix4x4<T, O> const int_result = {
        m.__mtl_at(5) * m.__mtl_at(10) * m.__mtl_at(15) -
            m.__mtl_at(5) * m.__mtl_at(11) * m.__mtl_at(14) -
            m.__mtl_at(9) * m.__mtl_at(6) * m.__mtl_at(15) +
            m.__mtl_at(9) * m.__mtl_at(7) * m.__mtl_at(14) +
            m.__mtl_at(13) * m.__mtl_at(6) * m.__mtl_at(11) -
            m.__mtl_at(13) * m.__mtl_at(7) * m.__mtl_at(10),

        -m.__mtl_at(1) * m.__mtl_at(10) * m.__mtl_at(15) +
            m.__mtl_at(1) * m.__mtl_at(11) * m.__mtl_at(14) +
            m.__mtl_at(9) * m.__mtl_at(2) * m.__mtl_at(15) -
            m.__mtl_at(9) * m.__mtl_at(3) * m.__mtl_at(14) -
            m.__mtl_at(13) * m.__mtl_at(2) * m.__mtl_at(11) +
            m.__mtl_at(13) * m.__mtl_at(3) * m.__mtl_at(10),

        m.__mtl_at(1) * m.__mtl_at(6) * m.__mtl_at(15) -
            m.__mtl_at(1) * m.__mtl_at(7) * m.__mtl_at(14) -
            m.__mtl_at(5) * m.__mtl_at(2) * m.__mtl_at(15) +
            m.__mtl_at(5) * m.__mtl_at(3) * m.__mtl_at(14) +
            m.__mtl_at(13) * m.__mtl_at(2) * m.__mtl_at(7) -
            m.__mtl_at(13) * m.__mtl_at(3) * m.__mtl_at(6),

        -m.__mtl_at(1) * m.__mtl_at(6) * m.__mtl_at(11) +
            m.__mtl_at(1) * m.__mtl_at(7) * m.__mtl_at(10) +
            m.__mtl_at(5) * m.__mtl_at(2) * m.__mtl_at(11) -
            m.__mtl_at(5) * m.__mtl_at(3) * m.__mtl_at(10) -
            m.__mtl_at(9) * m.__mtl_at(2) * m.__mtl_at(7) +
            m.__mtl_at(9) * m.__mtl_at(3) * m.__mtl_at(6),

        -m.__mtl_at(4) * m.__mtl_at(10) * m.__mtl_at(15) +
            m.__mtl_at(4) * m.__mtl_at(11) * m.__mtl_at(14) +
            m.__mtl_at(8) * m.__mtl_at(6) * m.__mtl_at(15) -
            m.__mtl_at(8) * m.__mtl_at(7) * m.__mtl_at(14) -
            m.__mtl_at(12) * m.__mtl_at(6) * m.__mtl_at(11) +
            m.__mtl_at(12) * m.__mtl_at(7) * m.__mtl_at(10),

        m.__mtl_at(0) * m.__mtl_at(10) * m.__mtl_at(15) -
            m.__mtl_at(0) * m.__mtl_at(11) * m.__mtl_at(14) -
            m.__mtl_at(8) * m.__mtl_at(2) * m.__mtl_at(15) +
            m.__mtl_at(8) * m.__mtl_at(3) * m.__mtl_at(14) +
            m.__mtl_at(12) * m.__mtl_at(2) * m.__mtl_at(11) -
            m.__mtl_at(12) * m.__mtl_at(3) * m.__mtl_at(10),

        -m.__mtl_at(0) * m.__mtl_at(6) * m.__mtl_at(15) +
            m.__mtl_at(0) * m.__mtl_at(7) * m.__mtl_at(14) +
            m.__mtl_at(4) * m.__mtl_at(2) * m.__mtl_at(15) -
            m.__mtl_at(4) * m.__mtl_at(3) * m.__mtl_at(14) -
            m.__mtl_at(12) * m.__mtl_at(2) * m.__mtl_at(7) +
            m.__mtl_at(12) * m.__mtl_at(3) * m.__mtl_at(6),

        m.__mtl_at(0) * m.__mtl_at(6) * m.__mtl_at(11) -
            m.__mtl_at(0) * m.__mtl_at(7) * m.__mtl_at(10) -
            m.__mtl_at(4) * m.__mtl_at(2) * m.__mtl_at(11) +
            m.__mtl_at(4) * m.__mtl_at(3) * m.__mtl_at(10) +
            m.__mtl_at(8) * m.__mtl_at(2) * m.__mtl_at(7) -
            m.__mtl_at(8) * m.__mtl_at(3) * m.__mtl_at(6),

        m.__mtl_at(4) * m.__mtl_at(9) * m.__mtl_at(15) -
            m.__mtl_at(4) * m.__mtl_at(11) * m.__mtl_at(13) -
            m.__mtl_at(8) * m.__mtl_at(5) * m.__mtl_at(15) +
            m.__mtl_at(8) * m.__mtl_at(7) * m.__mtl_at(13) +
            m.__mtl_at(12) * m.__mtl_at(5) * m.__mtl_at(11) -
            m.__mtl_at(12) * m.__mtl_at(7) * m.__mtl_at(9),

        -m.__mtl_at(0) * m.__mtl_at(9) * m.__mtl_at(15) +
            m.__mtl_at(0) * m.__mtl_at(11) * m.__mtl_at(13) +
            m.__mtl_at(8) * m.__mtl_at(1) * m.__mtl_at(15) -
            m.__mtl_at(8) * m.__mtl_at(3) * m.__mtl_at(13) -
            m.__mtl_at(12) * m.__mtl_at(1) * m.__mtl_at(11) +
            m.__mtl_at(12) * m.__mtl_at(3) * m.__mtl_at(9),

        m.__mtl_at(0) * m.__mtl_at(5) * m.__mtl_at(15) -
            m.__mtl_at(0) * m.__mtl_at(7) * m.__mtl_at(13) -
            m.__mtl_at(4) * m.__mtl_at(1) * m.__mtl_at(15) +
            m.__mtl_at(4) * m.__mtl_at(3) * m.__mtl_at(13) +
            m.__mtl_at(12) * m.__mtl_at(1) * m.__mtl_at(7) -
            m.__mtl_at(12) * m.__mtl_at(3) * m.__mtl_at(5),

        -m.__mtl_at(0) * m.__mtl_at(5) * m.__mtl_at(11) +
            m.__mtl_at(0) * m.__mtl_at(7) * m.__mtl_at(9) +
            m.__mtl_at(4) * m.__mtl_at(1) * m.__mtl_at(11) -
            m.__mtl_at(4) * m.__mtl_at(3) * m.__mtl_at(9) -
            m.__mtl_at(8) * m.__mtl_at(1) * m.__mtl_at(7) +
            m.__mtl_at(8) * m.__mtl_at(3) * m.__mtl_at(5),

        -m.__mtl_at(4) * m.__mtl_at(9) * m.__mtl_at(14) +
            m.__mtl_at(4) * m.__mtl_at(10) * m.__mtl_at(13) +
            m.__mtl_at(8) * m.__mtl_at(5) * m.__mtl_at(14) -
            m.__mtl_at(8) * m.__mtl_at(6) * m.__mtl_at(13) -
            m.__mtl_at(12) * m.__mtl_at(5) * m.__mtl_at(10) +
            m.__mtl_at(12) * m.__mtl_at(6) * m.__mtl_at(9),

        m.__mtl_at(0) * m.__mtl_at(9) * m.__mtl_at(14) -
            m.__mtl_at(0) * m.__mtl_at(10) * m.__mtl_at(13) -
            m.__mtl_at(8) * m.__mtl_at(1) * m.__mtl_at(14) +
            m.__mtl_at(8) * m.__mtl_at(2) * m.__mtl_at(13) +
            m.__mtl_at(12) * m.__mtl_at(1) * m.__mtl_at(10) -
            m.__mtl_at(12) * m.__mtl_at(2) * m.__mtl_at(9),

        -m.__mtl_at(0) * m.__mtl_at(5) * m.__mtl_at(14) +
            m.__mtl_at(0) * m.__mtl_at(6) * m.__mtl_at(13) +
            m.__mtl_at(4) * m.__mtl_at(1) * m.__mtl_at(14) -
            m.__mtl_at(4) * m.__mtl_at(2) * m.__mtl_at(13) -
            m.__mtl_at(12) * m.__mtl_at(1) * m.__mtl_at(6) +
            m.__mtl_at(12) * m.__mtl_at(2) * m.__mtl_at(5),

        m.__mtl_at(0) * m.__mtl_at(5) * m.__mtl_at(10) -
            m.__mtl_at(0) * m.__mtl_at(6) * m.__mtl_at(9) -
            m.__mtl_at(4) * m.__mtl_at(1) * m.__mtl_at(10) +
            m.__mtl_at(4) * m.__mtl_at(2) * m.__mtl_at(9) +
            m.__mtl_at(8) * m.__mtl_at(1) * m.__mtl_at(6) -
            m.__mtl_at(8) * m.__mtl_at(2) * m.__mtl_at(5)
    };

    T const d = m.__mtl_at(0) * int_result.__mtl_at(0) +
                m.__mtl_at(1) * int_result.__mtl_at(4) +
                m.__mtl_at(2) * int_result.__mtl_at(8) +
                m.__mtl_at(3) * int_result.__mtl_at(12);

    __mtl_expect(__mtl_is_unit(d));

    return int_result / d;
}
template <scalar T, vector_options O>
__mtl_mathfunction __mtl_interface_export constexpr matrix4x4<__mtl_floatify(T),
                                                              O>
    inverse(matrix4x4<T, O> const& m) {
    if constexpr (std::is_floating_point_v<T>) {
        return __mtl_inverse(m);
    }
    else {
        return __mtl_inverse(type_cast<__mtl_floatify(T)>(m));
    }
}
template <typename T, vector_options O>
    requires(std::is_integral_v<T>)
__mtl_mathfunction __mtl_interface_export constexpr matrix4x4<__mtl_floatify(T),
                                                              O>
    integral_inverse(matrix4x4<T, O> const& m) {
    return __mtl_inverse(m);
}

} // namespace _VMTL

#endif // __MTL_MATRIX_HPP_INCLUDED__
