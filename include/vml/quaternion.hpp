#ifndef __VML_QUATERNION_HPP_INCLUDED__
#define __VML_QUATERNION_HPP_INCLUDED__

#include <cmath>
#include <iosfwd>

#include "common.hpp"
#include "complex.hpp"
#include "fwd.hpp"
#include "vector.hpp"

namespace _VVML {

inline constexpr vector_options __vml_quaternion_vector_options =
    vector_options{}.packed(false);

/// MARK: - struct quaternion
template <real_scalar T>
struct quaternion<T>: public vector<T, 4, __vml_quaternion_vector_options> {
    static constexpr auto __vml_options = __vml_quaternion_vector_options;
    using __vml_base = vector<T, 4, __vml_options>;
    using typename __vml_base::value_type;

public:
    using __vector_data_<T, 4, __vml_options>::real;
    using __vector_data_<T, 4, __vml_options>::imag;
    using __vector_data_<T, 4, __vml_options>::vector;

    using __imag_t = _VVML::vector<T, 3, __vml_options>;

public:
    /// Value Constructors
    quaternion() = default;
    constexpr quaternion(T const& real): __vml_base(real, 0, 0, 0) {}
    template <vector_options P = __vml_quaternion_vector_options>
    constexpr quaternion(T const& real, _VVML::vector<T, 3, P> const& imag):
        __vml_base(real, imag.__vml_at(0), imag.__vml_at(1), imag.__vml_at(2)) {
    }
    constexpr quaternion(T const& a, T const& b, T const& c, T const& d):
        __vml_base(a, b, c, d) {}
    template <std::convertible_to<T> U>
    constexpr quaternion(complex<U> const& z):
        quaternion(z.__vml_at(0), z.__vml_at(1), 0, 0) {}
    template <vector_options P = __vml_quaternion_vector_options>
    constexpr quaternion(_VVML::vector<T, 4, P> const& v): __vml_base(v) {}

    /// Conversion Constructor
    template <std::convertible_to<T> U>
    constexpr quaternion(quaternion<U> const& rhs): __vml_base(rhs) {}

    quaternion& operator=(quaternion const&) & = default;

