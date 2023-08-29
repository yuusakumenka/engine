/* #include <string.h> (strlen) */

typedef struct String8 {
    unsigned long long length;
    char              *data;
} String8;

typedef String8 S8;

inline S8 s8(unsigned long long length, char *data) {
    S8 string;
    string.length = length;
    string.data   = data;
    return string;
}

#define S8_LIT(string_literal) (s8((sizeof(string_literal) - 1), (string_literal)))

#define S8_EMPTY (s8(0, ""))

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

/* Returns the unescaped version of given string, 
 * by overriding the underlying memory with it's unescaped version. */
inline S8 s8_unescape_override(S8 s8);

/* Returns the unescaped version of given string.
 * Unescaped version is writen in given buffer. */
inline S8 s8_unescape(S8 s8, U64 dst_buffer_size, char *dst_buffer);

int s8_equals(S8 l, S8 r) {
    if (l.length != l.length) return 0;
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