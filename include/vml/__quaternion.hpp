#ifndef __MTL_QUATERNION_HPP_INCLUDED__
#define __MTL_QUATERNION_HPP_INCLUDED__

#include <cmath>
#include <iosfwd>

#include "__base.hpp"
#include "__common.hpp"
#include "__fwd.hpp"
#include "__vector.hpp"
#include "__complex.hpp"

namespace _VMTL {
	
	inline constexpr vector_options __mtl_quaternion_vector_options = vector_options{}.packed(false);

	/// MARK: - struct quaternion
	template <real_scalar T>
	struct quaternion<T>: public vector<T, 4, __mtl_quaternion_vector_options> {
		static constexpr auto __mtl_options = __mtl_quaternion_vector_options;
		using __mtl_base = vector<T, 4, __mtl_options>;
		using typename __mtl_base::value_type;
		
	public:
		using __vector_data_<T, 4, __mtl_options>::real;
		using __vector_data_<T, 4, __mtl_options>::imag;
		using __vector_data_<T, 4, __mtl_options>::vector;
		
		using __imag_t = _VMTL::vector<T, 3, __mtl_options>;
		
	public:
		/// Value Constructors
		quaternion() = default;
		constexpr quaternion(T const& real): __mtl_base(real, 0, 0, 0) {}
		template <vector_options P = __mtl_quaternion_vector_options>
		constexpr quaternion(T const& real,  _VMTL::vector<T, 3, P> const& imag): __mtl_base(real, imag.__mtl_at(0), imag.__mtl_at(1), imag.__mtl_at(2)) {}
		constexpr quaternion(T const& a, T const& b, T const& c, T const& d): __mtl_base(a, b, c, d) {}
		template <std::convertible_to<T> U>
		constexpr quaternion(complex<U> const& z): quaternion(z.__mtl_at(0), z.__mtl_at(1), 0, 0) {}
		template <vector_options P = __mtl_quaternion_vector_options>
		constexpr quaternion(_VMTL::vector<T, 4, P> const& v): __mtl_base(v) {}
		
		/// Conversion Constructor
		template <std::convertible_to<T> U>
		constexpr quaternion(quaternion<U> const& rhs): __mtl_base(rhs) {}
		
		quaternion& operator=(quaternion const&)& = default;
		
		/// MARK: - Arithmetic Assignment
		constexpr quaternion& operator+=(quaternion const& rhs)& {
			*this = *this + rhs;
			return *this;
		}
		constexpr quaternion& operator+=(T const& rhs)& {
			*this = *this + rhs;
			return *this;
		}
		constexpr quaternion& operator-=(quaternion const& rhs)& {
			*this = *this - rhs;
			return *this;
		}
		constexpr quaternion& operator-=(T const& rhs)& {
			*this = *this - rhs;
			return *this;
		}
		constexpr quaternion& operator*=(quaternion const& rhs)& {
			*this = *this * rhs;
			return *this;
		}
		constexpr quaternion& operator*=(T const& rhs)& {
			*this = *this * rhs;
			return *this;
		}
		constexpr quaternion& operator/=(quaternion const& rhs)& {
			*this = *this / rhs;
			return *this;
		}
		constexpr quaternion& operator/=(T const& rhs)& {
			*this = *this / rhs;
			return *this;
		}
	};
	
	/// MARK: - Misc
	template <real_scalar T>
	quaternion(T) -> quaternion<T>;
	template <real_scalar T, real_scalar U, real_scalar V, real_scalar W>
	quaternion(T, U, V, W) -> quaternion<__mtl_promote(T, U, V, W)>;
	template <real_scalar T, real_scalar U, vector_options O = vector_options{}>
	quaternion(T, _VMTL::vector<U, 3, O>) -> quaternion<__mtl_promote(T, U)>;
	
	/// real, imag:
	template <typename T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr T real(quaternion<T> const& z) { return z.__mtl_at(0); }
	template <typename T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr vector<T, 3, __mtl_quaternion_vector_options> imag(quaternion<T> const& z) {
		return {
			z.__mtl_at(1), z.__mtl_at(2), z.__mtl_at(3)
		};
	}
	
	template <typename T>
	__mtl_mathfunction __mtl_always_inline
	constexpr vector<T, 4, __mtl_quaternion_vector_options>& __mtl_as_vector(quaternion<T>& z) {
		return static_cast<vector<T, 4, quaternion<T>::__mtl_options>&>(z);
	}
	template <typename T>
	__mtl_mathfunction __mtl_always_inline
	constexpr vector<T, 4, __mtl_quaternion_vector_options> const& __mtl_as_vector(quaternion<T> const& z) {
		return static_cast<vector<T, 4, quaternion<T>::__mtl_options> const&>(z);
	}
	
