#define XML_ASSERT(test, msg, ...)
#define XML_ASSERT_ERROR(test, error) if(!(test)) return error;

#define XML_READER_ERROR_TABLE(OP) \
OP(XML_READER_ERROR_NONE) \
OP(XML_READER_ERROR_TAG_OPPENER_WITHIN_TAG) \

/* endof XML_READER_ERROR_TABLE */

typedef U8 Xml_Reader_Error;
typedef enum Xml_Reader_Error_ {    
    #define OP(error) error,
    XML_READER_ERROR_TABLE(OP)
    #undef OP
    XML_READER_ERROR_COUNT,
};

#ifndef XML_READER_DISABLE_ERROR_STRINGS
static const char* const XML_READER_ERROR_STRINGS[] = {
    #define OP(error) #error,
    XML_READER_ERROR_TABLE(OP)
    #undef OP
};
#endif /* !XML_READER_DISABLE_ERROR_STRINGS */

typedef U8 Xml_Reader_Lexeme_Kind;
typedef enum Xml_Reader_Lexeme_Kind_ {
    XML_READER_LEXEME_KIND_EMPTY,
    XML_READER_LEXEME_KIND_WHITESPACE,       /* ' ', '\n', '\t' */
    XML_READER_LEXEME_KIND_TAG_OPPENER,      /* '<' */
    XML_READER_LEXEME_KIND_TAG_CLOSER,       /* '>' */
    XML_READER_LEXEME_KIND_TAG_ENDER,        /* '/' */
    XML_READER_LEXEME_KIND_IDENTIFIER,
    XML_READER_LEXEME_KIND_ATTRIBUTE_ASSIGN, /* '=' */
    XML_READER_LEXEME_KIND_STRING_LITERAL,
    XML_READER_LEXEME_KIND_COUNT,
};

typedef int Xml_Reader_Event_Kind;
typedef enum Xml_Reader_Event_Kind_ {
    XML_EVENT_KIND_ELEMENT_STARTS,
    XML_EVENT_KIND_ATTRIBUTE,
    XML_EVENT_KIND_ELEMENT_ENDS,
    XML_EVENT_KIND_TEXT_NODE,
    XML_EVENT_KIND_END_OF_FILE,
    XML_EVENT_KIND_COUNT,
} Xml_Reader_Event_Kind_;

typedef struct Xml_Reader_Event_Element_Starts {
    Xml_Reader_Event_Kind kind;
    S8             tag;
} Xml_Reader_Event_Element_Starts;

typedef struct Xml_Reader_Event_Element_Ends {
    Xml_Reader_Event_Kind kind;
    S8             tag;
} Xml_Reader_Event_Element_Ends;

typedef struct Xml_Reader_Event_Attribute {
    Xml_Reader_Event_Kind kind;
    S8             key;
    S8             value;
} Xml_Reader_Event_Attribute;

typedef struct Xml_Reader_Event_Text_Node {
    Xml_Reader_Event_Kind kind;
    S8             value;
} Xml_Reader_Event_Text_Node;

typedef union Xml_Reader_Event {
    Xml_Reader_Event_Kind kind;
    Xml_Reader_Event_Element_Starts as_element_starts;
    Xml_Reader_Event_Element_Ends   as_element_ends;
    Xml_Reader_Event_Attribute      as_attribute;
    Xml_Reader_Event_Text_Node      as_text_node;
} Xml_Reader_Event;

typedef U8 Xml_Reader_Flag;
typedef enum Xml_Reader_Flag_ {
    XML_READER_FLAG_NONE                = (0     ),
    XML_READER_FLAG_IS_IN_TAG           = (1 << 0),
    XML_READER_FLAG_TAG_NAME_FOUND      = (1 << 1),
    XML_READER_FLAG_IS_SELF_CLOSING_TAG = (1 << 2),
    XML_READER_FLAG_IS_END_TAG          = (1 << 3),
    XML_READER_FLAG_IS_IN_ATTRIBUTE     = (1 << 4),
};
#define XML_READER_FLAG_GET(flags, flag)          (((flags) & (XML_READER_FLAG_##flag)) != 0)
#define XML_READER_FLAG_SET(flags, flag, boolean) ((flags) = (boolean ? (flags) | (XML_READER_FLAG_##flag) : (flags) & ~(XML_READER_FLAG_##flag)))

typedef struct Xml_Reader {
    S8              tag_name;
    S8              attribute_name;
    U64             length;
    char           *head;
    Xml_Reader_Flag flags;
} Xml_Reader;

Bool xml_reader_is_whitespace(char c) {
    return
        c == ' '  ||
        c == '\n' ||
        c == '\t';
}

