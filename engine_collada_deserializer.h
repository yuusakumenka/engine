/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* ===========================================================================================
 * Design Goals 
 * =========================================================================================== 
 * This library does zero memory allocation. You need to do it yourself.
 * We use malloc() in the folowing example but replace it with the allocator of your choice. 
 *
 * ===========================================================================================
 * Usage 
 * =========================================================================================== 
 * 
 *  main() {
 *      char   *collada_serialized_xml_data;
 *      size_t  collada_serialized_xml_data_length;
 *   
 *      ... Use your io, filesystem calls, resource manager to get that data ...
 * 
 *      size_t num_work_buffer_bytes = collada_deserializer_num_work_buffer_bytes(4,8,8,4);
 *      // Look at what kind of files you handle and fill parameters acordingly.
 * 
 *      void *work_buffer = malloc(num_work_buffer_bytes);
 * 
 *      Collada_Deserializer deserializer = {
 *          .work_buffer      = work_buffer,
 *          .work_buffer_size = num_work_buffer_bytes,
 *          .work_buffer_head = work_buffer,
 *      };
 *   
 *      Collada_Deserializer_Mesh mesh;
 *      collada_deserialize_mesh(&deserializer, &mesh);
 * 
 *      size_t num_position_floats  = collada_deserialize_num_position_floats(&mesh);
 *      float *position_float_array = malloc(num_position_floats * sizeof(float));
 * 
 *      size_t num_vertices = collada_deserializer_mesh_get_num_vertices_floats(&mesh);
 *      typedef struct Vertex {
 *          float position_x, position_y, position_z;
 *      } Vertex;
 *      size_t num_vertices = collada_deserialize_num_vertices(&mesh);
 *      Vertex* vertex_buffer = malloc(num_vertices * vertex_buffer); * 
 * 
 *      size_t          num_triangles_indices = collada_deserialize_num_triangles_indices(&mesh);
 *      unsigned short *triangles_index_array = malloc(num_triangles_indices * unsigned short);
 * 
 *      collada_deserialize_mesh_vertex_positions_v3(
 *          &mesh, 
 *          triangles_index_array, 
 *          position_float_array,
 *          vertex_buffer, 
 *          sizeof(Vertex)
 *      );
 *      // you can divide similar operations to different threads
 * 
 *      size_t          num_triangles_indices  = collada_deserialize_num_triangles_indices(&mesh);
 *      unsigned short *triangles_index_buffer = malloc(num_triangles_indices * unsigned short);
 * 
 *      collada_deserialize_mesh_indices(
 *          &mesh, 
 *          triangles_index_array,
 *          index_buffer, 
 *          sizeof(unsigned short)
 *      );
 * 
 *      free(triangles_index_array);
 *      free(position_float_array);
 *      free(work_buffer);
 * 
 *      ... transfer vertex_buffer and index_buffer to gpu ...
 * 
 *      free(vertex_buffer);
 *      free(index_buffer);
 * }
 * */

/* ===========================================================================================
 * Macros 
 * =========================================================================================== */

/* Config */

#define COLLADA_DESERIALIZER_CONVERT_COORDINATE_SPACE_FROM_X_RIGHT_Y_FORWARD_Z_UP_TO_X_RIGHT_Y_UP_Z_FORWARD

/* Errors */
#define COLLADA_DESERIALIZER_ASSERT_ERROR(test, error) if(!(test)) return error;



/* ===========================================================================================
 * Data 
 * =========================================================================================== */

/* Collada deserializer errors are a superset of Xml errors. */
#define COLLADA_DESERIALIZER_ERROR_TABLE(OP) \
OP(COLADA_DESERIALIZER_ERROR_OUT_OF_WORK_BUFFER_MEMORY) \
OP(COLADA_DESERIALIZER_ERROR_NO_WEIGHTS_FOUND) \
OP(COLADA_DESERIALIZER_ERROR_NO_JOINTS_FOUND) \
OP(COLADA_DESERIALIZER_ERROR_NO_INVERSE_BIND_MATRICES_FOUND) \
/* endof COLLADA_DESERIALIZER_ERROR_TABLE */

#define COLLADA_READER_EACH_TRIANGLES(reader) \
    Collada_Deserializer_Triangles *it = (reader)->first_triangles;\
    it != 0;\
    it = it->next

typedef U8 Collada_Deserializer_Error;
typedef enum Collada_Deserializer_Error_ {
    /* Collada error codes start after the last Xml error code. */
    COLADA_ERROR_DUMMY = XML_READER_ERROR_COUNT - 1,
    #define OP(error) error,
    COLLADA_DESERIALIZER_ERROR_TABLE(OP)
    #undef OP
    COLADA_ERROR_COUNT,
} Collada_Deserializer_Error_;

#ifndef COLLADA_DISABLE_ERROR_STRINGS
static const char* const COLLADA_READER_ERROR_STRINGS[] = {
    #define OP(error) #error,
    XML_READER_ERROR_TABLE(OP)
    #undef OP
    #define OP(error) #error,
    COLLADA_DESERIALIZER_ERROR_TABLE(OP)
    #undef OP
};
#endif /* !COLLADA_DISABLE_ERROR_STRINGS */

