#ifndef __MTL_VECTOR_HPP_INCLUDED__
#define __MTL_VECTOR_HPP_INCLUDED__

#include <iosfwd>

#include "__arithmetic.hpp"
#include "__base.hpp"
#include "__common.hpp"
#include "__fwd.hpp"
#include "__intrin.hpp"

/// Synopsis
///
///     namespace mtl {
///
///     template <typename T, std::size_t Size, vector_options Options>
///     class vector {
///         // Type introspection:
///         value_type = T
///         size()      -> std::size_t    // Size
///         data_size() -> std::size_t    // Size including padding
///         options()   -> vector_options // Options
///
///         // Data members:
///         T x, y, ...;
///         T r, g, ...;
///
///         // Contructors:
///         vector() = default;
///         vector(T value);                 -> // vector{ value, value, ... }
///         vector(T value0, T value1, ...); -> // vector{ value, value, ... }
///         vector(f: () -> T);              -> // vector{ f(), f(), ... }
///         vector(f: (std::size_t) -> T);   -> // vector{ f(0), f(1), ... }
///
///         // operator[]:
///         operator[](std::size_t) -> T&
///         operator[](std::size_t) const -> T const&
///
///         // begin, end:
///         begin(), end()               -> T*
///         begin() const, end() const   -> T const*
///         cbegin() const, cend() const -> T const*
///
///         // data:
///         data() -> T*
///         data() const -> T const*
///
///         // functional operations:
///         swizzle(std::size_t i0, ...) -> vector<T, N, Options> // vector{
///         (*this)[i0], ... }
///
///         map(f: (T) -> Any) const -> vector<{ deduced type }, Size, Options>
///
///         fold(f: (T, T) -> T) const -> T
///         left_fold(f: (T, T) -> T) const -> T
///         right_fold(f: (T, T) -> T) const -> T
///
///
///         // statics
///         unit(std::size_t index, T value = 1) -> vector<T, Size, Options> //
///         vector{ 0..., value [at position 'index'], 0... }
///     };
///
///     operator==(vector, vector) -> bool
///     map(vector<Ts, Size>..., f: (Ts...) -> fResult) -> vector<fResult, Size>
///     fold(vector<T, Size> v, f: (T, T) -> T) -> T       // left_fold(v, f)
///     left_fold(vector<T, Size> v, f: (T, T) -> T) -> T  // ...f(f(f(v[0],
///     v[1]), v[2]),  ...) right_fold(vector<T, Size> v, f: (T, T) -> T) -> T
///     // f(...f(..., f(v[N-2], v[N-1])))
///
///     } // namespace mtl
///

namespace _VMTL {

template <typename... T, std::size_t S, vector_options... O,
          std::invocable<T...> F>
__mtl_mathfunction __mtl_always_inline constexpr auto __map_impl(
    F&& f, vector<T, S, O> const&... v) {
    using U = std::invoke_result_t<F, T...>;
    if constexpr (std::same_as<U, void>) {
        for (std::size_t i = 0; i < S; ++i) {
            std::invoke(__mtl_forward(f), v.__mtl_vec_at(i)...);
        }
    }
    else {
        constexpr auto P = combine(O...);
        return vector<U, S, P>([&](std::size_t i) {
            return std::invoke(__mtl_forward(f), v.__mtl_vec_at(i)...);
        });
    }
}

template <typename T0, std::size_t S, vector_options O0, std::invocable<T0> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, F&& f) {
    return __map_impl(__mtl_forward(f), v0);
}
template <typename T0, typename T1, std::size_t S, vector_options O0,
          vector_options O1, std::invocable<T0, T1> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, vector<T1, S, O1> const& v1, F&& f) {
    return __map_impl(__mtl_forward(f), v0, v1);
}
template <typename T0, typename T1, typename T2, std::size_t S,
          vector_options O0, vector_options O1, vector_options O2,
          std::invocable<T0, T1, T2> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, vector<T1, S, O1> const& v1,
        vector<T2, S, O2> const& v2, F&& f) {
    return __map_impl(__mtl_forward(f), v0, v1, v2);
}

