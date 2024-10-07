/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* #include <string.h> (strlen) */

typedef struct String8 {
    size_t length;
    char  *data;
} String8;

typedef String8 S8;

#define S8_BRACE_INIT(string_literal) { sizeof(string_literal) - 1, string_literal }

inline S8 s8(size_t length, char *data) {
    S8 string;
    string.length = length;
    string.data   = data;
    return string;
}


#define S8_LIT(string_literal) (s8((sizeof(string_literal) - 1), (string_literal)))
#define S8(string_literal) (s8((sizeof(string_literal) - 1), (string_literal)))

static const S8 S8_EMPTY = S8_BRACE_INIT("");

#define S8_FARG(s) (s).length, (s).data
#define S8_FARGS(s) (s).length, (s).data

inline S8 s8_cstr(char *cstr) {
    S8 string;
    string.data   = cstr;
    string.length = strlen(cstr);
    return string;
}

/* Return the same string with the left and right quotes removed */
inline S8 s8_unescape_cheap(S8 s8) {
    s8.length -= 2;
    s8.data   += 1;
    return s8;
}

inline S8 s8_eat_left(S8 s, size_t num_char) {
    s.length -= num_char;
    s.data   += num_char;
    return s;
}

/* Returns the unescaped version of given string, 
 * by overriding the underlying memory with it's unescaped version. */
inline S8 s8_unescape_override(S8 s8);

/* Returns the unescaped version of given string.
 * Unescaped version is writen in given buffer. */
inline S8 s8_unescape(S8 s8, U64 dst_buffer_size, char *dst_buffer);

int s8_equals(S8 l, S8 r) {
    if (l.length != r.length)
        return 0;
    return !strncmp(l.data, r.data, r.length);
}

#define S8_EQUALS_LIT(string8, string_literal) (s8_equals(string8, S8_LIT(string_literal)))

int s8_ends_by(S8 l, S8 r) {
    if (l.length < r.length) return 0;
    return !strncmp(&l.data[l.length - r.length], r.data, r.length);
}

#define S8_ENDS_BY_LIT(string8, string_literal) (string8.length < (sizeof(string_literal) - 1) ? 0 : (!strncmp(&a.data[a.length - (N - 1)], string_litteral, sizeof(string_literal) - 1)))

int s8_starts_by(S8 l, S8 r) {
    if (l.length < r.length) return 0;
    return !strncmp(l.data, r.data, r.length);
}

int s8_parse_int(S8 string) { 
    int total = 0;
    int power_of_10 = 1;
    for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            total = -total;
            break;
        }
        if(character == '+')
            break;
        total += (character - '0') * power_of_10;        
        power_of_10 *= 10;
    }
    return total;
}

signed long long s8_parse_s64(S8 string) { 
    signed long long total = 0;
    int power_of_10 = 1;
    for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            total = -total;
            break;
        }
        if(character == '+')
            break;
        total += (character - '0') * power_of_10;        
        power_of_10 *= 10;
    }
    return total;
}

unsigned long long s8_parse_u64(S8 string) { 
    unsigned long long total = 0;
    int power_of_10 = 1;
    for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
        char character = string.data[char_index];
        total += (character - '0') * power_of_10;        
        power_of_10 *= 10;
    }
    return total;
}

void s8_parse_space_separated_u16(S8 string, U16 *dst) { 
    U16 total = 0;
    for (U64 char_index = 0; char_index != string.length; ++char_index) {
        char character = string.data[char_index];
        if(character == ' ') {
            /* Submit */
            *(dst++) = total;
            total = 0;
            continue;
        }
        total *= 10;
        total += (character - '0');        
    }
    *(dst++) = total;
    return;
}

float s8_parse_float(S8 string) { 
    int characteristic = 0;
    int mantissa;
    int inverse_power_of_10;
    int power_of_10 = 1;
    for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            characteristic = -characteristic;
            break;
        }
        if(character == '+')
            break;
        if(character == '.' || character == ',') {
            mantissa = characteristic;
            inverse_power_of_10 = power_of_10;
            power_of_10 = 0;
            characteristic = 0;
        }
        characteristic += (character - '0') * power_of_10;      
        power_of_10 *= 10;
    }
    return (float)characteristic + ((float)mantissa / (float)inverse_power_of_10);
}