/* Float Array */
typedef struct Collada_Deserializer_Float_Array {
    S8  id;
    U64 count;
    S8  serialized_values;
} Collada_Deserializer_Float_Array;

typedef struct Collada_Deserializer_Name_Array {
    S8  id;
    U64 count;
    S8  serialized_values;
} Collada_Deserializer_Name_Array;

typedef union Collada_Deserializer_Array {
    Collada_Deserializer_Float_Array as_float_array;
    Collada_Deserializer_Name_Array  as_name_array;
} Collada_Deserializer_Array;

/* Input */
typedef struct Collada_Deserializer_Input Collada_Deserializer_Input;
typedef struct Collada_Deserializer_Input {
    Collada_Deserializer_Input *next;
    S8                    semantic;
    S8                    source;
    U8                    offset;
    U8                    set;
} Collada_Deserializer_Input;
typedef struct Collada_Deserializer_Input_List {
    U64                   num_inputs;
    Collada_Deserializer_Input *first_input;
    Collada_Deserializer_Input *last_input;
} Collada_Deserializer_Input_List;

/* Sources */
typedef struct Collada_Deserializer_Source Collada_Deserializer_Source;
typedef struct Collada_Deserializer_Source {
    Collada_Deserializer_Source     *next;
    S8                               id;
    Collada_Deserializer_Array       array;
} Collada_Deserializer_Source;

/* Vertices */
typedef struct Collada_Deserializer_Vertices {
    S8                              id;
    Collada_Deserializer_Input_List inputs;
} Collada_Deserializer_Vertices;

/* Triangles */
typedef struct Collada_Deserializer_Triangles Collada_Deserializer_Triangles;
typedef struct Collada_Deserializer_Triangles {
    Collada_Deserializer_Triangles *next;
    U64                             count;
    S8                              serialized_p_values;
    Collada_Deserializer_Input_List inputs;
} Collada_Deserializer_Triangles;

typedef struct Collada_Deserializer_Vertex_Weights {    
    U16                             count;
    Collada_Deserializer_Input_List inputs;
    S8                              vcount;
    S8                              v;
    U64                             v_num_values;
} Collada_Deserializer_Vertex_Weights;

typedef struct Collada_Deserializer_Node {
    struct Collada_Deserializer_Node *parent;
    struct Collada_Deserializer_Node *first_child;
    struct Collada_Deserializer_Node *last_child;
    struct Collada_Deserializer_Node *next_sibling;
    struct Collada_Deserializer_Node *previous_sibling;
    S8         type;
    S8         sid;
    V3         scale;
    V3         translate;
    Rotor3 rotation;
} Collada_Deserializer_Node;


typedef struct Collada_Deserializer_Mesh {    
    /* Sources */
    U64                             num_sources;
    Collada_Deserializer_Source    *first_source;
    Collada_Deserializer_Source    *last_source;
    
    /* Vertices */
    Collada_Deserializer_Vertices   vertices;
    
    /* Triangles */
    U64                       num_triangles;
    Collada_Deserializer_Triangles *first_triangles;
    Collada_Deserializer_Triangles *last_triangles;

    /* Skin */
    Collada_Deserializer_Input_List     joints_inputs;
    Collada_Deserializer_Vertex_Weights vertex_weights;

    /* Nodes */
    Collada_Deserializer_Node root;
} Collada_Deserializer_Mesh;

typedef struct Collada_Deserializer {
    size_t work_buffer_size;
    char  *work_buffer;
    char  *work_buffer_head;
} Collada_Deserializer;



/* ===========================================================================================
 * API 
 * =========================================================================================== */

U64  collada_deserializer_num_work_buffer_bytes(U64 num_input_max, U64 num_sources_max, U64 num_vertices_max, U64 num_triangles_max, U64 num_nodes_max);
Collada_Deserializer_Error collada_deserializer_mesh(Collada_Deserializer *deserializer, U64 source_length, char *source_data, M4 coordinate_system, M4 coordinate_system_rot, Collada_Deserializer_Mesh *mesh);

U64  collada_deserializer_mesh_get_num_triangles_indices(Collada_Deserializer_Mesh *mesh);
U16  collada_deserializer_mesh_get_num_vertex_position_floats (Collada_Deserializer_Mesh *mesh);
U16  collada_deserializer_mesh_get_num_vertices (Collada_Deserializer_Mesh *mesh);
U16  collada_deserializer_mesh_get_num_triangles(Collada_Deserializer_Mesh *mesh);
U8   collada_deserializer_mesh_get_num_joints   (Collada_Deserializer_Mesh *mesh);

void collada_deserializer_mesh_parse_vertex_positions_float_array(Collada_Deserializer_Mesh *mesh, F32 *float_array);
void collada_deserializer_mesh_parse_triangles_index_array(Collada_Deserializer_Mesh *mesh, U16 *float_array);

