/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

// #ifndef UNITY_BUILD
// #include "../../engine/engine.h"
// #include "../../engine/engine_physics2d.h"
// #endif

typedef struct Collision2d_Overlap {
    Body2d_Id l_id;
    Body2d_Id r_id;
} Collision2d_Overlap;

/* #Thread_Safe */
typedef struct Collision2d_Overlap_Dst {
    size_t               num_overlaps;
    Collision2d_Overlap *overlaps;
} Collision2d_Overlap_Dst;

typedef struct Collision2d_Aabb_Soa {
    int : 0;
} Collision2d_Aabb_Soa;

typedef struct Collision2d_Segment_Soa {
    __m128  ox;
    __m128  oy;
    __m128  nx;
    __m128  ny;
    __m128  m;
    __m128i id;
} Collision2d_Segment_Soa;

typedef struct Collision2d_Segment_View {
    F32       *ox;
    F32       *oy;
    F32       *nx;
    F32       *ny;
    F32       *m ;
    Body2d_Id *id;
} Collision2d_Segment_View;

inline void collision2d_segment_view(Collision2d_Segment_View *dst, Collision2d_Segment_Soa *src, size_t index) {
    size_t element_index = index & ((1ull << 2) - 1);
    size_t soa_index     = index >> 2;
    dst->ox =             &src[soa_index].ox.m128_f32 [element_index];
    dst->oy =             &src[soa_index].oy.m128_f32 [element_index];
    dst->nx =             &src[soa_index].nx.m128_f32 [element_index];
    dst->ny =             &src[soa_index].ny.m128_f32 [element_index];
    dst->m  =             &src[soa_index].m .m128_f32 [element_index];
    dst->id = (Body2d_Id*)&src[soa_index].id.m128i_i32[element_index];
}

typedef struct Collision2d_Ball_Soa {
    __m128  x;
    __m128  y;
    __m128  radius_squared;
    __m128i id;
} Collision2d_Ball_Soa;


Collision2d_Overlap *collision2d_overlap_dst_reserve(Collision2d_Overlap_Dst *dst, size_t count) {
    Collision2d_Overlap *reserved = &dst->overlaps[dst->num_overlaps];
    dst->num_overlaps += count;
    return reserved;
}

inline Collision2d_Overlap *collision2d_overlap_dst_reserve_one(Collision2d_Overlap_Dst *dst) {
    Collision2d_Overlap *reserved = &dst->overlaps[dst->num_overlaps];
    ++dst->num_overlaps;
    return reserved;
}

Collision2d_Overlap *collision2d_overlap_dst_reserve_threadsafe(Collision2d_Overlap_Dst *dst, size_t count) {
    Collision2d_Overlap *reserved = &dst->overlaps[dst->num_overlaps];
    INTERLOCKED_ADD(&dst->num_overlaps, count);
    return reserved;
}

Collision2d_Overlap *collision2d_overlap_dst_reserve_one_threadsafe(Collision2d_Overlap_Dst *dst) {
    Collision2d_Overlap *reserved = &dst->overlaps[dst->num_overlaps];
    INTERLOCKED_INCREMENT(&dst->num_overlaps);
    return reserved;
}

void collision2d_ball_dynamic_soa_segment_soa(
    Body2d_Ball_Position_Soa   *ball_positions,
    Body2d_Ball_Collisions_Soa *ball_collisions,
    Collision2d_Segment_Soa    *segments,
    Collision2d_Overlap_Dst    *overlap_dst
) {
    __m128 bx;   /* ball_x */
    __m128 by;   /* ball_y */
    __m128 rs;   /* ball_radius_saquared */
    __m128 dx;
    __m128 dy;
    __m128 fx;
    __m128 f ;
    __m128 ddfx;
    __m128 ddfy;
    __m128 dfx;
    __m128 df ;
    int o[4]; /* is_overlaping */
    #define COLLISION2D_BALL_SEGMENT_SOA(b) \
        bx   = _mm_permute_ps(ball_positions ->x , _MM_SHUFFLE(b,b,b,b));\
        by   = _mm_permute_ps(ball_positions ->y , _MM_SHUFFLE(b,b,b,b));\
        rs   = _mm_permute_ps(ball_collisions->rs, _MM_SHUFFLE(b,b,b,b));\
        dx   = _mm_sub_ps(bx, segments->ox);\
        dy   = _mm_sub_ps(by, segments->oy);\
        fx   = _mm_mul_ps(dx, segments->nx);\
        f    = _mm_fmadd_ps(dy, segments->ny, fx);\
        f    = _mm_max_ps(f, _mm_set1_ps(0));\
        f    = _mm_min_ps(f, segments->m);\
        ddfx = _mm_mul_ps(segments->nx, f);\
        ddfy = _mm_mul_ps(segments->ny, f);\
        ddfx = _mm_sub_ps(dx, ddfx);\
        ddfy = _mm_sub_ps(dy, ddfy);\
        dfx  = _mm_mul_ps(ddfx, ddfx);\
        df   = _mm_fmadd_ps(ddfy, ddfy, dfx);\
        o[b] = _mm_movemask_ps(_mm_cmple_ps(df, rs));
    COLLISION2D_BALL_SEGMENT_SOA(0);
    COLLISION2D_BALL_SEGMENT_SOA(1);
    COLLISION2D_BALL_SEGMENT_SOA(2);
    COLLISION2D_BALL_SEGMENT_SOA(3);    
    int num_overlaps = __popcnt(o[0]) + __popcnt(o[1]) + __popcnt(o[2]) + __popcnt(o[3]);
    Collision2d_Overlap *overlap;
    if(num_overlaps)
        overlap = collision2d_overlap_dst_reserve(overlap_dst, num_overlaps);
    for(size_t b = 0; b != 4; ++b) {
        for(size_t s = 0; s != 4; ++s) {
            if(o[b] & 1) {
                overlap->l_id.as_i32 = ball_collisions->id.m128i_i32[b];
                overlap->r_id.as_i32 = segments       ->id.m128i_i32[s];
                ++overlap;
            }
            o[b] = o[b] >> 1;
        }
    }
}

