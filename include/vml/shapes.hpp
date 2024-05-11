#ifndef __VML_SHAPES_HPP_INCLUDED__
#define __VML_SHAPES_HPP_INCLUDED__

#include <algorithm>
#include <iosfwd>

#include "fwd.hpp"
#include "vector.hpp"

namespace _VVML {

/// MARK: - AABB
template <typename T = double, size_t Dim = 3,
          vector_options O = vector_options{}>
class AABB {
public:
    /// Constructs an AABB located at the origin with size zero
    constexpr AABB(): _origin({}), _size({}) {}

    /// Construct the smallest AABB enclosing the points \p p...
    template <typename... U, vector_options... P>
    constexpr AABB(vector<U, Dim, P> const&... p):
        _origin(vml::min(p...)), _size(map(p..., [](auto... x) {
            return std::max({ x... }) - std::min({ x... });
        })) {}

    static constexpr AABB from_size(vector<T, Dim, O> const& position,
                                    vector<T, Dim, O> const& size) {
        return AABB(position, position + size);
    }

    constexpr vector<T, Dim, O> lower_bound() const { return _origin; };

    constexpr vector<T, Dim, O> upper_bound() const { return _origin + _size; };

    constexpr vector<T, Dim, O> size() const { return _size; };

private:
    vector<T, Dim, O> _origin;
    vector<T, Dim, O> _size;
};

template <typename T, size_t Dim, vector_options O>
std::ostream& operator<<(std::ostream& str, AABB<T, Dim, O> const& aabb) {
    return [](auto& x) -> auto& { return x; }(str)
                              << "AABB{ .lower_bound = " << aabb.lower_bound()
                              << ", .upper_bound = " << aabb.upper_bound()
                              << " }";
}

template <typename... T, size_t Dim, vector_options... O>
constexpr AABB<__vml_promote(T...), Dim, combine(O...)> enclosing(
    AABB<T, Dim, O> const&... aabb) {
    return { aabb.lower_bound()..., aabb.upper_bound()... };
}

template <typename T, size_t Dim, vector_options O>
constexpr bool encloses(AABB<T, Dim, O> const& bigger,
                        AABB<T, Dim, O> const& smaller) {
    return map(bigger.lower_bound(), smaller.lower_bound(),
               [](auto b, auto s) {
        return b - T(0.001) <= s;
    }).fold(__vml_logical_and) &&
           map(bigger.upper_bound(), smaller.upper_bound(), [](auto b, auto s) {
        return b + T(0.001) >= s;
    }).fold(__vml_logical_and);
}

template <typename T = double, vector_options O = vector_options{}>
using rectangle = AABB<T, 2, O>;

template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool operator==(AABB<T, Dim, O> r, AABB<U, Dim, P> s) {
    return r.lower_bound() == s.lower_bound() && r.size() == s.size();
}

template <typename T, size_t Dim, vector_options O>
constexpr T volume(AABB<T, Dim, O> const& b) {
    return fold(b.size(), _VVML::__vml_multiplies);
}

template <typename T, size_t Dim, vector_options O>
constexpr T surface_area(AABB<T, Dim, O> const& b) {
    static_assert(Dim >= 1 && Dim <= 3,
                  "Too lazy to implement higher dimensions");

    auto size = b.size();

    if constexpr (Dim == 1) {
        return size[0];
    }
    else if constexpr (Dim == 2) {
        return 2 * (size[0] + size[1]);
    }
    else if constexpr (Dim == 3) {
        return 2 * (size[0] * size[1] + size[1] * size[2] + size[2] * size[0]);
    }
}

template <typename T, vector_options O>
constexpr T area(rectangle<T, O> const& r) {
    return volume(r);
}

template <typename T, vector_options O>
constexpr T circumference(rectangle<T, O> const& r) {
    return surface_area(r);
}

/// MARK: Sphere
template <typename T = double, size_t Dim = 3,
          vector_options O = vector_options{}.packed(true)>
struct sphere {
    static_assert(std::is_floating_point<T>::value,
                  "T needs to be floating point");
    static_assert(Dim >= 2);
    static_assert(Dim <= 4);

public:
    sphere(vector<T, Dim, O> const& origin, T radius):
        _origin(origin), _radius(radius) {}