void collada_deserializer_mesh_vertex_positions_v3(Collada_Deserializer_Mesh *mesh, U16 *parsed_triangles_index_array, F32 *parsed_vertex_position_float_array, V3 *dst, Uint stride);
void collada_deserializer_mesh_vertex_texcoord0_v2(Collada_Deserializer_Mesh *mesh, V2 *dst, Uint stride);
void collada_deserializer_mesh_vertex_joints_v4u8 (Collada_Deserializer_Mesh *mesh, U8 dst[4], Uint stride);
Collada_Deserializer_Error collada_deserializer_mesh_vertex_weights_v4  (Collada_Deserializer_Mesh *mesh, V4* dst, Uint stride);

void collada_deserializer_mesh_triangle_indices(Collada_Deserializer *reader, U16* dst);

Collada_Deserializer_Error collada_deserializer_mesh_inverse_bind_matrices_m4(Collada_Deserializer_Mesh *mesh, M4 *dst);

// Internals
void *collada_deserializer_work_buffer_insert   (Collada_Deserializer            *collada, U64                             size     );
void  collada_deserializer_mesh_insert_source   (Collada_Deserializer_Mesh       *mesh   , Collada_Deserializer_Source    *source   );
void  collada_deserializer_mesh_insert_triangles(Collada_Deserializer_Mesh       *mesh   , Collada_Deserializer_Triangles *triangles);
void  collada_deserializer_input_list_insert    (Collada_Deserializer_Input_List *list   , Collada_Deserializer_Input     *input    );
Uint  collada_input_list_index_of_input_by_semantic(Collada_Deserializer_Input_List *list, S8 semantic);



/* ===========================================================================================
 * Implementation 
 * =========================================================================================== */

U64 collada_deserializer_num_work_buffer_bytes(
    U64 num_input_max, 
    U64 num_sources_max, 
    U64 num_vertices_max, 
    U64 num_triangles_max, 
    U64 num_nodes_max
) {
    return 
        sizeof(Collada_Deserializer_Vertices ) * num_vertices_max  +
        sizeof(Collada_Deserializer_Source   ) * num_sources_max   +
        sizeof(Collada_Deserializer_Triangles) * num_triangles_max +
        sizeof(Collada_Deserializer_Input    ) * num_input_max     +
        sizeof(Collada_Deserializer_Node     ) * num_nodes_max     +
        0;
}

void *collada_deserializer_work_buffer_insert(Collada_Deserializer *collada, U64 size) {
    void *data = collada->work_buffer_head;
    collada->work_buffer_head += size;
    if((size_t)(collada->work_buffer_head - collada->work_buffer) > collada->work_buffer_size)
        return 0; /* Out of memory. */
    #ifndef COLLADA_ASSUME_WORK_BUFFER_IS_ZERO_INITIALIZED
    memset(data, 0x00, size);
    #endif /* !COLLADA_ASSUME_WORK_BUFFER_IS_ZERO_INITIALIZED */
    return data;
}
#define COLLADA_DESERIALIZER_INSERT_STRUCT(collada, struct_type) \
    (struct_type*)collada_deserializer_work_buffer_insert(collada, sizeof(struct_type))

void collada_deserializer_mesh_insert_source(Collada_Deserializer_Mesh *mesh, Collada_Deserializer_Source *source) {
    ++mesh->num_sources;
    if(!mesh->first_source) {
        mesh->first_source = source;
    } else {
        mesh->last_source->next = source;
    }
    mesh->last_source = source;
}

void collada_deserializer_mesh_insert_triangles(Collada_Deserializer_Mesh *mesh, Collada_Deserializer_Triangles *triangles) {
    ++mesh->num_triangles;
    if(!mesh->first_triangles) {
        mesh->first_triangles = triangles;
    } else {
        mesh->last_triangles->next = triangles;
    }
    mesh->last_triangles = triangles;
}

void collada_deserializer_input_list_insert(Collada_Deserializer_Input_List *list, Collada_Deserializer_Input *input) {
    ++list->num_inputs;
    if(!list->first_input) {
        list->first_input = input;
    } else {
        list->last_input->next = input;
    }
    list->last_input = input;
}

void collada_deserializer_node_insert_child(Collada_Deserializer_Node *parent, Collada_Deserializer_Node *child) {
    if(parent->last_child) {
        Collada_Deserializer_Node *previous_last_child = parent->last_child;
        previous_last_child->next_sibling = child;
        parent->last_child      = child;
        child->parent           = parent;
        child->next_sibling     = 0;
        child->previous_sibling = previous_last_child;
    } else {
        parent->first_child     = child;
        parent->last_child      = child;
        child->parent           = parent;
        child->next_sibling     = 0;
        child->previous_sibling = 0;
    }
}

Collada_Deserializer_Node *collada_deserializer_node_next(Collada_Deserializer_Node *node) {
    if(node->first_child) {
        return node->first_child;
    }
    BACK_TO_PARENT:;
    if(node->next_sibling) {
        return node->next_sibling;
    }
    if(node->parent) {
        node = node->parent;
        goto BACK_TO_PARENT;
    }
    return 0;
}

