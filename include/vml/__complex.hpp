#ifndef __VML_COMPLEX_HPP_INCLUDED__
#define __VML_COMPLEX_HPP_INCLUDED__

#include <cmath>
#include <iosfwd>

#include "__base.hpp"
#include "__common.hpp"
#include "__fwd.hpp"
#include "__vector.hpp"

namespace _VVML {

inline constexpr vector_options __vml_complex_vector_options =
    vector_options{}.packed(false);

/// MARK: - struct complex
template <real_scalar T>
struct complex<T>: public vector<T, 2, vector_options{}.packed(false)> {
    constexpr static vector_options __vml_options =
        __vml_complex_vector_options;
    using __vml_base = vector<T, 2, __vml_options>;
    using typename __vml_base::value_type;

    using __vector_data_<T, 2, __vml_options>::real;
    using __vector_data_<T, 2, __vml_options>::imag;

    /// Value Constructors
    complex() = default;
    __vml_always_inline __vml_interface_export constexpr complex(T real):
        __vml_base(real, 0) {}
    __vml_always_inline __vml_interface_export constexpr complex(T real,
                                                                 T imag):
        __vml_base(real, imag) {}
    __vml_always_inline __vml_interface_export constexpr complex(
        vector<T, 2, __vml_options> const& v):
        __vml_base(v) {}
    __vml_always_inline __vml_interface_export explicit constexpr complex(
        quaternion<T> const& q):
        __vml_base(q.__vml_at(0), q.__vml_at(1)) {}

    /// Conversion Constructor
    template <std::convertible_to<T> U>
    __vml_always_inline __vml_interface_export constexpr complex(
        complex<U> const& rhs):
        __vml_base(rhs) {}

    complex& operator=(complex const&) & = default;

