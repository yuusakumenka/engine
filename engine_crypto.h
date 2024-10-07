/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

I32 i32_hash(I32 src) {
    src ^= src >> 11;
    src ^= src << 17;
    src ^= src >> 5;
    src *= 0x543EF21C;
    return src;
}

I32 i32_hash_murmur(I32 src) {
    const I32 M = 0x5bd1e995;
    I32 h = 1190494759;
    src *= M; src ^= src>>24; src *= M;
    h *= M; h ^= src;
    h ^= h>>13; h *= M; h ^= h>>15;
    return h;
}
// F32 f32_from_i32(I32 in) {
//     F32 f;
//     *((unsigned int*)(&f)) = (in >> 9) | 0x3F0000000u;
//     f = f * 2.f - 1.f;
//     return f;
// }
F32 f32_from_i32(I32 src) {
    F32 f;
    *((int*)(&f)) = src & 0x007fffff | 0x3f800000;
    return f - 1.f;
}