/*
// float s8_parse_float_forward(S8 string) { 
//     int characteristic = 0;
//     int mantissa = 0;
//     int inverse_power_of_10;
//     //int power_of_10 = 1;
//     int sign = 1;
//     int *cumul = &characteristic;
//     for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
//         char character = string.data[char_index];
//         if(character == '-') {
//             sign = -1;
//             continue;
//         }
//         if(character == '+')
//             continue;
//         if(character == '.' || character == ',') {
//             //characteristic = cumul;
//             inverse_power_of_10 = 1;
//             //power_of_10 = 0;
//             //characteristic = 0;
//             cumul = &mantissa;
//             continue;
//         }
//         cumul *= 10;        
//         cumul += (character - '0');
//         inverse_power_of_10 = inverse_power_of_10 * 10;
//     }

//     return (float)(characteristic + ((float)mantissa / (float)inverse_power_of_10);
// }
*/

void s8_parse_space_separated_f32_reverse(S8 string, F32* dst) { 
    int characteristic = 0;
    int mantissa = 0;
    int inverse_power_of_10;
    int power_of_10 = 1;
    for (int char_index = string.length - 1; char_index != (int)-1; --char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            characteristic = -characteristic;
            continue;
        }
        if(character == '+')
            continue;
        if(character == ' ')
            goto SUBMIT;
        if(character == '.' || character == ',') {
            mantissa = characteristic;
            inverse_power_of_10 = power_of_10;
            power_of_10 = 0;
            characteristic = 0;
            continue;
        }
        characteristic += (character - '0') * power_of_10;      
        power_of_10 *= 10;
        continue;

        SUBMIT:;
        *(dst++) = (F32)characteristic + ((F32)mantissa / (F32)inverse_power_of_10);
        characteristic = 0;
        mantissa = 0;
        power_of_10 = 1;
    }
    *(dst++) = (F32)characteristic + ((F32)mantissa / (F32)inverse_power_of_10);
}

void s8_parse_n_space_separated_f32(S8 string, size_t num_f32_to_parse, F32* dst) {
    float sign                         = +1.0f;
    float absolute_value               = 0.0f;
    float mantissa_inverse_power_of_10 = 0.1f;
    int   done_with_characteristic     = 0;
    int   doing_exponent               = 0;
    size_t parsed_f32_index            = 0;
    for (int char_index = 0; char_index != string.length; ++char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            if(doing_exponent) {
                continue;
            } else {
                sign = -1.0f;
            }
        } else if(character == '+') {
            sign = +1.0f;
        } else if(character == ' ') {
            if(doing_exponent) {
               *(dst++) = copysignf(absolute_value, sign) * 0.0001;
            } else {
                *(dst++) = copysignf(absolute_value, sign);
            }
            sign                         = +1.0f;
            absolute_value               = 0.0f;
            done_with_characteristic     = 0;
            mantissa_inverse_power_of_10 = 0.1f;
            doing_exponent = 0;
        } else if(character == '.' || character == ',') {
            done_with_characteristic = 1;
        } else if(character == 'e') {
            doing_exponent = 1;
        } else if(doing_exponent) {
            //exponent = (character - '0');
        } else if(!done_with_characteristic) {
            absolute_value *= 10.0f;
            absolute_value += (float)(character - '0');
        } else {
            absolute_value += (float)(character - '0') * mantissa_inverse_power_of_10;
            mantissa_inverse_power_of_10 *= 0.1f;
        }
    }
    if(doing_exponent) {
       *(dst++) = copysignf(absolute_value, sign) * 0.0001;
    } else {
        *(dst++) = copysignf(absolute_value, sign);
    }
}

