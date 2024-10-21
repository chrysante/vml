#ifndef __VML_EXT_HPP_INCLUDED__
#define __VML_EXT_HPP_INCLUDED__

#include <cmath>
#include <concepts>
#include <cstring>

#include "common.hpp"
#include "fwd.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"
#include "vector.hpp"

namespace _VVML {

/// MARK: - 4x4 Matrix Factories
/// look_at() -> View Matrix
template <handedness H = default_handedness, typename T = double,
          vector_options O = vector_options{}, typename U = T,
          vector_options P = O, typename V = T, vector_options Q = O>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<
    __vml_promote(T, U, V), combine(O, P, Q)>
    look_at(vector3<T, O> const& eye, vector3<U, P> const& center,
            vector3<V, Q> const& up) {
    using TT = __vml_promote(T, U, V);
    auto constexpr OO = combine(O, P, Q);

    if constexpr (H == left_handed) {
        vector3<TT, OO> const f = normalize(center - eye);
        vector3<TT, OO> const s = normalize(cross(up, f));
        vector3<TT, OO> const u = cross(f, s);

        matrix4x4<TT, OO> Result(1);
        Result.__vml_at(0, 0) = s.__vml_at(0);
        Result.__vml_at(0, 1) = s.__vml_at(1);
        Result.__vml_at(0, 2) = s.__vml_at(2);
        Result.__vml_at(1, 0) = u.__vml_at(0);
        Result.__vml_at(1, 1) = u.__vml_at(1);
        Result.__vml_at(1, 2) = u.__vml_at(2);
        Result.__vml_at(2, 0) = f.__vml_at(0);
        Result.__vml_at(2, 1) = f.__vml_at(1);
        Result.__vml_at(2, 2) = f.__vml_at(2);
        Result.__vml_at(0, 3) = -dot(s, eye);
        Result.__vml_at(1, 3) = -dot(u, eye);
        Result.__vml_at(2, 3) = -dot(f, eye);
        return Result;
    }
    else {
        static_assert(H == right_handed);
        vector3<TT, OO> const f = normalize(center - eye);
        vector3<TT, OO> const s = normalize(cross(f, up));
        vector3<TT, OO> const u = cross(s, f);

        matrix4x4<TT, OO> Result(1);
        Result.__vml_at(0, 0) = s.__vml_at(0);
        Result.__vml_at(0, 1) = s.__vml_at(1);
        Result.__vml_at(0, 2) = s.__vml_at(2);
        Result.__vml_at(1, 0) = u.__vml_at(0);
        Result.__vml_at(1, 1) = u.__vml_at(1);
        Result.__vml_at(1, 2) = u.__vml_at(2);
        Result.__vml_at(2, 0) = -f.__vml_at(0);
        Result.__vml_at(2, 1) = -f.__vml_at(1);
        Result.__vml_at(2, 2) = -f.__vml_at(2);
        Result.__vml_at(0, 3) = -dot(s, eye);
        Result.__vml_at(1, 3) = -dot(u, eye);
        Result.__vml_at(2, 3) = dot(f, eye);
        return Result;
    }
}

/// ortho() -> Orthogonal Projection Matrix
template <handedness H = default_handedness, real_scalar T,
          vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O> ortho(
    T const& left, T const& right, T const& bottom, T const& top,
    T const& zNear, T const& zFar) {
    if constexpr (H == left_handed) {
        return { T(2) / (right - left),
                 0,
                 0,
                 -(right + left) / (right - left),
                 0,
                 T(2) / (top - bottom),
                 0,
                 -(top + bottom) / (top - bottom),
                 0,
                 0,
                 T(1) / (zFar - zNear),
                 -zNear / (zFar - zNear),
                 0,
                 0,
                 0,
                 1 };
    }
    else {
        static_assert(H == right_handed);
        matrix4x4<T, O> Result(1);
        Result.__vml_at(0, 0) = T(2) / (right - left);
        Result.__vml_at(1, 1) = T(2) / (top - bottom);
        Result.__vml_at(2, 2) = -T(1) / (zFar - zNear);
        Result.__vml_at(0, 3) = -(right + left) / (right - left);
        Result.__vml_at(1, 3) = -(top + bottom) / (top - bottom);
        Result.__vml_at(2, 3) = -zNear / (zFar - zNear);
        return Result;
    }
}

template <handedness H = default_handedness,
          vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr auto ortho(
    real_scalar auto const& left, real_scalar auto const& right,
    real_scalar auto const& bottom, real_scalar auto const& top,
    real_scalar auto const& zNear, real_scalar auto const& zFar) {
    using T = __vml_promote(__vml_decltype_stripped(left),
                            __vml_decltype_stripped(right),
                            __vml_decltype_stripped(bottom),
                            __vml_decltype_stripped(top),
                            __vml_decltype_stripped(zNear),
                            __vml_decltype_stripped(zFar));
    return ortho<H, T>(left, right, bottom, top, zNear, zFar);
}

template <handedness H = default_handedness,
          vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr auto infinite_perspective(
    real_scalar auto const& field_of_view, real_scalar auto const& aspect_ratio,
    real_scalar auto const& near) {
    using T = __vml_promote(__vml_decltype_stripped(field_of_view),
                            __vml_decltype_stripped(aspect_ratio),
                            __vml_decltype_stripped(near));
    T const range = std::tan(field_of_view / T(2)) * near;
    T const left = -range * aspect_ratio;
    T const right = range * aspect_ratio;
    T const bottom = -range;
    T const top = range;
    if constexpr (H == left_handed) {
        matrix<T, 4, 4, O> result{};
        result.__vml_at(0, 0) = (T(2) * near) / (right - left);
        result.__vml_at(1, 1) = (T(2) * near) / (top - bottom);
        result.__vml_at(2, 2) = T(1);
        result.__vml_at(3, 2) = T(1);
        result.__vml_at(2, 3) = -T(2) * near;
        return result;
    }
    else {
        matrix<T, 4, 4, O> result{};
        result.__vml_at(0, 0) = (T(2) * near) / (right - left);
        result.__vml_at(1, 1) = (T(2) * near) / (top - bottom);
        result.__vml_at(2, 2) = -T(1);
        result.__vml_at(3, 2) = -T(1);
        result.__vml_at(2, 3) = -T(2) * near;
        return result;
    }
}

template <handedness H = default_handedness,
          vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr auto perspective(
    real_scalar auto const& field_of_view, real_scalar auto const& aspect_ratio,
    real_scalar auto const& near, real_scalar auto const& far) {
    using T = __vml_promote(__vml_decltype_stripped(field_of_view),
                            __vml_decltype_stripped(aspect_ratio),
                            __vml_decltype_stripped(near),
                            __vml_decltype_stripped(far));
    __vml_expect(std::abs(aspect_ratio - std::numeric_limits<T>::epsilon()) >
                 T(0));

    T const tanHalfFovy = std::tan(field_of_view / T(2));

    if constexpr (H == left_handed) {
        matrix<T, 4, 4, O> result{};
        result.__vml_at(0, 0) = T(1) / (aspect_ratio * tanHalfFovy);
        result.__vml_at(1, 1) = T(1) / (tanHalfFovy);
        result.__vml_at(2, 2) = far / (far - near);
        result.__vml_at(3, 2) = T(1);
        result.__vml_at(2, 3) = -(far * near) / (far - near);
        return result;
    }
    else {
        matrix<T, 4, 4, O> result{};
        result.__vml_at(0, 0) = T(1) / (aspect_ratio * tanHalfFovy);
        result.__vml_at(1, 1) = T(1) / (tanHalfFovy);
        result.__vml_at(2, 2) = far / (near - far);
        result.__vml_at(3, 2) = -T(1);
        result.__vml_at(2, 3) = -(far * near) / (far - near);
        return result;
    }
}

/// MARK: Transforms
template <std::floating_point T, vector_options O>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O> translation(
    vector3<T, O> const& offset) {
    return { T(1), T(0), T(0), offset.__vml_at(0),
             T(0), T(1), T(0), offset.__vml_at(1),
             T(0), T(0), T(1), offset.__vml_at(2),
             T(0), T(0), T(0), T(1) };
}

template <std::floating_point T, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O> rotation(
    quaternion<T> const& q) {
    return { 2 * (__vml_sqr(q[0]) + __vml_sqr(q[1])) - 1,
             2 * (q[1] * q[2] - q[0] * q[3]),
             2 * (q[1] * q[3] + q[0] * q[2]),
             0,
             2 * (q[1] * q[2] + q[0] * q[3]),
             2 * (__vml_sqr(q[0]) + __vml_sqr(q[2])) - 1,
             2 * (q[2] * q[3] - q[0] * q[1]),
             0,
             2 * (q[1] * q[3] - q[0] * q[2]),
             2 * (q[2] * q[3] + q[0] * q[1]),
             2 * (__vml_sqr(q[0]) + __vml_sqr(q[3])) - 1,
             0,
             0,
             0,
             0,
             1 };
}

template <std::floating_point T, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O> scale(
    vector3<T> const& s) {
    return { s[0], 0, 0, 0, 0, s[1], 0, 0, 0, 0, s[2], 0, 0, 0, 0, 1 };
}

template <std::floating_point T, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O> scale(T s) {
    scale({ s, s, s });
}

template <std::floating_point T, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr matrix4x4<T, O>
    make_transform(vector3<T, O> const& position,
                   quaternion<T> const& orientation, vector3<T, O> const& s) {
    return translation(position) * rotation(orientation) * scale(s);
}

__vml_mathfunction __vml_interface_export constexpr auto to_quaternion(
    real_scalar auto roll, real_scalar auto pitch, real_scalar auto yaw) {
    // Abbreviations for the various angular functions
    using T = __vml_floatify(
        __vml_promote(decltype(yaw), decltype(pitch), decltype(roll)));
    double cy = std::cos(yaw * T(0.5));
    double sy = std::sin(yaw * T(0.5));
    double cp = std::cos(pitch * T(0.5));
    double sp = std::sin(pitch * T(0.5));
    double cr = std::cos(roll * T(0.5));
    double sr = std::sin(roll * T(0.5));

    return quaternion<T>{ cr * cp * cy + sr * sp * sy,
                          sr * cp * cy - cr * sp * sy,
                          cr * sp * cy + sr * cp * sy,
                          cr * cp * sy - sr * sp * cy };
}

template <typename T>
__vml_mathfunction __vml_interface_export constexpr quaternion<
    __vml_floatify(T)>
    to_quaternion(vector3<T> const& euler) {
    return to_quaternion(euler.__vml_at(0), euler.__vml_at(1),
                         euler.__vml_at(2));
}

template <std::floating_point T>
__vml_mathfunction __vml_interface_export constexpr vector3<T> to_euler(
    quaternion<T> const& q) {
    // roll (x-axis rotation)
    T const sinr_cosp =
        2 * (q.__vml_at(0) * q.__vml_at(1) + q.__vml_at(2) * q.__vml_at(3));
    T const cosr_cosp =
        1 - 2 * (q.__vml_at(1) * q.__vml_at(1) + q.__vml_at(2) * q.__vml_at(2));
    T const roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    T const sinp =
        2 * (q.__vml_at(0) * q.__vml_at(2) - q.__vml_at(3) * q.__vml_at(1));
    T const pitch = std::abs(sinp) >= 1 ?
                        std::copysign(_VVML::constants<T>::pi / 2,
                                      sinp) : // use 90 degrees if out of range
                        std::asin(sinp);

    // yaw (z-axis rotation)
    T const siny_cosp =
        2 * (q.__vml_at(0) * q.__vml_at(3) + q.__vml_at(1) * q.__vml_at(2));
    T const cosy_cosp =
        1 - 2 * (q.__vml_at(2) * q.__vml_at(2) + q.__vml_at(3) * q.__vml_at(3));
    T const yaw = std::atan2(siny_cosp, cosy_cosp);

    return { roll, pitch, yaw };
}

/// MARK: - Matrix Decomposition
/// Decompose Rotation matrix (special orthogonal / determinant 1) to a
/// quaternion
template <std::floating_point T, vector_options O>
__vml_mathfunction __vml_interface_export constexpr quaternion<T>
    decompose_rotation(matrix3x3<T, O> m) {
    T const tr = trace(m);
    if (tr > 0) {
        T const S = std::sqrt(tr + T(1.0)) * 2; // S=4*qw
        return { T(0.25) * S, (m.__vml_at(2, 1) - m.__vml_at(1, 2)) / S,
                 (m.__vml_at(0, 2) - m.__vml_at(2, 0)) / S,
                 (m.__vml_at(1, 0) - m.__vml_at(0, 1)) / S };
    }
    else if (m.__vml_at(0, 0) > m.__vml_at(1, 1) &&
             m.__vml_at(0, 0) > m.__vml_at(2, 2))
    {
        T const S = std::sqrt(T(1.0) + m.__vml_at(0, 0) - m.__vml_at(1, 1) -
                              m.__vml_at(2, 2)) *
                    2; // S=4*qx
        return { (m.__vml_at(2, 1) - m.__vml_at(1, 2)) / S, 0.25 * S,
                 (m.__vml_at(0, 1) + m.__vml_at(1, 0)) / S,
                 (m.__vml_at(0, 2) + m.__vml_at(2, 0)) / S };
    }
    else if (m.__vml_at(1, 1) > m.__vml_at(2, 2)) {
        T const S = std::sqrt(T(1.0) + m.__vml_at(1, 1) - m.__vml_at(0, 0) -
                              m.__vml_at(2, 2)) *
                    2; // S=4*qy
        return { (m.__vml_at(0, 2) - m.__vml_at(2, 0)) / S,
                 (m.__vml_at(0, 1) + m.__vml_at(1, 0)) / S, 0.25 * S,
                 (m.__vml_at(1, 2) + m.__vml_at(2, 1)) / S };
    }
    else {
        float S = std::sqrt(T(1.0) + m.__vml_at(2, 2) - m.__vml_at(0, 0) -
                            m.__vml_at(1, 1)) *
                  2; // S=4*qz
        return { (m.__vml_at(1, 0) - m.__vml_at(0, 1)) / S,
                 (m.__vml_at(0, 2) + m.__vml_at(2, 0)) / S,
                 (m.__vml_at(1, 2) + m.__vml_at(2, 1)) / S, T(0.25) * S };
    }
}

/// Decompose a 4x4 affine transform matrix into translation, orientation and
/// scale
template <std::floating_point T, vector_options O>
__vml_mathfunction __vml_interface_export constexpr std::tuple<
    vector3<T, O>, quaternion<T>, vector3<T, O>>
    decompose_transform(matrix4x4<T, O> t) {
    vector3<T, O> translation = t.column(3).swizzle(0, 1, 2);
    vector3<T, O> scale = { norm(t.column(0)), norm(t.column(1)),
                            norm(t.column(2)) };

    for (int i = 0; i < 3; ++i) {
        t.set_column(i, t.column(i) / scale[i]);
    }

    return { translation, decompose_rotation(dimension_cast<3, 3>(t)), scale };
}

template <std::size_t Dim, std::floating_point T, vector_options O,
          std::floating_point U, vector_options P, real_scalar V>
__vml_mathfunction __vml_interface_export constexpr vector<
    __vml_promote(T, U, V), Dim, combine(O, P)>
    lerp(vector<T, Dim, O> const& a, vector<U, Dim, P> const& b, V t) {
    return (V(1) - t) * a + t * b;
}

template <std::size_t Dim, std::floating_point T, vector_options O,
          std::floating_point U, vector_options P, real_scalar V>
__vml_mathfunction __vml_interface_export constexpr vector<
    __vml_promote(T, U, V), Dim, combine(O, P)>
    slerp(vector<T, Dim, O> const& a, vector<U, Dim, P> const& b, V t) {
    auto omega = std::acos(dot(a, b));
    return (std::sin((V(1) - t) * omega) * a + std::sin(t * omega) * b) /
           std::sin(omega);
}

template <std::floating_point T, std::floating_point U, real_scalar V>
__vml_mathfunction __vml_interface_export constexpr quaternion<
    __vml_promote(T, U, V)>
    slerp(quaternion<T> const& a, quaternion<U> const& b, V t) {
    return pow(b * inverse(a), t) * a;
}

/// MARK: - 2D Transforms

template <vector_options O = vector_options{}, real_scalar S>
__vml_mathfunction __vml_interface_export constexpr matrix<__vml_floatify(S), 2,
                                                           2, O>
    make_rotation2x2(S angle) {
    return {
        std::cos(angle),
        std::sin(angle),
        -std::sin(angle),
        std::cos(angle),
    };
}

template <typename T, vector_options O, real_scalar S>
__vml_mathfunction __vml_interface_export constexpr vector2<
    __vml_floatify(__vml_promote(T, S)), O>
    rotate(vml::vector<T, 2, O> const& v, S angle) {
    return make_rotation2x2(angle) * v;
}

/// MARK: - Color
template <typename = double4>
struct colors;

template <typename T, vector_options O>
    requires std::is_floating_point_v<T>
struct colors<vector<T, 3, O>> {
    static constexpr vector<T, 3, O> hex(std::uint32_t value) {
        return colors<vector<T, 4, O>>::hex(value).swizzle(0, 1, 2);
    }

