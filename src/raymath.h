/**********************************************************************************************
*
*   raymath v2.0 - Math functions to work with RaylibVector2, RaylibVector3, RaylibMatrix and Quaternions
*
*   CONVENTIONS:
*     - RaylibMatrix structure is defined as row-major (memory layout) but parameters naming AND all
*       math operations performed by the library consider the structure as it was column-major
*       It is like transposed versions of the matrices are used for all the maths
*       It benefits some functions making them cache-friendly and also avoids matrix
*       transpositions sometimes required by OpenGL
*       Example: In memory order, row0 is [m0 m4 m8 m12] but in semantic math row0 is [m0 m1 m2 m3]
*     - Functions are always self-contained, no function use another raymath function inside,
*       required code is directly re-implemented inside
*     - Functions input parameters are always received by value (2 unavoidable exceptions)
*     - Functions use always a "result" variable for return (except C++ operators)
*     - Functions are always defined inline
*     - Angles are always in radians (RAYLIB_DEG2RAD/RAYLIB_RAD2DEG macros provided for convenience)
*     - No compound literals used to make sure libray is compatible with C++
*
*   CONFIGURATION:
*       #define RAYMATH_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RAYMATH_STATIC_INLINE
*           Define static inline functions code, so #include header suffices for use.
*           This may use up lots of memory.
*
*       #define RAYMATH_DISABLE_CPP_OPERATORS
*           Disables C++ operator overloads for raymath types.
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYLIB_RAYMATH_H
#define RAYLIB_RAYMATH_H

#if defined(RAYMATH_IMPLEMENTATION) && defined(RAYMATH_STATIC_INLINE)
    #error "Specifying both RAYMATH_IMPLEMENTATION and RAYMATH_STATIC_INLINE is contradictory"
#endif

// Function specifiers definition
#if defined(RAYMATH_IMPLEMENTATION)
    #if defined(_WIN32) && defined(BUILD_LIBTYPE_SHARED)
        #define RAYLIB_RMAPI __declspec(dllexport) extern inline // We are building raylib as a Win32 shared library (.dll)
    #elif defined(BUILD_LIBTYPE_SHARED)
        #define RAYLIB_RMAPI __attribute__((visibility("default"))) // We are building raylib as a Unix shared library (.so/.dylib)
    #elif defined(_WIN32) && defined(USE_LIBTYPE_SHARED)
        #define RAYLIB_RMAPI __declspec(dllimport)         // We are using raylib as a Win32 shared library (.dll)
    #else
        #define RAYLIB_RMAPI extern inline // Provide external definition
    #endif
#elif defined(RAYMATH_STATIC_INLINE)
    #define RAYLIB_RMAPI static inline // Functions may be inlined, no external out-of-line definition
#else
    #if defined(__TINYC__)
        #define RAYLIB_RMAPI static inline // plain inline not supported by tinycc (See issue #435)
    #else
        #define RAYLIB_RMAPI inline        // Functions may be inlined or external definition used
    #endif
#endif


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef RAYLIB_PI
    #define RAYLIB_PI 3.14159265358979323846f
#endif

#ifndef RAYLIB_EPSILON
    #define RAYLIB_EPSILON 0.000001f
#endif

#ifndef RAYLIB_DEG2RAD
    #define RAYLIB_DEG2RAD (RAYLIB_PI/180.0f)
#endif

#ifndef RAYLIB_RAD2DEG
    #define RAYLIB_RAD2DEG (180.0f/RAYLIB_PI)
#endif

// Get float vector for RaylibMatrix
#ifndef RAYLIB_MatrixToFloat
    #define RAYLIB_MatrixToFloat(mat) (RaylibMatrixToFloatV(mat).v)
#endif

// Get float vector for RaylibVector3
#ifndef RAYLIB_Vector3ToFloat
    #define RAYLIB_Vector3ToFloat(vec) (RaylibVector3ToFloatV(vec).v)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
#if !defined(RL_VECTOR2_TYPE)
// RaylibVector2 type
typedef struct RaylibVector2 {
    float x;
    float y;
} RaylibVector2;
#define RL_VECTOR2_TYPE
#endif

#if !defined(RL_VECTOR3_TYPE)
// RaylibVector3 type
typedef struct RaylibVector3 {
    float x;
    float y;
    float z;
} RaylibVector3;
#define RL_VECTOR3_TYPE
#endif

#if !defined(RL_VECTOR4_TYPE)
// RaylibVector4 type
typedef struct RaylibVector4 {
    float x;
    float y;
    float z;
    float w;
} RaylibVector4;
#define RL_VECTOR4_TYPE
#endif

#if !defined(RL_QUATERNION_TYPE)
// Quaternion type
typedef RaylibVector4 Quaternion;
#define RL_QUATERNION_TYPE
#endif

#if !defined(RL_MATRIX_TYPE)
// RaylibMatrix type (OpenGL style 4x4 - right handed, column major)
typedef struct RaylibMatrix {
    float m0, m4, m8, m12;      // RaylibMatrix first row (4 components)
    float m1, m5, m9, m13;      // RaylibMatrix second row (4 components)
    float m2, m6, m10, m14;     // RaylibMatrix third row (4 components)
    float m3, m7, m11, m15;     // RaylibMatrix fourth row (4 components)
} RaylibMatrix;
#define RL_MATRIX_TYPE
#endif

// NOTE: Helper types to be used instead of array return types for *ToFloat functions
typedef struct Raylibfloat3 {
    float v[3];
} Raylibfloat3;

typedef struct Raylibfloat16 {
    float v[16];
} Raylibfloat16;

#include <math.h>       // Required for: sinf(), cosf(), tan(), atan2f(), sqrtf(), floor(), fminf(), fmaxf(), fabsf()

//----------------------------------------------------------------------------------
// Module Functions Definition - Utils math
//----------------------------------------------------------------------------------

// RaylibClamp float value
RAYLIB_RMAPI float RaylibClamp(float value, float min, float max)
{
    float result = (value < min)? min : value;

    if (result > max) result = max;

    return result;
}

// Calculate linear interpolation between two floats
RAYLIB_RMAPI float RaylibLerp(float start, float end, float amount)
{
    float result = start + amount*(end - start);

    return result;
}

// RaylibNormalize input value within input range
RAYLIB_RMAPI float RaylibNormalize(float value, float start, float end)
{
    float result = (value - start)/(end - start);

    return result;
}

// RaylibRemap input value within input range to output range
RAYLIB_RMAPI float RaylibRemap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
    float result = (value - inputStart)/(inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;

    return result;
}

// RaylibWrap input value from min to max
RAYLIB_RMAPI float RaylibWrap(float value, float min, float max)
{
    float result = value - (max - min)*floorf((value - min)/(max - min));

    return result;
}

// Check whether two given floats are almost equal
RAYLIB_RMAPI int RaylibFloatEquals(float x, float y)
{
#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    int result = (fabsf(x - y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - RaylibVector2 math
//----------------------------------------------------------------------------------

// Vector with components value 0.0f
RAYLIB_RMAPI RaylibVector2 RaylibVector2Zero(void)
{
    RaylibVector2 result = { 0.0f, 0.0f };

    return result;
}

// Vector with components value 1.0f
RAYLIB_RMAPI RaylibVector2 RaylibVector2One(void)
{
    RaylibVector2 result = { 1.0f, 1.0f };

    return result;
}

// Add two vectors (v1 + v2)
RAYLIB_RMAPI RaylibVector2 RaylibVector2Add(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { v1.x + v2.x, v1.y + v2.y };

    return result;
}

// Add vector and float value
RAYLIB_RMAPI RaylibVector2 RaylibVector2AddValue(RaylibVector2 v, float add)
{
    RaylibVector2 result = { v.x + add, v.y + add };

    return result;
}

// Subtract two vectors (v1 - v2)
RAYLIB_RMAPI RaylibVector2 RaylibVector2Subtract(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { v1.x - v2.x, v1.y - v2.y };

    return result;
}

// Subtract vector by float value
RAYLIB_RMAPI RaylibVector2 RaylibVector2SubtractValue(RaylibVector2 v, float sub)
{
    RaylibVector2 result = { v.x - sub, v.y - sub };

    return result;
}

// Calculate vector length
RAYLIB_RMAPI float RaylibVector2Length(RaylibVector2 v)
{
    float result = sqrtf((v.x*v.x) + (v.y*v.y));

    return result;
}

// Calculate vector square length
RAYLIB_RMAPI float RaylibVector2LengthSqr(RaylibVector2 v)
{
    float result = (v.x*v.x) + (v.y*v.y);

    return result;
}

// Calculate two vectors dot product
RAYLIB_RMAPI float RaylibVector2DotProduct(RaylibVector2 v1, RaylibVector2 v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y);

    return result;
}

// Calculate distance between two vectors
RAYLIB_RMAPI float RaylibVector2Distance(RaylibVector2 v1, RaylibVector2 v2)
{
    float result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));

    return result;
}

// Calculate square distance between two vectors
RAYLIB_RMAPI float RaylibVector2DistanceSqr(RaylibVector2 v1, RaylibVector2 v2)
{
    float result = ((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));

    return result;
}

// Calculate angle between two vectors
// NOTE: Angle is calculated from origin point (0, 0)
RAYLIB_RMAPI float RaylibVector2Angle(RaylibVector2 v1, RaylibVector2 v2)
{
    float result = 0.0f;

    float dot = v1.x*v2.x + v1.y*v2.y;
    float det = v1.x*v2.y - v1.y*v2.x;

    result = atan2f(det, dot);

    return result;
}

// Calculate angle defined by a two vectors line
// NOTE: Parameters need to be normalized
// Current implementation should be aligned with glm::angle
RAYLIB_RMAPI float RaylibVector2LineAngle(RaylibVector2 start, RaylibVector2 end)
{
    float result = 0.0f;

    // TODO(10/9/2023): Currently angles move clockwise, determine if this is wanted behavior
    result = -atan2f(end.y - start.y, end.x - start.x);

    return result;
}

// Scale vector (multiply by value)
RAYLIB_RMAPI RaylibVector2 RaylibVector2Scale(RaylibVector2 v, float scale)
{
    RaylibVector2 result = { v.x*scale, v.y*scale };

    return result;
}

// Multiply vector by vector
RAYLIB_RMAPI RaylibVector2 RaylibVector2Multiply(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { v1.x*v2.x, v1.y*v2.y };

    return result;
}

// Negate vector
RAYLIB_RMAPI RaylibVector2 RaylibVector2Negate(RaylibVector2 v)
{
    RaylibVector2 result = { -v.x, -v.y };

    return result;
}

// Divide vector by vector
RAYLIB_RMAPI RaylibVector2 RaylibVector2Divide(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { v1.x/v2.x, v1.y/v2.y };

    return result;
}

// RaylibNormalize provided vector
RAYLIB_RMAPI RaylibVector2 RaylibVector2Normalize(RaylibVector2 v)
{
    RaylibVector2 result = { 0 };
    float length = sqrtf((v.x*v.x) + (v.y*v.y));

    if (length > 0)
    {
        float ilength = 1.0f/length;
        result.x = v.x*ilength;
        result.y = v.y*ilength;
    }

    return result;
}

// Transforms a RaylibVector2 by a given RaylibMatrix
RAYLIB_RMAPI RaylibVector2 RaylibVector2Transform(RaylibVector2 v, RaylibMatrix mat)
{
    RaylibVector2 result = { 0 };

    float x = v.x;
    float y = v.y;
    float z = 0;

    result.x = mat.m0*x + mat.m4*y + mat.m8*z + mat.m12;
    result.y = mat.m1*x + mat.m5*y + mat.m9*z + mat.m13;

    return result;
}

// Calculate linear interpolation between two vectors
RAYLIB_RMAPI RaylibVector2 RaylibVector2Lerp(RaylibVector2 v1, RaylibVector2 v2, float amount)
{
    RaylibVector2 result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);

    return result;
}

// Calculate reflected vector to normal
RAYLIB_RMAPI RaylibVector2 RaylibVector2Reflect(RaylibVector2 v, RaylibVector2 normal)
{
    RaylibVector2 result = { 0 };

    float dotProduct = (v.x*normal.x + v.y*normal.y); // Dot product

    result.x = v.x - (2.0f*normal.x)*dotProduct;
    result.y = v.y - (2.0f*normal.y)*dotProduct;

    return result;
}

// Get min value for each pair of components
RAYLIB_RMAPI RaylibVector2 RaylibVector2Min(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);

    return result;
}

// Get max value for each pair of components
RAYLIB_RMAPI RaylibVector2 RaylibVector2Max(RaylibVector2 v1, RaylibVector2 v2)
{
    RaylibVector2 result = { 0 };

    result.x = fmaxf(v1.x, v2.x);
    result.y = fmaxf(v1.y, v2.y);

    return result;
}

// Rotate vector by angle
RAYLIB_RMAPI RaylibVector2 RaylibVector2Rotate(RaylibVector2 v, float angle)
{
    RaylibVector2 result = { 0 };

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x*cosres - v.y*sinres;
    result.y = v.x*sinres + v.y*cosres;

    return result;
}

// Move Vector towards target
RAYLIB_RMAPI RaylibVector2 RaylibVector2MoveTowards(RaylibVector2 v, RaylibVector2 target, float maxDistance)
{
    RaylibVector2 result = { 0 };

    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float value = (dx*dx) + (dy*dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance))) return target;

    float dist = sqrtf(value);

    result.x = v.x + dx/dist*maxDistance;
    result.y = v.y + dy/dist*maxDistance;

    return result;
}

// Invert the given vector
RAYLIB_RMAPI RaylibVector2 RaylibVector2Invert(RaylibVector2 v)
{
    RaylibVector2 result = { 1.0f/v.x, 1.0f/v.y };

    return result;
}

// RaylibClamp the components of the vector between
// min and max values specified by the given vectors
RAYLIB_RMAPI RaylibVector2 RaylibVector2Clamp(RaylibVector2 v, RaylibVector2 min, RaylibVector2 max)
{
    RaylibVector2 result = { 0 };

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));

    return result;
}

// RaylibClamp the magnitude of the vector between two min and max values
RAYLIB_RMAPI RaylibVector2 RaylibVector2ClampValue(RaylibVector2 v, float min, float max)
{
    RaylibVector2 result = v;

    float length = (v.x*v.x) + (v.y*v.y);
    if (length > 0.0f)
    {
        length = sqrtf(length);

        float scale = 1;    // By default, 1 as the neutral element.
        if (length < min)
        {
            scale = min/length;
        }
        else if (length > max)
        {
            scale = max/length;
        }

        result.x = v.x*scale;
        result.y = v.y*scale;
    }

    return result;
}

// Check whether two given vectors are almost equal
RAYLIB_RMAPI int RaylibVector2Equals(RaylibVector2 p, RaylibVector2 q)
{
#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    int result = ((fabsf(p.x - q.x)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                  ((fabsf(p.y - q.y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y)))));

    return result;
}

// Compute the direction of a refracted ray
// v: normalized direction of the incoming ray
// n: normalized normal vector of the interface of two optical media
// r: ratio of the refractive index of the medium from where the ray comes
//    to the refractive index of the medium on the other side of the surface
RAYLIB_RMAPI RaylibVector2 RaylibVector2Refract(RaylibVector2 v, RaylibVector2 n, float r)
{
    RaylibVector2 result = { 0 };

    float dot = v.x*n.x + v.y*n.y;
    float d = 1.0f - r*r*(1.0f - dot*dot);

    if (d >= 0.0f)
    {
        d = sqrtf(d);
        v.x = r*v.x - (r*dot + d)*n.x;
        v.y = r*v.y - (r*dot + d)*n.y;

        result = v;
    }

    return result;
}


//----------------------------------------------------------------------------------
// Module Functions Definition - RaylibVector3 math
//----------------------------------------------------------------------------------

// Vector with components value 0.0f
RAYLIB_RMAPI RaylibVector3 RaylibVector3Zero(void)
{
    RaylibVector3 result = { 0.0f, 0.0f, 0.0f };

    return result;
}

// Vector with components value 1.0f
RAYLIB_RMAPI RaylibVector3 RaylibVector3One(void)
{
    RaylibVector3 result = { 1.0f, 1.0f, 1.0f };

    return result;
}

// Add two vectors
RAYLIB_RMAPI RaylibVector3 RaylibVector3Add(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };

    return result;
}

// Add vector and float value
RAYLIB_RMAPI RaylibVector3 RaylibVector3AddValue(RaylibVector3 v, float add)
{
    RaylibVector3 result = { v.x + add, v.y + add, v.z + add };

    return result;
}

// Subtract two vectors
RAYLIB_RMAPI RaylibVector3 RaylibVector3Subtract(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

    return result;
}

// Subtract vector by float value
RAYLIB_RMAPI RaylibVector3 RaylibVector3SubtractValue(RaylibVector3 v, float sub)
{
    RaylibVector3 result = { v.x - sub, v.y - sub, v.z - sub };

    return result;
}

// Multiply vector by scalar
RAYLIB_RMAPI RaylibVector3 RaylibVector3Scale(RaylibVector3 v, float scalar)
{
    RaylibVector3 result = { v.x*scalar, v.y*scalar, v.z*scalar };

    return result;
}

// Multiply vector by vector
RAYLIB_RMAPI RaylibVector3 RaylibVector3Multiply(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z };

    return result;
}

// Calculate two vectors cross product
RAYLIB_RMAPI RaylibVector3 RaylibVector3CrossProduct(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };

    return result;
}

// Calculate one vector perpendicular vector
RAYLIB_RMAPI RaylibVector3 RaylibVector3Perpendicular(RaylibVector3 v)
{
    RaylibVector3 result = { 0 };

    float min = fabsf(v.x);
    RaylibVector3 cardinalAxis = {1.0f, 0.0f, 0.0f};

    if (fabsf(v.y) < min)
    {
        min = fabsf(v.y);
        RaylibVector3 tmp = {0.0f, 1.0f, 0.0f};
        cardinalAxis = tmp;
    }

    if (fabsf(v.z) < min)
    {
        RaylibVector3 tmp = {0.0f, 0.0f, 1.0f};
        cardinalAxis = tmp;
    }

    // Cross product between vectors
    result.x = v.y*cardinalAxis.z - v.z*cardinalAxis.y;
    result.y = v.z*cardinalAxis.x - v.x*cardinalAxis.z;
    result.z = v.x*cardinalAxis.y - v.y*cardinalAxis.x;

    return result;
}

// Calculate vector length
RAYLIB_RMAPI float RaylibVector3Length(const RaylibVector3 v)
{
    float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

    return result;
}

// Calculate vector square length
RAYLIB_RMAPI float RaylibVector3LengthSqr(const RaylibVector3 v)
{
    float result = v.x*v.x + v.y*v.y + v.z*v.z;

    return result;
}

// Calculate two vectors dot product
RAYLIB_RMAPI float RaylibVector3DotProduct(RaylibVector3 v1, RaylibVector3 v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);

    return result;
}

// Calculate distance between two vectors
RAYLIB_RMAPI float RaylibVector3Distance(RaylibVector3 v1, RaylibVector3 v2)
{
    float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = sqrtf(dx*dx + dy*dy + dz*dz);

    return result;
}

// Calculate square distance between two vectors
RAYLIB_RMAPI float RaylibVector3DistanceSqr(RaylibVector3 v1, RaylibVector3 v2)
{
    float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = dx*dx + dy*dy + dz*dz;

    return result;
}

// Calculate angle between two vectors
RAYLIB_RMAPI float RaylibVector3Angle(RaylibVector3 v1, RaylibVector3 v2)
{
    float result = 0.0f;

    RaylibVector3 cross = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
    float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
    float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    result = atan2f(len, dot);

    return result;
}

// Negate provided vector (invert direction)
RAYLIB_RMAPI RaylibVector3 RaylibVector3Negate(RaylibVector3 v)
{
    RaylibVector3 result = { -v.x, -v.y, -v.z };

    return result;
}

// Divide vector by vector
RAYLIB_RMAPI RaylibVector3 RaylibVector3Divide(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z };

    return result;
}

// RaylibNormalize provided vector
RAYLIB_RMAPI RaylibVector3 RaylibVector3Normalize(RaylibVector3 v)
{
    RaylibVector3 result = v;

    float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f)
    {
        float ilength = 1.0f/length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }

    return result;
}

//Calculate the projection of the vector v1 on to v2
RAYLIB_RMAPI RaylibVector3 RaylibVector3Project(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { 0 };

    float v1dv2 = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    float v2dv2 = (v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);

    float mag = v1dv2/v2dv2;

    result.x = v2.x*mag;
    result.y = v2.y*mag;
    result.z = v2.z*mag;

    return result;
}

//Calculate the rejection of the vector v1 on to v2
RAYLIB_RMAPI RaylibVector3 RaylibVector3Reject(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { 0 };

    float v1dv2 = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    float v2dv2 = (v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);

    float mag = v1dv2/v2dv2;

    result.x = v1.x - (v2.x*mag);
    result.y = v1.y - (v2.y*mag);
    result.z = v1.z - (v2.z*mag);

    return result;
}

// Orthonormalize provided vectors
// Makes vectors normalized and orthogonal to each other
// Gram-Schmidt function implementation
RAYLIB_RMAPI void RaylibVector3OrthoNormalize(RaylibVector3 *v1, RaylibVector3 *v2)
{
    float length = 0.0f;
    float ilength = 0.0f;

    // RaylibVector3Normalize(*v1);
    RaylibVector3 v = *v1;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    v1->x *= ilength;
    v1->y *= ilength;
    v1->z *= ilength;

    // RaylibVector3CrossProduct(*v1, *v2)
    RaylibVector3 vn1 = { v1->y*v2->z - v1->z*v2->y, v1->z*v2->x - v1->x*v2->z, v1->x*v2->y - v1->y*v2->x };

    // RaylibVector3Normalize(vn1);
    v = vn1;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    vn1.x *= ilength;
    vn1.y *= ilength;
    vn1.z *= ilength;

    // RaylibVector3CrossProduct(vn1, *v1)
    RaylibVector3 vn2 = { vn1.y*v1->z - vn1.z*v1->y, vn1.z*v1->x - vn1.x*v1->z, vn1.x*v1->y - vn1.y*v1->x };

    *v2 = vn2;
}

// Transforms a RaylibVector3 by a given RaylibMatrix
RAYLIB_RMAPI RaylibVector3 RaylibVector3Transform(RaylibVector3 v, RaylibMatrix mat)
{
    RaylibVector3 result = { 0 };

    float x = v.x;
    float y = v.y;
    float z = v.z;

    result.x = mat.m0*x + mat.m4*y + mat.m8*z + mat.m12;
    result.y = mat.m1*x + mat.m5*y + mat.m9*z + mat.m13;
    result.z = mat.m2*x + mat.m6*y + mat.m10*z + mat.m14;

    return result;
}

// RaylibTransform a vector by quaternion rotation
RAYLIB_RMAPI RaylibVector3 RaylibVector3RotateByQuaternion(RaylibVector3 v, Quaternion q)
{
    RaylibVector3 result = { 0 };

    result.x = v.x*(q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z) + v.y*(2*q.x*q.y - 2*q.w*q.z) + v.z*(2*q.x*q.z + 2*q.w*q.y);
    result.y = v.x*(2*q.w*q.z + 2*q.x*q.y) + v.y*(q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z) + v.z*(-2*q.w*q.x + 2*q.y*q.z);
    result.z = v.x*(-2*q.w*q.y + 2*q.x*q.z) + v.y*(2*q.w*q.x + 2*q.y*q.z)+ v.z*(q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);

    return result;
}

// Rotates a vector around an axis
RAYLIB_RMAPI RaylibVector3 RaylibVector3RotateByAxisAngle(RaylibVector3 v, RaylibVector3 axis, float angle)
{
    // Using Euler-Rodrigues Formula
    // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

    RaylibVector3 result = v;

    // RaylibVector3Normalize(axis);
    float length = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;
    axis.x *= ilength;
    axis.y *= ilength;
    axis.z *= ilength;

    angle /= 2.0f;
    float a = sinf(angle);
    float b = axis.x*a;
    float c = axis.y*a;
    float d = axis.z*a;
    a = cosf(angle);
    RaylibVector3 w = { b, c, d };

    // RaylibVector3CrossProduct(w, v)
    RaylibVector3 wv = { w.y*v.z - w.z*v.y, w.z*v.x - w.x*v.z, w.x*v.y - w.y*v.x };

    // RaylibVector3CrossProduct(w, wv)
    RaylibVector3 wwv = { w.y*wv.z - w.z*wv.y, w.z*wv.x - w.x*wv.z, w.x*wv.y - w.y*wv.x };

    // RaylibVector3Scale(wv, 2*a)
    a *= 2;
    wv.x *= a;
    wv.y *= a;
    wv.z *= a;

    // RaylibVector3Scale(wwv, 2)
    wwv.x *= 2;
    wwv.y *= 2;
    wwv.z *= 2;

    result.x += wv.x;
    result.y += wv.y;
    result.z += wv.z;

    result.x += wwv.x;
    result.y += wwv.y;
    result.z += wwv.z;

    return result;
}

// Move Vector towards target
RAYLIB_RMAPI RaylibVector3 RaylibVector3MoveTowards(RaylibVector3 v, RaylibVector3 target, float maxDistance)
{
    RaylibVector3 result = { 0 };

    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float dz = target.z - v.z;
    float value = (dx*dx) + (dy*dy) + (dz*dz);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance))) return target;

    float dist = sqrtf(value);

    result.x = v.x + dx/dist*maxDistance;
    result.y = v.y + dy/dist*maxDistance;
    result.z = v.z + dz/dist*maxDistance;

    return result;
}

// Calculate linear interpolation between two vectors
RAYLIB_RMAPI RaylibVector3 RaylibVector3Lerp(RaylibVector3 v1, RaylibVector3 v2, float amount)
{
    RaylibVector3 result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);
    result.z = v1.z + amount*(v2.z - v1.z);

    return result;
}

// Calculate cubic hermite interpolation between two vectors and their tangents
// as described in the GLTF 2.0 specification: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic
RAYLIB_RMAPI RaylibVector3 RaylibVector3CubicHermite(RaylibVector3 v1, RaylibVector3 tangent1, RaylibVector3 v2, RaylibVector3 tangent2, float amount)
{
    RaylibVector3 result = { 0 };

    float amountPow2 = amount*amount;
    float amountPow3 = amount*amount*amount;

    result.x = (2*amountPow3 - 3*amountPow2 + 1)*v1.x + (amountPow3 - 2*amountPow2 + amount)*tangent1.x + (-2*amountPow3 + 3*amountPow2)*v2.x + (amountPow3 - amountPow2)*tangent2.x;
    result.y = (2*amountPow3 - 3*amountPow2 + 1)*v1.y + (amountPow3 - 2*amountPow2 + amount)*tangent1.y + (-2*amountPow3 + 3*amountPow2)*v2.y + (amountPow3 - amountPow2)*tangent2.y;
    result.z = (2*amountPow3 - 3*amountPow2 + 1)*v1.z + (amountPow3 - 2*amountPow2 + amount)*tangent1.z + (-2*amountPow3 + 3*amountPow2)*v2.z + (amountPow3 - amountPow2)*tangent2.z;

    return result;
}

// Calculate reflected vector to normal
RAYLIB_RMAPI RaylibVector3 RaylibVector3Reflect(RaylibVector3 v, RaylibVector3 normal)
{
    RaylibVector3 result = { 0 };

    // I is the original vector
    // N is the normal of the incident plane
    // R = I - (2*N*(DotProduct[I, N]))

    float dotProduct = (v.x*normal.x + v.y*normal.y + v.z*normal.z);

    result.x = v.x - (2.0f*normal.x)*dotProduct;
    result.y = v.y - (2.0f*normal.y)*dotProduct;
    result.z = v.z - (2.0f*normal.z)*dotProduct;

    return result;
}

// Get min value for each pair of components
RAYLIB_RMAPI RaylibVector3 RaylibVector3Min(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);
    result.z = fminf(v1.z, v2.z);

    return result;
}

// Get max value for each pair of components
RAYLIB_RMAPI RaylibVector3 RaylibVector3Max(RaylibVector3 v1, RaylibVector3 v2)
{
    RaylibVector3 result = { 0 };

    result.x = fmaxf(v1.x, v2.x);
    result.y = fmaxf(v1.y, v2.y);
    result.z = fmaxf(v1.z, v2.z);

    return result;
}

// Compute barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c)
// NOTE: Assumes P is on the plane of the triangle
RAYLIB_RMAPI RaylibVector3 RaylibVector3Barycenter(RaylibVector3 p, RaylibVector3 a, RaylibVector3 b, RaylibVector3 c)
{
    RaylibVector3 result = { 0 };

    RaylibVector3 v0 = { b.x - a.x, b.y - a.y, b.z - a.z };   // RaylibVector3Subtract(b, a)
    RaylibVector3 v1 = { c.x - a.x, c.y - a.y, c.z - a.z };   // RaylibVector3Subtract(c, a)
    RaylibVector3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z };   // RaylibVector3Subtract(p, a)
    float d00 = (v0.x*v0.x + v0.y*v0.y + v0.z*v0.z);    // RaylibVector3DotProduct(v0, v0)
    float d01 = (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z);    // RaylibVector3DotProduct(v0, v1)
    float d11 = (v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);    // RaylibVector3DotProduct(v1, v1)
    float d20 = (v2.x*v0.x + v2.y*v0.y + v2.z*v0.z);    // RaylibVector3DotProduct(v2, v0)
    float d21 = (v2.x*v1.x + v2.y*v1.y + v2.z*v1.z);    // RaylibVector3DotProduct(v2, v1)

    float denom = d00*d11 - d01*d01;

    result.y = (d11*d20 - d01*d21)/denom;
    result.z = (d00*d21 - d01*d20)/denom;
    result.x = 1.0f - (result.z + result.y);

    return result;
}

// Projects a RaylibVector3 from screen space into object space
// NOTE: We are avoiding calling other raymath functions despite available
RAYLIB_RMAPI RaylibVector3 RaylibVector3Unproject(RaylibVector3 source, RaylibMatrix projection, RaylibMatrix view)
{
    RaylibVector3 result = { 0 };

    // Calculate unprojected matrix (multiply view matrix by projection matrix) and invert it
    RaylibMatrix matViewProj = {      // RaylibMatrixMultiply(view, projection);
        view.m0*projection.m0 + view.m1*projection.m4 + view.m2*projection.m8 + view.m3*projection.m12,
        view.m0*projection.m1 + view.m1*projection.m5 + view.m2*projection.m9 + view.m3*projection.m13,
        view.m0*projection.m2 + view.m1*projection.m6 + view.m2*projection.m10 + view.m3*projection.m14,
        view.m0*projection.m3 + view.m1*projection.m7 + view.m2*projection.m11 + view.m3*projection.m15,
        view.m4*projection.m0 + view.m5*projection.m4 + view.m6*projection.m8 + view.m7*projection.m12,
        view.m4*projection.m1 + view.m5*projection.m5 + view.m6*projection.m9 + view.m7*projection.m13,
        view.m4*projection.m2 + view.m5*projection.m6 + view.m6*projection.m10 + view.m7*projection.m14,
        view.m4*projection.m3 + view.m5*projection.m7 + view.m6*projection.m11 + view.m7*projection.m15,
        view.m8*projection.m0 + view.m9*projection.m4 + view.m10*projection.m8 + view.m11*projection.m12,
        view.m8*projection.m1 + view.m9*projection.m5 + view.m10*projection.m9 + view.m11*projection.m13,
        view.m8*projection.m2 + view.m9*projection.m6 + view.m10*projection.m10 + view.m11*projection.m14,
        view.m8*projection.m3 + view.m9*projection.m7 + view.m10*projection.m11 + view.m11*projection.m15,
        view.m12*projection.m0 + view.m13*projection.m4 + view.m14*projection.m8 + view.m15*projection.m12,
        view.m12*projection.m1 + view.m13*projection.m5 + view.m14*projection.m9 + view.m15*projection.m13,
        view.m12*projection.m2 + view.m13*projection.m6 + view.m14*projection.m10 + view.m15*projection.m14,
        view.m12*projection.m3 + view.m13*projection.m7 + view.m14*projection.m11 + view.m15*projection.m15 };

    // Calculate inverted matrix -> RaylibMatrixInvert(matViewProj);
    // Cache the matrix values (speed optimization)
    float a00 = matViewProj.m0, a01 = matViewProj.m1, a02 = matViewProj.m2, a03 = matViewProj.m3;
    float a10 = matViewProj.m4, a11 = matViewProj.m5, a12 = matViewProj.m6, a13 = matViewProj.m7;
    float a20 = matViewProj.m8, a21 = matViewProj.m9, a22 = matViewProj.m10, a23 = matViewProj.m11;
    float a30 = matViewProj.m12, a31 = matViewProj.m13, a32 = matViewProj.m14, a33 = matViewProj.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    RaylibMatrix matViewProjInv = {
        (a11*b11 - a12*b10 + a13*b09)*invDet,
        (-a01*b11 + a02*b10 - a03*b09)*invDet,
        (a31*b05 - a32*b04 + a33*b03)*invDet,
        (-a21*b05 + a22*b04 - a23*b03)*invDet,
        (-a10*b11 + a12*b08 - a13*b07)*invDet,
        (a00*b11 - a02*b08 + a03*b07)*invDet,
        (-a30*b05 + a32*b02 - a33*b01)*invDet,
        (a20*b05 - a22*b02 + a23*b01)*invDet,
        (a10*b10 - a11*b08 + a13*b06)*invDet,
        (-a00*b10 + a01*b08 - a03*b06)*invDet,
        (a30*b04 - a31*b02 + a33*b00)*invDet,
        (-a20*b04 + a21*b02 - a23*b00)*invDet,
        (-a10*b09 + a11*b07 - a12*b06)*invDet,
        (a00*b09 - a01*b07 + a02*b06)*invDet,
        (-a30*b03 + a31*b01 - a32*b00)*invDet,
        (a20*b03 - a21*b01 + a22*b00)*invDet };

    // Create quaternion from source point
    Quaternion quat = { source.x, source.y, source.z, 1.0f };

    // Multiply quat point by unprojecte matrix
    Quaternion qtransformed = {     // RaylibQuaternionTransform(quat, matViewProjInv)
        matViewProjInv.m0*quat.x + matViewProjInv.m4*quat.y + matViewProjInv.m8*quat.z + matViewProjInv.m12*quat.w,
        matViewProjInv.m1*quat.x + matViewProjInv.m5*quat.y + matViewProjInv.m9*quat.z + matViewProjInv.m13*quat.w,
        matViewProjInv.m2*quat.x + matViewProjInv.m6*quat.y + matViewProjInv.m10*quat.z + matViewProjInv.m14*quat.w,
        matViewProjInv.m3*quat.x + matViewProjInv.m7*quat.y + matViewProjInv.m11*quat.z + matViewProjInv.m15*quat.w };

    // Normalized world points in vectors
    result.x = qtransformed.x/qtransformed.w;
    result.y = qtransformed.y/qtransformed.w;
    result.z = qtransformed.z/qtransformed.w;

    return result;
}

// Get RaylibVector3 as float array
RAYLIB_RMAPI Raylibfloat3 RaylibVector3ToFloatV(RaylibVector3 v)
{
    Raylibfloat3 buffer = { 0 };

    buffer.v[0] = v.x;
    buffer.v[1] = v.y;
    buffer.v[2] = v.z;

    return buffer;
}

// Invert the given vector
RAYLIB_RMAPI RaylibVector3 RaylibVector3Invert(RaylibVector3 v)
{
    RaylibVector3 result = { 1.0f/v.x, 1.0f/v.y, 1.0f/v.z };

    return result;
}

// RaylibClamp the components of the vector between
// min and max values specified by the given vectors
RAYLIB_RMAPI RaylibVector3 RaylibVector3Clamp(RaylibVector3 v, RaylibVector3 min, RaylibVector3 max)
{
    RaylibVector3 result = { 0 };

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));
    result.z = fminf(max.z, fmaxf(min.z, v.z));

    return result;
}

// RaylibClamp the magnitude of the vector between two values
RAYLIB_RMAPI RaylibVector3 RaylibVector3ClampValue(RaylibVector3 v, float min, float max)
{
    RaylibVector3 result = v;

    float length = (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
    if (length > 0.0f)
    {
        length = sqrtf(length);

        float scale = 1;    // By default, 1 as the neutral element.
        if (length < min)
        {
            scale = min/length;
        }
        else if (length > max)
        {
            scale = max/length;
        }

        result.x = v.x*scale;
        result.y = v.y*scale;
        result.z = v.z*scale;
    }

    return result;
}

// Check whether two given vectors are almost equal
RAYLIB_RMAPI int RaylibVector3Equals(RaylibVector3 p, RaylibVector3 q)
{
#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    int result = ((fabsf(p.x - q.x)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                 ((fabsf(p.y - q.y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                 ((fabsf(p.z - q.z)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z)))));

    return result;
}

// Compute the direction of a refracted ray
// v: normalized direction of the incoming ray
// n: normalized normal vector of the interface of two optical media
// r: ratio of the refractive index of the medium from where the ray comes
//    to the refractive index of the medium on the other side of the surface
RAYLIB_RMAPI RaylibVector3 RaylibVector3Refract(RaylibVector3 v, RaylibVector3 n, float r)
{
    RaylibVector3 result = { 0 };

    float dot = v.x*n.x + v.y*n.y + v.z*n.z;
    float d = 1.0f - r*r*(1.0f - dot*dot);

    if (d >= 0.0f)
    {
        d = sqrtf(d);
        v.x = r*v.x - (r*dot + d)*n.x;
        v.y = r*v.y - (r*dot + d)*n.y;
        v.z = r*v.z - (r*dot + d)*n.z;

        result = v;
    }

    return result;
}


//----------------------------------------------------------------------------------
// Module Functions Definition - RaylibVector4 math
//----------------------------------------------------------------------------------

RAYLIB_RMAPI RaylibVector4 RaylibVector4Zero(void)
{
    RaylibVector4 result = { 0.0f, 0.0f, 0.0f, 0.0f };
    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4One(void)
{
    RaylibVector4 result = { 1.0f, 1.0f, 1.0f, 1.0f };
    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4Add(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w
    };
    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4AddValue(RaylibVector4 v, float add)
{
    RaylibVector4 result = {
        v.x + add,
        v.y + add,
        v.z + add,
        v.w + add
    };
    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4Subtract(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w
    };
    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4SubtractValue(RaylibVector4 v, float add)
{
    RaylibVector4 result = {
        v.x - add,
        v.y - add,
        v.z - add,
        v.w - add
    };
    return result;
}

RAYLIB_RMAPI float RaylibVector4Length(RaylibVector4 v)
{
    float result = sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));
    return result;
}

RAYLIB_RMAPI float RaylibVector4LengthSqr(RaylibVector4 v)
{
    float result = (v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w);
    return result;
}

RAYLIB_RMAPI float RaylibVector4DotProduct(RaylibVector4 v1, RaylibVector4 v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w);
    return result;
}

// Calculate distance between two vectors
RAYLIB_RMAPI float RaylibVector4Distance(RaylibVector4 v1, RaylibVector4 v2)
{
    float result = sqrtf(
        (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) +
        (v1.z - v2.z)*(v1.z - v2.z) + (v1.w - v2.w)*(v1.w - v2.w));
    return result;
}

// Calculate square distance between two vectors
RAYLIB_RMAPI float RaylibVector4DistanceSqr(RaylibVector4 v1, RaylibVector4 v2)
{
    float result =
        (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y) +
        (v1.z - v2.z)*(v1.z - v2.z) + (v1.w - v2.w)*(v1.w - v2.w);

    return result;
}

RAYLIB_RMAPI RaylibVector4 RaylibVector4Scale(RaylibVector4 v, float scale)
{
    RaylibVector4 result = { v.x*scale, v.y*scale, v.z*scale, v.w*scale };
    return result;
}

// Multiply vector by vector
RAYLIB_RMAPI RaylibVector4 RaylibVector4Multiply(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w };
    return result;
}

// Negate vector
RAYLIB_RMAPI RaylibVector4 RaylibVector4Negate(RaylibVector4 v)
{
    RaylibVector4 result = { -v.x, -v.y, -v.z, -v.w };
    return result;
}

// Divide vector by vector
RAYLIB_RMAPI RaylibVector4 RaylibVector4Divide(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = { v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w };
    return result;
}

// RaylibNormalize provided vector
RAYLIB_RMAPI RaylibVector4 RaylibVector4Normalize(RaylibVector4 v)
{
    RaylibVector4 result = { 0 };
    float length = sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));

    if (length > 0)
    {
        float ilength = 1.0f/length;
        result.x = v.x*ilength;
        result.y = v.y*ilength;
        result.z = v.z*ilength;
        result.w = v.w*ilength;
    }

    return result;
}

// Get min value for each pair of components
RAYLIB_RMAPI RaylibVector4 RaylibVector4Min(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);
    result.z = fminf(v1.z, v2.z);
    result.w = fminf(v1.w, v2.w);

    return result;
}

// Get max value for each pair of components
RAYLIB_RMAPI RaylibVector4 RaylibVector4Max(RaylibVector4 v1, RaylibVector4 v2)
{
    RaylibVector4 result = { 0 };

    result.x = fmaxf(v1.x, v2.x);
    result.y = fmaxf(v1.y, v2.y);
    result.z = fmaxf(v1.z, v2.z);
    result.w = fmaxf(v1.w, v2.w);

    return result;
}

// Calculate linear interpolation between two vectors
RAYLIB_RMAPI RaylibVector4 RaylibVector4Lerp(RaylibVector4 v1, RaylibVector4 v2, float amount)
{
    RaylibVector4 result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);
    result.z = v1.z + amount*(v2.z - v1.z);
    result.w = v1.w + amount*(v2.w - v1.w);

    return result;
}

// Move Vector towards target
RAYLIB_RMAPI RaylibVector4 RaylibVector4MoveTowards(RaylibVector4 v, RaylibVector4 target, float maxDistance)
{
    RaylibVector4 result = { 0 };

    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float dz = target.z - v.z;
    float dw = target.w - v.w;
    float value = (dx*dx) + (dy*dy) + (dz*dz) + (dw*dw);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance))) return target;

    float dist = sqrtf(value);

    result.x = v.x + dx/dist*maxDistance;
    result.y = v.y + dy/dist*maxDistance;
    result.z = v.z + dz/dist*maxDistance;
    result.w = v.w + dw/dist*maxDistance;

    return result;
}

// Invert the given vector
RAYLIB_RMAPI RaylibVector4 RaylibVector4Invert(RaylibVector4 v)
{
    RaylibVector4 result = { 1.0f/v.x, 1.0f/v.y, 1.0f/v.z, 1.0f/v.w };
    return result;
}

// Check whether two given vectors are almost equal
RAYLIB_RMAPI int RaylibVector4Equals(RaylibVector4 p, RaylibVector4 q)
{
#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    int result = ((fabsf(p.x - q.x)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                  ((fabsf(p.y - q.y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                  ((fabsf(p.z - q.z)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
                  ((fabsf(p.w - q.w)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))));
    return result;
}


//----------------------------------------------------------------------------------
// Module Functions Definition - RaylibMatrix math
//----------------------------------------------------------------------------------

// Compute matrix determinant
RAYLIB_RMAPI float RaylibMatrixDeterminant(RaylibMatrix mat)
{
    float result = 0.0f;

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    result = a30*a21*a12*a03 - a20*a31*a12*a03 - a30*a11*a22*a03 + a10*a31*a22*a03 +
             a20*a11*a32*a03 - a10*a21*a32*a03 - a30*a21*a02*a13 + a20*a31*a02*a13 +
             a30*a01*a22*a13 - a00*a31*a22*a13 - a20*a01*a32*a13 + a00*a21*a32*a13 +
             a30*a11*a02*a23 - a10*a31*a02*a23 - a30*a01*a12*a23 + a00*a31*a12*a23 +
             a10*a01*a32*a23 - a00*a11*a32*a23 - a20*a11*a02*a33 + a10*a21*a02*a33 +
             a20*a01*a12*a33 - a00*a21*a12*a33 - a10*a01*a22*a33 + a00*a11*a22*a33;

    return result;
}

// Get the trace of the matrix (sum of the values along the diagonal)
RAYLIB_RMAPI float RaylibMatrixTrace(RaylibMatrix mat)
{
    float result = (mat.m0 + mat.m5 + mat.m10 + mat.m15);

    return result;
}

// Transposes provided matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixTranspose(RaylibMatrix mat)
{
    RaylibMatrix result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

// Invert provided matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixInvert(RaylibMatrix mat)
{
    RaylibMatrix result = { 0 };

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    result.m0 = (a11*b11 - a12*b10 + a13*b09)*invDet;
    result.m1 = (-a01*b11 + a02*b10 - a03*b09)*invDet;
    result.m2 = (a31*b05 - a32*b04 + a33*b03)*invDet;
    result.m3 = (-a21*b05 + a22*b04 - a23*b03)*invDet;
    result.m4 = (-a10*b11 + a12*b08 - a13*b07)*invDet;
    result.m5 = (a00*b11 - a02*b08 + a03*b07)*invDet;
    result.m6 = (-a30*b05 + a32*b02 - a33*b01)*invDet;
    result.m7 = (a20*b05 - a22*b02 + a23*b01)*invDet;
    result.m8 = (a10*b10 - a11*b08 + a13*b06)*invDet;
    result.m9 = (-a00*b10 + a01*b08 - a03*b06)*invDet;
    result.m10 = (a30*b04 - a31*b02 + a33*b00)*invDet;
    result.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
    result.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
    result.m13 = (a00*b09 - a01*b07 + a02*b06)*invDet;
    result.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
    result.m15 = (a20*b03 - a21*b01 + a22*b00)*invDet;

    return result;
}

// Get identity matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixIdentity(void)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Add two matrices
RAYLIB_RMAPI RaylibMatrix RaylibMatrixAdd(RaylibMatrix left, RaylibMatrix right)
{
    RaylibMatrix result = { 0 };

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;
    result.m4 = left.m4 + right.m4;
    result.m5 = left.m5 + right.m5;
    result.m6 = left.m6 + right.m6;
    result.m7 = left.m7 + right.m7;
    result.m8 = left.m8 + right.m8;
    result.m9 = left.m9 + right.m9;
    result.m10 = left.m10 + right.m10;
    result.m11 = left.m11 + right.m11;
    result.m12 = left.m12 + right.m12;
    result.m13 = left.m13 + right.m13;
    result.m14 = left.m14 + right.m14;
    result.m15 = left.m15 + right.m15;

    return result;
}

// Subtract two matrices (left - right)
RAYLIB_RMAPI RaylibMatrix RaylibMatrixSubtract(RaylibMatrix left, RaylibMatrix right)
{
    RaylibMatrix result = { 0 };

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;
    result.m4 = left.m4 - right.m4;
    result.m5 = left.m5 - right.m5;
    result.m6 = left.m6 - right.m6;
    result.m7 = left.m7 - right.m7;
    result.m8 = left.m8 - right.m8;
    result.m9 = left.m9 - right.m9;
    result.m10 = left.m10 - right.m10;
    result.m11 = left.m11 - right.m11;
    result.m12 = left.m12 - right.m12;
    result.m13 = left.m13 - right.m13;
    result.m14 = left.m14 - right.m14;
    result.m15 = left.m15 - right.m15;

    return result;
}

// Get two matrix multiplication
// NOTE: When multiplying matrices... the order matters!
RAYLIB_RMAPI RaylibMatrix RaylibMatrixMultiply(RaylibMatrix left, RaylibMatrix right)
{
    RaylibMatrix result = { 0 };

    result.m0 = left.m0*right.m0 + left.m1*right.m4 + left.m2*right.m8 + left.m3*right.m12;
    result.m1 = left.m0*right.m1 + left.m1*right.m5 + left.m2*right.m9 + left.m3*right.m13;
    result.m2 = left.m0*right.m2 + left.m1*right.m6 + left.m2*right.m10 + left.m3*right.m14;
    result.m3 = left.m0*right.m3 + left.m1*right.m7 + left.m2*right.m11 + left.m3*right.m15;
    result.m4 = left.m4*right.m0 + left.m5*right.m4 + left.m6*right.m8 + left.m7*right.m12;
    result.m5 = left.m4*right.m1 + left.m5*right.m5 + left.m6*right.m9 + left.m7*right.m13;
    result.m6 = left.m4*right.m2 + left.m5*right.m6 + left.m6*right.m10 + left.m7*right.m14;
    result.m7 = left.m4*right.m3 + left.m5*right.m7 + left.m6*right.m11 + left.m7*right.m15;
    result.m8 = left.m8*right.m0 + left.m9*right.m4 + left.m10*right.m8 + left.m11*right.m12;
    result.m9 = left.m8*right.m1 + left.m9*right.m5 + left.m10*right.m9 + left.m11*right.m13;
    result.m10 = left.m8*right.m2 + left.m9*right.m6 + left.m10*right.m10 + left.m11*right.m14;
    result.m11 = left.m8*right.m3 + left.m9*right.m7 + left.m10*right.m11 + left.m11*right.m15;
    result.m12 = left.m12*right.m0 + left.m13*right.m4 + left.m14*right.m8 + left.m15*right.m12;
    result.m13 = left.m12*right.m1 + left.m13*right.m5 + left.m14*right.m9 + left.m15*right.m13;
    result.m14 = left.m12*right.m2 + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14;
    result.m15 = left.m12*right.m3 + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15;

    return result;
}

// Get translation matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixTranslate(float x, float y, float z)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, x,
                      0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Create rotation matrix from axis and angle
// NOTE: Angle should be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotate(RaylibVector3 axis, float angle)
{
    RaylibMatrix result = { 0 };

    float x = axis.x, y = axis.y, z = axis.z;

    float lengthSquared = x*x + y*y + z*z;

    if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
    {
        float ilength = 1.0f/sqrtf(lengthSquared);
        x *= ilength;
        y *= ilength;
        z *= ilength;
    }

    float sinres = sinf(angle);
    float cosres = cosf(angle);
    float t = 1.0f - cosres;

    result.m0 = x*x*t + cosres;
    result.m1 = y*x*t + z*sinres;
    result.m2 = z*x*t - y*sinres;
    result.m3 = 0.0f;

    result.m4 = x*y*t - z*sinres;
    result.m5 = y*y*t + cosres;
    result.m6 = z*y*t + x*sinres;
    result.m7 = 0.0f;

    result.m8 = x*z*t + y*sinres;
    result.m9 = y*z*t - x*sinres;
    result.m10 = z*z*t + cosres;
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

// Get x-rotation matrix
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotateX(float angle)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // RaylibMatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m5 = cosres;
    result.m6 = sinres;
    result.m9 = -sinres;
    result.m10 = cosres;

    return result;
}

// Get y-rotation matrix
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotateY(float angle)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // RaylibMatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m2 = -sinres;
    result.m8 = sinres;
    result.m10 = cosres;

    return result;
}

// Get z-rotation matrix
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotateZ(float angle)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // RaylibMatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m1 = sinres;
    result.m4 = -sinres;
    result.m5 = cosres;

    return result;
}


// Get xyz-rotation matrix
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotateXYZ(RaylibVector3 angle)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // RaylibMatrixIdentity()

    float cosz = cosf(-angle.z);
    float sinz = sinf(-angle.z);
    float cosy = cosf(-angle.y);
    float siny = sinf(-angle.y);
    float cosx = cosf(-angle.x);
    float sinx = sinf(-angle.x);

    result.m0 = cosz*cosy;
    result.m1 = (cosz*siny*sinx) - (sinz*cosx);
    result.m2 = (cosz*siny*cosx) + (sinz*sinx);

    result.m4 = sinz*cosy;
    result.m5 = (sinz*siny*sinx) + (cosz*cosx);
    result.m6 = (sinz*siny*cosx) - (cosz*sinx);

    result.m8 = -siny;
    result.m9 = cosy*sinx;
    result.m10= cosy*cosx;

    return result;
}

// Get zyx-rotation matrix
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixRotateZYX(RaylibVector3 angle)
{
    RaylibMatrix result = { 0 };

    float cz = cosf(angle.z);
    float sz = sinf(angle.z);
    float cy = cosf(angle.y);
    float sy = sinf(angle.y);
    float cx = cosf(angle.x);
    float sx = sinf(angle.x);

    result.m0 = cz*cy;
    result.m4 = cz*sy*sx - cx*sz;
    result.m8 = sz*sx + cz*cx*sy;
    result.m12 = 0;

    result.m1 = cy*sz;
    result.m5 = cz*cx + sz*sy*sx;
    result.m9 = cx*sz*sy - cz*sx;
    result.m13 = 0;

    result.m2 = -sy;
    result.m6 = cy*sx;
    result.m10 = cy*cx;
    result.m14 = 0;

    result.m3 = 0;
    result.m7 = 0;
    result.m11 = 0;
    result.m15 = 1;

    return result;
}

// Get scaling matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixScale(float x, float y, float z)
{
    RaylibMatrix result = { x, 0.0f, 0.0f, 0.0f,
                      0.0f, y, 0.0f, 0.0f,
                      0.0f, 0.0f, z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Get perspective projection matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixFrustum(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    RaylibMatrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(farPlane - nearPlane);

    result.m0 = ((float)nearPlane*2.0f)/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;

    result.m4 = 0.0f;
    result.m5 = ((float)nearPlane*2.0f)/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;

    result.m8 = ((float)right + (float)left)/rl;
    result.m9 = ((float)top + (float)bottom)/tb;
    result.m10 = -((float)farPlane + (float)nearPlane)/fn;
    result.m11 = -1.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = -((float)farPlane*(float)nearPlane*2.0f)/fn;
    result.m15 = 0.0f;

    return result;
}

// Get perspective projection matrix
// NOTE: Fovy angle must be provided in radians
RAYLIB_RMAPI RaylibMatrix RaylibMatrixPerspective(double fovY, double aspect, double nearPlane, double farPlane)
{
    RaylibMatrix result = { 0 };

    double top = nearPlane*tan(fovY*0.5);
    double bottom = -top;
    double right = top*aspect;
    double left = -right;

    // RaylibMatrixFrustum(-right, right, -top, top, near, far);
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(farPlane - nearPlane);

    result.m0 = ((float)nearPlane*2.0f)/rl;
    result.m5 = ((float)nearPlane*2.0f)/tb;
    result.m8 = ((float)right + (float)left)/rl;
    result.m9 = ((float)top + (float)bottom)/tb;
    result.m10 = -((float)farPlane + (float)nearPlane)/fn;
    result.m11 = -1.0f;
    result.m14 = -((float)farPlane*(float)nearPlane*2.0f)/fn;

    return result;
}

// Get orthographic projection matrix
RAYLIB_RMAPI RaylibMatrix RaylibMatrixOrtho(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    RaylibMatrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(farPlane - nearPlane);

    result.m0 = 2.0f/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;
    result.m4 = 0.0f;
    result.m5 = 2.0f/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 0.0f;
    result.m9 = 0.0f;
    result.m10 = -2.0f/fn;
    result.m11 = 0.0f;
    result.m12 = -((float)left + (float)right)/rl;
    result.m13 = -((float)top + (float)bottom)/tb;
    result.m14 = -((float)farPlane + (float)nearPlane)/fn;
    result.m15 = 1.0f;

    return result;
}

// Get camera look-at matrix (view matrix)
RAYLIB_RMAPI RaylibMatrix RaylibMatrixLookAt(RaylibVector3 eye, RaylibVector3 target, RaylibVector3 up)
{
    RaylibMatrix result = { 0 };

    float length = 0.0f;
    float ilength = 0.0f;

    // RaylibVector3Subtract(eye, target)
    RaylibVector3 vz = { eye.x - target.x, eye.y - target.y, eye.z - target.z };

    // RaylibVector3Normalize(vz)
    RaylibVector3 v = vz;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    vz.x *= ilength;
    vz.y *= ilength;
    vz.z *= ilength;

    // RaylibVector3CrossProduct(up, vz)
    RaylibVector3 vx = { up.y*vz.z - up.z*vz.y, up.z*vz.x - up.x*vz.z, up.x*vz.y - up.y*vz.x };

    // RaylibVector3Normalize(x)
    v = vx;
    length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f/length;
    vx.x *= ilength;
    vx.y *= ilength;
    vx.z *= ilength;

    // RaylibVector3CrossProduct(vz, vx)
    RaylibVector3 vy = { vz.y*vx.z - vz.z*vx.y, vz.z*vx.x - vz.x*vx.z, vz.x*vx.y - vz.y*vx.x };

    result.m0 = vx.x;
    result.m1 = vy.x;
    result.m2 = vz.x;
    result.m3 = 0.0f;
    result.m4 = vx.y;
    result.m5 = vy.y;
    result.m6 = vz.y;
    result.m7 = 0.0f;
    result.m8 = vx.z;
    result.m9 = vy.z;
    result.m10 = vz.z;
    result.m11 = 0.0f;
    result.m12 = -(vx.x*eye.x + vx.y*eye.y + vx.z*eye.z);   // RaylibVector3DotProduct(vx, eye)
    result.m13 = -(vy.x*eye.x + vy.y*eye.y + vy.z*eye.z);   // RaylibVector3DotProduct(vy, eye)
    result.m14 = -(vz.x*eye.x + vz.y*eye.y + vz.z*eye.z);   // RaylibVector3DotProduct(vz, eye)
    result.m15 = 1.0f;

    return result;
}

// Get float array of matrix data
RAYLIB_RMAPI Raylibfloat16 RaylibMatrixToFloatV(RaylibMatrix mat)
{
    Raylibfloat16 result = { 0 };

    result.v[0] = mat.m0;
    result.v[1] = mat.m1;
    result.v[2] = mat.m2;
    result.v[3] = mat.m3;
    result.v[4] = mat.m4;
    result.v[5] = mat.m5;
    result.v[6] = mat.m6;
    result.v[7] = mat.m7;
    result.v[8] = mat.m8;
    result.v[9] = mat.m9;
    result.v[10] = mat.m10;
    result.v[11] = mat.m11;
    result.v[12] = mat.m12;
    result.v[13] = mat.m13;
    result.v[14] = mat.m14;
    result.v[15] = mat.m15;

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Quaternion math
//----------------------------------------------------------------------------------

// Add two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionAdd(Quaternion q1, Quaternion q2)
{
    Quaternion result = {q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w};

    return result;
}

// Add quaternion and float value
RAYLIB_RMAPI Quaternion RaylibQuaternionAddValue(Quaternion q, float add)
{
    Quaternion result = {q.x + add, q.y + add, q.z + add, q.w + add};

    return result;
}

// Subtract two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionSubtract(Quaternion q1, Quaternion q2)
{
    Quaternion result = {q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w};

    return result;
}

// Subtract quaternion and float value
RAYLIB_RMAPI Quaternion RaylibQuaternionSubtractValue(Quaternion q, float sub)
{
    Quaternion result = {q.x - sub, q.y - sub, q.z - sub, q.w - sub};

    return result;
}

// Get identity quaternion
RAYLIB_RMAPI Quaternion RaylibQuaternionIdentity(void)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Computes the length of a quaternion
RAYLIB_RMAPI float RaylibQuaternionLength(Quaternion q)
{
    float result = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);

    return result;
}

// RaylibNormalize provided quaternion
RAYLIB_RMAPI Quaternion RaylibQuaternionNormalize(Quaternion q)
{
    Quaternion result = { 0 };

    float length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;

    result.x = q.x*ilength;
    result.y = q.y*ilength;
    result.z = q.z*ilength;
    result.w = q.w*ilength;

    return result;
}

// Invert provided quaternion
RAYLIB_RMAPI Quaternion RaylibQuaternionInvert(Quaternion q)
{
    Quaternion result = q;

    float lengthSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;

    if (lengthSq != 0.0f)
    {
        float invLength = 1.0f/lengthSq;

        result.x *= -invLength;
        result.y *= -invLength;
        result.z *= -invLength;
        result.w *= invLength;
    }

    return result;
}

// Calculate two quaternion multiplication
RAYLIB_RMAPI Quaternion RaylibQuaternionMultiply(Quaternion q1, Quaternion q2)
{
    Quaternion result = { 0 };

    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

    result.x = qax*qbw + qaw*qbx + qay*qbz - qaz*qby;
    result.y = qay*qbw + qaw*qby + qaz*qbx - qax*qbz;
    result.z = qaz*qbw + qaw*qbz + qax*qby - qay*qbx;
    result.w = qaw*qbw - qax*qbx - qay*qby - qaz*qbz;

    return result;
}

// Scale quaternion by float value
RAYLIB_RMAPI Quaternion RaylibQuaternionScale(Quaternion q, float mul)
{
    Quaternion result = { 0 };

    result.x = q.x*mul;
    result.y = q.y*mul;
    result.z = q.z*mul;
    result.w = q.w*mul;

    return result;
}

// Divide two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionDivide(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x/q2.x, q1.y/q2.y, q1.z/q2.z, q1.w/q2.w };

    return result;
}

// Calculate linear interpolation between two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionLerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    result.x = q1.x + amount*(q2.x - q1.x);
    result.y = q1.y + amount*(q2.y - q1.y);
    result.z = q1.z + amount*(q2.z - q1.z);
    result.w = q1.w + amount*(q2.w - q1.w);

    return result;
}

// Calculate slerp-optimized interpolation between two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionNlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    // RaylibQuaternionLerp(q1, q2, amount)
    result.x = q1.x + amount*(q2.x - q1.x);
    result.y = q1.y + amount*(q2.y - q1.y);
    result.z = q1.z + amount*(q2.z - q1.z);
    result.w = q1.w + amount*(q2.w - q1.w);

    // RaylibQuaternionNormalize(q);
    Quaternion q = result;
    float length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;

    result.x = q.x*ilength;
    result.y = q.y*ilength;
    result.z = q.z*ilength;
    result.w = q.w*ilength;

    return result;
}

// Calculates spherical linear interpolation between two quaternions
RAYLIB_RMAPI Quaternion RaylibQuaternionSlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    float cosHalfTheta = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

    if (cosHalfTheta < 0)
    {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        cosHalfTheta = -cosHalfTheta;
    }

    if (fabsf(cosHalfTheta) >= 1.0f) result = q1;
    else if (cosHalfTheta > 0.95f) result = RaylibQuaternionNlerp(q1, q2, amount);
    else
    {
        float halfTheta = acosf(cosHalfTheta);
        float sinHalfTheta = sqrtf(1.0f - cosHalfTheta*cosHalfTheta);

        if (fabsf(sinHalfTheta) < RAYLIB_EPSILON)
        {
            result.x = (q1.x*0.5f + q2.x*0.5f);
            result.y = (q1.y*0.5f + q2.y*0.5f);
            result.z = (q1.z*0.5f + q2.z*0.5f);
            result.w = (q1.w*0.5f + q2.w*0.5f);
        }
        else
        {
            float ratioA = sinf((1 - amount)*halfTheta)/sinHalfTheta;
            float ratioB = sinf(amount*halfTheta)/sinHalfTheta;

            result.x = (q1.x*ratioA + q2.x*ratioB);
            result.y = (q1.y*ratioA + q2.y*ratioB);
            result.z = (q1.z*ratioA + q2.z*ratioB);
            result.w = (q1.w*ratioA + q2.w*ratioB);
        }
    }

    return result;
}

// Calculate quaternion cubic spline interpolation using Cubic Hermite Spline algorithm
// as described in the GLTF 2.0 specification: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic
RAYLIB_RMAPI Quaternion RaylibQuaternionCubicHermiteSpline(Quaternion q1, Quaternion outTangent1, Quaternion q2, Quaternion inTangent2, float t)
{
    float t2 = t*t;
    float t3 = t2*t;
    float h00 = 2*t3 - 3*t2 + 1;
    float h10 = t3 - 2*t2 + t;
    float h01 = -2*t3 + 3*t2;
    float h11 = t3 - t2;

    Quaternion p0 = RaylibQuaternionScale(q1, h00);
    Quaternion m0 = RaylibQuaternionScale(outTangent1, h10);
    Quaternion p1 = RaylibQuaternionScale(q2, h01);
    Quaternion m1 = RaylibQuaternionScale(inTangent2, h11);

    Quaternion result = { 0 };

    result = RaylibQuaternionAdd(p0, m0);
    result = RaylibQuaternionAdd(result, p1);
    result = RaylibQuaternionAdd(result, m1);
    result = RaylibQuaternionNormalize(result);

    return result;
}

// Calculate quaternion based on the rotation from one vector to another
RAYLIB_RMAPI Quaternion RaylibQuaternionFromVector3ToVector3(RaylibVector3 from, RaylibVector3 to)
{
    Quaternion result = { 0 };

    float cos2Theta = (from.x*to.x + from.y*to.y + from.z*to.z);    // RaylibVector3DotProduct(from, to)
    RaylibVector3 cross = { from.y*to.z - from.z*to.y, from.z*to.x - from.x*to.z, from.x*to.y - from.y*to.x }; // RaylibVector3CrossProduct(from, to)

    result.x = cross.x;
    result.y = cross.y;
    result.z = cross.z;
    result.w = 1.0f + cos2Theta;

    // RaylibQuaternionNormalize(q);
    // NOTE: RaylibNormalize to essentially nlerp the original and identity to 0.5
    Quaternion q = result;
    float length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;

    result.x = q.x*ilength;
    result.y = q.y*ilength;
    result.z = q.z*ilength;
    result.w = q.w*ilength;

    return result;
}

// Get a quaternion for a given rotation matrix
RAYLIB_RMAPI Quaternion RaylibQuaternionFromMatrix(RaylibMatrix mat)
{
    Quaternion result = { 0 };

    float fourWSquaredMinus1 = mat.m0  + mat.m5 + mat.m10;
    float fourXSquaredMinus1 = mat.m0  - mat.m5 - mat.m10;
    float fourYSquaredMinus1 = mat.m5  - mat.m0 - mat.m10;
    float fourZSquaredMinus1 = mat.m10 - mat.m0 - mat.m5;

    int biggestIndex = 0;
    float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }

    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }

    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f)*0.5f;
    float mult = 0.25f/biggestVal;

    switch (biggestIndex)
    {
        case 0:
            result.w = biggestVal;
            result.x = (mat.m6 - mat.m9)*mult;
            result.y = (mat.m8 - mat.m2)*mult;
            result.z = (mat.m1 - mat.m4)*mult;
            break;
        case 1:
            result.x = biggestVal;
            result.w = (mat.m6 - mat.m9)*mult;
            result.y = (mat.m1 + mat.m4)*mult;
            result.z = (mat.m8 + mat.m2)*mult;
            break;
        case 2:
            result.y = biggestVal;
            result.w = (mat.m8 - mat.m2)*mult;
            result.x = (mat.m1 + mat.m4)*mult;
            result.z = (mat.m6 + mat.m9)*mult;
            break;
        case 3:
            result.z = biggestVal;
            result.w = (mat.m1 - mat.m4)*mult;
            result.x = (mat.m8 + mat.m2)*mult;
            result.y = (mat.m6 + mat.m9)*mult;
            break;
    }

    return result;
}

// Get a matrix for a given quaternion
RAYLIB_RMAPI RaylibMatrix RaylibQuaternionToMatrix(Quaternion q)
{
    RaylibMatrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // RaylibMatrixIdentity()

    float a2 = q.x*q.x;
    float b2 = q.y*q.y;
    float c2 = q.z*q.z;
    float ac = q.x*q.z;
    float ab = q.x*q.y;
    float bc = q.y*q.z;
    float ad = q.w*q.x;
    float bd = q.w*q.y;
    float cd = q.w*q.z;

    result.m0 = 1 - 2*(b2 + c2);
    result.m1 = 2*(ab + cd);
    result.m2 = 2*(ac - bd);

    result.m4 = 2*(ab - cd);
    result.m5 = 1 - 2*(a2 + c2);
    result.m6 = 2*(bc + ad);

    result.m8 = 2*(ac + bd);
    result.m9 = 2*(bc - ad);
    result.m10 = 1 - 2*(a2 + b2);

    return result;
}

// Get rotation quaternion for an angle and axis
// NOTE: Angle must be provided in radians
RAYLIB_RMAPI Quaternion RaylibQuaternionFromAxisAngle(RaylibVector3 axis, float angle)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    float axisLength = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);

    if (axisLength != 0.0f)
    {
        angle *= 0.5f;

        float length = 0.0f;
        float ilength = 0.0f;

        // RaylibVector3Normalize(axis)
        length = axisLength;
        if (length == 0.0f) length = 1.0f;
        ilength = 1.0f/length;
        axis.x *= ilength;
        axis.y *= ilength;
        axis.z *= ilength;

        float sinres = sinf(angle);
        float cosres = cosf(angle);

        result.x = axis.x*sinres;
        result.y = axis.y*sinres;
        result.z = axis.z*sinres;
        result.w = cosres;

        // RaylibQuaternionNormalize(q);
        Quaternion q = result;
        length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
        if (length == 0.0f) length = 1.0f;
        ilength = 1.0f/length;
        result.x = q.x*ilength;
        result.y = q.y*ilength;
        result.z = q.z*ilength;
        result.w = q.w*ilength;
    }

    return result;
}

// Get the rotation angle and axis for a given quaternion
RAYLIB_RMAPI void RaylibQuaternionToAxisAngle(Quaternion q, RaylibVector3 *outAxis, float *outAngle)
{
    if (fabsf(q.w) > 1.0f)
    {
        // RaylibQuaternionNormalize(q);
        float length = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
        if (length == 0.0f) length = 1.0f;
        float ilength = 1.0f/length;

        q.x = q.x*ilength;
        q.y = q.y*ilength;
        q.z = q.z*ilength;
        q.w = q.w*ilength;
    }

    RaylibVector3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f*acosf(q.w);
    float den = sqrtf(1.0f - q.w*q.w);

    if (den > RAYLIB_EPSILON)
    {
        resAxis.x = q.x/den;
        resAxis.y = q.y/den;
        resAxis.z = q.z/den;
    }
    else
    {
        // This occurs when the angle is zero.
        // Not a problem: just set an arbitrary normalized axis.
        resAxis.x = 1.0f;
    }

    *outAxis = resAxis;
    *outAngle = resAngle;
}

// Get the quaternion equivalent to Euler angles
// NOTE: Rotation order is ZYX
RAYLIB_RMAPI Quaternion RaylibQuaternionFromEuler(float pitch, float yaw, float roll)
{
    Quaternion result = { 0 };

    float x0 = cosf(pitch*0.5f);
    float x1 = sinf(pitch*0.5f);
    float y0 = cosf(yaw*0.5f);
    float y1 = sinf(yaw*0.5f);
    float z0 = cosf(roll*0.5f);
    float z1 = sinf(roll*0.5f);

    result.x = x1*y0*z0 - x0*y1*z1;
    result.y = x0*y1*z0 + x1*y0*z1;
    result.z = x0*y0*z1 - x1*y1*z0;
    result.w = x0*y0*z0 + x1*y1*z1;

    return result;
}

// Get the Euler angles equivalent to quaternion (roll, pitch, yaw)
// NOTE: Angles are returned in a RaylibVector3 struct in radians
RAYLIB_RMAPI RaylibVector3 RaylibQuaternionToEuler(Quaternion q)
{
    RaylibVector3 result = { 0 };

    // Roll (x-axis rotation)
    float x0 = 2.0f*(q.w*q.x + q.y*q.z);
    float x1 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
    result.x = atan2f(x0, x1);

    // Pitch (y-axis rotation)
    float y0 = 2.0f*(q.w*q.y - q.z*q.x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = asinf(y0);

    // Yaw (z-axis rotation)
    float z0 = 2.0f*(q.w*q.z + q.x*q.y);
    float z1 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
    result.z = atan2f(z0, z1);

    return result;
}

// RaylibTransform a quaternion given a transformation matrix
RAYLIB_RMAPI Quaternion RaylibQuaternionTransform(Quaternion q, RaylibMatrix mat)
{
    Quaternion result = { 0 };

    result.x = mat.m0*q.x + mat.m4*q.y + mat.m8*q.z + mat.m12*q.w;
    result.y = mat.m1*q.x + mat.m5*q.y + mat.m9*q.z + mat.m13*q.w;
    result.z = mat.m2*q.x + mat.m6*q.y + mat.m10*q.z + mat.m14*q.w;
    result.w = mat.m3*q.x + mat.m7*q.y + mat.m11*q.z + mat.m15*q.w;

    return result;
}

// Check whether two given quaternions are almost equal
RAYLIB_RMAPI int RaylibQuaternionEquals(Quaternion p, Quaternion q)
{
#if !defined(RAYLIB_EPSILON)
    #define RAYLIB_EPSILON 0.000001f
#endif

    int result = (((fabsf(p.x - q.x)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                  ((fabsf(p.y - q.y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                  ((fabsf(p.z - q.z)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
                  ((fabsf(p.w - q.w)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))))) ||
                 (((fabsf(p.x + q.x)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
                  ((fabsf(p.y + q.y)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
                  ((fabsf(p.z + q.z)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
                  ((fabsf(p.w + q.w)) <= (RAYLIB_EPSILON*fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w))))));

    return result;
}

// Decompose a transformation matrix into its rotational, translational and scaling components
RAYLIB_RMAPI void RaylibMatrixDecompose(RaylibMatrix mat, RaylibVector3 *translation, Quaternion *rotation, RaylibVector3 *scale)
{
    // Extract translation.
    translation->x = mat.m12;
    translation->y = mat.m13;
    translation->z = mat.m14;

    // Extract upper-left for determinant computation
    const float a = mat.m0;
    const float b = mat.m4;
    const float c = mat.m8;
    const float d = mat.m1;
    const float e = mat.m5;
    const float f = mat.m9;
    const float g = mat.m2;
    const float h = mat.m6;
    const float i = mat.m10;
    const float A = e*i - f*h;
    const float B = f*g - d*i;
    const float C = d*h - e*g;

    // Extract scale
    const float det = a*A + b*B + c*C;
    RaylibVector3 abc = { a, b, c };
    RaylibVector3 def = { d, e, f };
    RaylibVector3 ghi = { g, h, i };

    float scalex = RaylibVector3Length(abc);
    float scaley = RaylibVector3Length(def);
    float scalez = RaylibVector3Length(ghi);
    RaylibVector3 s = { scalex, scaley, scalez };

    if (det < 0) s = RaylibVector3Negate(s);

    *scale = s;

    // Remove scale from the matrix if it is not close to zero
    RaylibMatrix clone = mat;
    if (!RaylibFloatEquals(det, 0))
    {
        clone.m0 /= s.x;
        clone.m4 /= s.x;
        clone.m8 /= s.x;
        clone.m1 /= s.y;
        clone.m5 /= s.y;
        clone.m9 /= s.y;
        clone.m2 /= s.z;
        clone.m6 /= s.z;
        clone.m10 /= s.z;

        // Extract rotation
        *rotation = RaylibQuaternionFromMatrix(clone);
    }
    else
    {
        // Set to identity if close to zero
        *rotation = RaylibQuaternionIdentity();
    }
}

#if defined(__cplusplus) && !defined(RAYMATH_DISABLE_CPP_OPERATORS)

// Optional C++ math operators
//-------------------------------------------------------------------------------

// RaylibVector2 operators
static constexpr RaylibVector2 Vector2Zeros = { 0, 0 };
static constexpr RaylibVector2 Vector2Ones = { 1, 1 };
static constexpr RaylibVector2 Vector2UnitX = { 1, 0 };
static constexpr RaylibVector2 Vector2UnitY = { 0, 1 };

inline RaylibVector2 operator + (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return RaylibVector2Add(lhs, rhs);
}

inline const RaylibVector2& operator += (RaylibVector2& lhs, const RaylibVector2& rhs)
{
    lhs = RaylibVector2Add(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator - (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return RaylibVector2Subtract(lhs, rhs);
}

inline const RaylibVector2& operator -= (RaylibVector2& lhs, const RaylibVector2& rhs)
{
    lhs = RaylibVector2Subtract(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator * (const RaylibVector2& lhs, const float& rhs)
{
    return RaylibVector2Scale(lhs, rhs);
}

inline const RaylibVector2& operator *= (RaylibVector2& lhs, const float& rhs)
{
    lhs = RaylibVector2Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator * (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return RaylibVector2Multiply(lhs, rhs);
}

inline const RaylibVector2& operator *= (RaylibVector2& lhs, const RaylibVector2& rhs)
{
    lhs = RaylibVector2Multiply(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator * (const RaylibVector2& lhs, const RaylibMatrix& rhs)
{
    return RaylibVector2Transform(lhs, rhs);
}

inline const RaylibVector2& operator -= (RaylibVector2& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibVector2Transform(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator / (const RaylibVector2& lhs, const float& rhs)
{
    return RaylibVector2Scale(lhs, 1.0f / rhs);
}

inline const RaylibVector2& operator /= (RaylibVector2& lhs, const float& rhs)
{
    lhs = RaylibVector2Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector2 operator / (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return RaylibVector2Divide(lhs, rhs);
}

inline const RaylibVector2& operator /= (RaylibVector2& lhs, const RaylibVector2& rhs)
{
    lhs = RaylibVector2Divide(lhs, rhs);
    return lhs;
}

inline bool operator == (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return RaylibFloatEquals(lhs.x, rhs.x) && RaylibFloatEquals(lhs.y, rhs.y);
}

inline bool operator != (const RaylibVector2& lhs, const RaylibVector2& rhs)
{
    return !RaylibFloatEquals(lhs.x, rhs.x) || !RaylibFloatEquals(lhs.y, rhs.y);
}

// RaylibVector3 operators
static constexpr RaylibVector3 Vector3Zeros = { 0, 0, 0 };
static constexpr RaylibVector3 Vector3Ones = { 1, 1, 1 };
static constexpr RaylibVector3 Vector3UnitX = { 1, 0, 0 };
static constexpr RaylibVector3 Vector3UnitY = { 0, 1, 0 };
static constexpr RaylibVector3 Vector3UnitZ = { 0, 0, 1 };

inline RaylibVector3 operator + (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return RaylibVector3Add(lhs, rhs);
}

inline const RaylibVector3& operator += (RaylibVector3& lhs, const RaylibVector3& rhs)
{
    lhs = RaylibVector3Add(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator - (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return RaylibVector3Subtract(lhs, rhs);
}

inline const RaylibVector3& operator -= (RaylibVector3& lhs, const RaylibVector3& rhs)
{
    lhs = RaylibVector3Subtract(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator * (const RaylibVector3& lhs, const float& rhs)
{
    return RaylibVector3Scale(lhs, rhs);
}

inline const RaylibVector3& operator *= (RaylibVector3& lhs, const float& rhs)
{
    lhs = RaylibVector3Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator * (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return RaylibVector3Multiply(lhs, rhs);
}

inline const RaylibVector3& operator *= (RaylibVector3& lhs, const RaylibVector3& rhs)
{
    lhs = RaylibVector3Multiply(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator * (const RaylibVector3& lhs, const RaylibMatrix& rhs)
{
    return RaylibVector3Transform(lhs, rhs);
}

inline const RaylibVector3& operator -= (RaylibVector3& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibVector3Transform(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator / (const RaylibVector3& lhs, const float& rhs)
{
    return RaylibVector3Scale(lhs, 1.0f / rhs);
}

inline const RaylibVector3& operator /= (RaylibVector3& lhs, const float& rhs)
{
    lhs = RaylibVector3Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector3 operator / (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return RaylibVector3Divide(lhs, rhs);
}

inline const RaylibVector3& operator /= (RaylibVector3& lhs, const RaylibVector3& rhs)
{
    lhs = RaylibVector3Divide(lhs, rhs);
    return lhs;
}

inline bool operator == (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return RaylibFloatEquals(lhs.x, rhs.x) && RaylibFloatEquals(lhs.y, rhs.y) && RaylibFloatEquals(lhs.z, rhs.z);
}

inline bool operator != (const RaylibVector3& lhs, const RaylibVector3& rhs)
{
    return !RaylibFloatEquals(lhs.x, rhs.x) || !RaylibFloatEquals(lhs.y, rhs.y) || !RaylibFloatEquals(lhs.z, rhs.z);
}

// RaylibVector4 operators
static constexpr RaylibVector4 Vector4Zeros = { 0, 0, 0, 0 };
static constexpr RaylibVector4 Vector4Ones = { 1, 1, 1, 1 };
static constexpr RaylibVector4 Vector4UnitX = { 1, 0, 0, 0 };
static constexpr RaylibVector4 Vector4UnitY = { 0, 1, 0, 0 };
static constexpr RaylibVector4 Vector4UnitZ = { 0, 0, 1, 0 };
static constexpr RaylibVector4 Vector4UnitW = { 0, 0, 0, 1 };

inline RaylibVector4 operator + (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return RaylibVector4Add(lhs, rhs);
}

inline const RaylibVector4& operator += (RaylibVector4& lhs, const RaylibVector4& rhs)
{
    lhs = RaylibVector4Add(lhs, rhs);
    return lhs;
}

inline RaylibVector4 operator - (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return RaylibVector4Subtract(lhs, rhs);
}

inline const RaylibVector4& operator -= (RaylibVector4& lhs, const RaylibVector4& rhs)
{
    lhs = RaylibVector4Subtract(lhs, rhs);
    return lhs;
}

inline RaylibVector4 operator * (const RaylibVector4& lhs, const float& rhs)
{
    return RaylibVector4Scale(lhs, rhs);
}

inline const RaylibVector4& operator *= (RaylibVector4& lhs, const float& rhs)
{
    lhs = RaylibVector4Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector4 operator * (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return RaylibVector4Multiply(lhs, rhs);
}

inline const RaylibVector4& operator *= (RaylibVector4& lhs, const RaylibVector4& rhs)
{
    lhs = RaylibVector4Multiply(lhs, rhs);
    return lhs;
}

inline RaylibVector4 operator / (const RaylibVector4& lhs, const float& rhs)
{
    return RaylibVector4Scale(lhs, 1.0f / rhs);
}

inline const RaylibVector4& operator /= (RaylibVector4& lhs, const float& rhs)
{
    lhs = RaylibVector4Scale(lhs, rhs);
    return lhs;
}

inline RaylibVector4 operator / (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return RaylibVector4Divide(lhs, rhs);
}

inline const RaylibVector4& operator /= (RaylibVector4& lhs, const RaylibVector4& rhs)
{
    lhs = RaylibVector4Divide(lhs, rhs);
    return lhs;
}

inline bool operator == (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return RaylibFloatEquals(lhs.x, rhs.x) && RaylibFloatEquals(lhs.y, rhs.y) && RaylibFloatEquals(lhs.z, rhs.z) && RaylibFloatEquals(lhs.w, rhs.w);
}

inline bool operator != (const RaylibVector4& lhs, const RaylibVector4& rhs)
{
    return !RaylibFloatEquals(lhs.x, rhs.x) || !RaylibFloatEquals(lhs.y, rhs.y) || !RaylibFloatEquals(lhs.z, rhs.z) || !RaylibFloatEquals(lhs.w, rhs.w);
}

// Quaternion operators
static constexpr Quaternion QuaternionZeros = { 0, 0, 0, 0 };
static constexpr Quaternion QuaternionOnes = { 1, 1, 1, 1 };
static constexpr Quaternion QuaternionUnitX = { 0, 0, 0, 1 };

inline Quaternion operator + (const Quaternion& lhs, const float& rhs)
{
    return RaylibQuaternionAddValue(lhs, rhs);
}

inline const Quaternion& operator += (Quaternion& lhs, const float& rhs)
{
    lhs = RaylibQuaternionAddValue(lhs, rhs);
    return lhs;
}

inline Quaternion operator - (const Quaternion& lhs, const float& rhs)
{
    return RaylibQuaternionSubtractValue(lhs, rhs);
}

inline const Quaternion& operator -= (Quaternion& lhs, const float& rhs)
{
    lhs = RaylibQuaternionSubtractValue(lhs, rhs);
    return lhs;
}

inline Quaternion operator * (const Quaternion& lhs, const RaylibMatrix& rhs)
{
    return RaylibQuaternionTransform(lhs, rhs);
}

inline const Quaternion& operator *= (Quaternion& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibQuaternionTransform(lhs, rhs);
    return lhs;
}

// RaylibMatrix operators
inline RaylibMatrix operator + (const RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    return RaylibMatrixAdd(lhs, rhs);
}

inline const RaylibMatrix& operator += (RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibMatrixAdd(lhs, rhs);
    return lhs;
}

inline RaylibMatrix operator - (const RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    return RaylibMatrixSubtract(lhs, rhs);
}

inline const RaylibMatrix& operator -= (RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibMatrixSubtract(lhs, rhs);
    return lhs;
}

inline RaylibMatrix operator * (const RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    return RaylibMatrixMultiply(lhs, rhs);
}

inline const RaylibMatrix& operator *= (RaylibMatrix& lhs, const RaylibMatrix& rhs)
{
    lhs = RaylibMatrixMultiply(lhs, rhs);
    return lhs;
}
//-------------------------------------------------------------------------------
#endif  // C++ operators

#endif  // RAYLIB_RAYMATH_H
