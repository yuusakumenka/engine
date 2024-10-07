/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#ifdef __INTELLISENSE__
#include "..\..\engine\engine.h"
#include "index_pool_u16.h"
#endif

// Topology Builder is a Quad Edge based helper libary.

//#define TOPOLOGY_BUILDER_DISABLE_DUAL

#define TOPOLOGY_BUILDER_INVALID_EDGE       0xFFFF
#define TOPOLOGY_BUILDER_INVALID_POLE       0xFFFF
#define TOPOLOGY_BUILDER_NUM_SLOTS_PER_EDGE 2

typedef struct Topology_Builder_Pole {
    U16 index_of_any_connected_edge;
} Topology_Builder_Pole;

typedef struct Topology_Builder_Edge { // 8
    U16 darts[TOPOLOGY_BUILDER_NUM_SLOTS_PER_EDGE]; /* The next counter clock wise edge connected to the pole of same index. */
    U16 poles[TOPOLOGY_BUILDER_NUM_SLOTS_PER_EDGE]; /* A pole is either a vertex or a face depending if we are on the primal or dual topology. */
} Topology_Builder_Edge;

typedef struct Topology_Builder_Topology {
    Topology_Builder_Pole *poles;
    Topology_Builder_Edge *edges;
    Index_Pool_U16         index_pool;
} Topology_Builder_Topology;

typedef U8 Topology_Builder_Direction;
typedef enum Topology_Builder_Direction_ {
    TOPOLOGY_BUILDER_DIRECTION_RIGHT = 0x00,
    TOPOLOGY_BUILDER_DIRECTION_UP    = 0x01,
    TOPOLOGY_BUILDER_DIRECTION_LEFT  = 0x02,
    TOPOLOGY_BUILDER_DIRECTION_DOWN  = 0x03,
    TOPOLOGY_BUILDER_DIRECTION_MASK  = 0x03
} Topology_Builder_Direction_;

// Mesh builder
typedef struct Topology_Builder {
    Topology_Builder_Topology vertex_topology; /* Primal */
    #ifndef TOPOLOGY_BUILDER_DISABLE_DUAL
    Topology_Builder_Topology face_topology; /* Dual */
    #endif /* !TOPOLOGY_BUILDER_DISABLE_DUAL */
    U16            num_edges;
    Index_Pool_U16 edge_index_pool;
} Topology_Builder;

void topology_builder_insert_poles     (Topology_Builder *builder, U16 num_poles, U16** poles);
void topology_builder_insert_edges     (Topology_Builder *builder, U16 num_edges, U16** edges);
U16  topology_builder_remove_edge      (Topology_Builder *builder);
U16  topology_builder_connect_poles    (Topology_Builder *builder, U16 origin_pole_index, U16 destination_pole_index);
void topology_builder_topology_get_edge(Topology_Builder_Topology *builder_topology, U16 edge_index, Topology_Builder_Edge **edge);
//void topology_builder_defragment(Topology_Builder *builder);

void topology_builder_topology_get_edge(Topology_Builder_Topology *builder_topology, U16 edge_index, Topology_Builder_Edge **edge) {
    *edge = &builder_topology->edges[edge_index];
}

Topology_Builder_Topology* topology_builder_get_topology_from_direction(Topology_Builder *builder, Topology_Builder_Direction direction) {
    Topology_Builder_Topology *topologies[2];
    topologies[0] = &((Topology_Builder*)0)->vertex_topology;
    topologies[1] = &((Topology_Builder*)0)->face_topology;
    return (ptrdiff_t)builder + (ptrdiff_t)(topologies[direction & 1]);
}

U16 topology_builder_get_edge_pole(Topology_Builder *builder, U16 edge_index, U8 direction) {
    return topology_builder_get_topology_from_direction(builder, direction)->edges[edge_index].poles[(direction >> 1) & 1];
}

U16 topology_builder_topology_get_pole_edge(Topology_Builder_Topology *topology, U16 pole_index) {
    return topology->poles[pole_index].index_of_any_connected_edge;
}

Topology_Builder_Direction topology_builder_horizontal_direction(B8 backward) {
    return backward << 1 | 0;
}

Topology_Builder_Direction topology_builder_vertical_direction(B8 backward) {
    return backward << 1 | 1;
}

Topology_Builder_Direction topology_builder_rotate(Topology_Builder_Direction direction, I8 quarter_turns) {
    return (direction + quarter_turns) & TOPOLOGY_BUILDER_DIRECTION_MASK;
}

