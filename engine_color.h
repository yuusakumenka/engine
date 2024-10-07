/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

static const int DEFAULT_COLORS[] = {
    I32_R8G8B8A8_HEX(0xcfa730),
    I32_R8G8B8A8_HEX(0x735f8b),
    I32_R8G8B8A8_HEX(0x325e7a),
    I32_R8G8B8A8_HEX(0xcb869b),
    I32_R8G8B8A8_HEX(0xe8826c),
    I32_R8G8B8A8_HEX(0xb8dcc9),
    I32_R8G8B8A8_HEX(0xf1d897),
    I32_R8G8B8A8_HEX(0xf1e8d8),
};
#define NUM_DEFAULT_COLORS (sizeof(DEFAULT_COLORS) / sizeof(DEFAULT_COLORS[0]))

#define C32_HEX(c)  ((((c##FF) >> 24) & 0x000000FF) | (((c##FF) >> 8) & 0x0000FF00) | (((c##FF) << 8) & 0x00FF0000) | (((c##FF) << 24) & 0xFF000000))

typedef I32 Color32;
typedef Color32 C32;
typedef __m128i V4C32;

inline C32 c32(float r, float g, float b, float a) {
    return
        (int)(r * 255)       | 
        (int)(g * 255) << 8  |
        (int)(b * 255) << 16 |
        (int)(a * 255) << 24;
}

#define C32_BLACK     0xFF000000
#define C32_RED       0xFF0000FF
#define C32_GREEN     0xFF00FF00
#define C32_BLUE      0xFFFF0000
#define C32_WHITE     0xFFFFFFFF
#define C32_MAGENTA   0xFFFF00FF
#define C32_GRAY      0xFF7F7F7F
#define C32_LIGHTGRAY 0xFFBFBFBF
#define C32_DARKGRAY  0xFF3F3F3F
#define C32_ORANGE    0xFF007FFF
#define C32_CYAN      0xFFFFFF00
#define C32_ALPHA_MASK(a) (0x00FFFFFF | (a << 24))

#ifndef C32_PALETTE
static const C32 C32_PALETTE[] = {
    C32_HEX(0xcfa730),
    C32_HEX(0xf1e8d8),
    C32_HEX(0x325e7a),
    C32_HEX(0xf1d897),
    C32_HEX(0xe8826c),
    C32_HEX(0xb8dcc9),
    C32_HEX(0xcb869b),
    C32_HEX(0x735f8b),
};
#define C32_PALETTE(id) (C32_PALETTE[(id) & (sizeof(C32_PALETTE) / sizeof(C32_PALETTE[0]) - 1)])
#endif

#ifndef C32_PALETTE_BG
static const C32 C32_PALETTE_BG[] = {
    C32_HEX(0x211811),
    C32_HEX(0xe5b892),
    C32_HEX(0x131b21),
    C32_HEX(0x7690a0),
    C32_HEX(0x21120f),
    C32_HEX(0xe8826c),
    C32_HEX(0x17211c),
    C32_HEX(0xb8dcc9),
};
#define C32_PALETTE_BG(id) (C32_PALETTE_BG[(id) & (sizeof(C32_PALETTE_BG) / sizeof(C32_PALETTE_BG[0]) - 1)])
#endif

#ifndef C32_PALETTE_FG
static const C32 C32_PALETTE_FG[] = {
    C32_HEX(0xe5b892),
    C32_HEX(0x211811),
    C32_HEX(0x7690a0),
    C32_HEX(0x131b21),
    C32_HEX(0xe8826c),
    C32_HEX(0x21120f),
    C32_HEX(0xb8dcc9),
    C32_HEX(0x17211c),
};
#define C32_PALETTE_FG(id) (C32_PALETTE_FG[(id) & (sizeof(C32_PALETTE_FG) / sizeof(C32_PALETTE_FG[0]) - 1)])
#endif

#define C32_R(color) (((unsigned char*)&(color)) + 0)
#define C32_G(color) (((unsigned char*)&(color)) + 1)
#define C32_B(color) (((unsigned char*)&(color)) + 2)
#define C32_A(color) (((unsigned char*)&(color)) + 3)

C32 c32_brighten(C32 color, F32 intensity) {
    C32 brightened = color;
    F32 r = *C32_R(brightened) * (1.f / 255.f);
    F32 g = *C32_G(brightened) * (1.f / 255.f);
    F32 b = *C32_B(brightened) * (1.f / 255.f);
    r = r + (1 - r) * intensity;
    g = g + (1 - g) * intensity;
    b = b + (1 - b) * intensity;
    *C32_R(brightened) = r * 255.f;
    *C32_G(brightened) = g * 255.f;
    *C32_B(brightened) = b * 255.f;
    return brightened;
}

C32 c32_darken(C32 color, F32 intensity) {
    C32 darkened = color;
    F32 r = *C32_R(darkened) * (1.f / 255.f);
    F32 g = *C32_G(darkened) * (1.f / 255.f);
    F32 b = *C32_B(darkened) * (1.f / 255.f);
    r = r * (1.f - intensity);
    g = g * (1.f - intensity);
    b = b * (1.f - intensity);
    *C32_R(darkened) = r * 255.f;
    *C32_G(darkened) = g * 255.f;
    *C32_B(darkened) = b * 255.f;
    return darkened;
}

C32 c32_hide(C32 color, F32 intensity) {
    C32 hidden = color;
    F32 a = *C32_A(hidden) * (1.f / 255.f);
    a = a * (1.f - intensity);
    *C32_A(hidden) = a * 255.f;
    return hidden;
}