template <typename T0, typename T1, typename T2, typename T3, std::size_t S,
          vector_options O0, vector_options O1, vector_options O2,
          vector_options O3, std::invocable<T0, T1, T2, T3> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, vector<T1, S, O1> const& v1,
        vector<T2, S, O2> const& v2, vector<T3, S, O3> const& v3, F&& f) {
    return __map_impl(__mtl_forward(f), v0, v1, v2, v3);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          std::size_t S, vector_options O0, vector_options O1,
          vector_options O2, vector_options O3, vector_options O4,
          std::invocable<T0, T1, T2, T3, T4> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, vector<T1, S, O1> const& v1,
        vector<T2, S, O2> const& v2, vector<T3, S, O3> const& v3,
        vector<T4, S, O4> const& v4, F&& f) {
    return __map_impl(__mtl_forward(f), v0, v1, v2, v3, v4);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, std::size_t S, vector_options O0, vector_options O1,
          vector_options O2, vector_options O3, vector_options O4,
          vector_options O5, std::invocable<T0, T1, T2, T3, T4, T5> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    map(vector<T0, S, O0> const& v0, vector<T1, S, O1> const& v1,
        vector<T2, S, O2> const& v2, vector<T3, S, O3> const& v3,
        vector<T4, S, O4> const& v4, vector<T5, S, O5> const& v5, F&& f) {
    return __map_impl(__mtl_forward(f), v0, v1, v2, v3, v4, v5);
}

__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    __left_fold_impl(auto&& f, auto&& a) {
    return a;
}

__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    __left_fold_impl(auto&& f, auto&& a, auto&& b) {
    return std::invoke(f, __mtl_forward(a), __mtl_forward(b));
}

__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    __left_fold_impl(auto&& f, auto&& a, auto&& b, auto&&... c) {
    return __left_fold_impl(f,
                            std::invoke(f, __mtl_forward(a), __mtl_forward(b)),
                            __mtl_forward(c)...);
}

template <typename T, std::size_t S, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T
    left_fold(vector<T, S, O> const& v, F&& f) {
    return __mtl_with_index_sequence((I, S), {
        return __left_fold_impl(f, v.__mtl_at(I)...);
    });
}

template <typename T, std::size_t S, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T
    right_fold(vector<T, S, O> const& v, F&& f) {
    return __mtl_with_index_sequence((I, S), {
        return __left_fold_impl(f, v.__mtl_at(S - 1 - I)...);
    });
}

template <typename T, std::size_t S, vector_options O,
          _VMTL::invocable_r<T, T, T> F>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr T fold(
    vector<T, S, O> const& v, F&& f) {
    return _VMTL::left_fold(v, __mtl_forward(f));
}

template <typename, std::size_t, vector_options, typename, typename>
struct __vector_base;

template <typename T, std::size_t Size, vector_options O>
struct __vector_data;

template <typename, std::size_t, vector_options>
struct __vector_data_;

template <typename T, std::size_t Size, vector_options O>
struct __vector_data_ {
    __vector_data_() = default;
    constexpr __vector_data_(T const& a, T const& b, T const& c, T const& d,
                             auto const&... r):
        __data{ a, b, c, d, r... } {
        if constexpr (std::is_same_v<T, float> && Size == 4 && !O.packed()) {
            if (!std::is_constant_evaluated()) {
                alignas(16) float arr[4]{ a, b, c, d };
                __vec = _mm_load_ps(arr);
            }
        }
#if MTL_AVX
        else if constexpr (std::is_same_v<T, double> && Size == 4 &&
                           !O.packed())
        {
            if (!std::is_constant_evaluated()) {
                alignas(32) double arr[4]{ a, b, c, d };
                __vec = _mm256_load_pd(arr);
            }
        }
#endif
    }

    template <typename, std::size_t, vector_options, typename, typename>
    friend struct __vector_base;
    friend struct __vector_data<T, Size, O>;
    friend struct vector<T, Size, O>;
    friend struct quaternion<T>;

private:
    union {
        T __data[Size];
        __simd_type_t<T, Size, O.packed()> __vec;
        struct {
            T hue, saturation, value, alpha;
        };
        struct {
            T x;
            union {
                struct {
                    T y;
                    union {
                        struct {
                            T z, w;
                        };
                        _VMTL::vector<T, 2, O.packed(true)> zw;
                    };
                };
                _VMTL::vector<T, 3, O.packed(true)> yzw;
                _VMTL::vector<T, 2, O.packed(true)> yz;
            };
        };
        _VMTL::vector<T, 3, O.packed(true)> xyz;
        _VMTL::vector<T, 2, O.packed(true)> xy;

        //  for quaternion
        struct {
            T real;
            union {
                _VMTL::vector<T, 3, O.packed(true)> vector;
                _VMTL::vector<T, 3, O.packed(true)> imag;
            };
        };

        struct {
            T r;
            union {
                struct {
                    T g;
                    union {
                        struct {
                            T b, a;
                        };
                        _VMTL::vector<T, 2, O.packed(true)> ba;
                    };
                };
                _VMTL::vector<T, 3, O.packed(true)> gba;
                _VMTL::vector<T, 2, O.packed(true)> gb;
            };
        };
        _VMTL::vector<T, 3, O.packed(true)> rgb;
        _VMTL::vector<T, 2, O.packed(true)> rg;
    };
};

template <typename T, std::size_t Size, vector_options O>
struct __vector_data: __vector_data_<T, Size, O> {
    using __mtl_base = __vector_data_<T, Size, O>;
    using __mtl_base::__mtl_base;

    using __mtl_base::__data;
    using __mtl_base::__vec;

    using __mtl_base::w;
    using __mtl_base::x;
    using __mtl_base::xy;
    using __mtl_base::xyz;
    using __mtl_base::y;
    using __mtl_base::yz;
    using __mtl_base::yzw;
    using __mtl_base::z;
    using __mtl_base::zw;

    using __mtl_base::a;
    using __mtl_base::b;
    using __mtl_base::ba;
    using __mtl_base::g;
    using __mtl_base::gb;
    using __mtl_base::gba;
    using __mtl_base::r;
    using __mtl_base::rg;
    using __mtl_base::rgb;

    using __mtl_base::alpha;
    using __mtl_base::hue;
    using __mtl_base::saturation;
    using __mtl_base::value;
};

/// MARK: Size = 3
template <typename T, vector_options O>
struct __vector_data_<T, 3, O> {
    __vector_data_() = default;
    constexpr __vector_data_(T const& a, T const& b, T const& c):
        __vec{ a, b, c } {}

    template <typename, std::size_t, vector_options, typename, typename>
    friend struct __vector_base;
    friend struct __vector_data<T, 3, O>;
    friend struct vector<T, 3, O>;

private:
    union {
        T __data[3];
        typename __simd_type<T, 3, O.packed()>::type __vec;
        struct {
            T hue, saturation, value;
        };
        struct {
            T x;
            union {
                struct {
                    T y, z;
                };
                vector<T, 2, O.packed(true)> yz;
            };
        };
        vector<T, 2, O.packed(true)> xy;

        struct {
            T r;
            union {
                struct {
                    T g, b;
                };
                vector<T, 2, O.packed(true)> gb;
            };
        };
        vector<T, 2, O.packed(true)> rg;
    };
};

template <typename T, vector_options O>
struct __vector_data<T, 3, O>: __vector_data_<T, 3, O> {
    using __mtl_base = __vector_data_<T, 3, O>;
    using __mtl_base::__mtl_base;

    using __mtl_base::__data;
    using __mtl_base::__vec;

    using __mtl_base::x;
    using __mtl_base::xy;
    using __mtl_base::y;
    using __mtl_base::yz;
    using __mtl_base::z;

    using __mtl_base::b;
    using __mtl_base::g;
    using __mtl_base::gb;
    using __mtl_base::r;
    using __mtl_base::rg;

    using __mtl_base::hue;
    using __mtl_base::saturation;
    using __mtl_base::value;
};

template <typename T, vector_options O>
struct __vector_data_<T, 2, O> {
    __vector_data_() = default;
    constexpr __vector_data_(T const& a, T const& b): __vec{ a, b } {}

    template <typename, std::size_t, vector_options, typename, typename>
    friend struct __vector_base;
    friend struct __vector_data<T, 2, O>;
    friend struct vector<T, 2, O>;
    friend struct complex<T>;

private:
    union {
        T __data[2];
        typename __simd_type<T, 2, O.packed()>::type __vec;
        struct {
            T x, y;
        };
        struct {
            T r, g;
        };
        //  for complex
        struct {
            T real, imag;
        };
    };
};

template <typename T, vector_options O>
struct __vector_data<T, 2, O>: __vector_data_<T, 2, O> {
    using __mtl_base = __vector_data_<T, 2, O>;
    using __mtl_base::__mtl_base;

    using __mtl_base::__data;
    using __mtl_base::__vec;

    using __mtl_base::x;
    using __mtl_base::y;

    using __mtl_base::g;
    using __mtl_base::r;
};

/// MARK: - struct __vector_base
constexpr std::size_t __calculate_alignment(std::size_t TAlign, std::size_t S,
                                            bool Packed) {
    auto const _s = [&] {
        switch (S) {
        case 0: // fallthrough
        case 1:
            __mtl_debugbreak("invalid");
            return -1;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 4;
        case 8:
            return 8;

        default:
            return 1;
        }
    }();

    return TAlign * (Packed ? 1 : _s);
}

template <typename T, std::size_t Size, vector_options O,
          typename = __mtl_make_type_sequence<T, Size>,
          typename = __mtl_make_index_sequence<Size>>
struct __vector_base;

template <typename T, std::size_t Size, vector_options O, typename... AllT,
          std::size_t... I>
class alignas(__calculate_alignment(alignof(T), Size, O.packed()))
    __vector_base<T, Size, O, __mtl_type_sequence<AllT...>,
                  __mtl_index_sequence<I...>>:
    public __vector_data<T, Size, O> {
    using __mtl_base = __vector_data<T, Size, O>;

public:
    // constructors:
    __vector_base() = default;
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        T const& x):
        __mtl_base{ ((void)I, x)... } {}
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        AllT const&... x):
        __mtl_base{ x... } {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __vector_base(
        _VMTL::invocable_r<T> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f)>):
        __mtl_base{ ((void)I, std::invoke(f))... } {}
    __mtl_always_inline __mtl_interface_export explicit constexpr __vector_base(
        _VMTL::invocable_r<T, std::size_t> auto&&
            f) noexcept(std::is_nothrow_invocable_v<decltype(f), std::size_t>):
        __mtl_base{ std::invoke(f, I)... } {}

    template <std::convertible_to<T> U, vector_options P>
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        vector<U, Size, P> const& rhs):
        __mtl_base{ static_cast<T>(rhs.__mtl_at(I))... } {}

    /// Construct from foreign tuple type
    template <__tuple_of_types<AllT...> Tuple>
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        Tuple&& t)
        requires(_VMTL::__is_foreign_type<Tuple>::value)
        : __mtl_base{ get<I>(t)... } {}

    /// Construct from foreign vector type
    template <typename VectorType>
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        VectorType&& v)
        requires(Size == 2) &&
                requires {
                    {
                        v.x
                    } -> std::convertible_to<T>;
                    {
                        v.y
                    } -> std::convertible_to<T>;
                } && (_VMTL::__is_foreign_type<VectorType>::value)
        : __mtl_base{ v.x, v.y } {}
    template <typename VectorType>
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        VectorType&& v)
        requires(Size == 3) &&
                requires {
                    {
                        v.x
                    } -> std::convertible_to<T>;
                    {
                        v.y
                    } -> std::convertible_to<T>;
                    {
                        v.z
                    } -> std::convertible_to<T>;
                } && (_VMTL::__is_foreign_type<VectorType>::value)
        : __mtl_base{ v.x, v.y, v.z } {}
    template <typename VectorType>
    __mtl_always_inline __mtl_interface_export constexpr __vector_base(
        VectorType&& v)
        requires(Size == 4) &&
                requires {
                    {
                        v.x
                    } -> std::convertible_to<T>;
                    {
                        v.y
                    } -> std::convertible_to<T>;
                    {
                        v.z
                    } -> std::convertible_to<T>;
                    {
                        v.w
                    } -> std::convertible_to<T>;
                } && (_VMTL::__is_foreign_type<VectorType>::value)
        : __mtl_base{ v.x, v.y, v.z, v.w } {}

    /// Convert to foreign vector type
    template <__foreign_vector_type<T, Size> VectorType>
    constexpr operator VectorType() {
        return const_cast<__vector_base const&>(*this).operator VectorType();
    }
    template <__foreign_vector_type<T, Size> VectorType>
    constexpr operator VectorType() const {
        return VectorType{ this->__data[I]... };
    }
};

