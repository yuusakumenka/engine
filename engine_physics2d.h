/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Right now we are using manually writing AVX intransics... Not very cross platform.
 * @SebAaltonen suggest having a look at Intel's ISPC SPMD compiler.
 * Adding this as a requirement of the toolchain sounds sad tho.
 * Maybe we can find a way to make it optional? */

typedef union Body2d_Id { /* Why is there spatial datastructure code in Physics? */
    struct Body2d_Id_As_Struct {
        U16 chunk_cache_slot_id;
        U16 body_chunk_id;
    } as_struct;
    I32 as_i32;
} Body2d_Id;

typedef struct Body2d_Ball_View {
    F32       *velocity_x;
    F32       *velocity_y;
    F32       *acceleration_x;
    F32       *acceleration_y;
    F32       *position_x;
    F32       *position_y;
    F32       *radius_squared;
    Body2d_Id *id;
} Body2d_Ball_View;

typedef struct Body2d_Ball_Physics_Soa {
    __m128  vx;
    __m128  vy;
    __m128  ax;
    __m128  ay;
} Body2d_Ball_Physics_Soa;

typedef struct Body2d_Ball_Position_Soa {
    __m128  x;
    __m128  y;
} Body2d_Ball_Position_Soa;

inline void body2d_ball_soa_view_position(F32 **x, F32 **y, Body2d_Ball_Position_Soa *src, size_t ball_index) {
    size_t element_index = ball_index & ((1ull << 2) - 1);
    size_t soa_index     = ball_index >> 2;
    *x = &src[soa_index].x.m128_f32 [element_index];
    *y = &src[soa_index].y.m128_f32 [element_index];
}

typedef struct Body2d_Ball_Collisions_Soa {
    __m128  rs;
    __m128i id;
} Body2d_Ball_Collisions_Soa;

inline void body2d_ball_soa_view_radius_squared(F32 **dst_rs, Body2d_Ball_Collisions_Soa *src, size_t ball_index) {
    size_t element_index = ball_index & ((1ull << 2) - 1);
    size_t soa_index     = ball_index >> 2;
    *dst_rs = &src[soa_index].rs.m128_f32 [element_index];
}

typedef struct Body2d_Ball_Soa {
    Body2d_Ball_Physics_Soa    *physics;
    Body2d_Ball_Position_Soa   *positions;
    Body2d_Ball_Collisions_Soa *collisions;
} Body2d_Ball_Soa;

inline void body2d_ball_soa_view(Body2d_Ball_View *dst, Body2d_Ball_Soa *src, size_t ball_index) {
    size_t element_index = ball_index & ((1ull << 2) - 1);
    size_t soa_index     = ball_index >> 2;
    dst->velocity_x     =             &src->physics   [soa_index].vx.m128_f32 [element_index];
    dst->velocity_y     =             &src->physics   [soa_index].vy.m128_f32 [element_index];
    dst->acceleration_x =             &src->physics   [soa_index].ax.m128_f32 [element_index];
    dst->acceleration_y =             &src->physics   [soa_index].ay.m128_f32 [element_index];
    dst->position_x     =             &src->positions [soa_index].x .m128_f32 [element_index];
    dst->position_y     =             &src->positions [soa_index].y .m128_f32 [element_index];
    dst->radius_squared =             &src->collisions[soa_index].rs.m128_f32 [element_index];
    dst->id             = (Body2d_Id*)&src->collisions[soa_index].id.m128i_i32[element_index];
}

typedef struct Physics2d_Ball_Set_Acceleration_Job {
    size_t                    num_soas;
    Body2d_Ball_Physics_Soa  *physics;
    F32                       acceleration_x;
    F32                       acceleration_y;
} Physics2d_Ball_Set_Acceleration_Job;

void physics2d_ball_set_acceleration_job(Physics2d_Ball_Set_Acceleration_Job *job, F32 timestep) {
    F32 acceleration_x_premultiplied = job->acceleration_x * timestep;
    F32 acceleration_y_premultiplied = job->acceleration_y * timestep;
    for(size_t b = 0; b != job->num_soas; ++b) {
        job->physics[b].ax = _mm_set1_ps(acceleration_x_premultiplied);
        job->physics[b].ay = _mm_set1_ps(acceleration_y_premultiplied);
    }
}

typedef struct Physics2d_Ball_Update_Job {
    size_t                  num_soas;
    Body2d_Ball_Physics_Soa  *physics;
    Body2d_Ball_Position_Soa *positions;
} Physics2d_Ball_Update_Job;

void physics2d_ball_update_job(Physics2d_Ball_Update_Job *job, F32 timestep) {
    __m128 point5 = _mm_set1_ps(0.5f);
    __m128 step   = _mm_set1_ps(timestep);
    for(size_t b = 0; b != job->num_soas; ++b) {
        /* p += (0.5at + v)t */
        job->positions[b].x  = _mm_fmadd_ps(_mm_fmadd_ps(point5, job->physics[b].ax, job->physics[b].vx), step, job->positions[b].x);
        job->positions[b].y  = _mm_fmadd_ps(_mm_fmadd_ps(point5, job->physics[b].ay, job->physics[b].vy), step, job->positions[b].y);
        /* v += at */
        job->physics  [b].vx = _mm_add_ps(job->physics  [b].vx, job->physics[b].ax);
        job->physics  [b].vy = _mm_add_ps(job->physics  [b].vy, job->physics[b].ay);
    }
}