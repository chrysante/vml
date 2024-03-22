#ifndef __MTL_BASE_HPP_INCLUDED__
#define __MTL_BASE_HPP_INCLUDED__

// MARK: Check Compiler Compatibility

#if !(__clang__ || __GNUC__ || _MSC_VER)
#error Unsupported Compiler
#endif

// MARK: Customization Points

#ifndef MTL_DEBUG_LEVEL
#ifdef NDEBUG
#define MTL_DEBUG_LEVEL 0
#else
#define MTL_DEBUG_LEVEL 1
#endif
#endif

#ifndef MTL_SAFE_MATH
#define MTL_SAFE_MATH 1
#endif

#ifndef MTL_DEFAULT_PACKED
#define MTL_DEFAULT_PACKED 0
#endif

#ifndef MTL_AVX
#define MTL_AVX 0
#endif

#ifndef MTL_NAMESPACE_NAME
#define MTL_NAMESPACE_NAME mtl
#endif

#ifndef MTL_UNICODE_MATH_PARANTHESES
#if defined(__APPLE__)
#define MTL_UNICODE_MATH_PARANTHESES 1
#else
#define MTL_UNICODE_MATH_PARANTHESES 0
#endif
#endif

/// MARK: - Attributes

#if defined(__GNUC__) || defined(__clang__)

#define __mtl_pure         __attribute__((const))
#define __mtl_nodiscard    [[nodiscard]]
#define __mtl_mathfunction __mtl_nodiscard
#if MTL_DEBUG_LEVEL > 1
#define __mtl_always_inline
#else // MTL_DEBUG_LEVEL > 1
#define __mtl_always_inline __attribute__((always_inline))
#endif // MTL_DEBUG_LEVEL > 1

#define __mtl_noinline __attribute__((noinline))
#if MTL_DEBUG_LEVEL > 1
#define __mtl_interface_export
#else // MTL_DEBUG_LEVEL > 1
#define __mtl_interface_export __attribute__((nodebug))
#endif // MTL_DEBUG_LEVEL > 1

#else

#define __mtl_pure
#define __mtl_nodiscard
#define __mtl_mathfunction
#if MTL_DEBUG_LEVEL > 1
#define __mtl_always_inline
#else // MTL_DEBUG_LEVEL > 1
#define __mtl_always_inline
#endif // MTL_DEBUG_LEVEL > 1

#define __mtl_noinline
#if MTL_DEBUG_LEVEL > 1
#define __mtl_interface_export
#else // MTL_DEBUG_LEVEL > 1
#define __mtl_interface_export
#endif // MTL_DEBUG_LEVEL > 1

#endif

/// MARK: - Debug

#if MTL_DEBUG_LEVEL > 0
#define __mtl_assert(COND)                                                     \
    (!(COND) ? (__mtl_debugbreak("Assertion Failed"), (void)0) : (void)0)
#else // MTL_DEBUG_LEVEL > 0
#define __mtl_assert(COND) (void)0
#endif // MTL_DEBUG_LEVEL > 0

#if MTL_DEBUG_LEVEL > 1
#define __mtl_assert_audit(COND) __mtl_assert(COND)
#else // MTL_DEBUG_LEVEL > 1
#define __mtl_assert_audit(COND) (void)0
#endif // MTL_DEBUG_LEVEL > 1

#define __mtl_expect(COND) __mtl_assert(COND)
#define __mtl_ensure(COND) __mtl_assert(COND)

#if defined(__clang__)
#define __mtl_debugbreak(msg) __builtin_debugtrap()
#elif defined(__GNUC__)
#define __mtl_debugbreak(msg) __builtin_trap()
#elif defined(_MSC_VER)
#define __mtl_debugbreak(msg) __debugbreak()
#else
#error
#endif

#define __mtl_bounds_check(index, lower, upper)                                \
    (__mtl_expect(lower <= index), __mtl_expect(index < upper))

/// MARK: Namespace

#ifdef _VMTL
#error We need this macro name _VMTL
#else
#define _VMTL MTL_NAMESPACE_NAME
#endif

/// MARK: Safe Arithmetic

#if MTL_SAFE_MATH
#define __mtl_safe_math_if(...) if (__VA_ARGS__)
#else
#define __mtl_safe_math_if(...) if constexpr ((0))
#endif

#endif // __MTL_BASE_HPP_INCLUDED__
