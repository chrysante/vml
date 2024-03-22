# Yet Another Linear Algebra Library

Vector math library for small dimensional vector intended for use in computer graphics

## Primary Templates

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
`int4`, `short3x3`, `ulong3x4` etc. Specifically there are typedefs for
dimensions 2-4 and all builtin element types (float, double, long double 
and integer types). 

The `vector` and `matrix` templates can be instantiated for any type or dimension.

`vector` and `matrix` are intentionally separate templates (instead of `vector` 
being a `matrix` with `Rows == 1` or `Columns == 1`) to avoid the confusion and 
headache of keeping track if vectors are row vectors or column vectors. This 
library makes no distinction between the two. 

## Mathematical Operations

All common mathematical operations are defined for each combination of types. 

## Functional Interface

On top of that there are the higher order functions `map` and `fold`.

### Map

`map` allows you to transform any vector or matrix into another vector or matrix
of the same dimension:
 
    vml::float3 v = { 1, 2, 3 };
    
    // Transform v into a vector of complex numbers 
    // w is of type vector<complex<float>, 3>
    auto w = vml::map(v, [](float elem) { return vml::complex(0.0f, elem); });
    
    // map can also be used with multiple inputs
    auto s = vml::map(v, vml::bool3{ false, true, false }, 
                      [](float elem, bool pass) { return pass ? elem : 0.0f; });
    assert(s == vml::float3(0.0, 2.0, 0.0);
    
    // map is also exposed as a member function
    auto t = v.map([](float elem) { return 2.0f * elem; });
       
### Fold

`fold` allows you to reduce a vector to its element type:

    assert(vml::int3(1, 2, 3).fold(std::plus{}) == 6);       

## Matrix Factories

There are factory functions for transform, view and projection matrices, color 
conversion functions and more. Read the (undocumented source) to find out more ;-)
