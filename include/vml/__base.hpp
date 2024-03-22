#ifndef __VML_BASE_HPP_INCLUDED__
#define __VML_BASE_HPP_INCLUDED__

// MARK: Check Compiler Compatibility

#if !(__clang__ || __GNUC__ || _MSC_VER)
#error Unsupported Compiler
#endif

// MARK: Customization Points

#ifndef VML_DEBUG_LEVEL
#ifdef NDEBUG
#define VML_DEBUG_LEVEL 0
#else
#define VML_DEBUG_LEVEL 1
#endif
#endif

#ifndef VML_SAFE_MATH
#define VML_SAFE_MATH 1
#endif

#ifndef VML_DEFAULT_PACKED
#define VML_DEFAULT_PACKED 0
#endif

#ifndef VML_AVX
#define VML_AVX 0
#endif

#ifndef VML_NAMESPACE_NAME
#define VML_NAMESPACE_NAME vml
#endif

#ifndef VML_UNICODE_MATH_PARANTHESES
#if defined(__APPLE__)
#define VML_UNICODE_MATH_PARANTHESES 1
#else
#define VML_UNICODE_MATH_PARANTHESES 0
#endif
#endif

/// MARK: - Attributes

#if defined(__GNUC__) || defined(__clang__)

#define __vml_pure         __attribute__((const))
#define __vml_nodiscard    [[nodiscard]]
#define __vml_mathfunction __vml_nodiscard
#if VML_DEBUG_LEVEL > 1
#define __vml_always_inline
#else // VML_DEBUG_LEVEL > 1
#define __vml_always_inline __attribute__((always_inline))
#endif // VML_DEBUG_LEVEL > 1

#define __vml_noinline __attribute__((noinline))
#if VML_DEBUG_LEVEL > 1
#define __vml_interface_export
#else // VML_DEBUG_LEVEL > 1
#define __vml_interface_export __attribute__((nodebug))
#endif // VML_DEBUG_LEVEL > 1

#else

#define __vml_pure
#define __vml_nodiscard
#define __vml_mathfunction
#if VML_DEBUG_LEVEL > 1
#define __vml_always_inline
#else // VML_DEBUG_LEVEL > 1
#define __vml_always_inline
#endif // VML_DEBUG_LEVEL > 1

#define __vml_noinline
#if VML_DEBUG_LEVEL > 1
#define __vml_interface_export
#else // VML_DEBUG_LEVEL > 1
#define __vml_interface_export
#endif // VML_DEBUG_LEVEL > 1

#endif

/// MARK: - Debug

#if VML_DEBUG_LEVEL > 0
#define __vml_assert(COND)                                                     \
    (!(COND) ? (__vml_debugbreak("Assertion Failed"), (void)0) : (void)0)
#else // VML_DEBUG_LEVEL > 0
#define __vml_assert(COND) (void)0
#endif // VML_DEBUG_LEVEL > 0

#if VML_DEBUG_LEVEL > 1
#define __vml_assert_audit(COND) __vml_assert(COND)
#else // VML_DEBUG_LEVEL > 1
#define __vml_assert_audit(COND) (void)0
#endif // VML_DEBUG_LEVEL > 1

#define __vml_expect(COND) __vml_assert(COND)
#define __vml_ensure(COND) __vml_assert(COND)

#if defined(__clang__)
#define __vml_debugbreak(msg) __builtin_debugtrap()
#elif defined(__GNUC__)
#define __vml_debugbreak(msg) __builtin_trap()
#elif defined(_MSC_VER)
#define __vml_debugbreak(msg) __debugbreak()
#else
#error
#endif

#define __vml_bounds_check(index, lower, upper)                                \
    (__vml_expect(lower <= index), __vml_expect(index < upper))

/// MARK: Namespace

#ifdef _VVML
#error We need this macro name _VVML
#else
#define _VVML VML_NAMESPACE_NAME
#endif

/// MARK: Safe Arithmetic

#if VML_SAFE_MATH
#define __vml_safe_math_if(...) if (__VA_ARGS__)
#else
#define __vml_safe_math_if(...) if constexpr ((0))
#endif

#endif // __VML_BASE_HPP_INCLUDED__
