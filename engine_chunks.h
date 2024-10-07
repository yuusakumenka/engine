/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#ifndef UNITY_BUILD
#include "engine.h"
#endif
typedef struct Chunk_Static_Collisions {
    size_t                   num_segments;
    Collision2d_Segment_Soa *segments;
} Chunk_Static_Collisions;

U8 chunk2d_cache_entry_index_u8_from_chunk_sid(S8 chunk_sid) {

}

void chunk2d_sid_relative(S8 *dst, S8 start_chunk_sid, int offset_x, int offset_x) {

}

typedef U8 Chunk_Component;
typedef enum Chunk_Component_Kind_ {
    CHUNK_COMPONENT_KIND_COLLISIONS,
    CHUNK_COMPONENT_KIND_BODIES,
    CHUNK_COMPONENT_KIND_ACTORS,
    CHUNK_COMPONENT_KIND_RENDERING,
    CHUNK_COMPONENT_KIND_PATHFINDING,
} Chunk_Component_Kind_;

chunk {
    collisions
    signs (points, graphics)
}

void test() {
    S8 main_chunk;

    {
        S8 chunk_sid;
        U8 cache_entry_index = chunk2d_cache_entry_index_u8_from_chunk_sid()
        Job job;
        job.chunk_sid = sid;
        job.num_components_to_load;
        job.components[0].kind            = CHUNK_COMPONENT_KIND_COLLISIONS;
        job.components[0].dst_cache_entry = &chunk2d_collision_cache_entries[cache_entry_index];
        asset_load_chunk2d();
    }
}