template <typename T, std::size_t Size, vector_options O>
struct vector: public __vector_base<T, Size, O> {
    using __mtl_base = __vector_base<T, Size, O>;

    __mtl_always_inline constexpr T& __mtl_at(std::size_t i) & {
        __mtl_assert_audit(i < Size);
        return this->__data[i];
    }
    __mtl_always_inline constexpr T const& __mtl_at(std::size_t i) const& {
        __mtl_assert_audit(i < Size);
        return this->__data[i];
    }
    __mtl_always_inline constexpr T&& __mtl_at(std::size_t i) && {
        __mtl_assert_audit(i < Size);
        return std::move(this->__data[i]);
    }
    __mtl_always_inline constexpr T const&& __mtl_at(std::size_t i) const&& {
        __mtl_assert_audit(i < Size);
        return std::move(this->__data[i]);
    }

    __mtl_always_inline constexpr T __mtl_vec_at(std::size_t i) const {
        __mtl_assert_audit(i < Size);
        if (std::is_constant_evaluated()) {
            return this->__data[i];
        }
        else {
            return __simd_type<T, Size, O.packed()>::get(this->__vec, i);
        }
    }

    using value_type = T;
    __mtl_pure __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        size() {
        return Size;
    }
    __mtl_pure __mtl_always_inline
        __mtl_interface_export static constexpr std::size_t
        data_size() {
        return Size + (Size == 3 && !O.packed());
    }
    __mtl_pure __mtl_always_inline
        __mtl_interface_export static constexpr vector_options
        options() {
        return O;
    }

