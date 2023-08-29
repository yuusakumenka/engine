#define F32_PI 3.141524f

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


// vector
typedef struct V2 {
    F32 x;
    F32 y;
} V2;

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
inline V2 v2_delta(V2 a, V2 b) {
    return v2(b.x - a.x, b.y - a.y);
}
inline V2 v2_sub(V2 a, V2 b) {
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
inline F32 v2_dot(V2 a, V2 b) {
    return a.x * b.x + a.y * b.y;
}
inline F32 v2_magnitude(V2 vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}
inline F32 v2_magnitude_squared(V2 vector) {
    return vector.x * vector.x + vector.y * vector.y;
}
inline V2 v2_normalize_vector(V2 vector) {
    F32 inverse_magnitude = 1.0f / sqrtf(vector.x * vector.x + vector.y * vector.y);
    return v2(vector.x * inverse_magnitude, vector.y * inverse_magnitude);
}
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
    return v2_add(current, v2_multiply_f32(v2_sub(current, target), 1.0f - powf(0.01f, time_delta)));
}
// Euclidean distance
inline F32 v2_distance(V2 point_a, V2 point_b) {
    F32 delta_x = point_b.x - point_a.x;
    F32 delta_y = point_b.y - point_a.y;
    return sqrtf(delta_x * delta_x + delta_y * delta_y);
}
inline V2  v2_move_towards(V2 current, V2 target, F32 max_distance) {
    V2 delta = v2_delta(current, target);
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

V2 v2_clamp_magnitude(V2 vector, F32 max_magnitude) {
    F32 magnitude = v2_magnitude(vector);
    F32 clamped_magnitude = fminf(max_magnitude, magnitude);
    return v2_mul_f32(vector, fmaxf(clamped_magnitude / magnitude, 0));
}

typedef struct V4 {
    F32 x;
    F32 y;
    F32 z;
    F32 w;
} V4;

inline V4 v4(F32 x, F32 y, F32 z, F32 w) {
    V4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}
#define V4_ZERO (v4(0.f, 0.f, 0.f, 0.f))

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
inline V2 m3x2_multiply_point(M3x2 m, V2 v) {
    return v2(m.a * v.x + m.b * v.y + m.c, m.d * v.x + m.e * v.y + m.f);
}
inline V2 m3x2_multiply_vector(M3x2 m, V2 v) {
    return v2(m.a * v.x + m.b * v.y, m.d * v.x + m.e * v.y);
}
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
int algo_nearest_neighbor_v2(int num_points, V2* points, V2 point) {
    int i_nearest_point       = -1;
    F32 dist_to_nearest_point = F32_INFINITY;
    for(int i_point = 0; i_point < num_points; ++i_point) {
        F32 dist_to_point = v2_distance(points[i_point], point);
        if(dist_to_point < dist_to_nearest_point) {
            i_nearest_point       = i_point;
            dist_to_nearest_point = dist_to_point;
        }
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
            V2  ab          = v2_sub(a, b);
            V2  ap          = v2_sub(a, point);
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