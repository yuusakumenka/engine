#ifdef __INTELLISENSE__
#undef __INTELLISENSE__
#endif

//#pragma warning(error : 4001)

// #define ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION
// #define ENGINE_ENABLE_DYNAMIC_LINKING_USER_APP

// external dependencies
#include <math.h>
#include <emmintrin.h> // SSE2
#include <string.h> // memset, strlen

// types
#include "engine_types.h"

// math
#include "engine_math.h"

// colors
#include "engine_color.h"

// memory
#include "engine_arena.h"

// audio
//#define ENGINE_DISABLE_MODULE_AUDIO
#ifndef ENGINE_DISABLE_MODULE_AUDIO
#endif // ENGINE_DISABLE_MODULE_AUDIO

// inputs
#ifndef ENGINE_DISABLE_MODULE_INPUT
#include "engine_input.h"
#include "engine_keybind.h"
#endif // ENGINE_DISABLE_MODULE_INPUT

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

// camera
#include "engine_camera.h"
#include "engine_draw.h"

/* String */
#include "engine_string.h"

/* XML Reader */
#include "engine_xml_reader.h"

/* Collada Reader */
#include "engine_collada_reader.h"

// core
#define ENGINE_V2F32 V2
#define ENGINE_Arena Arena*
#include "engine_core.h"

/* Gui */
/* Engine Gui */
#include "engine_gui.h"
/* Dear ImGui */
#ifndef ENGINE_DISABLE_DEARIMGUI
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "include/cimgui.h"
#pragma comment(lib, "cimgui.lib")
#endif /* !ENGINE_DISABLE_DEARIMGUI */

/* Scene */

/* Jam */
#include "engine_jam.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")