void topology_builder_topology_next_edge(Topology_Builder_Topology *topology, U16 *edge_index, B8 *direction) {
    Topology_Builder_Edge *edge = &topology->edges[*edge_index];
    U16 pole_index      = edge->poles[*direction ^ 1];
    U16 next_edge_index = edge->darts[*direction ^ 1];
    if(next_edge_index != TOPOLOGY_BUILDER_INVALID_EDGE)
        *direction ^= topology->edges[next_edge_index].poles[*direction] != pole_index;
    *edge_index = next_edge_index;
}

void topology_builder_next_edge(Topology_Builder *builder, U16 *edge_index, Topology_Builder_Direction *direction) {
    Topology_Builder_Topology *topology = topology_builder_get_topology_from_direction(builder, *direction);
    Topology_Builder_Edge *edge = &topology->edges[*edge_index];
    U16 pole_index      = edge->poles[((*direction >> 1) & 1) ^ 1];
    U16 next_edge_index = edge->darts[((*direction >> 1) & 1) ^ 1];
    if(next_edge_index != TOPOLOGY_BUILDER_INVALID_EDGE)
        *direction ^= (topology->edges[next_edge_index].poles[(*direction >> 1) & 1] != pole_index) << 1;
    *edge_index = next_edge_index;
}

void topology_builder_topology_previous_edge(Topology_Builder_Topology *topology, U16 *edge_index, B8 *direction) {
    U16                        start_edge_index = *edge_index;
    U16                        end_edge_index;
    Topology_Builder_Direction end_edge_direction;
    do {
        end_edge_index     = *edge_index;
        end_edge_direction = *direction;
        *direction ^= 1;
        topology_builder_topology_next_edge(topology, edge_index, direction);
    } while(*edge_index != TOPOLOGY_BUILDER_INVALID_EDGE && *edge_index != start_edge_index);
    *edge_index = end_edge_index;
    *direction  = end_edge_direction ^ 1;
}
void topology_builder_previous_edge(Topology_Builder *builder, U16 *edge_index, Topology_Builder_Direction *direction) {
    U16                        start_edge_index = *edge_index;
    U16                        end_edge_index;
    Topology_Builder_Direction end_edge_direction;
    do {
        end_edge_index     = *edge_index;
        end_edge_direction = *direction;
        *direction = topology_builder_rotate(*direction, 2);
        topology_builder_next_edge(builder, edge_index, direction);
    } while(*edge_index != TOPOLOGY_BUILDER_INVALID_EDGE && *edge_index != start_edge_index);
    *edge_index = end_edge_index;
    *direction  = topology_builder_rotate(end_edge_direction, -2);
}

U16 topology_builder_get_next_edge_around_pole(Topology_Builder *builder, Topology_Builder_Topology *topology, U16 edge_index, U16 pole_index) {
    Topology_Builder_Edge *edge = &topology->edges[edge_index];
    Int index_of_pole_within_edge = edge->poles[0] == pole_index ? 0 : 1;
    return edge->darts[index_of_pole_within_edge];
}

B8 topology_builder_topology_get_direction(Topology_Builder_Topology *topology, U16 edge_index, U16 pole_index) {
    return topology->edges[edge_index].poles[0] != pole_index;
}

Topology_Builder_Direction topology_builder_get_direction(
    Topology_Builder_Topology *topology, 
    U16 edge_index, 
    U16 tail_pole_index,
    B8 in_dual
) {
    return ((topology->edges[edge_index].poles[0] != tail_pole_index) << 1) | in_dual;
}