	/// isnan
	template <typename T>
	constexpr bool isnan(quaternion<T> const& z) {
		using std::isnan;
		return isnan(z.__mtl_at(0)) || isnan(z.__mtl_at(1)) || isnan(z.__mtl_at(2)) || isnan(z.__mtl_at(3));
	}
	
	/// isinf
	template <typename T>
	constexpr bool isinf(quaternion<T> const& z) {
		using std::isinf;
		return isinf(z.__mtl_at(0)) || isinf(z.__mtl_at(1)) || isinf(z.__mtl_at(2)) || isinf(z.__mtl_at(3));
	}
	
	/// isfinite
	template <typename T>
	constexpr bool isfinite(quaternion<T> const& z) {
		using std::isfinite;
		return isfinite(z.__mtl_at(0)) || isfinite(z.__mtl_at(1)) || isfinite(z.__mtl_at(2)) || isfinite(z.__mtl_at(3));
	}
	
	/// isnormal
	template <typename T>
	constexpr bool isnormal(quaternion<T> const& z) {
		using std::isnormal;
		return isnormal(z.__mtl_at(0)) || isnormal(z.__mtl_at(1)) || isnormal(z.__mtl_at(2)) || isnormal(z.__mtl_at(3));
	}
	
	/// MARK: - Literals
	inline namespace literals {
		inline namespace quaternion_literals {
			/*
			 _i* literals are defined in complex.hpp and can be used in conjunction with these
			 'using namespace mtl::quaternion_literals;' also pulls in the _i* complex literals
			 */
			inline constexpr quaternion<double>             operator"" _j(long double x) { return { 0, 0, static_cast<double>(x), 0 }; }
			inline constexpr quaternion<float>              operator"" _jf(long double x) { return { 0, 0, static_cast<float>(x), 0 }; }
			inline constexpr quaternion<long double>        operator"" _jld(long double x) { return { 0, 0, static_cast<long double>(x), 0 }; }
			inline constexpr quaternion<int>                operator"" _j(unsigned long long x) { return { 0, 0, static_cast<int>(x), 0 }; }
			inline constexpr quaternion<long>               operator"" _jl(unsigned long long x) { return { 0, 0, static_cast<long>(x), 0 }; }
			inline constexpr quaternion<long long>          operator"" _jll(unsigned long long x) { return { 0, 0, static_cast<long long>(x), 0 }; }
			inline constexpr quaternion<unsigned int>       operator"" _ju(unsigned long long x) { return { 0, 0, static_cast<unsigned int>(x), 0 }; }
			inline constexpr quaternion<unsigned long>      operator"" _jul(unsigned long long x) { return { 0, 0, static_cast<unsigned long>(x), 0 }; }
			inline constexpr quaternion<unsigned long long> operator"" _jull(unsigned long long x) { return { 0, 0, static_cast<unsigned long long>(x), 0 }; }
			inline constexpr quaternion<double>             operator"" _k(long double x) { return { 0, 0, 0, static_cast<double>(x) }; }
			inline constexpr quaternion<float>              operator"" _kf(long double x) { return { 0, 0, 0, static_cast<float>(x) }; }
			inline constexpr quaternion<long double>        operator"" _kld(long double x) { return { 0, 0, 0, static_cast<long double>(x) }; }
			inline constexpr quaternion<int>                operator"" _k(unsigned long long x) { return { 0, 0, 0, static_cast<int>(x) }; }
			inline constexpr quaternion<long>               operator"" _kl(unsigned long long x) { return { 0, 0, 0, static_cast<long>(x) }; }
			inline constexpr quaternion<long long>          operator"" _kll(unsigned long long x) { return { 0, 0, 0, static_cast<long long>(x) }; }
			inline constexpr quaternion<unsigned int>       operator"" _ku(unsigned long long x) { return { 0, 0, 0, static_cast<unsigned int>(x) }; }
			inline constexpr quaternion<unsigned long>      operator"" _kul(unsigned long long x) { return { 0, 0, 0, static_cast<unsigned long>(x) }; }
			inline constexpr quaternion<unsigned long long> operator"" _kull(unsigned long long x) { return { 0, 0, 0, static_cast<unsigned long long>(x) }; }
		}
	}
	