    /// MARK: - Arithmetic Assignment
    constexpr quaternion& operator+=(quaternion const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    constexpr quaternion& operator+=(T const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    constexpr quaternion& operator-=(quaternion const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    constexpr quaternion& operator-=(T const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    constexpr quaternion& operator*=(quaternion const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    constexpr quaternion& operator*=(T const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    constexpr quaternion& operator/=(quaternion const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
    constexpr quaternion& operator/=(T const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
};

/// MARK: - Misc
template <real_scalar T>
quaternion(T) -> quaternion<T>;
template <real_scalar T, real_scalar U, real_scalar V, real_scalar W>
quaternion(T, U, V, W) -> quaternion<__vml_promote(T, U, V, W)>;
template <real_scalar T, real_scalar U, vector_options O = vector_options{}>
quaternion(T, _VVML::vector<U, 3, O>) -> quaternion<__vml_promote(T, U)>;

/// real, imag:
template <typename T>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr T real(
    quaternion<T> const& z) {
    return z.__vml_at(0);
}
template <typename T>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr vector<
    T, 3, __vml_quaternion_vector_options>
    imag(quaternion<T> const& z) {
    return { z.__vml_at(1), z.__vml_at(2), z.__vml_at(3) };
}

template <typename T>
__vml_mathfunction __vml_always_inline constexpr vector<
    T, 4, __vml_quaternion_vector_options>&
    __vml_as_vector(quaternion<T>& z) {
    return static_cast<vector<T, 4, quaternion<T>::__vml_options>&>(z);
}
template <typename T>
__vml_mathfunction __vml_always_inline constexpr vector<
    T, 4, __vml_quaternion_vector_options> const&
    __vml_as_vector(quaternion<T> const& z) {
    return static_cast<vector<T, 4, quaternion<T>::__vml_options> const&>(z);
}

/// isnan
template <typename T>
constexpr bool isnan(quaternion<T> const& z) {
    using std::isnan;
    return isnan(z.__vml_at(0)) || isnan(z.__vml_at(1)) ||
           isnan(z.__vml_at(2)) || isnan(z.__vml_at(3));
}

/// isinf
template <typename T>
constexpr bool isinf(quaternion<T> const& z) {
    using std::isinf;
    return isinf(z.__vml_at(0)) || isinf(z.__vml_at(1)) ||
           isinf(z.__vml_at(2)) || isinf(z.__vml_at(3));
}

/// isfinite
template <typename T>
constexpr bool isfinite(quaternion<T> const& z) {
    using std::isfinite;
    return isfinite(z.__vml_at(0)) || isfinite(z.__vml_at(1)) ||
           isfinite(z.__vml_at(2)) || isfinite(z.__vml_at(3));
}

/// isnormal
template <typename T>
constexpr bool isnormal(quaternion<T> const& z) {
    using std::isnormal;
    return isnormal(z.__vml_at(0)) || isnormal(z.__vml_at(1)) ||
           isnormal(z.__vml_at(2)) || isnormal(z.__vml_at(3));
}

/// MARK: - Literals
inline namespace literals {
inline namespace quaternion_literals {
/*
 _i* literals are defined in complex.hpp and can be used in conjunction with
 these 'using namespace vml::quaternion_literals;' also pulls in the _i* complex
 literals
 */
inline constexpr quaternion<double> operator"" _j(long double x) {
    return { 0, 0, static_cast<double>(x), 0 };
}
inline constexpr quaternion<float> operator"" _jf(long double x) {
    return { 0, 0, static_cast<float>(x), 0 };
}
inline constexpr quaternion<long double> operator"" _jld(long double x) {
    return { 0, 0, static_cast<long double>(x), 0 };
}
inline constexpr quaternion<int> operator"" _j(unsigned long long x) {
    return { 0, 0, static_cast<int>(x), 0 };
}
inline constexpr quaternion<long> operator"" _jl(unsigned long long x) {
    return { 0, 0, static_cast<long>(x), 0 };
}
inline constexpr quaternion<long long> operator"" _jll(unsigned long long x) {
    return { 0, 0, static_cast<long long>(x), 0 };
}
inline constexpr quaternion<unsigned int> operator"" _ju(unsigned long long x) {
    return { 0, 0, static_cast<unsigned int>(x), 0 };
}
inline constexpr quaternion<unsigned long> operator"" _jul(
    unsigned long long x) {
    return { 0, 0, static_cast<unsigned long>(x), 0 };
}
inline constexpr quaternion<unsigned long long> operator"" _jull(
    unsigned long long x) {
    return { 0, 0, static_cast<unsigned long long>(x), 0 };
}
inline constexpr quaternion<double> operator"" _k(long double x) {
    return { 0, 0, 0, static_cast<double>(x) };
}
inline constexpr quaternion<float> operator"" _kf(long double x) {
    return { 0, 0, 0, static_cast<float>(x) };
}
inline constexpr quaternion<long double> operator"" _kld(long double x) {
    return { 0, 0, 0, static_cast<long double>(x) };
}
inline constexpr quaternion<int> operator"" _k(unsigned long long x) {
    return { 0, 0, 0, static_cast<int>(x) };
}
inline constexpr quaternion<long> operator"" _kl(unsigned long long x) {
    return { 0, 0, 0, static_cast<long>(x) };
}
inline constexpr quaternion<long long> operator"" _kll(unsigned long long x) {
    return { 0, 0, 0, static_cast<long long>(x) };
}
inline constexpr quaternion<unsigned int> operator"" _ku(unsigned long long x) {
    return { 0, 0, 0, static_cast<unsigned int>(x) };
}
inline constexpr quaternion<unsigned long> operator"" _kul(
    unsigned long long x) {
    return { 0, 0, 0, static_cast<unsigned long>(x) };
}
inline constexpr quaternion<unsigned long long> operator"" _kull(
    unsigned long long x) {
    return { 0, 0, 0, static_cast<unsigned long long>(x) };
}
} // namespace quaternion_literals
} // namespace literals

/// MARK: - Quaternion Arithmetic
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<T>
    operator+(quaternion<T> const& z) {
    return z;
}
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<T>
    operator-(quaternion<T> const& z) {
    return -__vml_as_vector(z);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator+(quaternion<T> const& a, quaternion<U> const& b) {
    return __vml_as_vector(a) + __vml_as_vector(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator+(quaternion<T> const& a, complex<U> const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) + real(b),
                                           imag(b) + a.__vml_at(1),
                                           a.__vml_at(2), a.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator+(quaternion<T> const& a, U const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) + b, a.__vml_at(1),
                                           a.__vml_at(2), a.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator+(complex<T> const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) + real(b),
                                           imag(a) + b.__vml_at(1),
                                           b.__vml_at(2), b.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator+(T const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(a + real(b), b.__vml_at(1),
                                           b.__vml_at(2), b.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator-(quaternion<T> const& a, quaternion<U> const& b) {
    return __vml_as_vector(a) - __vml_as_vector(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator-(quaternion<T> const& a, complex<U> const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) - real(b),
                                           a.__vml_at(1) - imag(b),
                                           a.__vml_at(2), a.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator-(quaternion<T> const& a, U const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) - b, a.__vml_at(1),
                                           a.__vml_at(2), a.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator-(complex<T> const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(real(a) - real(b),
                                           imag(a) - b.__vml_at(1),
                                           -b.__vml_at(2), -b.__vml_at(3));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator-(T const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(a - real(b), -imag(b));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator*(quaternion<T> const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(
        a.__vml_at(0) * b.__vml_at(0) - a.__vml_at(1) * b.__vml_at(1) -
            a.__vml_at(2) * b.__vml_at(2) - a.__vml_at(3) * b.__vml_at(3), // 1
        a.__vml_at(0) * b.__vml_at(1) + a.__vml_at(1) * b.__vml_at(0) +
            a.__vml_at(2) * b.__vml_at(3) - a.__vml_at(3) * b.__vml_at(2), // i
        a.__vml_at(0) * b.__vml_at(2) - a.__vml_at(1) * b.__vml_at(3) +
            a.__vml_at(2) * b.__vml_at(0) + a.__vml_at(3) * b.__vml_at(1), // j
        a.__vml_at(0) * b.__vml_at(3) + a.__vml_at(1) * b.__vml_at(2) -
            a.__vml_at(2) * b.__vml_at(1) + a.__vml_at(3) * b.__vml_at(0) // k
    );
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator*(quaternion<T> const& a, complex<U> const& b) {
    return quaternion<__vml_promote(T, U)>(
        a.__vml_at(0) * b.__vml_at(0) - a.__vml_at(1) * b.__vml_at(1), // 1
        a.__vml_at(0) * b.__vml_at(1) + a.__vml_at(1) * b.__vml_at(0), // i
        a.__vml_at(2) * b.__vml_at(0) + a.__vml_at(3) * b.__vml_at(1), // j
        -a.__vml_at(2) * b.__vml_at(1) + a.__vml_at(3) * b.__vml_at(0) // k
    );
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator*(complex<T> const& a, quaternion<U> const& b) {
    return quaternion<__vml_promote(T, U)>(
        a.__vml_at(0) * b.__vml_at(0) - a.__vml_at(1) * b.__vml_at(1), // 1
        a.__vml_at(0) * b.__vml_at(1) + a.__vml_at(1) * b.__vml_at(0), // i
        a.__vml_at(0) * b.__vml_at(2) - a.__vml_at(1) * b.__vml_at(3), // j
        a.__vml_at(0) * b.__vml_at(3) + a.__vml_at(1) * b.__vml_at(2)  // k
    );
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator*(quaternion<T> const& a, U const& b) {
    return __vml_as_vector(a) * b;
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator*(T const& a, quaternion<U> const& b) {
    return a * __vml_as_vector(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator/(quaternion<T> const& a, quaternion<U> const& b) {
    return a * conj(b) / norm_squared(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator/(complex<T> const& a, quaternion<U> const& b) {
    return a * conj(b) / norm_squared(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator/(quaternion<T> const& a, complex<U> const& b) {
    return a * conj(b) / norm_squared(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator/(quaternion<T> const& a, U const& b) {
    return __vml_as_vector(a) / b;
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<__vml_promote(T, U)>
    operator/(T const& a, quaternion<U> const& b) {
    return a * conj(b) / norm_squared(b);
}

/// MARK: - operator==
template <typename T, typename U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(quaternion<T> const& a, quaternion<U> const& b) {
    return __vml_as_vector(a) == __vml_as_vector(b);
}

template <typename T, typename U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(quaternion<T> const& a, complex<U> const& b) {
    return real(a) == real(b) && a.__vml_at(1) == imag(b) &&
           a.__vml_at(2) == 0 && a.__vml_at(3) == 0;
}

template <typename T, real_scalar U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(quaternion<T> const& a, U const& b) {
    return real(a) == b && a.__vml_at(1) == 0 && a.__vml_at(2) == 0 &&
           a.__vml_at(3) == 0;
}

template <typename T, typename U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(complex<T> const& a, quaternion<U> const& b) {
    return real(a) == real(b) && imag(a) == b.__vml_at(1) &&
           0 == b.__vml_at(2) && 0 == b.__vml_at(3);
}

template <real_scalar T, typename U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(T const& a, quaternion<U> const& b) {
    return a == real(b) && 0 == b.__vml_at(1) && 0 == b.__vml_at(2) &&
           0 == b.__vml_at(3);
}

/// MARK: - Quaternion Math Functions
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<T>
    conj(quaternion<T> const& z) {
    return { z.__vml_at(0), -z.__vml_at(1), -z.__vml_at(2), -z.__vml_at(3) };
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<T>
    inverse(quaternion<T> const& z) {
    return conj(z) / length_squared(z);
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<T>
    normalize(quaternion<T> const& z) {
    return normalize(__vml_as_vector(z));
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr __vml_floatify(T)
        abs(quaternion<T> const& z) {
    return norm(z);
}

/// MARK: Power Functions
template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    quaternion<__vml_floatify(T)>
    exp(quaternion<T> const& z) {
    auto const r_exp = std::exp(real(z));
    if (z.vector == 0) {
        return r_exp;
    }
    auto const _vnorm = norm(z.vector);
    return r_exp * (std::cos(_vnorm) +
                    quaternion{ 0, z.imag } * (std::sin(_vnorm) / _vnorm));
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    quaternion<__vml_floatify(T)>
    log(quaternion<T> const& z) {
    __vml_expect(z != 0);
    auto const _norm = _VVML::norm(z);
    if (z.vector == 0) {
        return { std::log(_norm), 0, 0, 0 };
    }
    auto const _vnorm = norm(z.vector);
    return { std::log(_norm), z.vector * std::acos(z.real / _norm) / _vnorm };
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    quaternion<__vml_floatify(T)>
    log10(quaternion<T> const& z) {
    return _VVML::log(z) / std::log(T(10));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<
        __vml_floatify(__vml_promote(T, U))>
    pow(quaternion<T> const& x, quaternion<U> const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<
        __vml_floatify(__vml_promote(T, U))>
    pow(complex<T> const& x, quaternion<U> const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<
        __vml_floatify(__vml_promote(T, U))>
    pow(T const& x, quaternion<U> const& y) {
    return _VVML::exp(y * std::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<
        __vml_floatify(__vml_promote(T, U))>
    pow(quaternion<T> const& x, complex<U> const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr quaternion<
        __vml_floatify(__vml_promote(T, U))>
    pow(quaternion<T> const& x, U const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    quaternion<__vml_floatify(T)>
    sqrt(quaternion<T> const& z) {
    __vml_expect(real(z) >= 0 || imag(z) != 0);
    auto const r = _VVML::abs(z);
    return std::sqrt(r) * (z + r) / _VVML::abs(z + r);
}

/// MARK: Hyperbolic Functions
template <real_scalar T>
quaternion<__vml_floatify(T)> sinh(quaternion<T> const& _z) {
    using F = __vml_floatify(T);
    quaternion<F> z = _z;
    auto const v_norm = _VVML::norm(z.vector);
    __vml_safe_math_if(std::isinf(real(z)) &&
                       !std::isinf(v_norm)) return complex<F>(real(z), F(NAN));
    __vml_safe_math_if(real(z) == 0 &&
                       !std::isinf(v_norm)) return complex<F>(real(z), F(NAN));
    __vml_safe_math_if(v_norm == 0 && !std::isinf(real(z))) return z;
    return { std::sinh(real(z)) * std::cos(v_norm),
             std::cosh(real(z)) * std::sin(v_norm) / v_norm * z.vector };
}

template <real_scalar T>
quaternion<__vml_floatify(T)> cosh(quaternion<T> const& _z) {
    using F = __vml_floatify(T);
    quaternion<F> z = _z;
    auto const v_norm = _VVML::norm(z.vector);
    __vml_safe_math_if(std::isinf(real(z)) && !std::isinf(v_norm)) return {
        std::abs(real(z)), F(NAN), F(NAN), F(NAN)
    };
    __vml_safe_math_if(real(z) == 0 && !std::isinf(v_norm)) return { F(NAN), 0,
                                                                     0, 0 };
    __vml_safe_math_if(real(z) == 0 && v_norm == 0) return { F(1), 0, 0, 0 };
    __vml_safe_math_if(v_norm == 0 && !std::isinf(real(z))) return {
        std::abs(real(z)), 0, 0, 0
    };
    return { std::cosh(real(z)) * std::cos(v_norm),
             std::sinh(real(z)) * std::sin(v_norm) / v_norm * z.vector };
}

template <real_scalar T>
quaternion<__vml_floatify(T)> tanh(quaternion<T> const& z) {
    return _VVML::sinh(z) / _VVML::cosh(z);
}

template <real_scalar T>
quaternion<__vml_floatify(T)> asinh(quaternion<T> const& z);

template <real_scalar T>
quaternion<__vml_floatify(T)> acosh(quaternion<T> const& z);

template <real_scalar T>
quaternion<__vml_floatify(T)> atanh(quaternion<T> const& z);

/// MARK: Trigonometric Functions
template <real_scalar T>
quaternion<__vml_floatify(T)> sin(quaternion<T> const& z) {
    auto const v_norm = norm(z.vector);
    return { std::sin(real(z)) * std::cosh(v_norm),
             std::cos(real(z)) * std::sinh(v_norm) / v_norm * z.vector };
}

template <real_scalar T>
quaternion<__vml_floatify(T)> cos(quaternion<T> const& z) {
    auto const v_norm = norm(z.vector);
    return { std::cos(real(z)) * std::cosh(v_norm),
             -std::sin(real(z)) * std::sinh(v_norm) / v_norm * z.vector };
}

template <real_scalar T>
quaternion<__vml_floatify(T)> tan(quaternion<T> const& z) {
    return sin(z) / cos(z);
}

template <real_scalar T>
quaternion<__vml_floatify(T)> asin(quaternion<T> const& z);

template <real_scalar T>
quaternion<__vml_floatify(T)> acos(quaternion<T> const& z);

template <real_scalar T>
quaternion<__vml_floatify(T)> atan(quaternion<T> const& z);

/// MARK: Helpers
template <real_scalar T, typename U = T, vector_options O = vector_options{}>
constexpr quaternion<__vml_floatify(__vml_promote(T, U))> make_rotation(
    T angle, vector3<U, O> const& axis) {
    using V = __vml_floatify(__vml_promote(T, U));
    return quaternion<V>(std::cos(angle / 2),
                         std::sin(angle / 2) * normalize(axis));
}

template <real_scalar T = double, typename U = T,
          vector_options O = vector_options{}>
constexpr vector3<__vml_floatify(__vml_promote(T, U)), O> rotate(
    vector3<T, O> const& v, quaternion<U> const& q) {
    using V = __vml_floatify(__vml_promote(T, U));
    return imag(q * quaternion<V>(0, v) * conj(q));
}

} // namespace _VVML

/// MARK: - Decomposition
namespace _VVML {

template <size_t I, typename T>
    requires(I < 2)
auto const& get(quaternion<T> const& v) {
    if constexpr (I == 0) {
        return v.__vml_at(0);
    }
    else {
        return v.imag;
    }
}
template <size_t I, typename T>
    requires(I < 2)
auto& get(quaternion<T>& v) {
    if constexpr (I == 0) {
        return v.__vml_at(0);
    }
    else {
        return v.imag;
    }
}
template <size_t I, typename T>
auto&& get(quaternion<T>&& v) {
    return std::move(get<I>(v));
}
template <size_t I, typename T>
auto&& get(quaternion<T> const&& v) {
    return std::move(get<I>(v));
}

} // namespace _VVML

template <typename T>
struct std::tuple_size<_VVML::quaternion<T>>:
    std::integral_constant<size_t, 2> {};

template <typename T>
struct std::tuple_element<0, _VVML::quaternion<T>> {
    using type = T;
};
template <typename T>
struct std::tuple_element<1, _VVML::quaternion<T>> {
    using type = typename _VVML::quaternion<T>::__imag_t;
};

#endif // __VML_QUATERNION_HPP_INCLUDED__
