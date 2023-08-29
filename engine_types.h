// # Types
// ## Naturals
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32 ;
typedef unsigned long long U64;
typedef          U64       Uint;
// ## Integers
typedef signed   char      I8 ;
typedef signed   short     I16;
typedef signed   int       I32;
typedef signed   long long I64;
typedef          I64       Int;
// ## Rationals
typedef          float     F32;
typedef          double    F64;
typedef          F32       Float;
// ## Booleans
typedef U8  B8;
typedef U16 B16;
typedef U32 B32;
typedef U64 B64;
typedef B64 Bool;

// ## Special values
// ### Naturals
#define  U8_MAX ((U8) 0xFF)
#define U16_MAX ((U16)0xFFFF
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