    static constexpr vector<T, Size, O> unit(std::size_t index,
                                             T const& value = 1) {
        __mtl_bounds_check(index, 0, Size);
        vector<T, Size, O> result{};
        result.__mtl_at(index) = value;
        return result;
    }

    /// Inheriting Constructors from __vector_base:
    using __mtl_base::__mtl_base;
    vector() = default;

    /// Some more Constructors
    // vector3(vector2, scalar)
    __mtl_always_inline __mtl_interface_export constexpr vector(
        vector<T, 2, O> const& a, T const& b)
        requires(Size == 3)
        : vector(a.__mtl_at(0), a.__mtl_at(1), b) {}
    // vector3(scalar, vector2)
    __mtl_always_inline __mtl_interface_export constexpr vector(
        T const& a, vector<T, 2, O> const& b)
        requires(Size == 3)
        : vector(a, b.__mtl_at(0), b.__mtl_at(1)) {}
    // vector4(vector2, s, s)
    __mtl_always_inline __mtl_interface_export constexpr vector(
        vector<T, 2, O> const& a, T const& b, T const& c)
        requires(Size == 4)
        : vector(a.__mtl_at(0), a.__mtl_at(1), b, c) {}
    // vector4(scalar, vector2, scalar)
    __mtl_always_inline __mtl_interface_export constexpr vector(
        T const& a, vector<T, 2, O> const& b, T const& c)
        requires(Size == 4)
        : vector(a, b.__mtl_at(0), b.__mtl_at(1), c) {}
    // vector4(scalar, scalar, vector2)
    __mtl_always_inline __mtl_interface_export constexpr vector(
        T const& a, T const& b, vector<T, 2, O> const& c)
        requires(Size == 4)
        : vector(a, b, c.__mtl_at(0), c.__mtl_at(1)) {}
    // vector4(vector2, vector2)
    __mtl_always_inline __mtl_interface_export vector(vector<T, 2, O> const& a,
                                                      vector<T, 2, O> const& b)
        requires(Size == 4)
        : vector(a.__mtl_at(0), a.__mtl_at(1), b.__mtl_at(0), b.__mtl_at(1)) {}
    // vector4(vector3, scalar)
    __mtl_always_inline __mtl_interface_export vector(vector<T, 3, O> const& a,
                                                      T const& b)
        requires(Size == 4)
        : vector(a.__mtl_at(0), a.__mtl_at(1), a.__mtl_at(2), b) {}
    // vector4(scalar, vector3)
    __mtl_always_inline __mtl_interface_export vector(T const& a,
                                                      vector<T, 3, O> const& b)
        requires(Size == 4)
        : vector(a, b.__mtl_at(0), b.__mtl_at(1), b.__mtl_at(2)) {}

