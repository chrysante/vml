#ifndef __VML_COMMON_HPP_INCLUDED__
#define __VML_COMMON_HPP_INCLUDED__

#include <cmath>
#include <concepts>
#include <iosfwd>
#include <utility>

#include "fwd.hpp"

namespace _VVML {

/// Just so it exists
inline namespace literals {}

template <typename T = double>
    requires std::is_floating_point_v<T>
struct constants {
    static inline constexpr T pi =
        3.1415926535897932384626433832795028841971693993751058209749445923;
    static inline constexpr T e =
        2.7182818284590452353602874713526624977572470936999595749669676277;
    static inline constexpr T sqrt2 =
        1.4142135623730950488016887242096980785696718753769480731766797379;
};

template <typename T>
struct is_real_scalar: std::is_arithmetic<T> {};

template <typename T>
struct is_complex: std::false_type {};
template <typename T>
struct is_complex<complex<T>>: std::true_type {};
template <typename T>
struct is_complex<complex<T> const>: std::true_type {};
template <typename T>
struct is_complex<complex<T> volatile>: std::true_type {};
template <typename T>
struct is_complex<complex<T> const volatile>: std::true_type {};
template <typename T>
struct is_quaternion: std::false_type {};
template <typename T>
struct is_quaternion<quaternion<T>>: std::true_type {};
template <typename T>
struct is_quaternion<quaternion<T> const>: std::true_type {};
template <typename T>
struct is_quaternion<quaternion<T> volatile>: std::true_type {};
template <typename T>
struct is_quaternion<quaternion<T> const volatile>: std::true_type {};

template <typename T>
struct is_vector: std::false_type {};
template <typename T, size_t N, vector_options O>
struct is_vector<vector<T, N, O>>: std::true_type {};
template <typename T, size_t N, vector_options O>
struct is_vector<vector<T, N, O> const>: std::true_type {};
template <typename T, size_t N, vector_options O>
struct is_vector<vector<T, N, O> volatile>: std::true_type {};
template <typename T, size_t N, vector_options O>
struct is_vector<vector<T, N, O> const volatile>: std::true_type {};

template <typename T>
struct is_matrix: std::false_type {};
template <typename T, size_t R, size_t C, vector_options O>
struct is_matrix<matrix<T, R, C, O>>: std::true_type {};
template <typename T, size_t R, size_t C, vector_options O>
struct is_matrix<matrix<T, R, C, O> const>: std::true_type {};
template <typename T, size_t R, size_t C, vector_options O>
struct is_matrix<matrix<T, R, C, O> volatile>: std::true_type {};
template <typename T, size_t R, size_t C, vector_options O>
struct is_matrix<matrix<T, R, C, O> const volatile>: std::true_type {};

template <typename T>
struct __is_foreign_type_impl:
    std::conjunction<std::negation<is_complex<T>>,
                     std::negation<is_quaternion<T>>,
                     std::negation<is_vector<T>>, std::negation<is_matrix<T>>> {
};

template <typename T>
struct __is_foreign_type: __is_foreign_type_impl<std::decay_t<T>> {};

template <typename T>
concept __foreign_type = __is_foreign_type<T>::value;

template <typename Vector, typename ValueType, size_t N,
          typename = std::make_index_sequence<N>>
constexpr bool __is_vector_type_v = false;

template <typename Vector, typename ValueType, size_t N, size_t... I>
constexpr bool
    __is_vector_type_v<Vector, ValueType, N, std::index_sequence<I...>> =
        requires(ValueType&& t) { Vector{ (I, t)... }; };

template <typename V, typename T, size_t N>
concept __foreign_vector_type =
    __foreign_type<V> && __is_vector_type_v<V, T, N>;

template <typename T>
struct is_scalar:
    std::disjunction<is_real_scalar<T>, is_complex<T>, is_quaternion<T>> {};

template <typename>
struct get_underlying_type;

template <typename T>
struct get_underlying_type {
    using type = T;
};

template <typename T>
struct get_underlying_type<complex<T>> {
    using type = T;
};
template <typename T>
struct get_underlying_type<complex<T> const> {
    using type = T const;
};
template <typename T>
struct get_underlying_type<complex<T> volatile> {
    using type = T volatile;
};
template <typename T>
struct get_underlying_type<complex<T> const volatile> {
    using type = T const volatile;
};

template <typename T>
struct get_underlying_type<quaternion<T>> {
    using type = T;
};
template <typename T>
struct get_underlying_type<quaternion<T> const> {
    using type = T const;
};
template <typename T>
struct get_underlying_type<quaternion<T> volatile> {
    using type = T volatile;
};
template <typename T>
struct get_underlying_type<quaternion<T> const volatile> {
    using type = T const volatile;
};

template <typename T, size_t N, vector_options O>
struct get_underlying_type<vector<T, N, O>> {
    using type = T;
};
template <typename T, size_t N, vector_options O>
struct get_underlying_type<vector<T, N, O> const> {
    using type = T const;
};
template <typename T, size_t N, vector_options O>
struct get_underlying_type<vector<T, N, O> volatile> {
    using type = T volatile;
};
template <typename T, size_t N, vector_options O>
struct get_underlying_type<vector<T, N, O> const volatile> {
    using type = T const volatile;
};

template <typename T, size_t R, size_t C, vector_options O>
struct get_underlying_type<matrix<T, R, C, O>> {
    using type = T;
};
template <typename T, size_t R, size_t C, vector_options O>
struct get_underlying_type<matrix<T, R, C, O> const> {
    using type = T const;
};
template <typename T, size_t R, size_t C, vector_options O>
struct get_underlying_type<matrix<T, R, C, O> volatile> {
    using type = T volatile;
};
template <typename T, size_t R, size_t C, vector_options O>
struct get_underlying_type<matrix<T, R, C, O> const volatile> {
    using type = T const volatile;
};

template <typename T, bool = is_complex<T>::value || is_quaternion<T>::value ||
                             is_vector<T>::value || is_matrix<T>::value>
struct get_underlying_type_r /* bool = true */ {
    using type = typename get_underlying_type<T>::type;
};
template <typename T>
struct get_underlying_type_r<T, false> {
    using type = T;
};

template <typename>
struct get_vector_size;

template <typename T>
struct get_vector_size: std::integral_constant<size_t, 1> {};

template <typename T, size_t N, vector_options O>
struct get_vector_size<vector<T, N, O>>: std::integral_constant<size_t, N> {};
template <typename T, size_t N, vector_options O>
struct get_vector_size<vector<T, N, O> const>:
    std::integral_constant<size_t, N> {};
template <typename T, size_t N, vector_options O>
struct get_vector_size<vector<T, N, O> volatile>:
    std::integral_constant<size_t, N> {};
template <typename T, size_t N, vector_options O>
struct get_vector_size<vector<T, N, O> const volatile>:
    std::integral_constant<size_t, N> {};

template <typename>
struct get_vector_options;

template <typename T>
struct get_vector_options {
    static constexpr vector_options value = {};
};

template <typename T, size_t N, vector_options O>
struct get_vector_options<vector<T, N, O>> {
    static constexpr vector_options value = O;
};
template <typename T, size_t N, vector_options O>
struct get_vector_options<vector<T, N, O> const> {
    static constexpr vector_options value = O;
};
template <typename T, size_t N, vector_options O>
struct get_vector_options<vector<T, N, O> volatile> {
    static constexpr vector_options value = O;
};
template <typename T, size_t N, vector_options O>
struct get_vector_options<vector<T, N, O> const volatile> {
    static constexpr vector_options value = O;
};

template <typename...>
struct __vml_template_true_type: std::true_type {};

template <typename...>
struct __vml_template_false_type: std::false_type {};

/// MARK: Tuple Size
template <typename T, size_t = sizeof(T)>
std::true_type __is_defined_impl(T*);
std::false_type __is_defined_impl(...);

template <typename T>
using __is_defined = decltype(__is_defined_impl(std::declval<T*>()));

template <typename T, size_t N>
concept __tuple_of_size =
    __is_defined<std::tuple_size<std::decay_t<T>>>::value &&
    (std::tuple_size<std::decay_t<T>>::value == N);

template <typename, typename, typename...>
struct __tuple_conversion_test;

template <typename T, typename... Args, size_t... I>
struct __tuple_conversion_test<T, std::index_sequence<I...>, Args...> {
    static constexpr bool value = (requires(T&& t) {
        {
            get<I>(t)
        } -> std::convertible_to<Args>;
    } && ...);
};

template <typename T, typename... Args>
concept __tuple_of_types =
    __tuple_of_size<T, sizeof...(Args)> &&
    __tuple_conversion_test<T, std::index_sequence_for<Args...>,
                            Args...>::value;

template <typename V, typename T>
concept __foreign_vec2_type = (!__tuple_of_types<V, T, T>) &&
                              __is_foreign_type<V>::value && requires(V v) {
                                  {
                                      v.x
                                  } -> std::convertible_to<T>;
                                  {
                                      v.y
                                  } -> std::convertible_to<T>;
                              };

template <typename V, typename T>
concept __foreign_vec3_type = (!__tuple_of_types<V, T, T, T>) &&
                              __is_foreign_type<V>::value && requires(V v) {
                                  {
                                      v.x
                                  } -> std::convertible_to<T>;
                                  {
                                      v.y
                                  } -> std::convertible_to<T>;
                                  {
                                      v.z
                                  } -> std::convertible_to<T>;
                              };

template <typename V, typename T>
concept __foreign_vec4_type = (!__tuple_of_types<V, T, T, T, T>) &&
                              __is_foreign_type<V>::value && requires(V v) {
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
                              };

template <typename T>
concept real_scalar = is_real_scalar<T>::value;
template <typename T>
concept scalar = is_scalar<T>::value;

template <typename T, typename U, typename... V>
concept __vml_any_of = (std::same_as<T, U> || (std::same_as<T, V> || ...));

template <class F, class R, class... Args>
concept invocable_r = /*invocable<F, Args...> && */ std::convertible_to<
    std::invoke_result_t<F, Args...>, R>;

template <class R, class F, class... Args>
concept regular_invocable_r = invocable_r<R, F, Args...>;

#define __vml_floatify(__type) __vml_to_float_t<__type>

template <typename>
struct __vml_to_float;

template <typename T>
using __vml_to_float_t = typename __vml_to_float<T>::type;

template <typename T>
    requires std::is_arithmetic_v<T>
struct __vml_to_float<T> {
    using type = double;
};
template <>
struct __vml_to_float<float> {
    using type = float;
};
template <>
struct __vml_to_float<long double> {
    using type = long double;
};

template <typename T>
struct __vml_to_float<complex<T>> {
    using type = complex<__vml_to_float_t<T>>;
};

template <typename T>
struct __vml_to_float<quaternion<T>> {
    using type = quaternion<__vml_to_float_t<T>>;
};

template <typename>
struct __vml_to_complex;

template <typename T>
using __vml_to_complex_t = typename __vml_to_complex<T>::type;

template <typename T>
    requires std::is_arithmetic_v<T>
struct __vml_to_complex<T> {
    using type = complex<T>;
};

template <typename T>
struct __vml_to_complex<complex<T>> {
    using type = complex<T>;
};

#define __vml_decltype_stripped(...) std::decay_t<decltype(__VA_ARGS__)>

#define __vml_promote(...) ::std::common_type_t<__VA_ARGS__>

} // namespace _VVML