void topology_builder_insert_edges(Topology_Builder *builder, U16 num_edges, U16** edges) {
#ifndef TOPOLOGY_BUILDER_DISABLE_INDEX_POOL
    index_pool_u16_remove(&builder->edge_index_pool, num_edges, edges);
    for(U16 edge_index_index = 0; edge_index_index != num_edges; ++edge_index_index) {
        U16 edge_index = edges[edge_index_index];
        
        builder->vertex_topology.edges[edge_index].darts[0] = TOPOLOGY_BUILDER_INVALID_EDGE;
        builder->vertex_topology.edges[edge_index].darts[1] = TOPOLOGY_BUILDER_INVALID_EDGE;
        builder->vertex_topology.edges[edge_index].poles[0] = TOPOLOGY_BUILDER_INVALID_POLE;
        builder->vertex_topology.edges[edge_index].poles[1] = TOPOLOGY_BUILDER_INVALID_POLE;

        builder->face_topology.edges[edge_index].darts[0] = TOPOLOGY_BUILDER_INVALID_EDGE;
        builder->face_topology.edges[edge_index].darts[1] = TOPOLOGY_BUILDER_INVALID_EDGE;
        builder->face_topology.edges[edge_index].poles[0] = TOPOLOGY_BUILDER_INVALID_POLE;
        builder->face_topology.edges[edge_index].poles[1] = TOPOLOGY_BUILDER_INVALID_POLE;
    }
#else 
    U16 builder_num_edges = builder->num_edges;
    for (U16 idx_edge = 0; idx_edge != num_edges; ++idx_edge)
        edges[idx_edge] = builder_num_edges + idx_edge;
    builder_num_edges += num_edges;
    builder->num_edges = builder_num_edges;
#endif
}

U16 topology_builder_topology_find_edge_by_connected_poles(
    Topology_Builder_Topology *topology, 
    U16 some_pole_index, 
    U16 other_pole_index
) {
    U16 edge_index = topology_builder_topology_get_pole_edge(topology, some_pole_index);
    if(edge_index != TOPOLOGY_BUILDER_INVALID_EDGE) {
        U16 start_edge_index = edge_index;
        B8 direction = topology_builder_topology_get_direction(
            topology,
            edge_index,
            some_pole_index
        );
        for(;;) {
            Topology_Builder_Edge *edge;
            topology_builder_topology_get_edge(
                topology,
                edge_index,
                &edge
            );
            if(edge->poles[direction ^ 1] == other_pole_index) 
                return edge_index;
            direction = !direction;
            topology_builder_topology_next_edge(
                topology,
                &edge_index,
                &direction
            );
            if(edge_index == start_edge_index)
                break;
        }
    }
    return TOPOLOGY_BUILDER_INVALID_EDGE;
}

void topology_builder_topology_disconnect_edge_from_pole_by_edge_slot(
    Topology_Builder_Topology *topology,
    U16                        edge_index,
    B8                         edge_slot_index,
    B8                         insert_index_of_poles_with_no_connected_edge_back_to_pool
) {
    Topology_Builder_Edge *edge = &topology->edges[edge_index];
    U16 current_edge_pole_index = edge->poles[edge_slot_index];
    if(current_edge_pole_index != TOPOLOGY_BUILDER_INVALID_POLE) {
        /* We are trying to rewire an edge that already has a connection to some pole on the desired slot. 
        * Therefore, we first need to disconnect the edge before making a new connection. 
        * To do so, we need to remove the edge from the linked list that circles around the old pole. */
        U16 previous_edge_index = edge_index;
        U8  direction           = edge_slot_index;
        topology_builder_topology_previous_edge(
            topology,
            &previous_edge_index,
            &direction
        );
        if(previous_edge_index == edge_index) {
            /* The old pole only had that one edge attached to it. 
            * Since we are rewiring this edge to some other pole, the pole won't have any edge connected to it anymore.
            * Therefore, we mark the pole as having no connected edges left using the invalid index. */
            topology->poles[current_edge_pole_index].index_of_any_connected_edge = TOPOLOGY_BUILDER_INVALID_EDGE;
            if(insert_index_of_poles_with_no_connected_edge_back_to_pool) {

            }
        } else {
            /* The old pole has at least one other edge that the one we are trying to disconnect it from.
            * Therefore we edit the linked list so to remove the edge we are rewiring. */
            topology->edges[previous_edge_index].darts[!direction] = edge->darts[edge_slot_index];
            /* Just in case the old pole was directly referencing the edge we are rewiring,
            * we modify the reference to point at the previous edge, wich we know is attached to the old pole. */
            topology->poles[current_edge_pole_index].index_of_any_connected_edge = previous_edge_index;
        }
    }
}

