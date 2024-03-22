#ifndef __MTL_ARITHMETIC_HPP_INCLUDED__
#define __MTL_ARITHMETIC_HPP_INCLUDED__

#include "__base.hpp"
#include "__common.hpp"
#include "__intrin.hpp"

namespace _VMTL {

template <typename VectorType, typename T, vector_options O>
VectorType __mtl_load(vector<T, VectorType::size(), O> const& x) {
    return type_cast<typename VectorType::value_type>(x);
}

template <typename VectorType>
using __mtl_get_simd_type =
    __simd_type<typename VectorType::value_type, VectorType::size(),
                VectorType::options().packed()>;

/// MARK: - Vector Arithmetic

/// MARK: Plus
/// Unary Plus(Vector)
template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator+(vector<T, Size, O> const& v) {
    return v;
}

/// Add Vector to Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator+(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_plus);
}

/// Add Scalar to Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator+(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto _a) { return _a + b; });
}

/// Add Vector to Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator+(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a + b; });
}

/// MARK: Minus
/// Negate Vector
template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator-(vector<T, Size, O> const& v) {
    return map(v, _VMTL::__mtl_minus);
}

/// Subract Vector from Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator-(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_minus);
}

/// Subract Scalar from Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator-(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a - b; });
}

/// Subract Vector from Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator-(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a - b; });
}

/// MARK: Multiply
/// Multiply Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator*(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    if (std::is_constant_evaluated()) {
        return map(a, b, _VMTL::__mtl_multiplies);
    }

    using result_type = vector<__mtl_promote(T, U), Size, combine(O, P)>;
    using simd_type = __mtl_get_simd_type<result_type>;

    result_type result = __mtl_load<result_type>(a);
    simd_type::mul(result.__vec, __mtl_load<result_type>(b).__vec);

    return result;
}

/// Multiply Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator*(vector<T, Size, O> const& a, U const& b) {
    using result_type = vector<__mtl_promote(T, U), Size, O>;
    return a * result_type(b);
}

/// Multiply Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator*(T const& a, vector<U, Size, O> const& b) {
    using result_type = vector<__mtl_promote(T, U), Size, O>;
    return result_type(a) * b;
}

/// MARK: Divide
/// Divide Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator/(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    if (std::is_constant_evaluated()) {
        return map(a, b, _VMTL::__mtl_divides);
    }

    using result_type = vector<__mtl_promote(T, U), Size, combine(O, P)>;
    using simd_type = __mtl_get_simd_type<result_type>;

    result_type result = __mtl_load<result_type>(a);
    simd_type::div(result.__vec, __mtl_load<result_type>(b).__vec);

    return result;
}

/// Divide Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator/(vector<T, Size, O> const& a, U const& b) {
    using result_type = vector<__mtl_promote(T, U), Size, O>;
    return a / result_type(b);
}

/// Divide Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator/(T const& a, vector<U, Size, O> const& b) {
    using result_type = vector<__mtl_promote(T, U), Size, O>;
    return result_type(a) / b;
}

/// MARK: Modulo
/// Modulo divide Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
    requires requires(T&& t, U&& u) { t % u; }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator%(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_modulo);
}

/// Modulo divide Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t % u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator%(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a % b; });
}

/// Modulo divide Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t % u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator%(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a % b; });
}

/// MARK: Bitwise And
/// Bitwise and Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
    requires requires(T&& t, U&& u) { t & u; }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator&(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_and);
}

/// Bitwise and Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t & u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator&(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a & b; });
}

/// Bitwise and Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t & u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator&(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a & b; });
}

/// MARK: Bitwise Or
/// Bitwise or Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
    requires requires(T&& t, U&& u) { t | u; }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator|(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_or);
}

/// Bitwise and Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t | u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator|(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a | b; });
}

/// Bitwise and Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t | u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator|(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a | b; });
}

/// MARK: Bitwise Xor
/// Bitwise xor Vector by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
    requires requires(T&& t, U&& u) { t ^ u; }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), Size, combine(O, P)>
    operator^(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, _VMTL::__mtl_xor);
}

/// Bitwise and Vector by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t ^ u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator^(vector<T, Size, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a ^ b; });
}

/// Bitwise and Scalar by Vector (element-wise)
template <scalar T, scalar U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) { t ^ u; }
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), Size, O>
    operator^(T const& a, vector<U, Size, O> const& b) {
    return map(b, [&a](auto b) { return a ^ b; });
}

/// MARK: Unary Bitwise Not
template <typename T, std::size_t Size, vector_options O>
    requires requires(T&& t) { ~t; }
__mtl_always_inline __mtl_interface_export constexpr auto operator~(
    vector<T, Size, O> const& a) {
    return map(a, __mtl_not);
}

/// MARK: Unary Logical Not
template <typename T, std::size_t Size, vector_options O>
    requires requires(T&& t) { !t; }
__mtl_always_inline __mtl_interface_export constexpr auto operator!(
    vector<T, Size, O> const& a) {
    return map(a, __mtl_logical_not);
}

