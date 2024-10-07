/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

//#ifndef ENGINE_TYPES_H
//#define ENGINE_TYPES_H
// # Types

#ifdef __INTELLISENSE__ 
typedef unsigned long long size_t;
typedef unsigned long long ptrdiff_t;
#endif

typedef unsigned char               Unsigned_Integer_8bit ;
typedef unsigned short              Unsigned_Integer_16bit;
typedef unsigned int                Unsigned_Integer_32bit;
typedef unsigned long long          Unsigned_Integer_64bit;
typedef size_t                      Unsigned_Integer      ;

typedef unsigned char               Signed_Integer_8bit ;
typedef unsigned short              Signed_Integer_16bit;
typedef unsigned int                Signed_Integer_32bit;
typedef unsigned long long          Signed_Integer_64bit;
typedef ptrdiff_t                   Signed_Integer      ;

typedef float                       Floating_Point_Iee754_32bit;
typedef double                      Floating_Point_Iee754_64bit;
typedef float                       Floating_Point;

typedef Unsigned_Integer_8bit       U8;
typedef Unsigned_Integer_16bit      U16;
typedef Unsigned_Integer_32bit      U32 ;
typedef Unsigned_Integer_64bit      U64;
typedef Unsigned_Integer            Uint;

typedef Signed_Integer_8bit         I8 ;
typedef Signed_Integer_16bit        I16;
typedef Signed_Integer_32bit        I32;
typedef Signed_Integer_64bit        I64;
typedef Signed_Integer              Int;

typedef Floating_Point_Iee754_32bit F32;
typedef Floating_Point_Iee754_64bit F64;
typedef Floating_Point              Float;

typedef Unsigned_Integer_8bit       B8;
typedef Unsigned_Integer_16bit      B16;
typedef Unsigned_Integer_32bit      B32;
typedef Unsigned_Integer_64bit      B64;
typedef Unsigned_Integer            Bool;

typedef struct Vector2d_I16 { I16 x; I16 y; } Vector2d_I16;
typedef struct Vector2d_I32 { I32 x; I32 y; } Vector2d_I32;
typedef struct Vector2d_F32 { F32 x; F32 y; } Vector2d_F32;

typedef Vector2d_I16 V2I16;
typedef Vector2d_I32 V2I32;
typedef Vector2d_F32 V2F32;

typedef V2F32 V2;

// ## Special values
// ### Naturals
#define  U8_MAX ((U8) 0xFF)
#define U16_MAX ((U16)0xFFFF)
#define U32_MAX ((U32)0xFFFFFFFF)
#define U64_MAX ((U32)0xFFFFFFFFFFFFFFFF)
// ### Integers
#define  I8_MIN ((I8) 0x80)
#define  I8_MAX ((I8) 0x7F)
#define I16_MAX ((I16)0x7FFF)
#define I16_MIN ((I16)0x8000)
#define I32_MIN ((I32)0x80000000)
#define I32_MAX ((I32)0x7FFFFFFF)
#define I64_MIN ((I64)0x8000000000000000)
#define I64_MAX ((I64)0x7FFFFFFFFFFFFFFF)
#define I32_R8G8B8A8(r, g, b, a) ((r) | (g) << 8 | (b) << 16 | (a) << 24)
#define I32_R8G8B8A8_HEX(c)  ((((c##FF) >> 24) & 0x000000FF) | (((c##FF) >> 8) & 0x0000FF00) | (((c##FF) << 8) & 0x00FF0000) | (((c##FF) << 24) & 0xFF000000))


// ### Rationals
#define F32_EPSILON            1.192092896e-07F
#define F32_INFINITY           ((F32)0x1p256)
#define F32_MIN                1.175494351e-38F
#define F32_MAX                3.402823466e+38F
//#define F32_SIGN_BIT           (-0.0f)
#define F32_SIGN_BIT           ((U32)0x80000000)
#define F32_EXPONENT_BIT_MASK  0x7F800000
#define F32_FRACTION_BIT_MASK  0x007FFFFF
#define F32_EXPONENT_BIT_SHIFT 23
// ### Booleans
#define B8_FALSE 0
#define B8_TRUE  1
#define FALSE 0
#define TRUE  1

// Units
#define KiB(scalar) (scalar << 10)
#define MiB(scalar) (scalar << 20)
#define GiB(scalar) (scalar << 30)
//#endif