    vector& operator=(vector const&) & = default;

    /// operator[]:
    __mtl_always_inline __mtl_interface_export constexpr T& operator[](
        std::size_t index) & {
        return const_cast<T&>(const_cast<vector const&>(*this)[index]);
    }
    __mtl_always_inline __mtl_interface_export constexpr T const& operator[](
        std::size_t index) const& {
        __mtl_bounds_check(index, 0, Size);
        return __mtl_at(index);
    }
    __mtl_always_inline __mtl_interface_export constexpr T&& operator[](
        std::size_t index) && {
        return const_cast<T&&>(const_cast<vector const&&>(*this)[index]);
    }
    __mtl_always_inline __mtl_interface_export constexpr T const&& operator[](
        std::size_t index) const&& {
        __mtl_bounds_check(index, 0, Size);
        return std::move(__mtl_at(index));
    }

    /// begin, end:
    __mtl_always_inline __mtl_interface_export constexpr T* begin() {
        return this->__data;
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* begin()
        const {
        return this->__data;
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* cbegin()
        const {
        return this->__data;
    }
    __mtl_always_inline __mtl_interface_export constexpr T* end() {
        return this->__data + size();
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* end() const {
        return this->__data + size();
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* cend() const {
        return this->__data + size();
    }

    /// data:
    __mtl_always_inline __mtl_interface_export constexpr T* data() {
        return this->__data;
    }
    __mtl_always_inline __mtl_interface_export constexpr T const* data() const {
        return this->__data;
    }

    /// swizzle:
    template <std::convertible_to<std::size_t>... I>
    __mtl_always_inline __mtl_interface_export constexpr vector<T, sizeof...(I),
                                                                O>
        swizzle(I... i) const {
        return { (*this)[i]... };
    }

    /// map:
    __mtl_always_inline __mtl_interface_export constexpr auto map(
        std::invocable<T> auto&& f) const {
        return _VMTL::map(*this, __mtl_forward(f));
    }

    /// fold:
    __mtl_always_inline __mtl_interface_export constexpr T fold(
        _VMTL::invocable_r<T, T, T> auto&& f) const {
        return _VMTL::fold(*this, __mtl_forward(f));
    }

    /// MARK: Arithmetic Assignment
    __mtl_always_inline __mtl_interface_export constexpr vector& operator+=(
        vector const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator+=(
        T const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator-=(
        vector const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator-=(
        T const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator*=(
        vector const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator*=(
        T const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator/=(
        vector const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator/=(
        T const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator%=(
        vector const& rhs) &
        requires std::integral<T>
    {
        *this = *this % rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator%=(
        T const& rhs) &
        requires std::integral<T>
    {
        *this = *this % rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator&=(
        vector const& rhs) & {
        *this = *this & rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator|=(
        vector const& rhs) & {
        *this = *this | rhs;
        return *this;
    }
    __mtl_always_inline __mtl_interface_export constexpr vector& operator^=(
        vector const& rhs) & {
        *this = *this ^ rhs;
        return *this;
    }

    template <std::integral U, vector_options P>
    __mtl_always_inline __mtl_interface_export constexpr vector& operator<<=(
        vector<U, Size, P> const& rhs) & {
        *this = *this << rhs;
        return *this;
    }

    template <std::integral U, vector_options P>
    __mtl_always_inline __mtl_interface_export constexpr vector& operator>>=(
        vector<U, Size, P> const& rhs) & {
        *this = *this >> rhs;
        return *this;
    }

    template <std::integral U>
    __mtl_always_inline __mtl_interface_export constexpr vector& operator<<=(
        U rhs) & {
        *this = *this << rhs;
        return *this;
    }

    template <std::integral U>
    __mtl_always_inline __mtl_interface_export constexpr vector& operator>>=(
        U rhs) & {
        *this = *this >> rhs;
        return *this;
    }

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

    __mtl_always_inline __mtl_interface_export constexpr T& min()
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::min(__mtl_at(I)...);
        });
    }

    __mtl_always_inline __mtl_interface_export constexpr T const& min() const
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::min(__mtl_at(I)...);
        });
    }

    __mtl_always_inline __mtl_interface_export constexpr T& max()
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::max(__mtl_at(I)...);
        });
    }

    __mtl_always_inline __mtl_interface_export constexpr T const& max() const
        requires real_scalar<T>
    {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::max(__mtl_at(I)...);
        });
    }
};

template <typename... T>
vector(T...) -> vector<__mtl_promote(T...), sizeof...(T)>;

template <typename T, typename U, std::size_t Size, vector_options O,
          vector_options P>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(vector<T, Size, O> const& v, vector<U, Size, P> const& w) {
    return _VMTL::fold(_VMTL::map(v, w, _VMTL::__mtl_equals),
                       _VMTL::__mtl_logical_and);
}

template <typename T, typename U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(vector<T, Size, O> const& v, U const& w) {
    return _VMTL::fold(_VMTL::map(v, [&](auto i) { return i == w; }),
                       _VMTL::__mtl_logical_and);
}

template <typename T, typename U, std::size_t Size, vector_options O>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr bool
    operator==(U const& w, vector<T, Size, O> const& v) {
    return v == w;
}

template <typename CharT, typename T, std::size_t Size, vector_options O>
__mtl_interface_export std::basic_ostream<CharT>& operator<<(
    std::basic_ostream<CharT>& str, vector<T, Size, O> const& v) {
    str << '(';
    for (bool first = true; auto&& i: v) {
        str << (first ? ((void)(first = false), "") : ", ") << i;
    }
    return str << ')';
}

/// Concatenate Vectors and Scalars, handrolled to go easy on template meta
/// programming
// (s, s)
template <scalar T, scalar U>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), 2>
    concat(T const& a, U const& b) {
    return vector<__mtl_promote(T, U), 2>(a, b);
}

// (s, s, s)
template <scalar T, scalar U, scalar V>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U, V), 3>
    concat(T const& a, U const& b, V const& c) {
    return vector<__mtl_promote(T, U, V), 3>(a, b, c);
}

// (s, s, s, s)
template <scalar T, scalar U, scalar V, scalar W>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U, V, W), 4>
    concat(T const& a, U const& b, V const& c, W const& d) {
    return vector<__mtl_promote(T, U, V, W), 4>(a, b, c, d);
}

// (v2, s)
template <scalar T, scalar U, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export
    vector<__mtl_promote(T, U), 3, O>
    concat(vector<T, 2, O> const& a, U const& b) {
    return vector<__mtl_promote(T, U), 3, O>(a, b);
}

// (s, v2)
template <scalar T, scalar U, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), 3, O>
    concat(T const& a, vector<U, 2, O> const& b) {
    return vector<__mtl_promote(T, U), 3, O>(a, b);
}