Collada_Deserializer_Error collada_deserializer_mesh(
    Collada_Deserializer      *collada,
    U64                        source_length,
    char                      *source_data,
    M4                         coordinate_system,
    M4                         coordinate_system_rot,
    Collada_Deserializer_Mesh *mesh
) {
    /* Todo, move those to Collada_Reader */
    Collada_Deserializer_Source     *source;
    Collada_Deserializer_Triangles  *triangles;
    Collada_Deserializer_Input      *input;
    Collada_Deserializer_Node       *node = &mesh->root;
    S8                         active_element_tag;
    Collada_Deserializer_Input_List *active_element_inputs;

    Xml_Reader xml; /* Xml_Reader ? */
    xml_reader_begin(&xml, source_length, source_data);

    Xml_Reader_Event event;
    Xml_Reader_Error error;
    while((error = xml_reader_next_event(&xml, &event)) == XML_READER_ERROR_NONE) {
        switch(event.kind) {
        case XML_EVENT_KIND_ELEMENT_STARTS: {
            /* @FIXME(menyuu) No need for proc call here, just inline the inserts. */
            /*  */ if(S8_EQUALS_LIT(event.as_element_starts.tag, "source")) {
                source = COLLADA_DESERIALIZER_INSERT_STRUCT(collada, Collada_Deserializer_Source);
                COLLADA_DESERIALIZER_ASSERT_ERROR(source, COLADA_DESERIALIZER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
                collada_deserializer_mesh_insert_source(mesh, source);
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "triangles")) {
                triangles = COLLADA_DESERIALIZER_INSERT_STRUCT(collada, Collada_Deserializer_Triangles);
                COLLADA_DESERIALIZER_ASSERT_ERROR(triangles, COLADA_DESERIALIZER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
                collada_deserializer_mesh_insert_triangles(mesh, triangles);
                active_element_inputs = &triangles->inputs;
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "input")) {
                input = COLLADA_DESERIALIZER_INSERT_STRUCT(collada, Collada_Deserializer_Input);
                COLLADA_DESERIALIZER_ASSERT_ERROR(input, COLADA_DESERIALIZER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
                collada_deserializer_input_list_insert(active_element_inputs, input);
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "vertices")) {
                active_element_inputs = &mesh->vertices.inputs;
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "joints")) {
                active_element_inputs = &mesh->joints_inputs;
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "vertex_weights")) {
                active_element_inputs = &mesh->vertex_weights.inputs;
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "node")) {
                Collada_Deserializer_Node *parent = node;
                node = COLLADA_DESERIALIZER_INSERT_STRUCT(collada, Collada_Deserializer_Node);
                COLLADA_DESERIALIZER_ASSERT_ERROR(node, COLADA_DESERIALIZER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
                collada_deserializer_node_insert_child(parent, node);
                node->rotation = ROTOR3_IDENTITY;
            }
            
            /* Nothing to do for those elements:
             *   * float_array (Already in source.)
             *   * p           (Already in triangles, or other primitives...)
             */
            active_element_tag = event.as_element_starts.tag;
        } break;
        case XML_EVENT_KIND_ATTRIBUTE: {            
            #define HANDLE_ATTRIBUTE_S8(attribute, dst) \
                if(S8_EQUALS_LIT(event.as_attribute.key, #attribute)) \
                    dst = s8_unescape_cheap(event.as_attribute.value);
            #define HANDLE_ATTRIBUTE_U64(attribute, dst) \
                if(S8_EQUALS_LIT(event.as_attribute.key, #attribute)) \
                    dst = s8_parse_u64(s8_unescape_cheap(event.as_attribute.value));
                    
            /*  */ if(S8_EQUALS_LIT(active_element_tag, "source")) {
                HANDLE_ATTRIBUTE_S8(id, source->id);

            } else if(S8_EQUALS_LIT(active_element_tag, "float_array")) {
                HANDLE_ATTRIBUTE_S8 (id   , source->array.as_float_array.id   );
                HANDLE_ATTRIBUTE_U64(count, source->array.as_float_array.count);

            } else if(S8_EQUALS_LIT(active_element_tag, "Name_array")) {
                HANDLE_ATTRIBUTE_S8 (id   , source->array.as_name_array.id   );
                HANDLE_ATTRIBUTE_U64(count, source->array.as_name_array.count);

            } else if(S8_EQUALS_LIT(active_element_tag, "triangles")) {
                HANDLE_ATTRIBUTE_U64(count, triangles->count);

            } else if(S8_EQUALS_LIT(active_element_tag, "input")) {
                HANDLE_ATTRIBUTE_U64(offset  , input->offset  );
                HANDLE_ATTRIBUTE_U64(set     , input->set     );
                HANDLE_ATTRIBUTE_S8 (semantic, input->semantic);
                HANDLE_ATTRIBUTE_S8 (source  , input->source  );

            } else if(S8_EQUALS_LIT(active_element_tag, "vertices")) {
                HANDLE_ATTRIBUTE_S8 (id, mesh->vertices.id);

            } else if(S8_EQUALS_LIT(active_element_tag, "vertex_weights")) {
                HANDLE_ATTRIBUTE_U64(count, mesh->vertex_weights.count);

            } else if(S8_EQUALS_LIT(active_element_tag, "node")) {
                HANDLE_ATTRIBUTE_S8(sid, node->sid);
                HANDLE_ATTRIBUTE_S8(type, node->type);

            }
            /* Nothing to do for those elements:
             *   * p           (No attributes.)
             */

            #undef HANDLE_ATTRIBUTE_U64
            #undef HANDLE_ATTRIBUTE_S8
        } break;
        case XML_EVENT_KIND_TEXT_NODE: {
            /*  */ if(S8_EQUALS_LIT(active_element_tag, "float_array")) {
                source->array.as_float_array.serialized_values = event.as_text_node.value;
            } else if(S8_EQUALS_LIT(active_element_tag, "Name_array")) {
                source->array.as_name_array.serialized_values = event.as_text_node.value;
            } else if(S8_EQUALS_LIT(active_element_tag, "p")) {
                /* Todo, find the appropriate topology to associate primitives with. */
                triangles->serialized_p_values = event.as_text_node.value;
            } else if(S8_EQUALS_LIT(active_element_tag, "vcount")) {
                mesh->vertex_weights.vcount = event.as_text_node.value;
            } else if(S8_EQUALS_LIT(active_element_tag, "v")) {
                mesh->vertex_weights.v            = event.as_text_node.value;
                mesh->vertex_weights.v_num_values = event.as_text_node.num_words;
            } else if(S8_EQUALS_LIT(active_element_tag, "scale")) {
                s8_parse_space_separated_f32(event.as_text_node.value, &node->scale);
            } else if(S8_EQUALS_LIT(active_element_tag, "rotate")) {
                F32 as_axis_angle[4];
                s8_parse_space_separated_f32(event.as_text_node.value, as_axis_angle);
                V3 axis = v3(as_axis_angle[0], as_axis_angle[1], as_axis_angle[2]);
                axis = m4_mul_vector(coordinate_system_rot, axis);
                Rotor3 as_quaternion = rotor3_axis_angle(
                    axis,
                    F32_DEG_TO_RAD * as_axis_angle[3]
                );
                node->rotation = rotor3_mul(node->rotation, as_quaternion);
            } else if(S8_EQUALS_LIT(active_element_tag, "translate")) {
                V3 translate;
                s8_parse_space_separated_f32(event.as_text_node.value, &translate);
                translate = m4_mul_point(coordinate_system, translate);
                node->translate = translate;
            }
        } break;
        case XML_EVENT_KIND_ELEMENT_ENDS: {
            if(S8_EQUALS_LIT(event.as_element_ends.tag, "node")) {
                if(node->parent)
                    node = node->parent;
            }
        } break;
        case XML_EVENT_KIND_END_OF_FILE: 
            goto END_OF_FILE_REACHED;
        }
    }
    if(error)
        /* Propagate error. */
        return error;
    END_OF_FILE_REACHED:;
    return XML_READER_ERROR_NONE;
}

Uint collada_input_list_index_of_input_by_semantic(Collada_Deserializer_Input_List *list, S8 semantic) {
    Uint index = 0;
    for(Collada_Deserializer_Input *input = list->first_input; input; input = input->next) {
        if(s8_equals(input->semantic, semantic)) 
            return index;
        ++index;
    }
    return (Uint)-1;
}

Collada_Deserializer_Input *collada_input_list_find_input_by_semantic(Collada_Deserializer_Input_List *list, S8 semantic) {
    for(Collada_Deserializer_Input *input = list->first_input; input; input = input->next) {
        if(s8_equals(input->semantic, semantic)) 
            return input;
    }
    return 0;
}

Collada_Deserializer_Source *collada_deserializer_mesh_find_source_by_reference(Collada_Deserializer_Mesh *mesh, S8 reference) {
    for(Collada_Deserializer_Source *source = mesh->first_source; source; source = source->next) {
        /* @FIXME(menyuu) I mean this works for our dataset... */
        if(s8_ends_by(reference, source->id)) 
            return source;
    }
    return 0;
}

U16 collada_deserializer_mesh_get_num_vertex_position_floats(Collada_Deserializer_Mesh *mesh) {
    Collada_Deserializer_Input  *input  = collada_input_list_find_input_by_semantic(&mesh->vertices.inputs, S8_LIT("POSITION"));
    if(!input) 
        return 0;
    Collada_Deserializer_Source *source = collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    return (U16)source->array.as_float_array.count;
}

U16 collada_deserializer_mesh_get_num_vertex_weights_floats(Collada_Deserializer_Mesh *mesh) {
    Collada_Deserializer_Input  *input  = collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("WEIGHT"));
    if(!input) 
        return 0;
    Collada_Deserializer_Source *source = collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    return (U16)source->array.as_float_array.count;
}

U64 collada_deserializer_mesh_get_num_vertex_weights_vcount(Collada_Deserializer_Mesh *mesh) {
    return mesh->vertex_weights.count;
}

U64 collada_deserializer_mesh_get_num_vertex_weights_vs(Collada_Deserializer_Mesh *mesh) {
    return mesh->vertex_weights.v_num_values;
}

void collada_deserializer_mesh_parse_vertex_positions_float_array(Collada_Deserializer_Mesh *mesh, float *values) {
    Collada_Deserializer_Input  *input  = collada_input_list_find_input_by_semantic(&mesh->vertices.inputs, S8_LIT("POSITION"));
    Collada_Deserializer_Source *source = collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    s8_parse_space_separated_f32(source->array.as_float_array.serialized_values, values);
}

U64 collada_deserializer_mesh_get_num_triangles_indices(Collada_Deserializer_Mesh *mesh) {
    return mesh->first_triangles->count * mesh->first_triangles->inputs.num_inputs * 3;
}

void collada_deserializer_mesh_parse_triangle_index_array(Collada_Deserializer_Mesh *mesh, U16 *values) {
    /* @FIXME(menyuu) Handle multiple <triangles>. */
    s8_parse_space_separated_u16(mesh->first_triangles->serialized_p_values, values);
}

U16  collada_deserializer_mesh_get_num_vertices(Collada_Deserializer_Mesh *mesh) {
    /* @FIXME(menyuu) Maybe we want to recycle vertices? */
    return mesh->first_triangles->count * 3;
}
U16  collada_deserializer_mesh_get_num_triangles(Collada_Deserializer_Mesh *mesh) {
    /* @FIXME(menyuu) Handle multiple <triangles>. */
    return mesh->first_triangles->count;
}
U8   collada_deserializer_mesh_get_num_joints(Collada_Deserializer_Mesh *mesh) {
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->joints_inputs, S8_LIT("INV_BIND_MATRIX"));
    if(!input) 
        return 0;
    Collada_Deserializer_Source *source = 
        collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    return source->array.as_float_array.count >> 4; /* >> 4 === / 16 : 4x4 float matrices === 16 floats */
}