void s8_parse_space_separated_f32(S8 string, F32* dst) {
    float sign                         = +1.0f;
    float absolute_value               = 0.0f;
    float mantissa_inverse_power_of_10 = 0.1f;
    int   done_with_characteristic     = 0;
    int   doing_exponent               = 0;
    for (int char_index = 0; char_index != string.length; ++char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            if(doing_exponent) {
                continue;
            } else {
                sign = -1.0f;
            }
        } else if(character == '+') {
            sign = +1.0f;
        } else if(character == ' ') {
            if(doing_exponent) {
               *(dst++) = copysignf(absolute_value, sign) * 0.0001;
            } else {
                *(dst++) = copysignf(absolute_value, sign);
            }
            sign                         = +1.0f;
            absolute_value               = 0.0f;
            done_with_characteristic     = 0;
            mantissa_inverse_power_of_10 = 0.1f;
            doing_exponent = 0;
        } else if(character == '.' || character == ',') {
            done_with_characteristic = 1;
        } else if(character == 'e') {
            doing_exponent = 1;
        } else if(doing_exponent) {
            //exponent = (character - '0');
        } else if(!done_with_characteristic) {
            absolute_value *= 10.0f;
            absolute_value += (float)(character - '0');
        } else {
            absolute_value += (float)(character - '0') * mantissa_inverse_power_of_10;
            mantissa_inverse_power_of_10 *= 0.1f;
        }
    }
    if(doing_exponent) {
       *(dst++) = copysignf(absolute_value, sign) * 0.0001;
    } else {
        *(dst++) = copysignf(absolute_value, sign);
    }
}

S8 s8_parse_space_separated_f32_ex(
    S8 string, 
    F32* dst, 
    size_t num_f32_to_parse_max, 
    size_t num_f32_to_parse_before_applying_stride, 
    ptrdiff_t stride
) {
    if(!num_f32_to_parse_max)
        return string;
    float sign                         = +1.0f;
    float absolute_value               = 0.0f;
    float mantissa_inverse_power_of_10 = 0.1f;
    int   done_with_characteristic     = 0;
    int   doing_exponent               = 0;
    size_t chain = 0;
    size_t num_f32_parsed = 0; 
    for (int char_index = 0; char_index != string.length; ++char_index) {
        char character = string.data[char_index];
        if(character == '-') {
            if(doing_exponent) {
                continue;
            } else {
                sign = -1.0f;
            }
        } else if(character == '+') {
            sign = +1.0f;
        } else if(character == ' ') {
            if(doing_exponent) {
                *dst = copysignf(absolute_value, sign) * 0.0001;
            } else {
                *dst = copysignf(absolute_value, sign);
            }
            ++num_f32_parsed;
            if(num_f32_parsed == num_f32_to_parse_max) {
                return s8(string.length - (char_index + 1), &string.data[char_index + 1]);
            }
            ++chain;
            size_t n = num_f32_to_parse_before_applying_stride;
            if(chain == num_f32_to_parse_before_applying_stride) {
                dst -= (num_f32_to_parse_before_applying_stride - 1);
                dst = (F32*)(((char*)dst) + stride);
                chain = 0;
            } else {
                ++dst;
            }
            sign                         = +1.0f;
            absolute_value               = 0.0f;
            done_with_characteristic     = 0;
            mantissa_inverse_power_of_10 = 0.1f;
            doing_exponent = 0;
        } else if(character == '.' || character == ',') {
            done_with_characteristic = 1;
        } else if(character == 'e') {
            doing_exponent = 1;
        } else if(doing_exponent) {
            //exponent = (character - '0');
        } else if(!done_with_characteristic) {
            absolute_value *= 10.0f;
            absolute_value += (float)(character - '0');
        } else {
            absolute_value += (float)(character - '0') * mantissa_inverse_power_of_10;
            mantissa_inverse_power_of_10 *= 0.1f;
        }
    }
    if(doing_exponent) {
        *dst = copysignf(absolute_value, sign) * 0.0001;
    } else {
        *dst = copysignf(absolute_value, sign);
    }
    return S8_EMPTY;
}

