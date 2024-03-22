# Yet another linear algebra library

Vector math library for small dimensional vector intended for use in computer graphics

## Vectors

The primary class template of this library are 

    template <typename T, size_t Size, vector_options Options = {}>
    struct vector;

    template <typename T, size_t Rows, size_t , vector_options Options = {}>
    struct matrix;
    
    template <typename T>
    struct complex;

    template <typename T>
    struct quaternion;

Many typedefs for common use cases are defined, like `float2`, `double3`, 
`int4`, `short3x3`, `ulong3x4` etc. for all combinations of element types and 
dimensions.

There are factory functions for transform, view and projection matrices, color 
conversion functions and more. Read the (undocumented source) to find out more ;-)