// (v2, s, s)
template <scalar T, scalar U, scalar V, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U, V), 4, O>
    concat(vector<T, 2, O> const& a, U const& b, V const& c) {
    return vector<__mtl_promote(T, U, V), 4, O>(a, b, c);
}

// (s, v2, s)
template <scalar T, scalar U, scalar V, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U, V), 4, O>
    concat(T const& a, vector<U, 2, O> const& b, V const& c) {
    return vector<__mtl_promote(T, U, V), 4, O>(a, b, c);
}

// (s, s, v2)
template <scalar T, scalar U, scalar V, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U, V), 4, O>
    concat(T const& a, U const& b, vector<V, 2, O> const& c) {
    return vector<__mtl_promote(T, U, V), 4, O>(a, b, c);
}

// (v2, v2)
template <scalar T, scalar U, vector_options O, vector_options P>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), 4, O>
    concat(vector<T, 2, O> const& a, vector<U, 2, P> const& b) {
    return vector<__mtl_promote(T, U), 4, O>(a, b);
}

// (v3, s)
template <scalar T, scalar U, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), 4, O>
    concat(vector<T, 3, O> const& a, U const& b) {
    return vector<__mtl_promote(T, U), 4, O>(a, b);
}

// (s, v3)
template <scalar T, scalar U, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_promote(T, U), 4, O>
    concat(T const& a, vector<U, 3, O> const& b) {
    return vector<__mtl_promote(T, U), 4, O>(a, b);
}

template <typename T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    reverse(vector<T, Size, O> const& v) {
    return vector<T, Size, O>(
        [&](std::size_t i) { return v.__mtl_at(Size - 1 - i); });
}

template <typename To, typename T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<To, Size, O>
    type_cast(vector<T, Size, O> const& v) {
    return vector<To, Size, O>(
        [&](std::size_t i) { return static_cast<To>(v.__mtl_at(i)); });
}

template <std::size_t To, typename T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, To, O>
    dimension_cast(vector<T, Size, O> const& v) {
    return vector<T, To, O>([&](std::size_t i) {
        if constexpr (To > Size) {
            return i >= Size ? T(0) : v.__mtl_at(i);
        }
        else {
            return v.__mtl_at(i);
        }
    });
}

template <typename T, std::size_t Size, vector_options O>
constexpr vector<bool, Size, O> isnan(vector<T, Size, O> const& m) {
    return vector<bool, Size, O>([&](std::size_t i) {
        using std::isnan;
        return isnan(m.__mtl_at(i));
    });
}

