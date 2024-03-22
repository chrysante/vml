#ifndef __MTL_INTRIN_HPP_INCLUDED__
#define __MTL_INTRIN_HPP_INCLUDED__

#include <cstddef>

#include <immintrin.h>

#include "__common.hpp"

namespace mtl {

template <typename T, std::size_t Size, bool Packed>
struct __simd_type {
    using type = T[Size];
    static T get(type const& array, std::size_t index) { return array[index]; }

    static void mul(type& a, type const& b) {
        for (int i = 0; i < Size; ++i) {
            a[i] *= b[i];
        }
    }
    static void div(type& a, type const& b) {
        for (int i = 0; i < Size; ++i) {
            a[i] /= b[i];
        }
    }
};

template <typename T, std::size_t Size, bool Packed>
using __simd_type_t = typename __simd_type<T, Size, Packed>::type;

template <>
struct __simd_type<float, 4, false> {
    using type = __m128;

    static float get(type const& array, std::size_t index) {
#if defined(_MSC_VER)
        return array.m128_f32[index];
#else
        return array[index];
#endif
    }

    static void mul(type& a, type const& b) { a = _mm_mul_ps(a, b); }
    static void div(type& a, type const& b) { a = _mm_div_ps(a, b); }
};

#if MTL_AVX
template <>
struct __simd_type<double, 4, false> {
    using type = __m256d;

    static double get(type const& array, std::size_t index) {
#if defined(_MSC_VER)
        return array.m256d_f64[index];
#else
        return array[index];
#endif
    }

    static void mul(type& a, type const& b) { a = _mm256_mul_pd(a, b); }
    static void div(type& a, type const& b) { a = _mm256_div_pd(a, b); }
};

#endif

} // namespace mtl

#endif // __MTL_INTRIN_HPP_INCLUDED__