	/// MARK: - Quaternion Arithmetic
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<T> operator+(quaternion<T> const& z) {
		return z;
	}
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<T> operator-(quaternion<T> const& z) {
		return -__mtl_as_vector(z);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator+(quaternion<T> const& a, quaternion<U> const& b) {
		return __mtl_as_vector(a) + __mtl_as_vector(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator+(quaternion<T> const& a, complex<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) + real(b), imag(b) + a.__mtl_at(1), a.__mtl_at(2), a.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator+(quaternion<T> const& a, U const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) + b, a.__mtl_at(1), a.__mtl_at(2), a.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator+(complex<T> const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) + real(b), imag(a) + b.__mtl_at(1), b.__mtl_at(2), b.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator+(T const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(a + real(b), b.__mtl_at(1), b.__mtl_at(2), b.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator-(quaternion<T> const& a, quaternion<U> const& b) {
		return __mtl_as_vector(a) - __mtl_as_vector(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator-(quaternion<T> const& a, complex<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) - real(b), a.__mtl_at(1) - imag(b), a.__mtl_at(2), a.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator-(quaternion<T> const& a, U const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) - b, a.__mtl_at(1), a.__mtl_at(2), a.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator-(complex<T> const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(real(a) - real(b), imag(a) - b.__mtl_at(1), -b.__mtl_at(2), -b.__mtl_at(3));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator-(T const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(a - real(b), -imag(b));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator*(quaternion<T> const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(
			a.__mtl_at(0) * b.__mtl_at(0) - a.__mtl_at(1) * b.__mtl_at(1) - a.__mtl_at(2) * b.__mtl_at(2) - a.__mtl_at(3) * b.__mtl_at(3),  // 1
			a.__mtl_at(0) * b.__mtl_at(1) + a.__mtl_at(1) * b.__mtl_at(0) + a.__mtl_at(2) * b.__mtl_at(3) - a.__mtl_at(3) * b.__mtl_at(2),  // i
			a.__mtl_at(0) * b.__mtl_at(2) - a.__mtl_at(1) * b.__mtl_at(3) + a.__mtl_at(2) * b.__mtl_at(0) + a.__mtl_at(3) * b.__mtl_at(1),  // j
			a.__mtl_at(0) * b.__mtl_at(3) + a.__mtl_at(1) * b.__mtl_at(2) - a.__mtl_at(2) * b.__mtl_at(1) + a.__mtl_at(3) * b.__mtl_at(0)   // k
		);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator*(quaternion<T> const& a, complex<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(
			 a.__mtl_at(0) * b.__mtl_at(0) - a.__mtl_at(1) * b.__mtl_at(1),  // 1
			 a.__mtl_at(0) * b.__mtl_at(1) + a.__mtl_at(1) * b.__mtl_at(0),  // i
			 a.__mtl_at(2) * b.__mtl_at(0) + a.__mtl_at(3) * b.__mtl_at(1),  // j
			-a.__mtl_at(2) * b.__mtl_at(1) + a.__mtl_at(3) * b.__mtl_at(0)   // k
		);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator*(complex<T> const& a, quaternion<U> const& b) {
		return quaternion<__mtl_promote(T, U)>(
			a.__mtl_at(0) * b.__mtl_at(0) - a.__mtl_at(1) * b.__mtl_at(1),  // 1
			a.__mtl_at(0) * b.__mtl_at(1) + a.__mtl_at(1) * b.__mtl_at(0),  // i
			a.__mtl_at(0) * b.__mtl_at(2) - a.__mtl_at(1) * b.__mtl_at(3),  // j
			a.__mtl_at(0) * b.__mtl_at(3) + a.__mtl_at(1) * b.__mtl_at(2)   // k
		);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator*(quaternion<T> const& a, U const& b) {
		return __mtl_as_vector(a) * b;
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator*(T const& a, quaternion<U> const& b) {
		return a * __mtl_as_vector(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator/(quaternion<T> const& a, quaternion<U> const& b) {
		return a * conj(b) / norm_squared(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator/(complex<T> const& a, quaternion<U> const& b) {
		return a * conj(b) / norm_squared(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator/(quaternion<T> const& a, complex<U> const& b) {
		return a * conj(b) / norm_squared(b);
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator/(quaternion<T> const& a, U const& b) {
		return __mtl_as_vector(a) / b;
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_promote(T, U)> operator/(T const& a, quaternion<U> const& b) {
		return a * conj(b) / norm_squared(b);
	}
	
	/// MARK: - operator==
	template <typename T, typename U>
	requires requires(T&& t, U&& u) { { t == u } -> std::convertible_to<bool>; }
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr bool operator==(quaternion<T> const& a, quaternion<U> const& b) {
		return __mtl_as_vector(a) == __mtl_as_vector(b);
	}
	
	template <typename T, typename U>
	requires requires(T&& t, U&& u) { { t == u } -> std::convertible_to<bool>; }
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr bool operator==(quaternion<T> const& a, complex<U> const& b) {
		return real(a) == real(b) && a.__mtl_at(1) == imag(b) && a.__mtl_at(2) == 0 && a.__mtl_at(3) == 0;
	}
	
	template <typename T, real_scalar U>
	requires requires(T&& t, U&& u) { { t == u } -> std::convertible_to<bool>; }
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr bool operator==(quaternion<T> const& a, U const& b) {
		return real(a) == b && a.__mtl_at(1) == 0 && a.__mtl_at(2) == 0 && a.__mtl_at(3) == 0;
	}
	
	template <typename T, typename U>
	requires requires(T&& t, U&& u) { { t == u } -> std::convertible_to<bool>; }
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr bool operator==(complex<T> const& a, quaternion<U> const& b) {
		return real(a) == real(b) && imag(a) == b.__mtl_at(1) && 0 == b.__mtl_at(2) && 0 == b.__mtl_at(3);
	}
	
	template <real_scalar T, typename U>
	requires requires(T&& t, U&& u) { { t == u } -> std::convertible_to<bool>; }
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr bool operator==(T const& a, quaternion<U> const& b) {
		return a == real(b) && 0 == b.__mtl_at(1) && 0 == b.__mtl_at(2) && 0 == b.__mtl_at(3);
	}

	
	/// MARK: - Quaternion Math Functions
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<T> conj(quaternion<T> const& z) {
		return { z.__mtl_at(0), -z.__mtl_at(1), -z.__mtl_at(2), -z.__mtl_at(3) };
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<T> inverse(quaternion<T> const& z) {
		return conj(z) / length_squared(z);
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<T> normalize(quaternion<T> const& z) {
		return normalize(__mtl_as_vector(z));
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr __mtl_floatify(T) abs(quaternion<T> const& z) {
		return norm(z);
	}
	
	/// MARK: Power Functions
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	quaternion<__mtl_floatify(T)> exp(quaternion<T> const& z) {
		auto const r_exp = std::exp(real(z));
		if (z.vector == 0) {
			return r_exp;
		}
		auto const _vnorm = norm(z.vector);
		return r_exp * (std::cos(_vnorm) + quaternion{ 0, z.imag } * (std::sin(_vnorm) / _vnorm));
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	quaternion<__mtl_floatify(T)> log(quaternion<T> const& z) {
		__mtl_expect(z != 0);
		auto const _norm = _VMTL::norm(z);
		if (z.vector == 0) {
			return { std::log(_norm), 0, 0, 0 };
		}
		auto const _vnorm = norm(z.vector);
		return { std::log(_norm), z.vector * std::acos(z.real / _norm) / _vnorm };
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	quaternion<__mtl_floatify(T)> log10(quaternion<T> const& z) {
		return _VMTL::log(z) / std::log(T(10));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> pow(quaternion<T> const& x, quaternion<U> const& y) {
		return _VMTL::exp(y * _VMTL::log(x));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> pow(complex<T> const& x, quaternion<U> const& y) {
		return _VMTL::exp(y * _VMTL::log(x));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> pow(T const& x, quaternion<U> const& y) {
		return _VMTL::exp(y * std::log(x));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> pow(quaternion<T> const& x, complex<U> const& y) {
		return _VMTL::exp(y * _VMTL::log(x));
	}
	
	template <real_scalar T, real_scalar U>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> pow(quaternion<T> const& x, U const& y) {
		return _VMTL::exp(y * _VMTL::log(x));
	}
	
	template <real_scalar T>
	__mtl_mathfunction __mtl_always_inline __mtl_interface_export
	quaternion<__mtl_floatify(T)> sqrt(quaternion<T> const& z) {
		__mtl_expect(real(z) >= 0 || imag(z) != 0);
		auto const r = _VMTL::abs(z);
		return std::sqrt(r) * (z + r) / _VMTL::abs(z + r);
	}
	
	/// MARK: Hyperbolic Functions
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> sinh(quaternion<T> const& _z) {
		using F = __mtl_floatify(T);
		quaternion<F> z = _z;
		auto const v_norm = _VMTL::norm(z.vector);
		__mtl_safe_math_if (std::isinf(real(z)) && !std::isinf(v_norm))
			return complex<F>(real(z), F(NAN));
		__mtl_safe_math_if (real(z) == 0 && !std::isinf(v_norm))
			return complex<F>(real(z), F(NAN));
		__mtl_safe_math_if (v_norm == 0 && !std::isinf(real(z)))
			return z;
		return {
			std::sinh(real(z)) * std::cos(v_norm),
			std::cosh(real(z)) * std::sin(v_norm) / v_norm * z.vector
		};
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> cosh(quaternion<T> const& _z) {
		using F = __mtl_floatify(T);
		quaternion<F> z = _z;
		auto const v_norm = _VMTL::norm(z.vector);
		__mtl_safe_math_if (std::isinf(real(z)) && !std::isinf(v_norm))
			return { std::abs(real(z)), F(NAN), F(NAN), F(NAN) };
		__mtl_safe_math_if (real(z) == 0 && !std::isinf(v_norm))
			return { F(NAN), 0, 0, 0 };
		__mtl_safe_math_if (real(z) == 0 && v_norm == 0)
			return { F(1), 0, 0, 0 };
		__mtl_safe_math_if (v_norm == 0 && !std::isinf(real(z)))
			return { std::abs(real(z)), 0, 0, 0 };
		return {
			std::cosh(real(z)) * std::cos(v_norm),
			std::sinh(real(z)) * std::sin(v_norm) / v_norm * z.vector
		};
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> tanh(quaternion<T> const& z) {
		return _VMTL::sinh(z) / _VMTL::cosh(z);
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> asinh(quaternion<T> const& z);
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> acosh(quaternion<T> const& z);
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> atanh(quaternion<T> const& z);
	
	/// MARK: Trigonometric Functions
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> sin(quaternion<T> const& z) {
		auto const v_norm = norm(z.vector);
		return {
			std::sin(real(z)) * std::cosh(v_norm),
			std::cos(real(z)) * std::sinh(v_norm) / v_norm * z.vector
		};
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> cos(quaternion<T> const& z) {
		auto const v_norm = norm(z.vector);
		return {
			 std::cos(real(z)) * std::cosh(v_norm),
			-std::sin(real(z)) * std::sinh(v_norm) / v_norm * z.vector
		};
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> tan(quaternion<T> const& z) {
		return sin(z) / cos(z);
	}
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> asin(quaternion<T> const& z);
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> acos(quaternion<T> const& z);
	
	template <real_scalar T>
	quaternion<__mtl_floatify(T)> atan(quaternion<T> const& z);
	
	/// MARK: Helpers
	template <real_scalar T, typename U = T, vector_options O = vector_options{}>
	constexpr quaternion<__mtl_floatify(__mtl_promote(T, U))> make_rotation(T angle, vector3<U, O> const& axis) {
		using V = __mtl_floatify(__mtl_promote(T, U));
		return quaternion<V>(std::cos(angle / 2), std::sin(angle / 2) * normalize(axis));
	}
	
	template <real_scalar T = double, typename U = T, vector_options O = vector_options{}>
	constexpr vector3<__mtl_floatify(__mtl_promote(T, U)), O> rotate(vector3<T, O> const& v, quaternion<U> const& q) {
		using V = __mtl_floatify(__mtl_promote(T, U));
		return imag(q * quaternion<V>(0, v) * conj(q));
	}
	
}

/// MARK: - Decomposition
namespace _VMTL {

	template <std::size_t I, typename T> requires (I < 2)
	auto const& get(quaternion<T> const& v) {
		if constexpr (I == 0) {
			return v.__mtl_at(0);
		}
		else {
			return v.imag;
		}
	}
	template <std::size_t I, typename T> requires (I < 2)
	auto& get(quaternion<T>& v) {
		if constexpr (I == 0) {
			return v.__mtl_at(0);
		}
		else {
			return v.imag;
		}
	}
	template <std::size_t I, typename T>
	auto&& get(quaternion<T>&& v) {
		return std::move(get<I>(v));
	}
	template <std::size_t I, typename T>
	auto&& get(quaternion<T> const&& v) {
		return std::move(get<I>(v));
	}
	
}

template <typename T>
struct std::tuple_size<_VMTL::quaternion<T>>: std::integral_constant<std::size_t, 2> {};

template <typename T>
struct std::tuple_element<0, _VMTL::quaternion<T>> {
	using type = T;
};
template <typename T>
struct std::tuple_element<1, _VMTL::quaternion<T>> {
	using type = typename _VMTL::quaternion<T>::__imag_t;
};

#endif // __MTL_QUATERNION_HPP_INCLUDED__
