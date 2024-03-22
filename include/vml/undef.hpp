/// Undefines all macros defined by the library.
/// This file is included by "core.hpp" and "vml.hpp"
///
/// We have no include guard so including this file can act as a command to
/// undefine all macros

#ifndef __VML_NO_UNDEF__

#undef VML_DEBUG_LEVEL
#undef VML_SAFE_MATH
#undef VML_DEFAULT_PACKED
#undef VML_NAMESPACE_NAME
#undef VML_UNICODE_MATH_PARANTHESES

#undef __vml_pure
#undef __vml_nodiscard
#undef __vml_mathfunction
#undef __vml_always_inline
#undef __vml_noinline
#undef __vml_interface_export

#undef __vml_assert
#undef __vml_expect
#undef __vml_ensure
#undef __vml_debugbreak
#undef __vml_bounds_check

#undef _VVML

#undef __vml_safe_math_if

#undef __VML_DECLARE_STDINT_TYPEDEFS__
#undef __VML_DECLARE_COMPLEX_TYPEDEFS__
#undef __VML_DECLARE_QUATERNION_TYPEDEFS__
#undef __VML_DECLARE_VECTOR_TYPEDEFS__
#undef __VML_DECLARE_MATRIX_TYPEDEFS__

#undef __vml_floatify

#undef __vml_forward

#undef __VML_PRIV_WIS_FT
#undef __VML_PRIV_WIS_FP
#undef __VML_PRIV_WIS_FI
#undef __vml_with_index_sequence

#undef VML_STDLIB_HAS_CONCEPTS

#endif // __VML_NO_UNDEF__
