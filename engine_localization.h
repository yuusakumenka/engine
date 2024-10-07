/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define ENGINE_LOCALISATION_TABLE(ENTRY)\
    /*    key  ,  EN    , JP  , FR      , DE , ES */\
    ENTRY(right, "Right", "左", "Gauche", "Rechts", "Derecha")\
    ENTRY(left , "Left" , "右", "Droite", "Links" , "Izquierda")\
/* endof ENGINE_LOCALISATION_TABLE */

typedef enum ENGINE_LOCALISATION_KEY_ {
    #define PER_ENTRY(key, en, jp, fr) ENGINE_LOCALISATION_KEY_##key,
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
    ENGINE_LOCALISATION_KEY_COUNT
} ENGINE_LOCALISATION_KEY_;

static const S8 ENGINE_LOCALISATION_STRINGS[] = {
    /* EN */
    #define PER_ENTRY(key, en, jp, fr, de, es) S8_BRACE_INIT(en),
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
    
    /* JP */
    #define PER_ENTRY(key, en, jp, fr, de, es) S8_BRACE_INIT(jp),
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
    
    /* FR */
    #define PER_ENTRY(key, en, jp, fr, de, es) S8_BRACE_INIT(fr),
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
    
    /* DE */
    #define PER_ENTRY(key, en, jp, fr, de, es) S8_BRACE_INIT(de),
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
    
    /* ES */
    #define PER_ENTRY(key, en, jp, fr, de, es) S8_BRACE_INIT(es),
        ENGINE_LOCALISATION_TABLE(PER_ENTRY)
    #undef PER_ENTRY
};

#ifndef LOCALIZE
#define LOCALIZE(key) (ENGINE_LOCALISATION_STRINGS[ENGINE_LOCALISATION_KEY_##key])
#endif /* LOCALIZE */ /* Yes there is no way this works. Will have to offset by keycount in the future. */