template <typename T, std::size_t Size, vector_options O>
constexpr vector<bool, Size, O> isinf(vector<T, Size, O> const& m) {
    return vector<bool, Size, O>([&](std::size_t i) {
        using std::isinf;
        return isinf(m.__mtl_at(i));
    });
}

template <typename T, std::size_t Size, vector_options O>
constexpr vector<bool, Size, O> isfinite(vector<T, Size, O> const& m) {
    return vector<bool, Size, O>([&](std::size_t i) {
        using std::isfinite;
        return isfinite(m.__mtl_at(i));
    });
}

template <typename T, std::size_t Size, vector_options O>
constexpr vector<bool, Size, O> isnormal(vector<T, Size, O> const& m) {
    return vector<bool, Size, O>([&](std::size_t i) {
        using std::isnormal;
        return isnormal(m.__mtl_at(i));
    });
}

template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    dot(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return fold(a * b, _VMTL::__mtl_plus);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    norm_squared(vector<T, Size, O> const& a) {
    return map(a, [](auto const& x) {
        return norm_squared(x);
    }).fold(_VMTL::__mtl_plus);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    length_squared(vector<T, Size, O> const& a) {
    return norm_squared(a);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    norm(vector<T, Size, O> const& a) {
    if constexpr (real_scalar<T>) {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::hypot(a.__mtl_at(I)...);
        });
    }
    else {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::hypot(norm(a.__mtl_at(I))...);
        });
    }
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    fast_norm(vector<T, Size, O> const& a) {
    if constexpr (real_scalar<T>) {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::fast_hypot(a.__mtl_at(I)...);
        });
    }
    else {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::fast_hypot(fast_norm(a.__mtl_at(I))...);
        });
    }
}

template <scalar F, scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    pnorm(F p, vector<T, Size, O> const& a) {
    if constexpr (real_scalar<T>) {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::phypot(p, a.__mtl_at(I)...);
        });
    }
    else {
        return __mtl_with_index_sequence((I, Size), {
            return _VMTL::phypot(p, pnorm(p, a.__mtl_at(I))...);
        });
    }
}

template <typename T, std::size_t Size, vector_options O>
constexpr auto sum_norm(vector<T, Size, O> const& v) {
    return __mtl_with_index_sequence((I, Size), {
        using std::abs;
        return (abs(v.__mtl_at(I)) + ...);
    });
}