    static constexpr vector<T, 3, O> red =
        colors<vector<T, 4, O>>::red.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> orange =
        colors<vector<T, 4, O>>::orange.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> yellow =
        colors<vector<T, 4, O>>::yellow.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> green =
        colors<vector<T, 4, O>>::green.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> blue =
        colors<vector<T, 4, O>>::blue.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> purple =
        colors<vector<T, 4, O>>::purple.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> pink =
        colors<vector<T, 4, O>>::pink.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> black =
        colors<vector<T, 4, O>>::black.swizzle(0, 1, 2);
    static constexpr vector<T, 3, O> white =
        colors<vector<T, 4, O>>::white.swizzle(0, 1, 2);
};

template <std::floating_point T, vector_options O>
struct colors<vector<T, 4, O>> {
    static constexpr vector<T, 4, O> hex(std::uint32_t value) {
        value = _VVML::__vml_byte_swap(value);
        vector<std::uint8_t, 4, O> chars;
        std::memcpy(&chars, &value, sizeof(value));
        return vector<T, 4, O>(chars) / T((1 << 8) - 1);
    }

    static constexpr vector<T, 4, O> red = { 1.0, 0.0, 0.0, 1.0 };
    static constexpr vector<T, 4, O> orange = { 1.0, 0.5, 0.0, 1.0 };
    static constexpr vector<T, 4, O> yellow = { 1.0, 1.0, 0.0, 1.0 };
    static constexpr vector<T, 4, O> green = { 0.0, 1.0, 0.0, 1.0 };
    static constexpr vector<T, 4, O> blue = { 0.0, 0.0, 1.0, 1.0 };
    static constexpr vector<T, 4, O> purple = { 0.5, 0.0, 1.0, 1.0 };
    static constexpr vector<T, 4, O> pink = { 1.0, 0.0, 1.0, 1.0 };
    static constexpr vector<T, 4, O> black = { 0.0, 0.0, 0.0, 1.0 };
    static constexpr vector<T, 4, O> white = { 1.0, 1.0, 1.0, 1.0 };