typedef struct Collision2d_Balls_Segments_Job {
    /* ball buffer */
    size_t                      num_ball_soas;
    Body2d_Ball_Position_Soa   *ball_positions;
    Body2d_Ball_Collisions_Soa *ball_collisions;
    /* segment buffer */
    size_t                      num_segment_soas;
    Collision2d_Segment_Soa    *segments;
    /* overlaps */
    Collision2d_Overlap_Dst    *overlap_dst;
} Collision2d_Balls_Segments_Job;

void collision2d_balls_segments_job(
    Collision2d_Balls_Segments_Job *job
) {
    for(size_t b = 0; b != job->num_ball_soas; ++b) {
        for(size_t s = 0; s != job->num_segment_soas; ++s) {
            collision2d_ball_dynamic_soa_segment_soa(
                &job->ball_positions[b],
                &job->ball_collisions[b],
                &job->segments[s],
                job->overlap_dst
            );
        }
    }
}

void collision2d_ball_soa_point(
    Body2d_Ball_Position_Soa   *ball_positions,
    Body2d_Ball_Collisions_Soa *ball_collisions,
    __m128 px,
    __m128 py,
    Collision2d_Overlap_Dst    *overlap_dst
) {
    __m128 bx;   /* ball_x */
    __m128 by;   /* ball_y */
    __m128 rs;   /* ball_radius_saquared */
    __m128 ddfx;
    __m128 ddfy;
    __m128 dfx;
    __m128 df ;
    int o; /* is_overlaping */
    bx   = ball_positions ->x;
    by   = ball_positions ->y;
    rs   = ball_collisions->rs;
    ddfx = _mm_sub_ps(bx, px);
    ddfy = _mm_sub_ps(by, py);
    dfx  = _mm_mul_ps(ddfx, ddfx);
    df   = _mm_fmadd_ps(ddfy, ddfy, dfx);
    o    = _mm_movemask_ps(_mm_cmple_ps(df, rs));
    int num_overlaps = __popcnt(o);
    Collision2d_Overlap *overlap;
    if(num_overlaps)
        overlap = collision2d_overlap_dst_reserve_threadsafe(overlap_dst, num_overlaps);
    for(size_t b = 0; b != 4; ++b) {
        if(o & 1) {
            overlap->l_id.as_i32 = ball_collisions->id.m128i_i32[b];
            ++overlap;
        }
        o = o >> 1;
    }
}

typedef struct Collision2d_Balls_Point_Job {
    size_t                      num_ball_soas;
    Body2d_Ball_Position_Soa   *ball_positions;
    Body2d_Ball_Collisions_Soa *ball_collisions;
    V2 point;
    Collision2d_Overlap_Dst    *overlap_dst;
} Collision2d_Balls_Point_Job;

void collision2d_balls_point_job(
    Collision2d_Balls_Point_Job *job
) {
    __m128 px = _mm_set1_ps(job->point.x);
    __m128 py = _mm_set1_ps(job->point.y);
    for(size_t b = 0; b != job->num_ball_soas; ++b) {
        collision2d_ball_soa_point(
            &job->ball_positions[b],
            &job->ball_collisions[b],
            px,
            py,
            job->overlap_dst
        );
    }
}