Bool xml_reader_is_alphanumeric(char c) {
    char to_upper = c & ~(1 << 5);
    return
        (c        >= '0' && c        <= '9') || /* is numeric    */
        (to_upper >= 'A' && to_upper <= 'Z') || /* is alphabet   */
        c == '_'                             || /* is underscore */
        c == '?'; /* ??? */
}

Int xml_reader_lex(char *data, U64 length, S8 *lexeme) {
    U64  num_characters_eaten     = 0;
    Bool lexeme_is_whitespace     = FALSE;
    Bool lexeme_is_string_literal = FALSE;
    while (num_characters_eaten < length) {
        char character = data[num_characters_eaten];
        if(num_characters_eaten == 0) {
            lexeme_is_whitespace     = xml_reader_is_whitespace(character);
            lexeme_is_string_literal = character == '"';
            if(
                character == '<' ||
                character == '>' ||
                character == '/' ||
                character == '='
            ) {
                ++num_characters_eaten;
                break;
            }
        } else {
            Bool char_is_whitespace = xml_reader_is_whitespace(character);
            if(lexeme_is_whitespace) {
                if(!char_is_whitespace)
                    break;
            } else {
                if(lexeme_is_string_literal) {
                    if(character == '"') {
                        ++num_characters_eaten;
                        break;
                    }
                } else {
                    if(
                        char_is_whitespace ||
                        !xml_reader_is_alphanumeric(character)
                    ) {
                        break;
                    }
                }
            }
        }
        ++num_characters_eaten;
    }
    lexeme->length = num_characters_eaten;
    lexeme->data   = data;
    return num_characters_eaten;
}

Xml_Reader_Lexeme_Kind xml_reader_lexeme_kindof(S8 lexeme) {
    if(lexeme.length == 0)
        return XML_READER_LEXEME_KIND_EMPTY;
    if(xml_reader_is_whitespace(lexeme.data[0]))
        return XML_READER_LEXEME_KIND_WHITESPACE;
    if(lexeme.data[0] == '<')
        return XML_READER_LEXEME_KIND_TAG_OPPENER;
    if(lexeme.data[0] == '/')
        return XML_READER_LEXEME_KIND_TAG_ENDER;
    if(lexeme.data[0] == '>')
        return XML_READER_LEXEME_KIND_TAG_CLOSER;
    if(lexeme.data[0] == '=')
        return XML_READER_LEXEME_KIND_ATTRIBUTE_ASSIGN;
    if(lexeme.data[0] == '"')
        return XML_READER_LEXEME_KIND_STRING_LITERAL;
    return XML_READER_LEXEME_KIND_IDENTIFIER;
}

void xml_reader_begin(Xml_Reader *reader, U64 length, char *data) {
    reader->length = length;
    reader->head   = data;
    reader->flags  = XML_READER_FLAG_NONE;
}

/* Return:
 *   * XML_READER_ERROR_NONE (0): Uppon reaching end of file or buffer.
 *   * XML_SUCCESS    (1): Uppon submitting an event.
 *   * XML_ERROR_...(>=2): Uppon finding an error in the xml file.
 * Example:
 *   > Xml xml;
 *   > ...
 *   > Xml_Reader_Error error;
 *   > while((error = xml_next_event(&xml, &event)) != XML_READER_ERROR_NONE) {
 *   >     switch(event.kind) {
 *   >     case XML_EVENT_KIND_...: {
 *   >         ... Handle event here ...
 *   >     } break;
 *   >     }
 *   > }
 *   > if(error) {
 *   >    ... Handle error here ...
 *   > }
 */
