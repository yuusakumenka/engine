/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define F32_PI         3.1415927410125732421875f
#define F32_DEG_TO_RAD 0.01745329238474369049072265625f
#define F32_RAD_TO_DEG 57.295780181884765625f

F32 f32_inverse_square_root(F32 x) {
    return 1.0f / sqrtf(x);
}

inline F32 move_towards(F32 current, F32 target, F32 max_distance) {
    F32 delta = target - current;
    if (fabsf(delta) <= max_distance) return target;
    return current + copysignf(max_distance, delta);
}
inline F32 move_towards_smooth(F32 current, F32 target, F32 delta_time) {
    return current + (target - current) * (1.0f - powf(0.01f, delta_time));
}
inline F32 lerp(F32 a, F32 b, F32 i) {
    return a * (1 - i) + b * i;
}
inline F32 ease_in (const F32 t) { return t * t; }
inline F32 ease_out(const F32 t) { return t * (2.f - t); }
inline F32 ease_in_out(const F32 t) { return t * t * (3.f - 2.f * t); }
inline F32 ease_in_overshoot (const F32 t, const F32 overshoot) { return t * t * (+1.0f - overshoot) + t * (overshoot + 0.0f); }
inline F32 ease_in_overshoot_smooth(const F32 t, const F32 overshoot) { return t * (t * t * (+1.0f - overshoot) + t * (overshoot + 0.0f)); }
// aka. inverse lerp
inline F32 normalize_to_range(F32 source_range_min, F32 source_range_max, F32 value_in_range) {
    return (value_in_range - source_range_min) / (source_range_max - source_range_min);
}
/* remaps value from source range to target range
 * ```
 *                              __--- target_range_max
 *                          __--   |
 *                      __--       |
 *  source_range_max ---           |
 *                    |           _+ -> return
 *                    |       _ -  |
 *                    |   _ -      |
 *           value -> + -     ___---- target_range_min
 *                    | ___---   
 *  source_range_min ---
 * ``` */
inline F32 remap(
    F32 source_range_min,
    F32 source_range_max,
    F32 target_range_min,
    F32 target_range_max,
    F32 value
) {
    return lerp(
        target_range_min,
        target_range_max,
        normalize_to_range(
            source_range_min,
            source_range_max,
            value
        )
    );
}

/* Parabolic Motion */
void parabolic_motion_init(
    const F32  peak_height,        // the desired peak value, or more formally: the expected evaluated value when parabola's derivative equals zero
    const F32  time_to_reach_peak, // the desired time peak shall be reached, or more formally: the expected argument wich when fed to the parabola should evaluate to the peak value
    // outputs:
          F32* initial_velocity, // the initial velocity required to reach parabola's peak value at given time
          F32* acceleration      // the acceleration constant required to 
) {
    F32 peak_height_times_two = peak_height + peak_height;
    *initial_velocity =  peak_height_times_two / time_to_reach_peak;
    *acceleration     = -peak_height_times_two / (time_to_reach_peak * time_to_reach_peak);
}

F32 parabolic_motion_tick(
    F32* position, 
    F32* velocity, 
    const F32 acceleration, 
    const F32 delta_time
) {
    F32 delta_velocity = acceleration * delta_time;
    F32 delta_position = (*velocity + 0.5f * delta_velocity) * delta_time;
    *position += delta_position;
    *velocity += delta_velocity;
    return delta_position;
}

F32 parabolic_motion_init_tick_decelerating(
          F32* position, 
          F32* velocity, 
    const F32  acceleration,
    const F32  delta_time
) {
    // when decelerating,
    // we prevent time from going beyond parabola's peak
    return parabolic_motion_tick(
        position,
        velocity,
        acceleration,
        fminf(delta_time, -(*velocity / acceleration))
    );
}


