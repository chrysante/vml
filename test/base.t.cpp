#include <vml/vml.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
template <typename>
struct _wrap_type {};
template <typename T, typename U>
constexpr bool operator==(_wrap_type<T>, _wrap_type<U>) {
    return std::is_same_v<T, U>;
}
} // namespace

#define _wrap(...)                                                             \
    _wrap_type<__VA_ARGS__> {}
#define _wrap_promote(...) _wrap(std::common_type_t<__VA_ARGS__>)

TEST_CASE("promote") {
    using namespace mtl;
    CHECK(_wrap_promote(int, float) == _wrap(float));
    CHECK(_wrap_promote(float, double) == _wrap(double));
    CHECK(_wrap_promote(char, long double) == _wrap(long double));
    CHECK(_wrap_promote(complex<int>, double) == _wrap(complex<double>));
    CHECK(_wrap_promote(complex<int>, quaternion<long>, double) ==
          _wrap(quaternion<double>));
    CHECK(_wrap_promote(complex<int>, double, quaternion<long>) ==
          _wrap(quaternion<double>));
    CHECK(_wrap_promote(complex<double>, int, quaternion<long>) ==
          _wrap(quaternion<double>));

    CHECK(_wrap_promote(complex<double>, int3) ==
          _wrap(vector3<complex<double>>));
    CHECK(_wrap_promote(complex<short>, int3, double) ==
          _wrap(vector3<complex<double>>));
}