    static constexpr vector<T, 4, O> clear = { 0.0, 0.0, 0.0, 0.0 };
};

template <real_scalar T = float, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr vector3<__vml_floatify(T),
                                                            O>
    rgb_to_hsv(vector3<T, O> const& rgb) {
    using F = __vml_floatify(T);
    float K = F(0);
    auto [r, g, b] = rgb;

    if (g < b) {
        std::swap(g, b);
        K = -1.f;
    }
    if (r < g) {
        std::swap(r, g);
        K = -2.f / 6.f - K;
    }

    F const chroma = r - (g < b ? g : b);
    F const x = chroma == F(0) ? F(0) : (g - b) / (F(6) * chroma);
    F const s = r == F(0) ? F(0) : chroma / r;
    return { std::abs(K + x), s, r };
};

template <real_scalar T, real_scalar U, real_scalar V>
__vml_mathfunction __vml_interface_export constexpr vector3<__vml_floatify(T)>
    rgb_to_hsv(T const& r, U const& g, V const& b) {
    return rgb_to_hsv({ r, b, g });
}

template <real_scalar T = float, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr vector4<__vml_floatify(T),
                                                            O>
    rgba_to_hsva(vector4<T, O> const& rgba) {
    return { rgb_to_hsv(rgba.swizzle(0, 1, 2)), rgba.__vml_at(3) };
}

template <real_scalar T, real_scalar U, real_scalar V, real_scalar W>
__vml_mathfunction __vml_interface_export constexpr vector4<__vml_floatify(T)>
    rgba_to_hsva(T const& r, U const& g, V const& b, W const& a) {
    return rgba_to_hsva({ r, b, g, a });
}

template <real_scalar T = float, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr vector3<__vml_floatify(T),
                                                            O>
    hsv_to_rgb(vector3<T, O> const& hsv) {
    using V = vector3<__vml_floatify(T), O>;
    using F = __vml_floatify(T);

    auto [h, s, v] = hsv;

    if (s == F(0)) { // gray
        return V(v);
    }

    h = std::fmod(h, F(1)) / (F(60) / F(360));
    int const i = (int)h;
    F const f = h - (F)i;
    F const p = v * (F(1) - s);
    F const q = v * (F(1) - s * f);
    F const t = v * (F(1) - s * (F(1) - f));

    switch (i) {
    case 0:
        return { v, t, p };
    case 1:
        return { q, v, p };
    case 2:
        return { p, v, t };
    case 3:
        return { p, q, v };
    case 4:
        return { t, p, v };
    case 5:
    default:
        return { v, p, q };
    }
}

template <real_scalar T, real_scalar U, real_scalar V>
__vml_mathfunction __vml_interface_export constexpr vector3<__vml_floatify(T)>
    hsv_to_rgb(T const& h, U const& s, V const& v) {
    return hsv_to_rgb({ h, s, v });
}

template <real_scalar T = float, vector_options O = vector_options{}>
__vml_mathfunction __vml_interface_export constexpr vector4<__vml_floatify(T),
                                                            O>
    hsva_to_rgba(vector4<T, O> const& hsva) {
    return { hsv_to_rgb(hsva.swizzle(0, 1, 2)), hsva.__vml_at(3) };
}

template <real_scalar T, real_scalar U, real_scalar V, real_scalar W>
__vml_mathfunction __vml_interface_export constexpr vector4<__vml_floatify(T)>
    hsva_to_rgba(T const& h, U const& s, V const& v, W const& a) {
    return hsva_to_rgba({ h, s, v, a });
}

} // namespace _VVML