void collada_deserializer_mesh_vertex_positions_v3(
    Collada_Deserializer_Mesh *mesh, 
    F32                       *float_array,
    U16                       *index_array,
    M4                         matrix,
    V3                        *dst, 
    Uint                       stride
) {
    // find the index of the vertex index per triangle vertex in the index buffer
    U8 index_of_input_tagged_as_vertex_semantic =
        collada_input_list_index_of_input_by_semantic(
            &mesh->first_triangles->inputs,
            S8_LIT("VERTEX")
        );

    // for each triangles
    U64 num_triangles = mesh->first_triangles->count;
    U64 num_semantic  = mesh->first_triangles->inputs.num_inputs;
    for(Int triangle = 0; triangle < num_triangles; ++triangle) {
        // for each triangle vertex
        for(Int triangle_vertex = 0; triangle_vertex < 3; ++triangle_vertex){
            // fetch vertex index
            U16 vertex_index = index_array[
                triangle * 3               * num_semantic +
                           triangle_vertex * num_semantic
            ];
            // copy vertex position data to dst
            V3 position;
            memcpy(
                &position,
                &float_array[vertex_index * 3],
                sizeof(V3)
            );
            position = m4_mul_point(matrix, position);
            *dst = position;
            dst = (V3*)(((char*)dst) + stride);
            int visualstudioisbugged = 1;
        }
    }
}