// 2d Vector
inline V2 v2(F32 x, F32 y) {
    V2 v;
    v.x = x;
    v.y = y;
    return v;
}
#define V2_ZERO (v2(0 , 0))
inline void v2_set_xy(V2* v2, F32 x, F32 y) {
    v2->x = x;
    v2->y = y;
}
inline void v2_set(V2* dst, V2 src) {
    dst->x = src.x;
    dst->y = src.y;
}
inline V2 v2_neg(V2 v) {
    return v2(-v.x, -v.y);
}
inline V2 v2_add(V2 a, V2 b) {
    return v2(a.x + b.x, a.y + b.y);
}
inline V2 v2_sub(V2 a, V2 b) {
    return v2(a.x - b.x, a.y - b.y);
}
inline V2 v2_sub_f32(V2 a, F32 b) {
    return v2(a.x - b, a.y - b);
}
inline V2 v2_a_to_b(V2 a, V2 b) {
    return v2(b.x - a.x, b.y - a.y);
}
V2 v2_multiply_f32(V2 a, F32 b) {
    return v2(b * a.x, b * a.y);
}
inline V2 v2_mul(V2 a, V2 b) {
    return v2(a.x * b.x, a.y * b.y);
}
inline V2 v2_mul_f32(V2 a, F32 b) {
    return v2(b * a.x, b * a.y);
}
inline V2 v2_div(V2 a, V2 b) {
    return v2(a.x / b.x, a.y / b.y);
}
inline F32 v2_dot(V2 a, V2 b) {
    return a.x * b.x + a.y * b.y;
}
inline F32 v2_magnitude(V2 vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}
inline F32 v2_magnitude_squared(V2 vector) {
    return vector.x * vector.x + vector.y * vector.y;
}
inline V2 v2_normalize(V2 vector) {
    F32 inverse_magnitude = 1.0f / sqrtf(vector.x * vector.x + vector.y * vector.y);
    return v2(vector.x * inverse_magnitude, vector.y * inverse_magnitude);
}
#define v2_normalize_vector v2_normalize
V2  v2_rotate_quarter_pi(const V2 vector);
inline V2  v2_rotate_quarter_pi(const V2 vector) {
    return v2(-vector.y, vector.x);
}
inline V2  v2_rotate_minus_quarter_pi(const V2 vector) {
    return v2(vector.y, -vector.x);
}
inline V2  v2_round(const V2 vector) {
    return v2(roundf(vector.x), roundf(vector.y));
}
inline V2  v2_move_towards_smooth(V2 current, V2 target, F32 time_delta) {
    return v2_add(current, v2_multiply_f32(v2_a_to_b(current, target), 1.0f - powf(0.01f, time_delta)));
}
// Euclidean distance
inline F32 v2_distance(V2 point_a, V2 point_b) {
    F32 delta_x = point_b.x - point_a.x;
    F32 delta_y = point_b.y - point_a.y;
    return sqrtf(delta_x * delta_x + delta_y * delta_y);
}
inline F32 v2_distance_squared(V2 point_a, V2 point_b) {
    F32 delta_x = point_b.x - point_a.x;
    F32 delta_y = point_b.y - point_a.y;
    return delta_x * delta_x + delta_y * delta_y;
}
inline V2  v2_move_towards(V2 current, V2 target, F32 max_distance) {
    V2 delta = v2_a_to_b(current, target);
    if (v2_magnitude(delta) <= max_distance) return target;
    return v2_add(current, v2_multiply_f32(v2_normalize_vector(delta), max_distance));
}

inline V2 v2_lerp(V2 a, V2 b, F32 t) {
    return v2(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    );
}
inline V2 v2_lerp_precise(V2 a, V2 b, F32 t) {
    return v2(
        a.x * (1.0f - t) + b.x * t,
        a.y * (1.0f - t) + b.y * t
    );
}

inline V2 v2_normalize_to_range(V2 source_range_min, V2 source_range_max, V2 value_in_range) {
    return v2_div(v2_sub(value_in_range, source_range_min), v2_sub(source_range_max, source_range_min));
}

V2 v2_clamp_magnitude(V2 vector, F32 max_magnitude) {
    F32 magnitude = v2_magnitude(vector);
    F32 clamped_magnitude = fminf(max_magnitude, magnitude);
    return v2_mul_f32(vector, fmaxf(clamped_magnitude / magnitude, 0));
}

V2 v2_reflect(V2 incident, V2 normal) {
    v2_add(incident, v2_mul_f32(normal, v2_dot(incident, normal) * -2.0f));
}

inline V2 v2_reflect_with_absorbtion(V2 incident, V2 normal, F32 absorbtion) {
    F32 dot = v2_dot(incident, normal);
    return v2_add(incident, v2_mul_f32(normal, fmaxf(0, -dot) * lerp(2.0f, 1.0f, absorbtion)));
}

inline V2 v2_min(V2 l, V2 r) {
    return v2(fminf(l.x, r.x), fminf(l.y, r.y));
}

inline V2 v2_max(V2 l, V2 r) {
    return v2(fmaxf(l.x, r.x), fmaxf(l.y, r.y));
}

/* 3D Vector */

typedef struct V3 {
    F32 x;
    F32 y;
    F32 z;
} V3;