/// MARK: - utl::iota<vml::vector<...>>
namespace utl {

template <typename>
class iota;

template <typename T, size_t N, _VVML::vector_options O>
    requires std::is_integral_v<T>
iota(_VVML::vector<T, N, O>) -> iota<_VVML::vector<T, N, O>>;

template <typename T, typename U, size_t N, _VVML::vector_options O,
          _VVML::vector_options P>
    requires std::is_integral_v<T> && std::is_integral_v<U>
iota(_VVML::vector<T, N, O>, _VVML::vector<U, N, P>)
    -> iota<_VVML::vector<__vml_promote(T, U), N, combine(O, P)>>;

template <typename T, size_t N, _VVML::vector_options O>
    requires std::is_integral_v<T>
class iota<_VVML::vector<T, N, O>> {
public:
    using value_type = _VVML::vector<T, N, O>;
    using size_type = _VVML::vector<
        std::conditional_t<std::is_signed_v<T>, std::ptrdiff_t, size_t>, N, O>;

public:
    class iterator {
        friend class iota;
        constexpr explicit iterator(value_type begin, value_type last):
            _begin(begin), _current(begin), _last(last) {}

    public:
        __vml_interface_export constexpr value_type operator*() const {
            return _current;
        }
        __vml_interface_export constexpr iterator& operator++() {
            for (size_t i = N - 1;;) {
                if (++_current[i] != _last[i]) {
                    break;
                }
                _current[i] = _begin[i];

                if (i == 0) {
                    _done = true;
                    break;
                }
                --i;
            }

            return *this;
        }
        __vml_interface_export constexpr bool operator!=(
            iterator const& rhs) const {
            __vml_assert_audit(this->_begin == rhs._begin);
            __vml_assert_audit(this->_last == rhs._last);
            return !_done;
        }

    private:
        value_type _begin, _current, _last;
        bool _done = false;
    };

public:
    __vml_interface_export constexpr iota(value_type last):
        _first(0), _last(last) {}
    __vml_interface_export constexpr iota(value_type first, size_type last):
        _first(first), _last(last) {}

    __vml_interface_export constexpr iterator begin() const {
        return iterator(_first, _last);
    }
    __vml_interface_export constexpr iterator end() const {
        return iterator(_first, _last);
    }

    __vml_interface_export constexpr size_type size() const {
        return _last - _first;
    }

private:
    value_type _first;
    value_type _last;
};

} // namespace utl

#endif // __VML_EXT_HPP_INCLUDED__