void topology_builder_connect_edge_to_pole(
    Topology_Builder_Topology *topology,
    U16                        edge_index, 
    U16                        pole_index, 
    B8                         edge_slot_index
) {
    Topology_Builder_Edge *edge;
    topology_builder_topology_get_edge(topology, edge_index, &edge);
    U16 current_edge_pole_index = edge->poles[edge_slot_index];
    if(current_edge_pole_index != TOPOLOGY_BUILDER_INVALID_POLE)
        topology_builder_topology_disconnect_edge_from_pole_by_edge_slot(topology, edge_index, edge_slot_index, FALSE);
    edge->poles[edge_slot_index] = pole_index;
    Topology_Builder_Pole *pole      = &topology->poles[pole_index];
    U16 index_of_some_edge_connected_to_pole = pole->index_of_any_connected_edge;
    if(index_of_some_edge_connected_to_pole != 0xFFFF) {
        Topology_Builder_Edge *some_edge_connected_to_pole;
        some_edge_connected_to_pole = &topology->edges[index_of_some_edge_connected_to_pole];
        Int connected_edge_slot_index = some_edge_connected_to_pole->poles[0] == pole_index ? 0 : 1;
        edge->darts[edge_slot_index] = some_edge_connected_to_pole->darts[connected_edge_slot_index];
        some_edge_connected_to_pole->darts[connected_edge_slot_index] = edge_index;
    } else {
        edge->darts[edge_slot_index] = edge_index;
    }
    pole->index_of_any_connected_edge = edge_index;
}

// topology_builder_connect_edges
// topology_builder_connect_poles
// topology_builder_connect_edge_to_pole
// topology_builder_connect_pole_to_edge
U16 topology_builder_connect_poles(
    Topology_Builder *builder, 
    Topology_Builder_Topology *topology,
    U16 origin_pole_index, 
    U16 destination_pole_index
) {
    U16 edge_index =
        topology_builder_topology_find_edge_by_connected_poles(
                topology, 
                origin_pole_index, 
                destination_pole_index
        );
    if(edge_index != TOPOLOGY_BUILDER_INVALID_EDGE)
        /* Poles are already connected. */
        return edge_index;
    topology_builder_insert_edges(builder, 1, &edge_index);
    topology_builder_connect_edge_to_pole(topology, edge_index, origin_pole_index, 0);
    topology_builder_connect_edge_to_pole(topology, edge_index, destination_pole_index, 1);
    return edge_index;
}

void topology_builder_create_vertices(Topology_Builder *builder, U16 num_vertices, U16* vertices) {
    index_pool_u16_remove(&builder->vertex_topology.index_pool, num_vertices, vertices);
}

void topology_builder_destroy_vertices(Topology_Builder *builder, U16 num_vertices, U16* vertices) {
    for(U16 vertex_index_index = 0; vertex_index_index != num_vertices; ++vertex_index_index) {
        U16 vertex_index = vertices[vertex_index_index];
        Topology_Builder_Pole *vertex = &builder->vertex_topology.poles[vertex_index];
        U16 edge_index = vertex->index_of_any_connected_edge;
        if(edge_index != TOPOLOGY_BUILDER_INVALID_EDGE) {
            /* In the case that vertex is connected to any edge. */
            U16 start_edge_index = edge_index;
            B8 direction = topology_builder_topology_get_direction(
                &builder->vertex_topology,
                edge_index,
                vertex_index
            );
            for(;;) {
                /* Destroy evert single edge connected to that vertex */
                topology_builder_topology_disconnect_edge_from_pole_by_edge_slot(
                    &builder->vertex_topology,
                    edge_index,
                    direction ^ 1,
                    FALSE
                );
                #ifndef TOPOLOGY_BUILDER_DISABLE_DUAL
                topology_builder_topology_disconnect_edge_from_pole_by_edge_slot(
                    &builder->face_topology,
                    edge_index,
                    0,
                    TRUE
                );
                topology_builder_topology_disconnect_edge_from_pole_by_edge_slot(
                    &builder->face_topology,
                    edge_index,
                    1,
                    TRUE
                );
                #endif /* !TOPOLOGY_BUILDER_DISABLE_DUAL */
                index_pool_u16_insert(&builder->edge_index_pool , 1, &edge_index);
                direction = !direction;
                topology_builder_topology_next_edge(
                    &builder->vertex_topology,
                    &edge_index,
                    &direction
                );
                if(edge_index == start_edge_index)
                    break;
            }
            vertex->index_of_any_connected_edge = TOPOLOGY_BUILDER_INVALID_EDGE;
        }
    }
    index_pool_u16_insert(&builder->vertex_topology.index_pool, num_vertices, vertices);
}

void topology_builder_create_faces(Topology_Builder *builder, U16 num_faces, U16* face_indices) {
    index_pool_u16_remove(&builder->face_topology.index_pool, num_faces, face_indices);
}