Xml_Reader_Error xml_reader_next_event(Xml_Reader *xml, Xml_Reader_Event *event) {
    S8    lexeme;
    U64   length = xml->length;
    char *start = xml->head;
    char *head  = start;
    char *end   = head + length;
    
    /*  Pseudo-coroutine:
     *    > WHITESPACE
     *    > TAG OPENNER '<'
     *    > TAG ENDER '/' -> IS_END_TAG
     *    > TAG NAME "a... " (IDENTIFIER) -?> ELEMENT_STARTS
     *    > TAG ATTRIBUTE KEY "a... " (IDENTIFIER)
     *    > TAG ATTIRBUTE ASSIGNEMENT '='
     *    > TAG ATTRIBYTE VALUE ""..."" (STRING LITERAL) -> ATTRIBUTE
     *    > TAG ENDER '/' -> IS_SELF_CLOSING_TAG
     *    > TAG CLOSER '>' -?> ELEMENT_ENDS
    */
    char *last_valid_head = start;
    while((head += xml_reader_lex(head, length - (head - start), &lexeme)) <= end) {
        Xml_Reader_Lexeme_Kind kind = xml_reader_lexeme_kindof(lexeme);
        switch(kind) {
        case XML_READER_LEXEME_KIND_TAG_OPPENER: { /* '<' */
            XML_ASSERT_ERROR(
                !(xml->flags & XML_READER_FLAG_IS_IN_TAG), 
                XML_READER_ERROR_TAG_OPPENER_WITHIN_TAG
            );
            xml->flags = XML_READER_FLAG_NONE | XML_READER_FLAG_IS_IN_TAG;
        } break;
        case XML_READER_LEXEME_KIND_TAG_ENDER: { /* '/' */
            if(XML_READER_FLAG_GET(xml->flags, IS_IN_TAG)) {
                if(XML_READER_FLAG_GET(xml->flags, TAG_NAME_FOUND)) {
                    /* Tag closer after tag name indicates self closing tag. */
                    XML_READER_FLAG_SET(xml->flags, IS_SELF_CLOSING_TAG, TRUE);
                }
                XML_READER_FLAG_SET(xml->flags, IS_END_TAG, TRUE);
            }
        } break;
        case XML_READER_LEXEME_KIND_IDENTIFIER: { /* "a..z, 0..9" */
            if(XML_READER_FLAG_GET(xml->flags, IS_IN_TAG)) {
                /* The first identifier in a tag is the tag's name.
                 * Folowing idnetifiers are tag's attribute names */
                if(!XML_READER_FLAG_GET(xml->flags, TAG_NAME_FOUND)) {
                    xml->tag_name = lexeme;
                    XML_READER_FLAG_SET(xml->flags, TAG_NAME_FOUND, TRUE);
                    if(!XML_READER_FLAG_GET(xml->flags, IS_END_TAG)) {
                        /* Submit element start event. */
                        event->kind = XML_EVENT_KIND_ELEMENT_STARTS;
                        event->as_element_starts.tag = lexeme;
                        goto EXIT;
                    }
                } else {
                    if(XML_READER_FLAG_GET(xml->flags, IS_IN_ATTRIBUTE)) {
                        /* Valueless attribute */
                        XML_READER_FLAG_SET(xml->flags, IS_IN_ATTRIBUTE, FALSE);
                        /* Submit attribute event. */
                        event->kind                      = XML_EVENT_KIND_ATTRIBUTE;
                        event->as_attribute.key          = xml->attribute_name;
                        event->as_attribute.value.length = 0;
                        goto EXIT_BUT_DONT_EAT_LAST_LEXEME;
                    }
                    xml->attribute_name = lexeme;
                    XML_READER_FLAG_SET(xml->flags, IS_IN_ATTRIBUTE, TRUE);
                }
            } else {
                /* Identifier outside of tag? Proably text node. */
            }
        } break;
        case XML_READER_LEXEME_KIND_STRING_LITERAL: { /* "a..z, 0..9" */
            if(xml->flags & XML_READER_FLAG_IS_IN_ATTRIBUTE) {
                xml->flags &= ~XML_READER_FLAG_IS_IN_ATTRIBUTE;
                /* Submit attribute event. */
                event->kind = XML_EVENT_KIND_ATTRIBUTE;
                event->as_attribute.key   = xml->attribute_name;
                event->as_attribute.value = lexeme;
                goto EXIT;
            }
        } break;
        case XML_READER_LEXEME_KIND_TAG_CLOSER: { /* '>' */
            if(xml->flags & XML_READER_FLAG_IS_IN_ATTRIBUTE) {
                /* Valueless attribute */
                xml->flags &= ~XML_READER_FLAG_IS_IN_ATTRIBUTE;
                /* Submit attribute event. */
                event->kind                      = XML_EVENT_KIND_ATTRIBUTE;
                event->as_attribute.key          = xml->attribute_name;
                event->as_attribute.value.length = 0;
                goto EXIT_BUT_DONT_EAT_LAST_LEXEME;
            }
            xml->flags &= ~XML_READER_FLAG_IS_IN_TAG;
            if(xml->flags & XML_READER_FLAG_IS_END_TAG) {
                /* Submit element start event. */
                event->kind = XML_EVENT_KIND_ELEMENT_ENDS;
                event->as_element_ends.tag = xml->tag_name;
                goto EXIT;
            }
        } break;
        case XML_READER_LEXEME_KIND_EMPTY: { /* '' */
            /* Submit end of file event. */
            event->kind = XML_EVENT_KIND_END_OF_FILE;
            goto EXIT;
        } break;
        }
        last_valid_head = head;
    }
    EXIT:;
    last_valid_head = head;
    EXIT_BUT_DONT_EAT_LAST_LEXEME:;
    xml->head   = last_valid_head;
    xml->length = length - (last_valid_head - start);
    return XML_READER_ERROR_NONE;
}