/// Bitshift Vector by Vector (element-wise)
template <std::integral T, std::integral U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator<<(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, [](T t, U u) { return t << u; });
}

template <std::integral T, std::integral U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator>>(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, [](T t, U u) { return t >> u; });
}

/// Bitshift Vector by Scalar (element-wise)
template <std::integral T, std::integral U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator<<(vector<T, Size, O> const& a, U b) {
    return a.map([b](T t) { return t << b; });
}

template <std::integral T, std::integral U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator>>(vector<T, Size, O> const& a, U b) {
    return a.map([b](T t) { return t >> b; });
}

/// Bitshift Scalar by Vector (element-wise)
template <std::integral T, std::integral U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator<<(T a, vector<U, Size, O> const& b) {
    return b.map([a](U u) { return a << u; });
}

template <std::integral T, std::integral U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    operator>>(T a, vector<U, Size, O> const& b) {
    return b.map([a](U u) { return a >> u; });
}

/// MARK: - Matrix Arithmetic
///
///
///
/// MARK: Plus
/// Unary Plus(Matrix)
template <scalar T, std::size_t Rows, std::size_t Columns, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<T, Rows, Columns, O>
    operator+(matrix<T, Rows, Columns, O> const& v) {
    return v;
}

/// Add Matrix to Matrix
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O, vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, combine(O, P)>
    operator+(matrix<T, Rows, Columns, O> const& a,
              matrix<U, Rows, Columns, P> const& b) {
    return map(a, b, _VMTL::__mtl_plus);
}

/// MARK: Minus
/// Negate Matrix
template <scalar T, std::size_t Rows, std::size_t Columns, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr matrix<T, Rows, Columns, O>
    operator-(matrix<T, Rows, Columns, O> const& v) {
    return map(v, _VMTL::__mtl_minus);
}

/// Subtract Matrix from Matrix
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O, vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, combine(O, P)>
    operator-(matrix<T, Rows, Columns, O> const& a,
              matrix<U, Rows, Columns, P> const& b) {
    return map(a, b, _VMTL::__mtl_minus);
}

/// MARK: Multiply
/// Multiply Matrix by Scalar
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, O>
    operator*(matrix<T, Rows, Columns, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a * b; });
}

/// Multiply Scalar by Matrix
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, O>
    operator*(T const& a, matrix<U, Rows, Columns, O> const& b) {
    return map(b, [&a](auto b) { return a * b; });
}

/// Multiply Matrix by Matrix
template <scalar T, scalar U, std::size_t RowsA, std::size_t ColumnsA,
          std::size_t ColumnsB, vector_options O, vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), RowsA, ColumnsB, combine(O, P)>
    operator*(matrix<T, RowsA, ColumnsA, O> const& A,
              matrix<U, ColumnsA, ColumnsB, P> const& B) {
    return matrix<__mtl_promote(T, U), RowsA, ColumnsB, combine(O, P)>(
        [&](std::size_t i, std::size_t j) {
        return __mtl_with_index_sequence((K, ColumnsA), {
            return ((A.__mtl_at(i, K) * B.__mtl_at(K, j)) + ...);
        });
    });
}

/// Multiply Matrix by Vector
template <scalar T, scalar U, std::size_t RowsA, std::size_t ColumnsA,
          vector_options O, vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<__mtl_promote(T, U),
                                                           RowsA, combine(O, P)>
    operator*(matrix<T, RowsA, ColumnsA, O> const& A,
              vector<U, ColumnsA, P> const& v) {
    return vector<__mtl_promote(T, U), RowsA, combine(O, P)>(
        [&](std::size_t i) {
        return __mtl_with_index_sequence((K, ColumnsA), {
            return ((A.__mtl_at(i, K) * v.__mtl_at(K)) + ...);
        });
    });
}

/// Multiply Vector by Matrix
template <scalar T, scalar U, std::size_t ColumnsA, std::size_t ColumnsB,
          vector_options O, vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<
    __mtl_promote(T, U), ColumnsB, combine(O, P)>
    operator*(vector<T, ColumnsA, O> const& v,
              matrix<U, ColumnsA, ColumnsB, P> const& A) {
    return vector<__mtl_promote(T, U), ColumnsB, combine(O, P)>(
        [&](std::size_t j) {
        return __mtl_with_index_sequence((K, ColumnsA), {
            return ((v.__mtl_at(K) * A.__mtl_at(K, j)) + ...);
        });
    });
}

/// MARK: Divide
/// Divide Matrix by Scalar
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, O>
    operator/(matrix<T, Rows, Columns, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a / b; });
}

/// MARK: Modulo
/// Modulo divide Matrix by Scalar (element-wise)
template <scalar T, scalar U, std::size_t Rows, std::size_t Columns,
          vector_options O>
    requires requires(T&& t, U&& u) { t % u; }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr matrix<
    __mtl_promote(T, U), Rows, Columns, O>
    operator%(matrix<T, Rows, Columns, O> const& a, U const& b) {
    return map(a, [&b](auto a) { return a % b; });
}

} // namespace _VMTL

#endif // __MTL_ARITHMETIC_HPP_INCLUDED__
