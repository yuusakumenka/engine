/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#ifdef __INTELLISENSE__
#undef __INTELLISENSE__
#endif

//#pragma warning(error : 4001)

/* #define ENGINE_PARANOID */
// #define ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION
// #define ENGINE_ENABLE_DYNAMIC_LINKING_USER_APP

// external dependencies
#include <math.h>

/* SIMD */
#ifdef ENGINE_DISABLE_SSE2
#ifndef ENGINE_DISABLE_AVX
#define ENGINE_DISABLE_AVX
#endif
#endif
#ifndef ENGINE_DISABLE_AVX
#include <immintrin.h> /* AVX, AVX2, FMA */
#else /* ENGINE_DISABLE_AVX */
#ifndef ENGINE_DISABLE_SSE2
#include <emmintrin.h> /* SSE2 */
#endif /* !ENGINE_DISABLE_SSE2 */
#endif /* !ENGINE_DISABLE_AVX */

#include <string.h> // memset, strlen

/* Defines */
#define ALIGN(value, alignement_in_power_of_two_bytes) ((((size_t)(value)) + (((size_t)(alignement_in_power_of_two_bytes)) - 1)) & ~(((size_t)(alignement_in_power_of_two_bytes)) - 1))

// types
#include "engine_types.h"

/* alloca */
#define alloca _alloca

/* Error, Crash and Assert */
void engine_error(char *title, char *msg, ...);
void engine_crash(int error);
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
//#define _ENGINE_ASSERT(...)
//#define ENGINE_ASSERT(condition, ...) {if(!(condition)) { engine_error("ENGINE_ASSERT(" #condition ") failed in " __FUNCTION__ "(...)" , __FILE__ ":" STRINGIFY(__LINE__) "\n" __VA_ARGS__); engine_crash(1); }}

#define ASSERT(condition, ...) {if(!(condition)) { os_error("ASSERT(" #condition ") failed in " __FUNCTION__ "(...)" , __FILE__ ":" STRINGIFY(__LINE__) "\n" __VA_ARGS__); os_exit(0x29C); }}

// math
#include "engine_math.h"
#include "engine_crypto.h"

// colors
#include "engine_color.h"


/* String */
#include "engine_string.h"

/* Memory */
#define ARENA_ENABLE_INSERT_S8
#include "engine_arena.h"

// threading & scheduling
#include "engine_threading.h"
#include "engine_scheduling.h"

// audio
//#define ENGINE_DISABLE_MODULE_AUDIO
#ifndef ENGINE_DISABLE_MODULE_AUDIO
#endif // ENGINE_DISABLE_MODULE_AUDIO

/* Inputs */
#ifndef ENGINE_DISABLE_MODULE_INPUT
#define IN_SURFACE_BACKEND_TYPE Os_Surface
#include "engine_in.h"
#endif // ENGINE_DISABLE_MODULE_INPUT

/* Os */
#include "engine_os.h"

// graphics
#ifndef ENGINE_DISABLE_MODULE_GPU
#include "engine_gpu.h"
#endif // ENGINE_DISABLE_MODULE_GPU

// assets
#ifndef ENGINE_DISABLE_MODULE_ASSET
#define ENGINE_ASSET_Arena Arena
#include "engine_asset.h"
// asset module extentions:
    #ifndef ENGINE_DISABLE_MODULE_GPU
    #include "engine_asset_gpu.h"
    #endif // ENGINE_DISABLE_MODULE_GPU
#endif // ENGINE_DISABLE_MODULE_ASSET

/* Debug Draw */
/* @CLARITY might want to rename this library to Print Geometry geoprint? */
#include "engine_debug_draw.h"
#include "engine_debug_draw_renderer.h"

/* Gui */
/* Engine Gui */
#define UI_ASSERT ASSERT
#define UI_ENABLE_GEOPTRINT
#include "engine_ui.h"
#include "engine_ui_widgets.h"
#include "engine_ui_renderer.h"
/* Handle / 3d Gizmo */
// #include "engine_ui3d.h"

// camera
#include "engine_camera.h"
#include "engine_draw.h"

/* XML Reader */
#include "engine_xml_reader.h"

/* Collada Reader */
//#include "engine_collada_reader.h"
#include "engine_collada_deserializer.h"

/* Core */
#define ENGINE_V2F32 V2
#define ENGINE_Arena Arena*
#include "engine_core.h"

/* Physics */
#include "engine_physics2d.h"
#include "engine_collision2d.h"

/* Scene */

/* Jam */
#include "engine_jam.h"

/* Localization */
#include "engine_localization.h"

/* Data Structures */
#include "engine_index_pool_u16.h"
#include "engine_topology_builder.h"