    /// MARK: - Arithmetic Assignment
    __vml_always_inline __vml_interface_export constexpr complex& operator+=(
        complex const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator+=(
        T const& rhs) & {
        *this = *this + rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator-=(
        complex const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator-=(
        T const& rhs) & {
        *this = *this - rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator*=(
        complex const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator*=(
        T const& rhs) & {
        *this = *this * rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator/=(
        complex const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
    __vml_always_inline __vml_interface_export constexpr complex& operator/=(
        T const& rhs) & {
        *this = *this / rhs;
        return *this;
    }
};

/// MARK: - Misc
template <real_scalar T>
complex(T) -> complex<T>;
template <real_scalar T, real_scalar U>
complex(T, U) -> complex<__vml_promote(T, U)>;

template <typename T>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr T real(
    complex<T> const& z) {
    return z.__vml_at(0);
}
template <typename T>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr T imag(
    complex<T> const& z) {
    return z.__vml_at(1);
}

template <typename T>
__vml_mathfunction __vml_always_inline constexpr vector<
    T, 2, __vml_complex_vector_options>&
    __vml_as_vector(complex<T>& z) {
    return static_cast<vector<T, 2, complex<T>::__vml_options>&>(z);
}
template <typename T>
__vml_mathfunction __vml_always_inline constexpr vector<
    T, 2, __vml_complex_vector_options> const&
    __vml_as_vector(complex<T> const& z) {
    return static_cast<vector<T, 2, complex<T>::__vml_options> const&>(z);
}

template <typename T>
__vml_mathfunction __vml_always_inline constexpr complex<T> __vml_mul_by_i(
    complex<T> const& z) {
    return { -imag(z), real(z) };
}

template <typename T>
__vml_mathfunction __vml_always_inline constexpr complex<T> __vml_sqr(
    complex<T> const& z) {
    return { (real(z) - imag(z)) * (real(z) + imag(z)),
             T(2) * real(z) * imag(z) };
}

/// isnan
template <typename T>
constexpr bool isnan(complex<T> const& z) {
    using std::isnan;
    return isnan(z.__vml_at(0)) || isnan(z.__vml_at(1));
}

/// isinf
template <typename T>
constexpr bool isinf(complex<T> const& z) {
    using std::isinf;
    return isinf(z.__vml_at(0)) || isinf(z.__vml_at(1));
}

/// isfinite
template <typename T>
constexpr bool isfinite(complex<T> const& z) {
    using std::isfinite;
    return isfinite(z.__vml_at(0)) || isfinite(z.__vml_at(1));
}

/// isnormal
template <typename T>
constexpr bool isnormal(complex<T> const& z) {
    using std::isnormal;
    return isnormal(z.__vml_at(0)) || isnormal(z.__vml_at(1));
}

/// MARK: - Literals
inline namespace literals {
inline namespace quaternion_literals {
inline namespace complex_literals {
inline constexpr complex<double> operator"" _i(long double x) {
    return { 0, static_cast<double>(x) };
}
inline constexpr complex<float> operator"" _if(long double x) {
    return { 0, static_cast<float>(x) };
}
inline constexpr complex<long double> operator"" _ild(long double x) {
    return { 0, static_cast<long double>(x) };
}
inline constexpr complex<int> operator"" _i(unsigned long long x) {
    return { 0, static_cast<int>(x) };
}
inline constexpr complex<long> operator"" _il(unsigned long long x) {
    return { 0, static_cast<long>(x) };
}
inline constexpr complex<long long> operator"" _ill(unsigned long long x) {
    return { 0, static_cast<long long>(x) };
}
inline constexpr complex<unsigned int> operator"" _iu(unsigned long long x) {
    return { 0, static_cast<unsigned int>(x) };
}
inline constexpr complex<unsigned long> operator"" _iul(unsigned long long x) {
    return { 0, static_cast<unsigned long>(x) };
}
inline constexpr complex<unsigned long long> operator"" _iull(
    unsigned long long x) {
    return { 0, static_cast<unsigned long long>(x) };
}
} // namespace complex_literals
} // namespace quaternion_literals
} // namespace literals

/// MARK: - Complex Arithmetic
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<T>
    operator+(complex<T> const& z) {
    return z;
}
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<T>
    operator-(complex<T> const& z) {
    return { -real(z), -imag(z) };
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator+(complex<T> const& a, complex<U> const& b) {
    return __vml_as_vector(a) + __vml_as_vector(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator+(complex<T> const& a, U const& b) {
    return complex<__vml_promote(T, U)>(real(a) + b, imag(a));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator+(T const& a, complex<U> const& b) {
    return complex<__vml_promote(T, U)>(a + real(b), imag(b));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator-(complex<T> const& a, complex<U> const& b) {
    return __vml_as_vector(a) - __vml_as_vector(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator-(complex<T> const& a, U const& b) {
    return complex<__vml_promote(T, U)>(real(a) - b, imag(a));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator-(T const& a, complex<U> const& b) {
    return complex<__vml_promote(T, U)>(a - real(b), -imag(b));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator*(complex<T> const& a, complex<U> const& b) {
    return complex<__vml_promote(T, U)>(real(a) * real(b) - imag(a) * imag(b),
                                        real(a) * imag(b) + imag(a) * real(b));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator*(complex<T> const& a, U const& b) {
    return complex<__vml_promote(T, U)>(real(a) * b, imag(a) * b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator*(T const& a, complex<U> const& b) {
    return complex<__vml_promote(T, U)>(a * real(b), a * imag(b));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator/(complex<T> const& a, complex<U> const& b) {
    return a * conj(b) / norm_squared(b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator/(complex<T> const& a, U const& b) {
    return complex<__vml_promote(T, U)>(real(a) / b, imag(a) / b);
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_promote(T, U)>
    operator/(T const& a, complex<U> const& b) {
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
    operator==(complex<T> const& a, complex<U> const& b) {
    return __vml_as_vector(a) == __vml_as_vector(b);
}
template <typename T, real_scalar U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(complex<T> const& a, U const& b) {
    return real(a) == b && imag(a) == 0;
}
template <real_scalar T, typename U>
    requires requires(T&& t, U&& u) {
        {
            t == u
        } -> std::convertible_to<bool>;
    }
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    operator==(T const& a, complex<U> const& b) {
    return a == real(b) && 0 == imag(b);
}

/// MARK: - Complex Math Functions
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<T>
    conj(complex<T> const& z) {
    return { real(z), -imag(z) };
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<T>
    inverse(complex<T> const& z) {
    return conj(z) / norm_squared(z);
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<T>
    normalize(complex<T> const& z) {
    return normalize(__vml_as_vector(z));
}
///
template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr __vml_floatify(T)
        abs(complex<T> const& z) {
    return norm(z);
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr __vml_floatify(T)
        arg(complex<T> const& z) {
    return std::atan2(_VVML::imag(z), _VVML::real(z));
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export complex<T> proj(
    complex<T> const& z) {
    if (std::isinf(_VVML::real(z)) || std::isinf(_VVML::imag(z))) {
        return { INFINITY, std::copysign(T(0), _VVML::imag(z)) };
    }
    return z;
}

template <real_scalar T, bool CheckTheta = true>
__vml_mathfunction __vml_always_inline
    __vml_interface_export constexpr complex<__vml_floatify(T)>
    unit_polar(T const& theta) {
    using F = __vml_floatify(T);

    if constexpr (CheckTheta) {
        __vml_safe_math_if(std::isnan(theta)) { return { theta, theta }; }
        __vml_safe_math_if(std::isinf(theta)) { return { F(NAN), F(NAN) }; }
    }

    return { std::cos(theta), std::sin(theta) };
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr complex<
    __vml_floatify(__vml_promote(T, U))>
    polar(T const& _r, U const& _theta) {

    using F = __vml_floatify(T);
    F const r = _r;
    F const theta = _theta;
    __vml_safe_math_if(std::isnan(r) || signbit(r)) return { F(NAN), F(NAN) };
    __vml_safe_math_if(std::isnan(theta)) {
        __vml_safe_math_if(std::isinf(r)) return { r, theta };
        return { theta, theta };
    }
    __vml_safe_math_if(std::isinf(theta)) {
        __vml_safe_math_if(std::isinf(r)) return { r, F(NAN) };
        return { F(NAN), F(NAN) };
    }

    return r * unit_polar<T, false>(theta);
}

/// MARK: Power Functions
template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    complex<__vml_floatify(T)>
    exp(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    F i = imag(z);
    __vml_safe_math_if(std::isinf(real(z))) {
        __vml_safe_math_if(real(z) < F(0)) {
            __vml_safe_math_if(!std::isinf(i)) i = F(1);
        }
        else __vml_safe_math_if(i == 0 || !std::isinf(i)) {
            __vml_safe_math_if(std::isinf(i)) i = F(NAN);
            return complex<F>(real(z), i);
        }
    }
    else __vml_safe_math_if(std::isnan(real(z)) && imag(z) == 0) return z;
    F const e = std::exp(real(z));
    return { e * std::cos(i), e * std::sin(i) };
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    complex<__vml_floatify(T)>
    log(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_expect(z != 0);
    return { std::log(abs(z)), arg(z) };
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    complex<__vml_floatify(T)>
    log10(complex<T> const& z) {
    return log(z) / std::log(T(10));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr complex<
    __vml_floatify(__vml_promote(T, U))>
    pow(complex<T> const& x, complex<U> const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr complex<
    __vml_floatify(__vml_promote(T, U))>
    pow(T const& x, complex<U> const& y) {
    return _VVML::exp(y * std::log(x));
}

template <real_scalar T, real_scalar U>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr complex<
    __vml_floatify(__vml_promote(T, U))>
    pow(complex<T> const& x, U const& y) {
    return _VVML::exp(y * _VVML::log(x));
}

template <real_scalar T>
__vml_mathfunction __vml_always_inline __vml_interface_export
    complex<__vml_floatify(T)>
    sqrt(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(imag(z))) return complex<F>(F(INFINITY),
                                                              imag(z));
    __vml_safe_math_if(std::isinf(real(z))) {
        __vml_safe_math_if(
            real(z) > F(0)) return complex<F>(real(z),
                                              std::isnan(imag(z)) ?
                                                  imag(z) :
                                                  std::copysign(F(0), imag(z)));
        return complex<F>(std::isnan(imag(z)) ? imag(z) : F(0),
                          std::copysign(real(z), imag(z)));
    }
    return _VVML::polar(std::sqrt(_VVML::abs(z)), _VVML::arg(z) / F(2));
}

/// MARK: Hyperbolic Functions
template <real_scalar T>
complex<__vml_floatify(T)> sinh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(real(z)) &&
                       !std::isinf(imag(z))) return complex<F>(real(z), F(NAN));
    __vml_safe_math_if(real(z) == 0 &&
                       !std::isinf(imag(z))) return complex<F>(real(z), F(NAN));
    __vml_safe_math_if(imag(z) == 0 && !std::isinf(real(z))) return z;
    return { std::sinh(real(z)) * std::cos(imag(z)),
             std::cosh(real(z)) * std::sin(imag(z)) };
}

template <real_scalar T>
complex<__vml_floatify(T)> cosh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(real(z)) && !std::isinf(imag(z))) return {
        std::abs(real(z)), F(NAN)
    };
    __vml_safe_math_if(real(z) == 0 && !std::isinf(imag(z))) return { F(NAN),
                                                                      real(z) };
    __vml_safe_math_if(real(z) == 0 && imag(z) == 0) return { F(1), imag(z) };
    __vml_safe_math_if(imag(z) == 0 && !std::isinf(real(z))) return {
        std::abs(real(z)), imag(z)
    };
    return { std::cosh(real(z)) * std::cos(imag(z)),
             std::sinh(real(z)) * std::sin(imag(z)) };
}

template <real_scalar T>
complex<__vml_floatify(T)> tanh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(real(z))) {
        __vml_safe_math_if(!std::isinf(imag(z))) return { F(1), F(0) };
        return { F(1), std::copysign(F(0), sin(F(2) * imag(z))) };
    }
    __vml_safe_math_if(std::isnan(real(z)) && imag(z) == 0) return z;
    F const _2r = 2 * real(z);
    F const _2i = 2 * imag(z);
    F const _d = std::cosh(_2r) + std::cos(_2i);
    F const _2rsh = std::sinh(_2r);
    __vml_safe_math_if(std::isinf(_2rsh) && std::isinf(_d)) return {
        _2rsh > F(0) ? F(1) : F(-1), _2i > F(0) ? F(0) : F(-0.)
    };
    return { _2rsh / _d, std::sin(_2i) / _d };
}

template <real_scalar T>
complex<__vml_floatify(T)> asinh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(real(z))) {
        __vml_safe_math_if(std::isnan(imag(z))) return z;
        __vml_safe_math_if(std::isinf(imag(z))) return {
            real(z), std::copysign(_VVML::constants<F>::pi * F(0.25), imag(z))
        };
        return { real(z), std::copysign(F(0), imag(z)) };
    }
    __vml_safe_math_if(std::isnan(real(z))) {
        __vml_safe_math_if(std::isinf(imag(z))) return complex<F>(imag(z),
                                                                  real(z));
        __vml_safe_math_if(imag(z) == 0) return z;
        return { real(z), real(z) };
    }
    __vml_safe_math_if(std::isinf(imag(
        z))) return { std::copysign(imag(z), real(z)),
                      std::copysign(_VVML::constants<F>::pi / F(2), imag(z)) };
    complex<F> const x = _VVML::log(z + _VVML::sqrt(__vml_sqr(z) + F(1)));
    return complex<F>(copysign(real(x), real(z)),
                      std::copysign(imag(x), imag(z)));
}

template <real_scalar T>
complex<__vml_floatify(T)> acosh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(real(z))) {
        __vml_safe_math_if(std::isnan(imag(z))) return { std::abs(real(z)),
                                                         imag(z) };
        __vml_safe_math_if(std::isinf(imag(z))) {
            __vml_safe_math_if(real(z) > 0) return {
                real(z),
                std::copysign(_VVML::constants<F>::pi * F(0.25), imag(z))
            };
            return { -real(z), std::copysign(_VVML::constants<F>::pi * F(0.75),
                                             imag(z)) };
        }
        __vml_safe_math_if(real(z) < 0) return {
            -real(z), std::copysign(_VVML::constants<F>::pi, imag(z))
        };
        return { real(z), std::copysign(F(0), imag(z)) };
    }
    __vml_safe_math_if(std::isnan(real(z))) {
        __vml_safe_math_if(std::isinf(imag(z))) return { std::abs(imag(z)),
                                                         real(z) };
        return { real(z), real(z) };
    }
    __vml_safe_math_if(std::isinf(imag(
        z))) return { std::abs(imag(z)),
                      std::copysign(_VVML::constants<F>::pi / F(2), imag(z)) };
    complex<F> const x = _VVML::log(z + _VVML::sqrt(__vml_sqr(z) - F(1)));
    return { std::copysign(real(x), F(0)), std::copysign(imag(x), imag(z)) };
}

template <real_scalar T>
complex<__vml_floatify(T)> atanh(complex<T> const& _z) {
    using F = __vml_floatify(T);
    complex<F> z = _z;
    __vml_safe_math_if(std::isinf(imag(z))) {
        return { std::copysign(F(0), real(z)),
                 std::copysign(_VVML::constants<F>::pi / F(2), imag(z)) };
    }
    __vml_safe_math_if(std::isnan(imag(z))) {
        __vml_safe_math_if(std::isinf(real(z)) || real(z) == 0) return {
            std::copysign(F(0), real(z)), imag(z)
        };
        return { imag(z), imag(z) };
    }
    __vml_safe_math_if(std::isnan(real(z))) { return { real(z), real(z) }; }
    __vml_safe_math_if(std::isinf(real(z))) {
        return { std::copysign(F(0), real(z)),
                 std::copysign(_VVML::constants<F>::pi / F(2), imag(z)) };
    }
    __vml_safe_math_if(std::abs(real(z)) == 1 && imag(z) == 0) {
        return { std::copysign(F(INFINITY), real(z)),
                 std::copysign(F(0), imag(z)) };
    }
    complex<F> const x = _VVML::log((F(1) + z) / (F(1) - z)) / F(2);
    return { std::copysign(real(x), real(z)), std::copysign(imag(x), imag(z)) };
}

/// MARK: Trigonometric Functions
template <real_scalar T>
complex<__vml_floatify(T)> sin(complex<T> const& z) {
    return -__vml_mul_by_i(_VVML::sinh(__vml_mul_by_i(z)));
}

template <real_scalar T>
complex<__vml_floatify(T)> cos(complex<T> const& z) {
    return _VVML::cosh(__vml_mul_by_i(z));
}

template <real_scalar T>
complex<__vml_floatify(T)> tan(complex<T> const& z) {
    return -__vml_mul_by_i(_VVML::tanh(__vml_mul_by_i(z)));
}

template <real_scalar T>
complex<__vml_floatify(T)> asin(complex<T> const& z) {
    using F = __vml_floatify(T);
    return -__vml_mul_by_i(
        _VVML::log(__vml_mul_by_i(z) + _VVML::sqrt(1 - z * z)));
}

template <real_scalar T>
complex<__vml_floatify(T)> acos(complex<T> const& z) {
    using F = __vml_floatify(T);
    return constants<F>::pi / 2 +
           __vml_mul_by_i(
               _VVML::log(__vml_mul_by_i(z) + _VVML::sqrt(1 - z * z)));
}

template <real_scalar T>
complex<__vml_floatify(T)> atan(complex<T> const& z) {
    return -__vml_mul_by_i(_VVML::atanh(__vml_mul_by_i(z)));
}

} // namespace _VVML

/// MARK: - Decomposition
namespace _VVML {

template <std::size_t I, typename T>
T const& get(complex<T> const& v) {
    return v.__vml_at(I);
}
template <std::size_t I, typename T>
T& get(complex<T>& v) {
    return v.__vml_at(I);
}
template <std::size_t I, typename T>
T&& get(complex<T>&& v) {
    return std::move(v).__vml_at(I);
}
template <std::size_t I, typename T>
T const&& get(complex<T> const&& v) {
    return std::move(v).__vml_at(I);
}

} // namespace _VVML

template <typename T>
struct std::tuple_size<_VVML::complex<T>>:
    std::integral_constant<std::size_t, 2> {};

template <std::size_t I, typename T>
struct std::tuple_element<I, _VVML::complex<T>> {
    using type = T;
};

#endif // __VML_COMPLEX_HPP_INCLUDED__