void collada_deserializer_mesh_parse_vertex_weights_v_array(Collada_Deserializer_Mesh *mesh, U16 *values) {
    s8_parse_space_separated_u16(mesh->vertex_weights.v, values);
}

void collada_deserializer_mesh_parse_vertex_weights_vcount_array(Collada_Deserializer_Mesh *mesh, U16 *values) {
    s8_parse_space_separated_u16(mesh->vertex_weights.vcount, values);
}

void collada_deserializer_mesh_sum_vertex_weights_vcount_array(Collada_Deserializer_Mesh *mesh, U16 *vcount_array, U16 *summed_vcount_array) {
    size_t num_vertex = mesh->vertex_weights.count;
    U16 sum = 0;
    for(size_t vertex_index = 0; vertex_index != num_vertex; ++vertex_index) {
        *summed_vcount_array = sum;
        sum += *vcount_array;
        ++summed_vcount_array;
        ++vcount_array;
    }
}

void collada_deserializer_mesh_parse_vertex_weights_float_array(Collada_Deserializer_Mesh *mesh, float *values) {
    Collada_Deserializer_Input  *input  = collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("WEIGHT"));
    if(!input) return;
    Collada_Deserializer_Source *source = collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    s8_parse_space_separated_f32(source->array.as_float_array.serialized_values, values);
}