template <typename T, std::size_t Size, vector_options O>
constexpr auto max_norm(vector<T, Size, O> const& v) {
    return __mtl_with_index_sequence((I, Size), {
        using std::abs;
        return max(abs(v.__mtl_at(I))...);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    length(vector<T, Size, O> const& a) {
    return norm(a);
}

template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    distance_squared(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return norm_squared(a - b);
}

template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    distance(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return norm(a - b);
}

template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    fast_distance(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return fast_norm(a - b);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    normalize(vector<T, Size, O> const& a) {
    return a / norm(a);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    fast_normalize(vector<T, Size, O> const& a) {
    return a / fast_norm(a);
}

template <real_scalar T, real_scalar... U, std::size_t Size, vector_options O,
          vector_options... P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U...), Size, combine(O, P...)>
    min(vector<T, Size, O> const& v, vector<U, Size, P> const&... w) {
    return map(v, w..., [](auto&&... x) { return _VMTL::min(x...); });
}

template <real_scalar T, real_scalar... U, std::size_t Size, vector_options O,
          vector_options... P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr vector<
    __mtl_promote(T, U...), Size, combine(O, P...)>
    max(vector<T, Size, O> const& v, vector<U, Size, P> const&... w) {
    return map(v, w..., [](auto&&... x) { return _VMTL::max(x...); });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    abs(vector<T, Size, O> const& a) {
    return a.map(_VMTL::__mtl_abs);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    sqrt(vector<T, Size, O> const& a) {
    return a.map(_VMTL::__mtl_sqrt);
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    cbrt(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::cbrt;
        return cbrt(a);
    });
}

template <scalar T, scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    pow(vector<T, Size, O> const& base, vector<U, Size, P> const& exp) {
    return map(base, exp, [](T const& b, U const& e) {
        using std::pow;
        return pow(b, e);
    });
}

template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    pow(vector<T, Size, O> const& base, U const& exp) {
    return base.map([&](T const& b) {
        using std::pow;
        return pow(b, exp);
    });
}

template <scalar T, scalar U, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    pow(T const& base, vector<U, Size, O> const& exp) {
    return exp.map([&](U const& e) {
        using std::pow;
        return pow(base, e);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    exp(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::exp;
        return exp(a);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    exp2(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::exp2;
        return exp2(a);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    log(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::log;
        return log(a);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    log2(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::log2;
        return log2(a);
    });
}

template <scalar T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    log10(vector<T, Size, O> const& a) {
    return a.map([](T const& a) {
        using std::log10;
        return log10(a);
    });
}

template <typename T, typename U = T, std::size_t Size, vector_options O,
          vector_options P = O>
    requires std::is_integral_v<T> && std::is_integral_v<U>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    ceil_divide(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, [](auto&& x, auto&& y) {
        return _VMTL::__mtl_ceil_divide(x, y);
    });
}

template <typename T, typename U, std::size_t Size, vector_options O>
    requires std::is_integral_v<T> && std::is_integral_v<U>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    ceil_divide(vector<T, Size, O> const& a, U const& b) {
    return a.map([&](auto x) { return _VMTL::__mtl_ceil_divide(x, b); });
}

template <typename T, typename U, std::size_t Size, vector_options O>
    requires std::is_integral_v<T> && std::is_integral_v<U>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    ceil_divide(T const& a, vector<U, Size, O> const& b) {
    return b.map([&](auto x) { return _VMTL::__mtl_ceil_divide(a, x); });
}

template <typename T, std::size_t Size, vector_options O>
    requires std::is_floating_point_v<T>
__mtl_mathfunction __mtl_always_inline __mtl_interface_export constexpr auto
    fract(vector<T, Size, O> const& a) {
    return a.map([](auto&& x) { return _VMTL::__mtl_fract(x); });
}

template <typename T, std::size_t Size, vector_options O>
    requires std::is_floating_point_v<T>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    floor(vector<T, Size, O> const& a) {
    return a.map([](auto&& x) {
        using std::floor;
        return floor(x);
    });
}

template <typename T, std::size_t Size, vector_options O>
    requires std::is_floating_point_v<T>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<T, Size, O>
    ceil(vector<T, Size, O> const& a) {
    return a.map([](auto&& x) {
        using std::ceil;
        return ceil(x);
    });
}

template <typename T, std::size_t Size, vector_options O>
__mtl_mathfunction __mtl_always_inline
    __mtl_interface_export constexpr vector<__mtl_floatify(T), Size, O>
    floor(vector<T, Size, O> const& a) {
    return a.map([](auto&& x) {
        using std::exp;
        return exp(x);
    });
}

template <real_scalar T, real_scalar U, vector_options O, vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector3<__mtl_promote(T, U),
                                                            combine(O, P)>
    cross(vector3<T, O> const& a, vector3<U, P> const& b) {
    return { a.__mtl_at(1) * b.__mtl_at(2) - a.__mtl_at(2) * b.__mtl_at(1),
             a.__mtl_at(2) * b.__mtl_at(0) - a.__mtl_at(0) * b.__mtl_at(2),
             a.__mtl_at(0) * b.__mtl_at(1) - a.__mtl_at(1) * b.__mtl_at(0) };
}

template <real_scalar T, real_scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<bool, Size,
                                                           combine(O, P)>
    operator<(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, __mtl_less);
}

template <real_scalar T, real_scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<bool, Size,
                                                           combine(O, P)>
    operator<=(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, __mtl_less_eq);
}

template <real_scalar T, real_scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<bool, Size,
                                                           combine(O, P)>
    operator>(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, __mtl_greater);
}

template <real_scalar T, real_scalar U, std::size_t Size, vector_options O,
          vector_options P>
__mtl_mathfunction __mtl_interface_export constexpr vector<bool, Size,
                                                           combine(O, P)>
    operator>=(vector<T, Size, O> const& a, vector<U, Size, P> const& b) {
    return map(a, b, __mtl_greater_eq);
}

} // namespace _VMTL

template <typename T, std::size_t N, _VMTL::vector_options O>
class std::hash<_VMTL::vector<T, N, O>> {
public:
    std::size_t operator()(_VMTL::vector<T, N, O> const& v) const {
        std::size_t seed = _VMTL::__mtl_hash_seed;
        for (auto& i: v) {
            seed = _VMTL::__mtl_hash_combine(seed, i);
        }
        return seed;
    }
};

namespace _VMTL {

template <std::size_t I, typename T, std::size_t Size, vector_options O>
    requires(I < Size)
constexpr T const& get(vector<T, Size, O> const& v) {
    return v.__mtl_at(I);
}
template <std::size_t I, typename T, std::size_t Size, vector_options O>
    requires(I < Size)
constexpr T& get(vector<T, Size, O>& v) {
    return v.__mtl_at(I);
}
template <std::size_t I, typename T, std::size_t Size, vector_options O>
    requires(I < Size)
constexpr T&& get(vector<T, Size, O>&& v) {
    return std::move(v.__mtl_at(I));
}
template <std::size_t I, typename T, std::size_t Size, vector_options O>
    requires(I < Size)
constexpr T const&& get(vector<T, Size, O> const&& v) {
    return std::move(v.__mtl_at(I));
}

} // namespace _VMTL

template <typename T, std::size_t Size, _VMTL::vector_options O>
struct std::tuple_size<_VMTL::vector<T, Size, O>>:
    std::integral_constant<std::size_t, Size> {};

template <std::size_t I, typename T, std::size_t Size, _VMTL::vector_options O>
struct std::tuple_element<I, _VMTL::vector<T, Size, O>> {
    using type = T;
};

#endif // __MTL_VECTOR_HPP_INCLUDED__