inline V3 v3(F32 x, F32 y, F32 z) {
    V3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

#define V3_ZERO (v3(0.f,0.f,0.f))
#define V3_ONE  (v3(1.f,1.f,1.f))
#ifndef V3_FORWARD
#define V3_FORWARD (v3(0.f,1.f,0.f))
#endif /* !V3_FORWARD */ 

float v3_dot(V3 l, V3 r) {
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

V3 v3_cross(V3 l, V3 r) {
    return v3(
        l.y * r.z - l.z * r.y,
        l.z * r.x - l.x * r.z,
        l.x * r.y - l.y * r.x
    );
}

V3 v3_mul_f32(V3 l, F32 r) {
    return v3(
        l.x * r,
        l.y * r,
        l.z * r
    );
}

V3 v3_add(V3 l, V3 r) {
    return v3(
        l.x + r.x,
        l.y + r.y,
        l.z + r.z
    );
}

V3 v3_sub(V3 l, V3 r) {
    return v3(
        l.x - r.x,
        l.y - r.y,
        l.z - r.z
    );
}

V3 v3_normalize(V3 v) {
    return v3_mul_f32(v, f32_inverse_square_root(v3_dot(v, v)));
}

inline F32 v3_magnitude(V3 vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

inline V3  v3_move_towards(V3 current, V3 target, F32 max_distance) {
    V3 delta = v3_sub(target, current);
    F32 delta_magnitude_squared = v3_dot(delta, delta);
    if (delta_magnitude_squared <= max_distance * max_distance)
        return target;
    V3 delta_normalized = v3_mul_f32(delta, f32_inverse_square_root(delta_magnitude_squared));
    return v3_add(current, v3_mul_f32(delta_normalized, max_distance));
}

inline V3  v3_move_towards_zero(V3 current, F32 max_distance) {
    F32 magnitude_squared = v3_dot(current, current);
    if (magnitude_squared <= max_distance * max_distance)
        return current;
    return v3_mul_f32(current, sqrtf(magnitude_squared) - max_distance);
}



/* 4D Vector */

__declspec(align(16)) typedef struct V4 {
    F32 x;
    F32 y;
    F32 z;
    F32 w;
} V4;

__declspec(align(16)) typedef struct V4_Soa {
    __m128 x;
    __m128 y;
    __m128 z;
    __m128 w;
} V4_Soa;

inline V4 v4(F32 x, F32 y, F32 z, F32 w) {
    V4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}
#define V4_ZERO (v4(0.f, 0.f, 0.f, 0.f))

F32 *v4_x(V4 *v) {
    return &((__m128*)v)->m128_f32[0];
}

F32 *v4_y(V4 *v) {
    return &((__m128*)v)->m128_f32[1];
}

F32 *v4_z(V4 *v) {
    return &((__m128*)v)->m128_f32[2];
}

F32 *v4_w(V4 *v) {
    return &((__m128*)v)->m128_f32[3];
}

F32 v4_dot(V4 l, V4 r) {
    return
        l.x * r.x +
        l.y * r.y +
        l.z * r.z +
        l.w * r.w;
}

V4 v4_mul_f32(V4 l, F32 r) {
    return v4(
        l.x * r,
        l.y * r,
        l.z * r,
        l.w * r
    );
}

V4 v4_normalize(V4 v) {
    v4_mul_f32(v, f32_inverse_square_root(v4_dot(v, v)));
}

// matrices
#pragma region Matrices
typedef struct M3x2 {
    F32 a, b, c;
    F32 d, e, f;
} M3x2;
inline M3x2 m3x2(F32 a, F32 b, F32 c, F32 d, F32 e, F32 f) {
    M3x2 m;
    m.a = a; m.b = b; m.c = c;
    m.d = d; m.e = e; m.f = f;
    return m;
}
#define M3X2_IDENTITY (m3x2(1,0,0,0,1,0))
inline V2 m3x2_mul_point(M3x2 m, V2 v) {
    return v2(m.a * v.x + m.b * v.y + m.c, m.d * v.x + m.e * v.y + m.f);
}
#define m3x2_multiply_point m3x2_mul_point /* @CLEANUP */
inline V2 m3x2_mul_vector(M3x2 m, V2 v) {
    return v2(m.a * v.x + m.b * v.y, m.d * v.x + m.e * v.y);
}
#define m3x2_multiply_vector m3x2_mul_vector /* @CLEANUP */
inline M3x2 m3x2_multiply_matrix(M3x2 l, M3x2 r) {
    return m3x2(
        l.a * r.a + l.b * r.d,  l.a * r.b + l.b * r.e,  l.a * r.c + l.b * r.f + l.c,
        l.d * r.a + l.e * r.d,  l.d * r.b + l.e * r.e,  l.d * r.c + l.e * r.f + l.f
    );
}

inline M3x2 m3x2_from_remap(
    F32 src_x, F32 src_y, F32 src_w, F32 src_h,
    F32 dst_x, F32 dst_y, F32 dst_w, F32 dst_h
) {
    // i.x
    // n.x = (i.x - src_x) / src_w
    // r.x = n.x * dst_w + dst_x
    // r.x = (i.x - src_x) * (1.0 / src_w) * dst_w + dst_x
    // r.x = ((i.x * (1.0 / src_w)) + (-src_x) * (1.0 / src_w)) * dst_w + dst_x
    // r.x = ((i.x * (1.0 / src_w) * dst_w) + (-src_x) * (1.0 / src_w) * dst_w) + dst_x
    // r.x = (i.x * (dst_w / src_w)) + (((-src_x * dst_w) / src_w) * ) + dst_x)
    return m3x2(
        dst_w / src_w,  0,              ((-src_x * dst_w) / src_w) + dst_x,
        0,              dst_h / src_h,  ((-src_y * dst_h) / src_h) + dst_y
    );
}

/* Why are quaternion called 3d rotor? 
 * Freya Holmér's talk on "Why can't you multiply vectors?"
 * https://www.youtube.com/watch?v=htYh-Tq7ZBI */
typedef V4 Rotor3;
typedef Rotor3 R3;

Bool rotor3_equal(Rotor3 l, Rotor3 r) {
    return
        l.x == r.x &&
        l.y == r.y &&
        l.z == r.z &&
        l.w == r.w;
}

Rotor3 rotor3(F32 x, F32 y, F32 z, F32 w) {
    Rotor3 q;
    q.x = x;
    q.y = y;
    q.z = z;
    q.w = w;
    return q;
}

#define ROTOR3_IDENTITY (rotor3(0.f,0.f,0.f,1.f))

Rotor3 rotor3_mul(Rotor3 l, Rotor3 r) {
    Rotor3 q;
    q.x =   l.x * r.w  +  l.y * r.z  - (l.z * r.y) + l.w * r.x;
    q.y = -(l.x * r.z) +  l.y * r.w  +  l.z * r.x  + l.w * r.y;
    q.z =   l.x * r.y  - (l.y * r.x) +  l.z * r.w  + l.w * r.z;
    q.w = -(l.x * r.x) - (l.y * r.y) - (l.z * r.z) + l.w * r.w;
    return q;
}

Rotor3 rotor3_axis_angle(V3 axis, F32 radians) {
    F32 c = cosf(radians * 0.5f);
    F32 s = sinf(radians * 0.5f);
    Rotor3 q;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = c;
    return q;
}

/* https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles */
/* Equivalent to this sequence of rotation in order:
 *  r = identity
 *  r = r * axis_angle(v3(1,0,0), x_roll )
 *  r = r * axis_angle(v3(0,1,0), y_pitch)
 *  r = r * axis_angle(v3(0,0,1), z_yaw  )
 * */
Rotor3 rotor3_euler(F32 x_roll, F32 y_pitch, F32 z_yaw) {
    Rotor3 q;
    F32 cr = cosf(x_roll  * 0.5f);
    F32 sr = sinf(x_roll  * 0.5f);
    F32 cp = cosf(y_pitch * 0.5f);
    F32 sp = sinf(y_pitch * 0.5f);
    F32 cy = cosf(z_yaw   * 0.5f);
    F32 sy = sinf(z_yaw   * 0.5f);
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    q.w = cr * cp * cy + sr * sp * sy;
    #ifdef ENGINE_PARANOID
    Rotor3 q_paranoid;
    q_paranoid = ROTOR3_IDENTITY;
    q_paranoid = rotor3_mul(q_paranoid, rotor3_axis_angle(v3(0, 0, 1), z_yaw));
    q_paranoid = rotor3_mul(q_paranoid, rotor3_axis_angle(v3(0, 1, 0), y_pitch));
    q_paranoid = rotor3_mul(q_paranoid, rotor3_axis_angle(v3(1, 0, 0), x_roll));
    ENGINE_ASSERT(rotor3_equal(q, q_paranoid), "Invalid rotor3_euler.");
    #endif
    return q;
}

Rotor3 rotor3_inverse(Rotor3 q) {
    F32 inverse_norm = f32_inverse_square_root(v4_dot(q, q));
    F32 inverse_norm_squared = inverse_norm * inverse_norm;
    return rotor3(
        - q.x * inverse_norm_squared,
        - q.y * inverse_norm_squared,
        - q.z * inverse_norm_squared,
          q.w * inverse_norm_squared
    );
}

/* https://www.3dgep.com/understanding-quaternions/#Rotations */
/* https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion */
V3 rotor3_mul_v3(Rotor3 l, V3 r) {
    V3 v = v3(l.x, l.y, l.z);

    F32 dot_ll   = v3_dot  (v, v);
    F32 dot_lr   = v3_dot  (v, r);
    V3  cross_lr = v3_cross(v, r);

    V3 a = v3_mul_f32(v, dot_lr * 2.f);
    V3 b = v3_mul_f32(r, l.w * l.w - dot_ll);
    V3 c = v3_mul_f32(cross_lr, l.w * 2.f);
    return v3_add(v3_add(a, b), c);
}

/* Matrix4x4 */
#define M4_ALIGNEMENT 16

#ifndef ENGINE_DISABLE_SSE2
typedef __m128 M4_Row;
#else
typedef union M4_Row { float m128_f32[4]; } M4_Row;
#endif
__declspec(align(16)) typedef struct M4 {
    M4_Row rows[4];
} M4;

static const M4 M4_IDENTITY = { {
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f }
} };

inline M4 m4(
    F32 a, F32 b, F32 c, F32 d, 
    F32 e, F32 f, F32 g, F32 h, 
    F32 i, F32 j, F32 k, F32 l, 
    F32 m, F32 n, F32 o, F32 p
) {
    M4 m4;
    #ifndef ENGINE_DISABLE_SSE2    
    m4.rows[0] = _mm_setr_ps(a, b, c, d);
    m4.rows[1] = _mm_setr_ps(e, f, g, h);
    m4.rows[2] = _mm_setr_ps(i, j, k, l);
    m4.rows[3] = _mm_setr_ps(m, n, o, p);
    #else
    m4.rows[0].m128_f32[0] = a;
    m4.rows[0].m128_f32[1] = b;
    m4.rows[0].m128_f32[2] = c;
    m4.rows[0].m128_f32[3] = d;
    m4.rows[1].m128_f32[0] = e;
    m4.rows[1].m128_f32[1] = f;
    m4.rows[1].m128_f32[2] = g;
    m4.rows[1].m128_f32[3] = h;
    m4.rows[2].m128_f32[0] = i;
    m4.rows[2].m128_f32[1] = j;
    m4.rows[2].m128_f32[2] = k;
    m4.rows[2].m128_f32[3] = l;
    m4.rows[3].m128_f32[0] = m;
    m4.rows[3].m128_f32[1] = n;
    m4.rows[3].m128_f32[2] = o;
    m4.rows[3].m128_f32[3] = p;
    #endif
    return m4;
}

inline M4 m4_mul(M4 l, M4 r) {
    M4 m;
    #ifndef ENGINE_DISABLE_AVX /* AVX */
    m.rows[0] =   _mm_mul_ps (r.rows[0], _mm_permute_ps(l.rows[0], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[0] = _mm_fmadd_ps (r.rows[1], _mm_permute_ps(l.rows[0], _MM_SHUFFLE(1, 1, 1, 1)), m.rows[0]);
    m.rows[0] = _mm_fmadd_ps (r.rows[2], _mm_permute_ps(l.rows[0], _MM_SHUFFLE(2, 2, 2, 2)), m.rows[0]);
    m.rows[0] = _mm_fmadd_ps (r.rows[3], _mm_permute_ps(l.rows[0], _MM_SHUFFLE(3, 3, 3, 3)), m.rows[0]);
    m.rows[1] =   _mm_mul_ps (r.rows[0], _mm_permute_ps(l.rows[1], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[1] = _mm_fmadd_ps (r.rows[1], _mm_permute_ps(l.rows[1], _MM_SHUFFLE(1, 1, 1, 1)), m.rows[1]);
    m.rows[1] = _mm_fmadd_ps (r.rows[2], _mm_permute_ps(l.rows[1], _MM_SHUFFLE(2, 2, 2, 2)), m.rows[1]);
    m.rows[1] = _mm_fmadd_ps (r.rows[3], _mm_permute_ps(l.rows[1], _MM_SHUFFLE(3, 3, 3, 3)), m.rows[1]);
    m.rows[2] =   _mm_mul_ps (r.rows[0], _mm_permute_ps(l.rows[2], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[2] = _mm_fmadd_ps (r.rows[1], _mm_permute_ps(l.rows[2], _MM_SHUFFLE(1, 1, 1, 1)), m.rows[2]);
    m.rows[2] = _mm_fmadd_ps (r.rows[2], _mm_permute_ps(l.rows[2], _MM_SHUFFLE(2, 2, 2, 2)), m.rows[2]);
    m.rows[2] = _mm_fmadd_ps (r.rows[3], _mm_permute_ps(l.rows[2], _MM_SHUFFLE(3, 3, 3, 3)), m.rows[2]);
    m.rows[3] =   _mm_mul_ps (r.rows[0], _mm_permute_ps(l.rows[3], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[3] = _mm_fmadd_ps (r.rows[1], _mm_permute_ps(l.rows[3], _MM_SHUFFLE(1, 1, 1, 1)), m.rows[3]);
    m.rows[3] = _mm_fmadd_ps (r.rows[2], _mm_permute_ps(l.rows[3], _MM_SHUFFLE(2, 2, 2, 2)), m.rows[3]);
    m.rows[3] = _mm_fmadd_ps (r.rows[3], _mm_permute_ps(l.rows[3], _MM_SHUFFLE(3, 3, 3, 3)), m.rows[3]);
    #else /* no AVX */
    #ifndef ENGINE_DISABLE_SSE2 /* SSE2 */
    m.rows[0] =            _mm_mul_ps(r.rows[0], _mm_shuffle_ps(l.rows[0], l.rows[0], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[0] = _mm_add_ps(_mm_mul_ps(r.rows[1], _mm_shuffle_ps(l.rows[0], l.rows[0], _MM_SHUFFLE(1, 1, 1, 1))), m.rows[0]);
    m.rows[0] = _mm_add_ps(_mm_mul_ps(r.rows[2], _mm_shuffle_ps(l.rows[0], l.rows[0], _MM_SHUFFLE(2, 2, 2, 2))), m.rows[0]);
    m.rows[0] = _mm_add_ps(_mm_mul_ps(r.rows[3], _mm_shuffle_ps(l.rows[0], l.rows[0], _MM_SHUFFLE(3, 3, 3, 3))), m.rows[0]);
    m.rows[1] =            _mm_mul_ps(r.rows[0], _mm_shuffle_ps(l.rows[1], l.rows[1], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[1] = _mm_add_ps(_mm_mul_ps(r.rows[1], _mm_shuffle_ps(l.rows[1], l.rows[1], _MM_SHUFFLE(1, 1, 1, 1))), m.rows[1]);
    m.rows[1] = _mm_add_ps(_mm_mul_ps(r.rows[2], _mm_shuffle_ps(l.rows[1], l.rows[1], _MM_SHUFFLE(2, 2, 2, 2))), m.rows[1]);
    m.rows[1] = _mm_add_ps(_mm_mul_ps(r.rows[3], _mm_shuffle_ps(l.rows[1], l.rows[1], _MM_SHUFFLE(3, 3, 3, 3))), m.rows[1]);
    m.rows[2] =            _mm_mul_ps(r.rows[0], _mm_shuffle_ps(l.rows[2], l.rows[2], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[2] = _mm_add_ps(_mm_mul_ps(r.rows[1], _mm_shuffle_ps(l.rows[2], l.rows[2], _MM_SHUFFLE(1, 1, 1, 1))), m.rows[2]);
    m.rows[2] = _mm_add_ps(_mm_mul_ps(r.rows[2], _mm_shuffle_ps(l.rows[2], l.rows[2], _MM_SHUFFLE(2, 2, 2, 2))), m.rows[2]);
    m.rows[2] = _mm_add_ps(_mm_mul_ps(r.rows[3], _mm_shuffle_ps(l.rows[2], l.rows[2], _MM_SHUFFLE(3, 3, 3, 3))), m.rows[2]);
    m.rows[3] =            _mm_mul_ps(r.rows[0], _mm_shuffle_ps(l.rows[3], l.rows[3], _MM_SHUFFLE(0, 0, 0, 0)));
    m.rows[3] = _mm_add_ps(_mm_mul_ps(r.rows[1], _mm_shuffle_ps(l.rows[3], l.rows[3], _MM_SHUFFLE(1, 1, 1, 1))), m.rows[3]);
    m.rows[3] = _mm_add_ps(_mm_mul_ps(r.rows[2], _mm_shuffle_ps(l.rows[3], l.rows[3], _MM_SHUFFLE(2, 2, 2, 2))), m.rows[3]);
    m.rows[3] = _mm_add_ps(_mm_mul_ps(r.rows[3], _mm_shuffle_ps(l.rows[3], l.rows[3], _MM_SHUFFLE(3, 3, 3, 3))), m.rows[3]);
    #else /* no SIMD */
    /* See AlphaTensor's algoritm:
     * Fawzi, A., Balog, M., Huang, A. et al.
     * Discovering faster matrix multiplication algorithms with reinforcement learning.
     * Nature 610, 47–53 (2022). https://doi.org/10.1038/s41586-022-05172-4
     * Extended Data Fig. 1: Algorithm for multiplying 4 × 4 matrices in modular arithmetic (ℤ₂) with 47 multiplications.
     * https://www.nature.com/articles/s41586-022-05172-4/figures/6
     * 
     * @NOTE(menyuu) No need to optimize recurrent additions. gcc 13.2 and msvc v19 will optimize them out for us. */
    #define a11 l.rows[0].m128_f32[0]
    #define a12 l.rows[0].m128_f32[1]
    #define a13 l.rows[0].m128_f32[2]
    #define a14 l.rows[0].m128_f32[3]
    #define a21 l.rows[1].m128_f32[0]
    #define a22 l.rows[1].m128_f32[1]
    #define a23 l.rows[1].m128_f32[2]
    #define a24 l.rows[1].m128_f32[3]
    #define a31 l.rows[2].m128_f32[0]
    #define a32 l.rows[2].m128_f32[1]
    #define a33 l.rows[2].m128_f32[2]
    #define a34 l.rows[2].m128_f32[3]
    #define a41 l.rows[3].m128_f32[0]
    #define a42 l.rows[3].m128_f32[1]
    #define a43 l.rows[3].m128_f32[2]
    #define a44 l.rows[3].m128_f32[3]
    
    #define b11 r.rows[0].m128_f32[0]
    #define b12 r.rows[0].m128_f32[1]
    #define b13 r.rows[0].m128_f32[2]
    #define b14 r.rows[0].m128_f32[3]
    #define b21 r.rows[1].m128_f32[0]
    #define b22 r.rows[1].m128_f32[1]
    #define b23 r.rows[1].m128_f32[2]
    #define b24 r.rows[1].m128_f32[3]
    #define b31 r.rows[2].m128_f32[0]
    #define b32 r.rows[2].m128_f32[1]
    #define b33 r.rows[2].m128_f32[2]
    #define b34 r.rows[2].m128_f32[3]
    #define b41 r.rows[3].m128_f32[0]
    #define b42 r.rows[3].m128_f32[1]
    #define b43 r.rows[3].m128_f32[2]
    #define b44 r.rows[3].m128_f32[3]

    F32 h1 = a11 * b13;
    F32 h2 = (a11 + a31 + a33) * (b11 + b31 + b33);
    F32 h3 = (a11 + a31 + a34) * (b12 + b42 + b43);
    F32 h4 = (a13 + a21 + a23) * (b13 + b14 + b34);
    F32 h5 = (a11 + a31) * (b11 + b12 + b13 + b31 + b33 + b42 + b43);

    #error unsuported
    
    #undef a11
    #undef a12
    #undef a13  
    #undef a14
    #undef a21
    #undef a22
    #undef a23
    #undef a24
    #undef a31
    #undef a32
    #undef a33
    #undef a34
    #undef a41
    #undef a42
    #undef a43
    #undef a44
    
    #undef b11
    #undef b12
    #undef b13
    #undef b14
    #undef b21
    #undef b22
    #undef b23
    #undef b24
    #undef b31
    #undef b32
    #undef b33
    #undef b34
    #undef b41
    #undef b42
    #undef b43
    #undef b44

    #endif /* end no SSE2 */
    #endif /* end no AVX */
    return m;
}

/* Transformations */

M4 m4_translation(V3 t) {
    M4 m = m4(
        1, 0, 0, t.x,
        0, 1, 0, t.y,
        0, 0, 1, t.z,
        0, 0, 0, 1  
    );
    return m;
}

M4 m4_translation_inverse(V3 t) {
    M4 m = m4(
        1, 0, 0, -t.x,
        0, 1, 0, -t.y,
        0, 0, 1, -t.z,
        0, 0, 0, 1  
    );
    return m;
}

#define M4_SET_TRANSLATION(m, x, y, z) { \
    (m).rows[0].m128_f32[3] = (x); \
    (m).rows[1].m128_f32[3] = (y); \
    (m).rows[2].m128_f32[3] = (z); \
}

M4 m4_scale(V3 t) {
    M4 m = m4(
        t.x, 0  , 0  , 0,
        0  , t.y, 0  , 0,
        0  , 0  , t.z, 0,
        0  , 0  , 0  , 1
    );
    return m;
}

M4 m4_rotation(Rotor3 q) {
    #ifndef ENGINE_DISABLE_SSE2
    __m128 squared = *(__m128*)&q;
    squared = _mm_mul_ps(squared, squared);
    #endif
    F32 xx = q.x * q.x;
    F32 yy = q.y * q.y;
    F32 zz = q.z * q.z;
    F32 xy = q.x * q.y;
    F32 xz = q.x * q.z;
    F32 xw = q.x * q.w;
    F32 yz = q.y * q.z;
    F32 yw = q.y * q.w;
    F32 zw = q.z * q.w;    
    M4 m = m4(
        1.f - 2.f * (yy + zz),       2.f * (xy - zw),       2.f * (xz + yw), 0.f,
              2.f * (xy + zw), 1.f - 2.f * (xx + zz),       2.f * (yz - xw), 0.f,
              2.f * (xz - yw),       2.f * (yz + xw), 1.f - 2.f * (xx + yy), 0.f,
        0.f                  , 0.f                  , 0.f                  , 1.f
    );
    return m;
}

M4 m4_rotation_inverse(Rotor3 q) {
    q = rotor3_inverse(q);
    return m4_rotation(q);
}

M4 m4_trs(V3 translation, Rotor3 rotation, V3 scale) {
    M4 t   = m4_translation(translation);
    M4 r   = m4_rotation(rotation);
    M4 s   = m4_scale(scale);
    return m4_mul(t, m4_mul(r, s));
}

M4 m4_tr(V3 translation, Rotor3 rotation) {
    M4 t   = m4_translation(translation);
    M4 r   = m4_rotation(rotation);
    return m4_mul(t, r);
}

M4 m4_tr_inverse(V3 translation, Rotor3 rotation) {
    M4 r   = m4_rotation_inverse(rotation);
    M4 t   = m4_translation_inverse(translation);
    return m4_mul(r, t);
}

M4 m4_orthographic(V2 scale) {
    m4(
        scale.x, 0      , 0, 0,
        0      , scale.y, 0, 0,
        0      , 0      , 0, 0,
        0      , 0      , 0, 1
    );
}

void m4_swap_rows(M4 *m_, size_t row_a, size_t row_b) {
    M4_Row temp = (*m_).rows[row_a];
    (*m_).rows[row_a] = (*m_).rows[row_b];
    (*m_).rows[row_b] = temp;
}

/* M4_SWAP_COLUMNS */
#define M4_SWAP_COLUMNS_NOSIMD(m, a, b) { \
    float temp; \
    temp = (m).rows[0].m128_f32[a]; \
    (m).rows[0].m128_f32[a] = (m).rows[0].m128_f32[b]; \
    (m).rows[0].m128_f32[b] = temp; \
    temp = (m).rows[1].m128_f32[a]; \
    (m).rows[1].m128_f32[a] = (m).rows[1].m128_f32[b]; \
    (m).rows[1].m128_f32[b] = temp; \
    temp = (m).rows[2].m128_f32[a]; \
    (m).rows[2].m128_f32[a] = (m).rows[2].m128_f32[b]; \
    (m).rows[2].m128_f32[b] = temp; \
    temp = (m).rows[3].m128_f32[a]; \
    (m).rows[3].m128_f32[a] = (m).rows[3].m128_f32[b]; \
    (m).rows[3].m128_f32[b] = temp; \
}
#ifndef ENGINE_DISABLE_SSE2 /* SSE2, AVX */
#define M4_SWAP_COLUMNS_SHUFFLE(a, b) \
    ((3 << 6) | (2 << 4) | (1 << 2) | 0) \
    & ~(0x3 << (a << 1) ) \
    & ~(0x3 << (b << 1) ) \
    | b << (a << 1) \
    | a << (b << 1)

#ifndef ENGINE_DISABLE_AVX /* AVX only */
#define M4_SWAP_COLUMNS(m, a, b) { \
    (m).rows[0] = _mm_permute_ps((m).rows[0], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[1] = _mm_permute_ps((m).rows[1], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[2] = _mm_permute_ps((m).rows[2], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[3] = _mm_permute_ps((m).rows[3], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
}
#else /* SSE2 only */
#define M4_SWAP_COLUMNS(m, a, b) { \
    (m).rows[0] = _mm_shuffle_ps((m).rows[0], (m).rows[0], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[1] = _mm_shuffle_ps((m).rows[1], (m).rows[1], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[2] = _mm_shuffle_ps((m).rows[2], (m).rows[2], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
    (m).rows[3] = _mm_shuffle_ps((m).rows[3], (m).rows[3], M4_SWAP_COLUMNS_SHUFFLE(a, b)); \
}
#endif
#else /* no SIMD */
#define M4_SWAP_COLUMNS M4_SWAP_COLUMNS_NOSIMD
#endif

M4 m4_transpose(M4 m) {
    #ifndef ENGINE_DISABLE_SS2
    _MM_TRANSPOSE4_PS(m.rows[0], m.rows[1], m.rows[2], m.rows[3]);
    #else /* no SIMD */
    M4_SWAP_COLUMNS_NOSIMD(m, 0, 3);
    M4_SWAP_COLUMNS_NOSIMD(m, 1, 2);
    m4_swap_rows(&m, 0, 3);
    m4_swap_rows(&m, 1, 2);
    #endif
    return m;
}

/* @TODO(menyuu) Use m4_transposed_mul_point(...) for better speed in a loop? */
V3 m4_mul_point(M4 m, V3 p) {
    #ifndef ENGINE_DISABLE_SS2
    _MM_TRANSPOSE4_PS(m.rows[0], m.rows[1], m.rows[2], m.rows[3]);
    __m128 x = _mm_set1_ps(p.x); 
    __m128 y = _mm_set1_ps(p.y);
    __m128 z = _mm_set1_ps(p.z);
    __m128 o;
    #ifndef ENGINE_DISABLE_AVX
    o = _mm_fmadd_ps(m.rows[2], z, m.rows[3]);
    o = _mm_fmadd_ps(m.rows[1], y, o        );
    o = _mm_fmadd_ps(m.rows[0], x, o        );
    #else 
    o = _mm_add_ps(_mm_mul_ps(m.rows[2], z), m.rows[3]);
    o = _mm_add_ps(_mm_mul_ps(m.rows[1], y), o        );
    o = _mm_add_ps(_mm_mul_ps(m.rows[0], x), o        );
    #endif /* !ENGINE_DISABLE_AVX */
    return v3(o.m128_f32[0], o.m128_f32[1], o.m128_f32[2]);
    #else
    return v3(
        m.rows[0].m128_f32[0] * p.x + m.rows[0].m128_f32[1] * p.y + m.rows[0].m128_f32[2] * p.z + m.rows[0].m128_f32[3],
        m.rows[1].m128_f32[0] * p.x + m.rows[1].m128_f32[1] * p.y + m.rows[1].m128_f32[2] * p.z + m.rows[1].m128_f32[3],
        m.rows[2].m128_f32[0] * p.x + m.rows[2].m128_f32[1] * p.y + m.rows[2].m128_f32[2] * p.z + m.rows[2].m128_f32[3]
    );
    #endif /* !ENGINE_DISABLE_SS2 */
}

/* @TODO(menyuu) Use m4_transposed_mul_vector(...) for better speed in a loop? */
V3 m4_mul_vector(M4 m, V3 v) {
    #ifndef ENGINE_DISABLE_SS2 /* SSE2, AVX */
        _MM_TRANSPOSE4_PS(m.rows[0], m.rows[1], m.rows[2], m.rows[3]);
        __m128 x = _mm_set1_ps(v.x); 
        __m128 y = _mm_set1_ps(v.y);
        __m128 z = _mm_set1_ps(v.z);
        __m128 o;
        o =   _mm_mul_ps(m.rows[2], z);
        #ifndef ENGINE_DISABLE_AVX /* AVX */
            o = _mm_fmadd_ps(m.rows[1], y, o);
            o = _mm_fmadd_ps(m.rows[0], x, o);
        #else /* SSE2 */
            o = _mm_add_ps(_mm_mul_ps(m.rows[1], y), o);
            o = _mm_add_ps(_mm_mul_ps(m.rows[0], x), o);
        #endif
        return v3(o.m128_f32[0], o.m128_f32[1], o.m128_f32[2]);
    #else /* no SIMD */
        return v3(
            m.rows[0].m128_f32[0] * v.x + m.rows[0].m128_f32[1] * v.y + m.rows[0].m128_f32[2] * v.z,
            m.rows[1].m128_f32[0] * v.x + m.rows[1].m128_f32[1] * v.y + m.rows[1].m128_f32[2] * v.z,
            m.rows[2].m128_f32[0] * v.x + m.rows[2].m128_f32[1] * v.y + m.rows[2].m128_f32[2] * v.z
        );
    #endif
}

#pragma endregion // Matrices



// geometry

#pragma region Geometry

F32 math_dot_2d(F32 ax, F32 ay, F32 bx, F32 by) {
    return ax * bx + ay * by;
}


#pragma endregion // Geometry

// distances
#pragma region Distances

// O(n)
int algo_nearest_neighbor_v2(int num_points, V2* points, ptrdiff_t stride, V2 point) {
    int i_nearest_point       = -1;
    F32 dist_to_nearest_point = F32_INFINITY;
    for(int i_point = 0; i_point < num_points; ++i_point) {
        F32 dist_to_point = v2_distance(*points, point);
        if(dist_to_point < dist_to_nearest_point) {
            i_nearest_point       = i_point;
            dist_to_nearest_point = dist_to_point;
        }
        points = (V2*)(((char*)points) + stride);
    }
    return i_nearest_point;
}

// finds the nearest edge from givent point (edges expressed as vertices + indices)
// returns: the index of the edge closest to given point
void algo_indexed_edge_nearest_neighbor (
    V2  *vertices,
    U8   vertex_stride,
    U16 *edges,   
    U8   edge_stride,      
    U64  num_edges, 
    V2   point,
    U64  index_offset,
    U64* nearest_neighbour_index,
    F32* min_distance
) {
    F32 local_min_distance = *min_distance;
    for (size_t i_edge = 0; i_edge < num_edges; ++i_edge) {
        F32 distance_from_point;
        {
            U16 *edge       = (U16*)(&((char*)edges)[edge_stride * i_edge]);
            V2  a           = *(V2*)(&((char*)vertices)[vertex_stride * edge[0]]);
            V2  b           = *(V2*)(&((char*)vertices)[vertex_stride * edge[1]]);
            V2  ab          = v2_a_to_b(a, b);
            V2  ap          = v2_a_to_b(a, point);
            F32 norm_ab     = v2_magnitude(ab);
            V2  tangent     = v2_normalize_vector(ab);
            F32 tangent_dot = v2_dot(tangent, ap);
            if (tangent_dot <= 0.0f) {
                // point is "under" line range
                distance_from_point = v2_distance(a, point);
            } else if (tangent_dot >= norm_ab) {
                // point is "above" line range
                distance_from_point = v2_distance(b, point);
            } else {
                // point is "within" line range
                V2 normal           = v2_rotate_quarter_pi(tangent);
                distance_from_point = fabsf(v2_dot(normal, ap));
            }
        }
        if (distance_from_point < local_min_distance) {
            *nearest_neighbour_index = index_offset + i_edge;
            local_min_distance       = distance_from_point;
        }
    }
    *min_distance = local_min_distance;
}

#pragma endregion // Geometry