Collada_Deserializer_Error collada_deserializer_mesh_vertex_weights_v4(
    Collada_Deserializer_Mesh *mesh, 
    U16                       *index_array,
    U16                       *vcount_array,
    U16                       *summed_vcount_array,
    U16                       *v_array,
    F32                       *float_array,
    V4                        *dst, 
    Uint                       stride
) {
    U16 num_vertices = mesh->vertex_weights.count;
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("WEIGHT"));
    if(!input) {
        /* no vertex weights, just fill with zeroes */
        U16 num_vertices = collada_deserializer_mesh_get_num_vertices(mesh);
        for(U16 vertex_index = 0; vertex_index != num_vertices; ++vertex_index) {
            dst->x = 0;
            dst->y = 0;
            dst->z = 0;
            dst->w = 0;
            dst = (V4*)(((char*)dst) + stride);
        }
        return COLADA_DESERIALIZER_ERROR_NO_WEIGHTS_FOUND;
    }
    U8  v_offset = input->offset;
    U16 v_stride = mesh->vertex_weights.inputs.num_inputs;
    // for each triangles
    U64 num_triangles = mesh->first_triangles->count;
    U64 num_semantic  = mesh->first_triangles->inputs.num_inputs;
    for(Int triangle = 0; triangle < num_triangles; ++triangle) {
        // for each triangle vertex
        for(Int triangle_vertex = 0; triangle_vertex < 3; ++triangle_vertex){
            // fetch vertex index
            U16 vertex_index = index_array[
                triangle * 3               * num_semantic +
                           triangle_vertex * num_semantic
            ];                        
            U16 vcount_offset        = summed_vcount_array[vertex_index];
            U16 vertex_vcount       = vcount_array[vertex_index];
            U16 vertex_weight_index = 0;
            for(; vertex_weight_index != vertex_vcount; ++vertex_weight_index) {
                if(vertex_weight_index < 4) {
                    U16 weight_float_index = v_array[(vcount_offset + vertex_weight_index) * v_stride + v_offset];
                    ((F32*)dst)[vertex_weight_index] = float_array[weight_float_index];
                }
            }
            for(; vertex_weight_index < 4; ++vertex_weight_index) {
                ((F32*)dst)[vertex_weight_index] = 0;
            }
            //*dst = v4_normalize(*dst);
            dst = (V4*)(((char*)dst) + stride);
        }
    }
    return XML_READER_ERROR_NONE;
}

Collada_Deserializer_Error collada_deserializer_mesh_joint_indices_as_v4u8_packed_as_little_endian_i32(
    Collada_Deserializer_Mesh *mesh, 
    U16                       *index_array,
    U16                       *vcount_array,        /* count  in v_array */
    U16                       *summed_vcount_array, /* offset in v_array */
    U16                       *v_array,
    I32                       *dst, 
    ptrdiff_t                  stride
) {
    U16 num_vertices = mesh->vertex_weights.count;
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("JOINT"));
    if(!input) {
        return COLADA_DESERIALIZER_ERROR_NO_WEIGHTS_FOUND;
    }
    U8  v_offset = input->offset;
    U16 v_stride = mesh->vertex_weights.inputs.num_inputs;
    // for each triangles
    U64 num_triangles = mesh->first_triangles->count;
    U64 num_semantic  = mesh->first_triangles->inputs.num_inputs;
    for(Int triangle = 0; triangle < num_triangles; ++triangle) {
        // for each triangle vertex
        for(Int triangle_vertex = 0; triangle_vertex < 3; ++triangle_vertex){
            // fetch vertex index
            U16 vertex_index = index_array[
                triangle * 3               * num_semantic +
                           triangle_vertex * num_semantic
            ];                        
            U16 weight_count         = vcount_array[vertex_index];
            U16 vcount_offset        = summed_vcount_array[vertex_index];
            U16 vertex_weight_index  = 0;
            I32 packed_joint_indices = 0;
            for(; vertex_weight_index != weight_count; ++vertex_weight_index) {
                U16 joint_index = v_array[(vcount_offset + vertex_weight_index) * v_stride + v_offset];
                packed_joint_indices |= joint_index << (vertex_weight_index << 3);
                if(vertex_weight_index == (4 - 1))
                    break;
            }
            *dst = packed_joint_indices;
            dst = (I32*)(((char*)dst) + stride);
        }
    }
    return XML_READER_ERROR_NONE;
}

void collada_deserializer_mesh_triangle_indices(Collada_Deserializer_Mesh *mesh, U16* dst) {
    U64 num_triangles = mesh->first_triangles->count;
    for(U16 triangle_index = 0; triangle_index != num_triangles; ++triangle_index) {
        dst[triangle_index * 3 + 0] = triangle_index * 3 + 0;
        dst[triangle_index * 3 + 1] = triangle_index * 3 + 2;
        dst[triangle_index * 3 + 2] = triangle_index * 3 + 1;
    }
}