    vector<T, Dim, O> origin() const { return _origin; };
    T radius() const { return _radius; };

private:
    vector<T, Dim, O> _origin;
    T _radius;
};

template <typename T = double, vector_options O = vector_options{}.packed(true)>
using disk = sphere<T, 2, O>;

template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool operator==(sphere<T, Dim, O> const& r,
                          sphere<U, Dim, P> const& s) {
    return r.origin() == s.origin() && r.radius() == s.radius();
}

template <typename T, size_t Dim, vector_options O>
constexpr T volume(sphere<T, Dim, O> const& s) {
    if constexpr (Dim == 2) {
        return constants<T>::pi * __vml_sqr(s.radius(), 2);
    }
    else if constexpr (Dim == 3) {
        return T(4.0 / 3.0) * constants<T>::pi * __vml_sqr(s.radius()) *
               s.radius();
    }
    else {
        static_assert(Dim == 4);
        return T(0.5) * __vml_sqr(constants<T>::pi) *
               __vml_sqr(__vml_sqr(s.radius()));
    }
}

template <typename T, vector_options O>
constexpr T area(sphere<T, 2, O> const& r) {
    return volume(r);
}

/// MARK: - Triangle
template <typename T, size_t Dim = 3, vector_options O = vector_options{}>
class triangle {
public:
    vector<T, Dim, O>& operator[](size_t index) {
        return const_cast<vector<T, Dim, O>&>(
            const_cast<triangle const*>(this)->operator[](index));
    }

    vector<T, Dim, O> const& operator[](size_t index) const {
        __vml_bounds_check(index, 0, 3);
        return _points[index];
    }

private:
    vector<T, Dim, O> _points[3];
};

/// MARK: - Line Segment
template <typename T, size_t Dim, vector_options O = vector_options{}>
class line_segment {
public:
    line_segment(vector<T, Dim, O> const& begin, vector<T, Dim, O> const& end):
        _begin(begin), _end(end) {}

    vector<T, Dim, O> begin() const { return _begin; }
    vector<T, Dim, O> end() const { return _end; }

    vector<T, Dim, O> extend() const { return _end - _begin; }
    vector<T, Dim, O> normal() const { return normalize(extend()); }
    auto length() const { return norm(extend()); }

private:
    vector<T, Dim, O> _begin;
    vector<T, Dim, O> _end;
};

template <typename T = double, vector_options O = vector_options{}>
using line_segment_2D = line_segment<T, 2, O>;

template <typename T = double, vector_options O = vector_options{}>
using line_segment_3D = line_segment<T, 3, O>;

template <typename T, typename U, size_t Dim, vector_options O,
          vector_options P>
auto distance(line_segment<T, Dim, O> const& l, vector<U, Dim, P> const& p) {
    // Return minimum distance between line segment l and point p
    using V = __vml_promote(T, U);

    auto const l2 =
        distance_squared(l.begin(), l.end()); // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == V(0)) {
        return distance(p, l.begin()); // v == w case
    }

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t to [0,1] to handle points outside the segment vw.
    auto const t =
        std::max(T(0), std::min(T(1), dot(p - l.begin(), l.extend()) / l2));
    auto const projection =
        l.begin() + t * (l.extend()); // Projection falls on the segment
    return distance(p, projection);
}

template <typename T, typename U, size_t Dim, vector_options O,
          vector_options P>
auto distance(vector<U, Dim, P> const& p, line_segment<T, Dim, O> const& l) {
    return distance(l, p);
}

/// MARK: - Intersections
/// Box - Point
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(AABB<T, Dim, O> r, vector<U, Dim, P> const& p) {
    return map(r.lower_bound(), r.size(), p, [](auto o, auto e, auto p) {
        return p >= o && p <= o + e;
    }).fold(_VVML::__vml_logical_and);
}

/// Point - Box
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(vector<U, Dim, P> const& p, AABB<T, Dim, O> r) {
    return do_intersect(r, p);
}

/// Box - Box
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(AABB<T, Dim, O> const& a,
                            AABB<U, Dim, P> const& b) {
    return map(a.lower_bound(), a.upper_bound(), b.lower_bound(),
               b.upper_bound(), [](auto aMin, auto aMax, auto bMin, auto bMax) {
        return aMin <= bMax && aMax >= bMin;
    }).fold(__vml_logical_and);
}

/// Sphere - Point
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(sphere<T, Dim, O> const& r,
                            vector<U, Dim, P> const& p) {
    return distance(r.origin(), p) <= r.radius();
}

/// Point - Sphere
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(vector<U, Dim, P> const& p,
                            sphere<T, Dim, O> const& r) {
    return do_intersect(r, p);
}

/// Sphere - Sphere
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(sphere<T, Dim, O> const& a,
                            sphere<U, Dim, P> const& b) {
    return distance(a.origin(), b.origin()) <= a.radius() + b.radius();
}

/// Sphere - Box
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(sphere<T, Dim, O> const& sphere,
                            AABB<U, Dim, P> const& box) {
    auto const closest_point_to_sphere =
        map(box.lower_bound(), box.size(), sphere.origin(),
            [](auto bMin, auto bMax, auto sOrigin) {
        return std::max(bMin, std::min(sOrigin, bMax));
    });

    return do_intersect(sphere, closest_point_to_sphere);
}

/// Box - Sphere
template <typename T, size_t Dim, vector_options O, typename U,
          vector_options P>
constexpr bool do_intersect(AABB<U, Dim, P> a, sphere<T, Dim, O> b) {
    return do_intersect(b, a);
}

} // namespace _VVML

#endif // __VML_SHAPES_HPP_INCLUDED__
