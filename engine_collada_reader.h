/* Errors */
#define COLLADA_READER_ASSERT_ERROR(test, error) if(!(test)) return error;

/* Collada reader errors are a superset of Xml errors. */
#define COLLADA_READER_ERROR_TABLE(OP) \
OP(COLADA_READER_ERROR_OUT_OF_WORK_BUFFER_MEMORY) \

/* endof COLLADA_READER_ERROR_TABLE */

typedef U8 Collada_Reader_Error;
typedef enum Collada_Reader_Error_ {
    /* Collada error codes start after the last Xml error code. */
    COLADA_ERROR_DUMMY = XML_READER_ERROR_COUNT - 1,
    #define OP(error) error,
    COLLADA_READER_ERROR_TABLE(OP)
    #undef OP
    COLADA_ERROR_COUNT,
};

#ifndef COLLADA_DISABLE_ERROR_STRINGS
static const char* const COLLADA_READER_ERROR_STRINGS[] = {
    #define OP(error) #error,
    XML_READER_ERROR_TABLE(OP)
    #undef OP
    #define OP(error) #error,
    COLLADA_READER_ERROR_TABLE(OP)
    #undef OP
};
#endif /* !COLLADA_DISABLE_ERROR_STRINGS */

typedef struct Collada_Reader_Config {
    U64 num_vertices_max;
    U64 num_sources_max;
    U64 num_triangles_max;
    U64 num_input_max;
} Collada_Reader_Config;

typedef struct Collada_Reader_Float_Array {
    S8     id;
    U64    count;
    S8     serialized_values;
} Collada_Reader_Float_Array;

typedef struct Collada_Reader_Source Collada_Reader_Source;
typedef struct Collada_Reader_Source {
    Collada_Reader_Source      *next;
    S8                   id;
    /* Fixme: might need to support other arrays */
    Collada_Reader_Float_Array  float_array;
} Collada_Reader_Source;

typedef struct Collada_Reader_Input Collada_Reader_Input;
typedef struct Collada_Reader_Input {
    Collada_Reader_Input *next;
    S8                    semantic;
    S8                    source;
    U8                    offset;
    U8                    set;
} Collada_Reader_Input;

typedef struct Collada_Reader_Input_List {
    U64                   num_inputs;
    Collada_Reader_Input *first_input;
    Collada_Reader_Input *last_input;
} Collada_Reader_Input_List;

typedef struct Collada_Reader_Triangles Collada_Reader_Triangles;
typedef struct Collada_Reader_Triangles {
    Collada_Reader_Triangles *next;
    U64                       count;
    S8                        serialized_p_values;
    Collada_Reader_Input_List inputs;
} Collada_Reader_Triangles;

typedef struct Collada_Reader_Vertices {
    S8                 id;
    Collada_Reader_Input_List inputs;
} Collada_Reader_Vertices;

typedef struct Collada_Reader_Mesh {
    Collada_Reader_Vertices   vertices;
    U64                num_sources;
    Collada_Reader_Source    *first_source;
    U64                num_triangles;
    Collada_Reader_Triangles *first_triangles;
    
    /* Internals */
    Collada_Reader_Source    *last_source;
    Collada_Reader_Triangles *last_triangles;
} Collada_Reader_Mesh;

typedef struct Collada_Reader {
    Collada_Reader_Vertices   vertices;
    U64                       num_sources;
    Collada_Reader_Source    *first_source;
    U64                       num_triangles;
    Collada_Reader_Triangles *first_triangles;
    
    /* Internals */
    U64                       _work_buffer_size;
    char                     *_work_buffer;
    char                     *_work_buffer_head;
    Collada_Reader_Source    *last_source;
    Collada_Reader_Triangles *last_triangles;
} Collada_Reader;

U64 collada_reader_work_buffer_size(Collada_Reader_Config *config) {
    return 
        sizeof(Collada_Reader_Vertices ) * config->num_vertices_max  +
        sizeof(Collada_Reader_Source   ) * config->num_sources_max   +
        sizeof(Collada_Reader_Triangles) * config->num_triangles_max +
        sizeof(Collada_Reader_Input    ) * config->num_input_max     +
        0;
}

void collada_reader_begin(
    Collada_Reader  *collada,
    U64       work_buffer_size,
    void     *work_buffer
) {
    collada->_work_buffer_size = work_buffer_size;
    collada->_work_buffer      = work_buffer;
    collada->_work_buffer_head = work_buffer;
    /* Should ask user zeros? */
    collada->num_sources                 = 0;
    collada->num_triangles               = 0;
    collada->first_source                = NULL;
    collada->first_triangles             = NULL;
    collada->vertices.inputs.num_inputs  = 0;
    collada->vertices.inputs.first_input = NULL;
}

void *collada_reader_work_buffer_insert(Collada_Reader *collada, U64 size) {
    void *data = collada->_work_buffer_head;
    collada->_work_buffer_head += size;
    if(collada->_work_buffer_head - collada->_work_buffer > collada->_work_buffer_size)
        return 0; /* Out of memory. */
    #ifndef COLLADA_ASSUME_WORK_BUFFER_IS_ZERO_INITIALIZED
    memset(data, 0x00, size);
    #endif /* !COLLADA_ASSUME_WORK_BUFFER_IS_ZERO_INITIALIZED */
    return data;
}
#define COLLADA_READER_INSERT_STRUCT(collada, struct_type) \
    (struct_type*)collada_reader_work_buffer_insert(collada, sizeof(struct_type))