/// Promote Complex Numbers
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::complex<T>, _VVML::complex<U>> {
    using type = _VVML::complex<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::complex<T>, U> {
    using type = _VVML::complex<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<T, _VVML::complex<U>> {
    using type = _VVML::complex<typename std::common_type<T, U>::type>;
};

/// Promote Quaternions
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::quaternion<T>, _VVML::quaternion<U>> {
    using type = _VVML::quaternion<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::quaternion<T>, _VVML::complex<U>> {
    using type = _VVML::quaternion<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::quaternion<T>, U> {
    using type = _VVML::quaternion<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<_VVML::complex<T>, _VVML::quaternion<U>> {
    using type = _VVML::quaternion<typename std::common_type<T, U>::type>;
};
template <_VVML::real_scalar T, _VVML::real_scalar U>
struct std::common_type<T, _VVML::quaternion<U>> {
    using type = _VVML::quaternion<typename std::common_type<T, U>::type>;
};

/// Promote Vectors
template <typename T, typename U, size_t Size, _VVML::vector_options O>
struct std::common_type<_VVML::vector<T, Size, O>, U> {
    using type = _VVML::vector<typename std::common_type<T, U>::type, Size, O>;
};
template <typename T, typename U, size_t Size, _VVML::vector_options O>
struct std::common_type<T, _VVML::vector<U, Size, O>> {
    using type = _VVML::vector<typename std::common_type<T, U>::type, Size, O>;
};
template <typename T, typename U, size_t Size, _VVML::vector_options O,
          _VVML::vector_options P>
struct std::common_type<_VVML::vector<T, Size, O>, _VVML::vector<U, Size, P>> {
    using type = _VVML::vector<typename std::common_type<T, U>::type, Size,
                               combine(O, P)>;
};
template <typename T, typename U, size_t S1, size_t S2, _VVML::vector_options O,
          _VVML::vector_options P>
struct std::common_type<_VVML::vector<T, S1, O>,
                        _VVML::vector<U, S2, P>>; /// Can't promote these

/// Promote Matrices
template <typename T, typename U, size_t Rows, size_t Columns,
          _VVML::vector_options O>
struct std::common_type<_VVML::matrix<T, Rows, Columns, O>, U> {
    using type =
        _VVML::matrix<typename std::common_type<T, U>::type, Rows, Columns, O>;
};
template <typename T, typename U, size_t Rows, size_t Columns,
          _VVML::vector_options O>
struct std::common_type<T, _VVML::matrix<U, Rows, Columns, O>> {
    using type =
        _VVML::matrix<typename std::common_type<T, U>::type, Rows, Columns, O>;
};
template <typename T, typename U, size_t Rows, size_t Columns,
          _VVML::vector_options O, _VVML::vector_options P>
struct std::common_type<_VVML::matrix<T, Rows, Columns, O>,
                        _VVML::matrix<U, Rows, Columns, P>> {
    using type = _VVML::matrix<typename std::common_type<T, U>::type, Rows,
                               Columns, combine(O, P)>;
};
template <typename T, typename U,
          size_t Rows1, // size_t Columns1,
          size_t Rows2, size_t Columns2, _VVML::vector_options O,
          _VVML::vector_options P>
struct std::common_type<_VVML::matrix<T, Rows1, Columns2, O>,
                        _VVML::matrix<U, Rows2, Columns2, P>> {
}; /// Can't promote two matrices of different dimensions
template <typename T, typename U, size_t Size, size_t Rows, size_t Columns,
          _VVML::vector_options O, _VVML::vector_options P>
struct std::common_type<_VVML::matrix<T, Rows, Columns, O>,
                        _VVML::vector<U, Size, P>> {
}; /// Can't promote matrices with vectors
template <typename T, typename U, size_t Size, size_t Rows, size_t Columns,
          _VVML::vector_options O, _VVML::vector_options P>
struct std::common_type<_VVML::vector<T, Size, P>,
                        _VVML::matrix<U, Rows, Columns, O>> {
}; /// Can't promote vectors with matrices

namespace vml {

/// MARK: - enum struct handedness
enum struct handedness { left_handed, right_handed };
inline constexpr handedness left_handed = handedness::left_handed;
inline constexpr handedness right_handed = handedness::right_handed;
inline constexpr handedness default_handedness = handedness::right_handed;

/// MARK: Basic Algorithms
template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T const& max(T const& a, T const& b) {
    return (a < b) ? b : a;
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T& max(T& a, T& b) {
    return const_cast<T&>(
        max(const_cast<T const&>(a), const_cast<T const&>(b)));
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T const& max(T const& a, std::same_as<T> auto const& b,
                       std::same_as<T> auto const&... c) {
    return max(max(a, b), c...);
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T& max(T& a, std::same_as<T> auto& b, std::same_as<T> auto&... c) {
    return const_cast<T&>(max(const_cast<T const&>(a), const_cast<T const&>(b),
                              const_cast<T const&>(c)...));
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T const& min(T const& a, T const& b) {
    return (b < a) ? b : a;
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T& min(T& a, T& b) {
    return const_cast<T&>(
        min(const_cast<T const&>(a), const_cast<T const&>(b)));
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T const& min(T const& a, std::same_as<T> auto const& b,
                       std::same_as<T> auto const&... c) {
    return min(min(a, b), c...);
}

template <typename T>
    requires(std::is_arithmetic_v<T>)
constexpr T& min(T& a, std::same_as<T> auto& b, std::same_as<T> auto&... c) {
    return const_cast<T&>(min(const_cast<T const&>(a), const_cast<T const&>(b),
                              const_cast<T const&>(c)...));
}

template <typename T>
size_t __vml_hash_combine(size_t seed, T const& v) {
    std::hash<T> const hash;
    return seed ^ hash(v) + 0x9e37'79b9'43e3'f411 + (seed << 6) + (seed >> 2);
}

constexpr size_t __vml_hash_seed = 0x5f23'ef3b'34b5'e321;

#define __vml_forward(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

// __vml_with_index_sequence
#define __VML_PRIV_WIS_FT(I, S) <size_t... I>
#define __VML_PRIV_WIS_FP(I, S) (::std::index_sequence<I...>)
#define __VML_PRIV_WIS_FI(I, S) (::std::make_index_sequence<S>{})
#define __vml_with_index_sequence(Index, ...)                                  \
    [&] __VML_PRIV_WIS_FT Index __VML_PRIV_WIS_FP Index                        \
        -> decltype(auto) __VA_ARGS__ __VML_PRIV_WIS_FI Index

template <typename T>
__vml_mathfunction __vml_always_inline __vml_interface_export constexpr bool
    __vml_is_unit(T const& x) {
    if constexpr (std::is_floating_point_v<T>) {
        return x != 0;
    }
    else if constexpr (std::is_integral_v<T>) {
        return x == 1 || x == -1;
    }
    else {
        static_assert(!__vml_template_true_type<T>::value);
    }
}

template <size_t... I>
using __vml_index_sequence = std::index_sequence<I...>;

template <size_t N>
using __vml_make_index_sequence = std::make_index_sequence<N>;

template <typename... T>
struct __vml_type_sequence {};

template <typename T, size_t N, typename... R>
struct __vml_make_type_sequence_impl {
    using type =
        typename __vml_make_type_sequence_impl<T, N - 1, R..., T>::type;
};

template <typename T, typename... R>
struct __vml_make_type_sequence_impl<T, 0, R...> {
    using type = __vml_type_sequence<R...>;
};

template <typename T, size_t N>
using __vml_make_type_sequence =
    typename __vml_make_type_sequence_impl<T, N>::type;

/// MARK: - class approx
template <typename T>
inline constexpr T __vml_float_threshold = 0;
template <>
inline constexpr float __vml_float_threshold<float> = 0.000000000000001f;
template <>
inline constexpr double __vml_float_threshold<double> = 0.000000000000001;
template <>
inline constexpr long double __vml_float_threshold<long double> =
    0.000000000000001;

template <typename T>
    requires std::is_floating_point_v<T>
bool __vml_nearly_equal(T a, T b, T epsilon = __vml_float_threshold<T>) {
    T const absA = std::abs(a);
    T const absB = std::abs(b);
    T const diff = std::abs(a - b);

    if (a == b) { // shortcut, handles infinities
        return true;
    }
    else if (a == 0 || b == 0 || (absA + absB < std::numeric_limits<T>::min()))
    {
        // a or b is zero or both are extremely close to zero
        // relative error is less meaningful here
        return diff < epsilon;
        // return diff < (epsilon * std::numeric_limits<T>::min());
    }
    else { // use relative error
        return diff / _VVML::min((absA + absB), std::numeric_limits<T>::max()) <
               epsilon;
    }
}

template <typename T>
    requires(std::is_arithmetic_v<T>) || requires(T const& t) {
        t.begin();
        t.end();
    }
class approx {
    using U = typename get_underlying_type_r<T>::type;

public:
    approx(T const& z, U epsilon = __vml_float_threshold<U>):
        _value(z), _epsilon(epsilon) {}

    bool __vml_comp_eq(T rhs) const {
        if constexpr (std::is_floating_point_v<T>) {
            return __vml_nearly_equal(_value, rhs, _epsilon);
        }
        else if constexpr (std::is_integral_v<T>) {
            return _value == rhs;
        }
        else {
            auto i = _value.begin();
            auto j = rhs.begin();
            auto end = _value.end();
            for (; i != end; ++i, ++j) {
                using underlying = typename get_underlying_type<T>::type;
                if (*i != approx<underlying>(*j, _epsilon)) {
                    return false;
                }
            }
            return true;
        }
    }

    approx& epsilon(U new_epsilon) {
        _epsilon = new_epsilon;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& str, approx<T> const& a) {
        return str << a._value;
    }

private:
    T _value;
    U _epsilon;
};

template <typename T, typename U>
bool operator==(approx<T> const& r, U const& l) {
    return r.__vml_comp_eq(l);
}
template <typename T, typename U>
bool operator==(U const& r, approx<T> const& l) {
    return l.__vml_comp_eq(r);
}

/// MARK: - Helpers
template <real_scalar T>
constexpr __vml_floatify(T) to_radians(T degrees) {
    using F = __vml_floatify(T);
    return degrees * constants<F>::pi / 180;
}

template <std::floating_point T>
constexpr T to_degrees(T radians) {
    return radians / constants<T>::pi * 180;
}

/// MARK: - Functions on Builtins
__vml_pure __vml_always_inline inline constexpr std::uint32_t __vml_byte_swap(
    std::uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

__vml_pure __vml_always_inline inline /*constexpr*/ float __vml_fract(float f) {
    float i;
    float result = std::modf(f, &i);
    return (result < 0.0f) + result;
}
__vml_pure __vml_always_inline inline /*constexpr*/ double __vml_fract(
    double f) {
    double i;
    double result = std::modf(f, &i);
    return (result < 0.0) + result;
}
__vml_pure __vml_always_inline inline /*constexpr*/ long double __vml_fract(
    long double f) {
    long double i;
    long double result = std::modf(f, &i);
    return (result < 0.0l) + result;
}

template <typename T, typename U = T>
    requires std::is_integral_v<T> && std::is_integral_v<U>
__vml_pure __vml_always_inline constexpr __vml_promote(T, U)
    __vml_ceil_divide(T a, U b) {
    return (a / b) + !!(a % b);
}

#define VML_FOR_EACH_BUILTIN_TYPE(F)                                           \
    F(signed char);                                                            \
    F(unsigned char);                                                          \
    F(signed short);                                                           \
    F(unsigned short);                                                         \
    F(signed int);                                                             \
    F(unsigned int);                                                           \
    F(signed long);                                                            \
    F(unsigned long);                                                          \
    F(signed long long);                                                       \
    F(unsigned long long);                                                     \
    F(float);                                                                  \
    F(double);                                                                 \
    F(long double)

#define VML_FOR_EACH_BUILTIN_ARITHMETIC_TYPE(F)                                \
    F(signed int);                                                             \
    F(unsigned int);                                                           \
    F(signed long);                                                            \
    F(unsigned long);                                                          \
    F(signed long long);                                                       \
    F(unsigned long long);                                                     \
    F(float);                                                                  \
    F(double);                                                                 \
    F(long double)

#define VML_LENGTH_SQUARED(T)                                                  \
    __vml_mathfunction __vml_pure __vml_always_inline inline constexpr T       \
        norm_squared(T x) {                                                    \
        return x * x;                                                          \
    }                                                                          \
    __vml_mathfunction __vml_pure __vml_always_inline inline constexpr T       \
        length_squared(T x) {                                                  \
        return norm_squared(x);                                                \
    }
VML_FOR_EACH_BUILTIN_TYPE(VML_LENGTH_SQUARED);
#undef VML_LENGTH_SQUARED

#define VML_SQR(T)                                                             \
    __vml_mathfunction __vml_pure __vml_always_inline constexpr inline T       \
        __vml_sqr(T x) {                                                       \
        return x * x;                                                          \
    }
VML_FOR_EACH_BUILTIN_TYPE(VML_SQR);
#undef VML_SQR

#define VML_CONJ(T)                                                            \
    __vml_mathfunction __vml_pure __vml_always_inline inline constexpr T conj( \
        T x) {                                                                 \
        return x;                                                              \
    }
VML_FOR_EACH_BUILTIN_TYPE(VML_CONJ);
#undef VML_CONJ

#define VML_IPOW(T)                                                            \
    __vml_mathfunction __vml_pure constexpr inline T __vml_ipow(T x, int i) {  \
        __vml_expect(i >= 0);                                                  \
        if (i == 1) return x;                                                  \
        if (i == 0) return 1;                                                  \
        if (i % 2 == 0) return __vml_ipow(x * x, i / 2);                       \
        return x * __vml_ipow(x * x, i / 2);                                   \
    }
VML_FOR_EACH_BUILTIN_ARITHMETIC_TYPE(VML_IPOW);
#undef VML_IPOW

#undef VML_FOR_EACH_BUILTIN_ARITHMETIC_TYPE
#undef VML_FOR_EACH_BUILTIN_TYPE

// MARK: - Hypot
template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T fast_hypot(T a,
                                                  std::same_as<T> auto... b) {
    return std::sqrt(((a * a) + ... + (b * b)));
}

template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T __vml_safe_hypot(
    T a, std::same_as<T> auto b) {
    return std::hypot(a, b);
}

template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T __vml_safe_hypot(
    T a, std::same_as<T> auto... b) {
    /// Make all arguments positive ...
    a = std::abs(a);
    ((b = std::abs(b)), ...);
    /// ... put the biggest argument into a ...
    std::swap(a, _VVML::max(a, b...));
    /// ... and do the math:
    /// b / a <= 1 for all b, so the square of b / a is also <= 1,
    /// so the argument to std::sqrt is <= count of arguments, so no  overflow
    /// can occur. Overflow can only occur if the actual result overflows.
    return a * std::sqrt((T(1.0) + ... + __vml_sqr(b / a)));
}

template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure __vml_always_inline
    __vml_interface_export inline T
    __vml_hypot(T a) {
    return std::abs(a);
}

template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T __vml_hypot(T a,
                                                   std::same_as<T> auto... b) {
    T sum_of_squares = ((a * a) + ... + (b * b));
    /// Only use __vml_safe_hypot if necessary. This is about 5 times faster on
    /// non-overflowing inputs and about 10% slower on always-overflowing inputs
    __vml_safe_math_if(isinf(sum_of_squares)) {
        return __vml_safe_hypot(a, b...);
    }
    return std::sqrt(sum_of_squares);
}

/// Interface
template <scalar T, scalar... U>
__vml_mathfunction __vml_pure __vml_always_inline
    __vml_interface_export inline auto
    hypot(T a, U... b) {
    using F = __vml_floatify(__vml_promote(T, U...));
    return __vml_hypot((F)a, (F)b...);
}

// MARK: - pHypot
template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T __vml_safe_phypot(
    T p, T a, std::same_as<T> auto... b) {
    a = std::abs(a);
    ((b = std::abs(b)), ...);
    std::swap(a, _VVML::max(a, b...));
    return a * std::pow((1.0 + ... + std::pow(b / a, p)), T(1) / p);
}

template <scalar T>
__vml_mathfunction __vml_pure __vml_always_inline
    __vml_interface_export inline T
    __vml_phypot(T p, T a) {
    return std::abs(a);
}

template <_VVML::__vml_any_of<float, double, long double> T>
__vml_mathfunction __vml_pure inline T __vml_phypot(T p, T a,
                                                    std::same_as<T> auto... b) {
    T sum_of_powers =
        ((std::pow(std::abs(a), p) + ... + (std::pow(std::abs(b), p))));
    __vml_safe_math_if(isinf(sum_of_powers)) {
        return __vml_safe_phypot(p, a, b...);
    }
    return std::pow(sum_of_powers, T(1) / p);
}

/// Interface
template <scalar T, scalar... U>
__vml_mathfunction __vml_pure __vml_always_inline
    __vml_interface_export inline auto
    phypot(scalar auto p, T a, U... b) {
    using F = __vml_floatify(__vml_promote(T, U...));
    return __vml_phypot((F)p, (F)a, (F)b...);
}

// MARK: - Basic Function Objects
#define _VML_FUNCOBJ_DEF_(NAME, PARAMS, EXPR)                                  \
    struct NAME##_t {                                                          \
        constexpr auto operator() PARAMS const                                 \
            requires requires { EXPR; }                                        \
        {                                                                      \
            return EXPR;                                                       \
        }                                                                      \
    } inline constexpr NAME {}

struct __vml_plus_t {
    constexpr auto operator()(auto const& a) const
        requires requires { +a; }
    {
        return +a;
    }
    constexpr auto operator()(auto const& a, auto const& b) const
        requires requires { a + b; }
    {
        return a + b;
    }
} inline constexpr __vml_plus{};

struct __vml_minus_t {
    constexpr auto operator()(auto const& a) const
        requires requires { -a; }
    {
        return -a;
    }
    constexpr auto operator()(auto const& a, auto const& b) const
        requires requires { a - b; }
    {
        return a - b;
    }
} inline constexpr __vml_minus{};

_VML_FUNCOBJ_DEF_(__vml_multiplies, (auto const& a, auto const& b), a* b);
_VML_FUNCOBJ_DEF_(__vml_divides, (auto const& a, auto const& b), a / b);
_VML_FUNCOBJ_DEF_(__vml_modulo, (auto const& a, auto const& b), a % b);
_VML_FUNCOBJ_DEF_(__vml_and, (auto const& a, auto const& b), a& b);
_VML_FUNCOBJ_DEF_(__vml_or, (auto const& a, auto const& b), a | b);
_VML_FUNCOBJ_DEF_(__vml_xor, (auto const& a, auto const& b), a ^ b);
_VML_FUNCOBJ_DEF_(__vml_not, (auto const& a), ~a);
_VML_FUNCOBJ_DEF_(__vml_equals, (auto const& a, auto const& b), a == b);
_VML_FUNCOBJ_DEF_(__vml_less, (auto const& a, auto const& b), a < b);
_VML_FUNCOBJ_DEF_(__vml_less_eq, (auto const& a, auto const& b), a <= b);
_VML_FUNCOBJ_DEF_(__vml_greater, (auto const& a, auto const& b), a > b);
_VML_FUNCOBJ_DEF_(__vml_greater_eq, (auto const& a, auto const& b), a >= b);
_VML_FUNCOBJ_DEF_(__vml_logical_and, (auto const& a, auto const& b), a&& b);
_VML_FUNCOBJ_DEF_(__vml_logical_or, (auto const& a, auto const& b), a || b);
_VML_FUNCOBJ_DEF_(__vml_logical_not, (auto const& a), !a);

struct __vml_abs_t {
    constexpr auto operator()(auto const& a) const {
        using std::abs;
        return abs(a);
    }
} inline constexpr __vml_abs{};

struct __vml_sqrt_t {
    constexpr auto operator()(auto const& a) const {
        using std::sqrt;
        return sqrt(a);
    }
} inline constexpr __vml_sqrt{};

} // namespace vml

#endif // __VML_COMMON_HPP_INCLUDED__