void utf8_to_utf16(
    size_t *dst_utf16_length,
    short  *dst_utf16,
    size_t  src_utf8_length,
    char   *src_utf8
) {
    if (dst_utf16) {
        size_t dst_index = 0;
        for(size_t src_index = 0; src_index != src_utf8_length;) {
            size_t codepoint_first_byte = src_utf8[src_index];
            size_t codepoint;
            if ((codepoint_first_byte & 0x80) == 0x00) {
                /* 1 byte -> 1 short */
                codepoint = codepoint_first_byte;
                src_index += 1;
            } else if ((codepoint_first_byte & 0xE0) == 0xC0) {
                /* 2 bytes -> 1 short */
                codepoint =
                    ((codepoint_first_byte & 0x1F) << 6) |
                    (dst_utf16[src_index + 1] & 0x3F);
                src_index += 2;
            } else if ((codepoint_first_byte & 0xF0) == 0xE0) {
                /* 3 bytes -> 2 shorts */
                codepoint =
                    ((codepoint_first_byte     & 0x1F) << 12) |
                    ((dst_utf16[src_index + 1] & 0x3F) <<  6) |
                     (dst_utf16[src_index + 2] & 0x3F);
                src_index += 3;
            } else {
                /* 4 bytes -> 2 shorts */
                codepoint =
                    ((codepoint_first_byte     & 0x1F) << 18) |
                    ((dst_utf16[src_index + 1] & 0x3F) << 12) |
                    ((dst_utf16[src_index + 2] & 0x3F) <<  6) |
                     (dst_utf16[src_index + 3] & 0x3F);
                src_index += 4;
            }
            if (codepoint < 0xE000) {
                dst_utf16[dst_index + 0] = (short)codepoint;
                dst_index += 1;
            } else {
                dst_utf16[dst_index + 0] = (short)(0xD800 | (codepoint >> 10));
                dst_utf16[dst_index + 1] = (short)(0xDC00 | (codepoint & 0x03FF));
                dst_index += 2;
            }
        }
        if (dst_utf16_length)
            *dst_utf16_length = dst_index;
    } else if (dst_utf16_length) {
        size_t dst_index = 0;
        for(size_t src_index = 0; src_index != src_utf8_length;) {
            size_t codepoint_first_byte = src_utf8[src_index];
            size_t codepoint;
            if (codepoint_first_byte & 0x80 == 0x00) {
                /* 1 byte -> 1 short */
                codepoint = codepoint_first_byte;
                src_index += 1;
            } else if (codepoint_first_byte & 0xE0 == 0xC0) {
                /* 2 bytes -> 1 short */
                codepoint =
                    ((codepoint_first_byte & 0x1F) << 6) |
                    (dst_utf16[src_index + 1] & 0x3F);
                src_index += 2;
            } else if (codepoint_first_byte & 0xF0 == 0xE0) {
                /* 3 bytes -> 2 shorts */
                codepoint =
                    ((codepoint_first_byte     & 0x1F) << 12) |
                    ((dst_utf16[src_index + 1] & 0x3F) <<  6) |
                     (dst_utf16[src_index + 2] & 0x3F);
                src_index += 3;
            } else {
                /* 4 bytes -> 2 shorts */
                codepoint =
                    ((codepoint_first_byte     & 0x1F) << 18) |
                    ((dst_utf16[src_index + 1] & 0x3F) << 12) |
                    ((dst_utf16[src_index + 2] & 0x3F) <<  6) |
                     (dst_utf16[src_index + 3] & 0x3F);
                src_index += 4;
            }
            if (codepoint < 0xE000) {
                dst_index += 1;
            } else {
                dst_index += 2;
            }
        }
        *dst_utf16_length = dst_index;
    }
}

typedef struct String16 {
    size_t length;
    short *data;
} String16;

typedef String16 S16;

// S16 s8_to_s16(Arena *dst_arena, S8 src_s8) {

// }