Collada_Reader_Source *collada_reader_insert_source(Collada_Reader *collada) {
    Collada_Reader_Source *source = COLLADA_READER_INSERT_STRUCT(collada, Collada_Reader_Source);
    if(source) {
        ++collada->num_sources;
        if(!collada->first_source) {
            collada->first_source = source;
        } else {
            collada->last_source->next = source;
        }
        collada->last_source = source;
    }
    return source;
}

Collada_Reader_Triangles *collada_reader_insert_triangles(Collada_Reader *collada) {
    Collada_Reader_Triangles *triangles = COLLADA_READER_INSERT_STRUCT(collada, Collada_Reader_Triangles);
    if(triangles) {
        ++collada->num_triangles;
        if(!collada->first_triangles) {
            collada->first_triangles = triangles;
        } else {
            collada->last_triangles->next = triangles;
        }
        collada->last_triangles = triangles;
    }
    return triangles;
}

Collada_Reader_Input *collada_reader_insert_input(Collada_Reader *collada, Collada_Reader_Input_List *list) {
    Collada_Reader_Input *input = COLLADA_READER_INSERT_STRUCT(collada, Collada_Reader_Input);
    if(input) {
        ++list->num_inputs;
        if(!list->first_input) {
            list->first_input = input;
        } else {
            list->last_input->next = input;
        }
        list->last_input = input;
    }
    return input;
}

Collada_Reader_Error collada_reader_read(
    Collada_Reader *collada,
    U64             source_length,
    char           *source_data
) {
    /* Todo, move those to Collada_Reader */
    Collada_Reader_Source     *source;
    Collada_Reader_Triangles  *triangles;
    Collada_Reader_Input      *input;
    S8                         active_element_tag;
    Collada_Reader_Input_List *active_element_inputs;

    Xml_Reader xml; /* Xml_Reader ? */
    xml_reader_begin(&xml, source_length, source_data);

    Xml_Reader_Event event;
    Xml_Reader_Error error;
    while((error = xml_reader_next_event(&xml, &event)) == XML_READER_ERROR_NONE) {
        switch(event.kind) {
        case XML_EVENT_KIND_ELEMENT_STARTS: {
            /*  */ if(S8_EQUALS_LIT(event.as_element_starts.tag, "source")) {
                source = collada_reader_insert_source(collada);
                COLLADA_READER_ASSERT_ERROR(source, COLADA_READER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "triangles")) {
                triangles = collada_reader_insert_triangles(collada);
                COLLADA_READER_ASSERT_ERROR(triangles, COLADA_READER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
                active_element_inputs = &triangles->inputs;
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "input")) {
                input = collada_reader_insert_input(collada, active_element_inputs);
                COLLADA_READER_ASSERT_ERROR(input, COLADA_READER_ERROR_OUT_OF_WORK_BUFFER_MEMORY);
            } else if(S8_EQUALS_LIT(event.as_element_starts.tag, "vertices")) {
                active_element_inputs = &collada->vertices.inputs;
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
                HANDLE_ATTRIBUTE_S8 (id   , source->float_array.id   );
                HANDLE_ATTRIBUTE_U64(count, source->float_array.count);

            } else if(S8_EQUALS_LIT(active_element_tag, "triangles")) {
                HANDLE_ATTRIBUTE_U64(count, triangles->count);

            } else if(S8_EQUALS_LIT(active_element_tag, "input")) {
                HANDLE_ATTRIBUTE_U64(offset  , input->offset  );
                HANDLE_ATTRIBUTE_U64(set     , input->set     );
                HANDLE_ATTRIBUTE_S8 (semantic, input->semantic);
                HANDLE_ATTRIBUTE_S8 (source  , input->source  );
                
            } else if(S8_EQUALS_LIT(active_element_tag, "vertices")) {
                HANDLE_ATTRIBUTE_S8 (id, collada->vertices.id);
            }
            /* Nothing to do for those elements:
             *   * p           (No attributes.)
             */

            #undef HANDLE_ATTRIBUTE_U64
            #undef HANDLE_ATTRIBUTE_S8
        } break;
        case XML_EVENT_KIND_TEXT_NODE: {
            /*  */ if(S8_EQUALS_LIT(active_element_tag, "float_array")) {
                source->float_array.serialized_values = event.as_text_node.value;
            } else if(S8_EQUALS_LIT(active_element_tag, "p")) {
                /* Todo, find the appropriate topology to associate primitives with. */
                triangles->serialized_p_values = event.as_text_node.value;
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

U64 collada_reader_float_array_size(Collada_Reader_Float_Array *float_array) {
    return float_array->count * sizeof(float);
}

void collada_reader_deserialize_float_array_values(Collada_Reader_Float_Array *float_array, float *values) {
    /* todo parse values here */
}

void collada_reader_deserialize_triangles_p_values(Collada_Reader_Triangles *triangles, U16 *values) {
    /* todo parse values here */
}

void collada_reader_relocate_work_buffer(Collada_Reader *reader, U64 new_work_buffer_size, void *new_work_buffer) {
    reader->_work_buffer_size = new_work_buffer_size;
    reader->_work_buffer_head = ((char*)new_work_buffer) + (reader->_work_buffer_head - reader->_work_buffer);
    reader->_work_buffer      = new_work_buffer;
}