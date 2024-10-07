/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Handles */
typedef struct Os_Surface Os_Surface;

/* Enums */
typedef U8 Os_Cursor;
#define OS_CURSOR_FOREACH(ENTRY) \
/*    identifier, win32*/\
ENTRY(OS_CURSOR_DEFAULT,    IDC_ARROW)\
ENTRY(OS_CURSOR_POINT,      IDC_HAND)\
ENTRY(OS_CURSOR_GRAB,       IDC_SIZEALL)\
ENTRY(OS_CURSOR_GRABBING,   IDC_ARROW)\
ENTRY(OS_CURSOR_HORIZONTAL, IDC_SIZEWE)\
ENTRY(OS_CURSOR_VERTICAL,   IDC_SIZENS)\
/* endof OS_CURSOR_TABLE */
typedef enum Os_Cursor_ {
	#define FOREACH(identifier, _) identifier,
		OS_CURSOR_FOREACH(FOREACH)
	#undef FOREACH
	OS_CURSOR_COUNT
} Os_Cursor_;

/* Structures */
typedef struct Os_Surface_Description {
	S8     title;
	size_t resolution_x;
	size_t resolution_y;
} Os_Surface_Description;
static const Os_Surface_Description OS_SURFACE_DESCRIPTION_DEFAULT = {
	S8_BRACE_INIT("利用者"),
	1280,
	720
};

/* Procedures */
void os_error(char* title, char *msg, ...);
void os_exit(int error_code);
size_t os_num_logical_core();
size_t os_get_logical_core(); /* terrible, always changes */
size_t os_thread_local_storage_alloc();
void   os_thread_local_storage_set(size_t index, void *value);
void  *os_thread_local_storage_get(size_t index);
void   os_set_cursor(Os_Cursor cursor); /* Don't call with invalid intermediary state. */
Os_Surface *os_surface_create(Os_Surface_Description *description);
Os_Surface *os_surface_create_child(Os_Surface *parent,Os_Surface_Description *description);
V2          os_screen_to_surface_v2(Os_Surface *surface, V2 v);
V2          os_surface_get_resolution(Os_Surface *surface);