Collada_Deserializer_Error collada_deserializer_mesh_inverse_bind_matrices_m4(
    Collada_Deserializer_Mesh *mesh,
    M4                         coordinate_system,
    M4                        *dst,
    ptrdiff_t                  stride
) {
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->joints_inputs, S8_LIT("INV_BIND_MATRIX"));
    if(!input) 
        return COLADA_DESERIALIZER_ERROR_NO_INVERSE_BIND_MATRICES_FOUND;
    Collada_Deserializer_Source *source = 
        collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    size_t num_inverse_bind_matrix = source->array.as_float_array.count / 16;
    S8 str = source->array.as_float_array.serialized_values;
    for(
        size_t inverse_bind_matrix_index = 0; 
        inverse_bind_matrix_index != num_inverse_bind_matrix; 
        ++inverse_bind_matrix_index
    ) {
        M4 inverse_bind_matrix;
        M4 inverse_bind_matrix2;
        str = s8_parse_space_separated_f32_ex(
            str, 
            &inverse_bind_matrix, 
            16, 
            16ull,
            stride
        );
        M4 new_inverse_bind_matrix = inverse_bind_matrix;

        #ifdef COLLADA_DESERIALIZER_CONVERT_COORDINATE_SPACE_FROM_X_RIGHT_Y_FORWARD_Z_UP_TO_X_RIGHT_Y_UP_Z_FORWARD
        /* swap y and z dimentions */
        m4_swap_rows(&new_inverse_bind_matrix, 1, 2);
        M4_SWAP_COLUMNS(new_inverse_bind_matrix, 1, 2);        
        #endif /* COLLADA_DESERIALIZER_CONVERT_COORDINATE_SPACE_FROM_X_RIGHT_Y_FORWARD_Z_UP_TO_X_RIGHT_Y_UP_Z_FORWARD */
        *dst = new_inverse_bind_matrix;
        dst = (M4*)(((char*)dst) + stride);
    }
    return XML_READER_ERROR_NONE;
}

Collada_Deserializer_Error collada_deserializer_mesh_joint_rotations_as_quaternion(
    Collada_Deserializer_Mesh *mesh, 
    Rotor3                *dst, 
    ptrdiff_t                  stride
) {
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("JOINT"));
    if(!input) {
        return COLADA_DESERIALIZER_ERROR_NO_JOINTS_FOUND;
    }
    Collada_Deserializer_Source *source = 
        collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    S8     joint_sid;
    char  *data   = source->array.as_name_array.serialized_values.data;
    size_t length = source->array.as_name_array.serialized_values.length;
    size_t num_characters_eaten;
    while(num_characters_eaten = xml_reader_lex(data, length, &joint_sid)) {
        for(Collada_Deserializer_Node *node = &mesh->root.first_child; node; node = collada_deserializer_node_next(node)) {
            int type_matches = S8_EQUALS_LIT(node->type, "JOINT");
            int sid_matches  = s8_equals(node->sid, joint_sid);
            if(type_matches && sid_matches) {
                *dst = node->rotation;
                dst = (Rotor3*)(((char*)dst) + stride);
            }
        }
        data += num_characters_eaten;
        length -= num_characters_eaten;
    }
    return XML_READER_ERROR_NONE;
}

void collada_deserializer_mesh_joint_inverse_rotations_as_quaternion(
    Collada_Deserializer_Mesh *mesh, 
    Rotor3                *dst, 
    ptrdiff_t                  stride
) {
    ASSERT((((size_t)dst) & (16 - 1)) == 0, "dst should be aligned to 16 bytes");
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("JOINT"));
    if(!input) {
        return COLADA_DESERIALIZER_ERROR_NO_JOINTS_FOUND;
    }
    Collada_Deserializer_Source *source = 
        collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    S8     joint_sid;
    char  *data   = source->array.as_name_array.serialized_values.data;
    size_t length = source->array.as_name_array.serialized_values.length;
    size_t num_characters_eaten;
    while(num_characters_eaten = xml_reader_lex(data, length, &joint_sid)) {
        for(Collada_Deserializer_Node *node = &mesh->root.first_child; node; node = collada_deserializer_node_next(node)) {
            int type_matches = S8_EQUALS_LIT(node->type, "JOINT");
            int sid_matches  = s8_equals(node->sid, joint_sid);
            if(type_matches && sid_matches) {
                Rotor3 r = node->rotation;
                r = rotor3_inverse(r);
                *dst = r;
                dst = (Rotor3*)(((char*)dst) + stride);
            }
        }
        data += num_characters_eaten;
        length -= num_characters_eaten;
    }
}

void collada_deserializer_mesh_joint_translation_as_v3(
    Collada_Deserializer_Mesh *mesh, 
    V3                        *dst, 
    ptrdiff_t                  stride
) {
    Collada_Deserializer_Input *input =
        collada_input_list_find_input_by_semantic(&mesh->vertex_weights.inputs, S8_LIT("JOINT"));
    if(!input) {
        return COLADA_DESERIALIZER_ERROR_NO_JOINTS_FOUND;
    }
    Collada_Deserializer_Source *source = 
        collada_deserializer_mesh_find_source_by_reference(mesh, input->source);
    S8     joint_sid;
    char  *data   = source->array.as_name_array.serialized_values.data;
    size_t length = source->array.as_name_array.serialized_values.length;
    size_t num_characters_eaten;
    while(num_characters_eaten = xml_reader_lex(data, length, &joint_sid)) {
        for(Collada_Deserializer_Node *node = &mesh->root.first_child; node; node = collada_deserializer_node_next(node)) {
            int type_matches = S8_EQUALS_LIT(node->type, "JOINT");
            int sid_matches  = s8_equals(node->sid, joint_sid);
            if(type_matches && sid_matches) {
                *dst = node->translate;
                dst = (V3*)(((char*)dst) + stride);
            }
        }
        data += num_characters_eaten;
        length -= num_characters_eaten;
    }
}