/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

// typedef struct Os_Win32 {

// } Os_Win32;
// Os_Win32 *os_win32;

void os_error(char* title, char *msg, ...) {
    size_t size         = 512 * sizeof(TCHAR);
    LPTSTR lpDisplayBuf = (LPTSTR)alloca(size); 
    va_list argList;
    va_start(argList, msg);
    StringCchVPrintfA(
        lpDisplayBuf, 
        size,
        msg,
        argList
    );
    va_end(argList);
    MessageBoxA(NULL, lpDisplayBuf, title, MB_OK);
}

void os_exit(int exit_code) {
    if(IsDebuggerPresent())
        __debugbreak();
    ExitProcess(exit_code); 
}

void os_win32_exit_with_error(LPTSTR lpszFunction) { 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
                                      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"), 
                    lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    if(IsDebuggerPresent())
        __debugbreak();
    ExitProcess(dw); 
}

size_t os_num_logical_core() {
    SYSTEM_INFO system_info = {0};
    GetSystemInfo(&system_info);
    return (size_t)system_info.dwNumberOfProcessors;
}

size_t os_get_logical_core() {
    return (size_t)GetCurrentProcessorNumber();
}

size_t os_thread_local_storage_alloc() {
    size_t index = TlsAlloc();
    ASSERT(index != TLS_OUT_OF_INDEXES, "Out of thread local storage indices.");
    return index;
}

void   os_thread_local_storage_set(size_t index, void *value) {
    BOOL result = TlsSetValue(index, value);
    ASSERT(result, "TlsSetValue failed.");
}

void  *os_thread_local_storage_get(size_t index) {
    void *value = TlsGetValue(index);
    return value;
}

/* Don't call with invalid intermediary state. */
void os_set_cursor(Os_Cursor cursor) {
    // if(cursor != engine->cursor) {
    //     engine->cursor = cursor;
    //     PostMessage(0, WM_SETCURSOR, 0, 0);
    //     POINT p = {0};
    //     GetCursorPos(&p);
    //     SetCursorPos(p.x, p.y);
    // }
    
    //PostMessage(win32->hwnd, WM_SETCURSOR, 0, 0);
    //PostMessage(0, WM_SETCURSOR, (WPARAM)cursor, (LPARAM)cursor);
    
    /* Since we can't pass the cursor trough the 
     * wParam or LParam of the WM_SETCURSOR message,
     * we store it as a global to be read later. */
    win32->cursor = cursor;
    /* Forcefully trigger a WM_SETCURSOR message */
    POINT p = {0};
    GetCursorPos(&p);
    SetCursorPos(p.x, p.y);
}

// void Timerproc(
//   HWND unnamedParam1,
//   UINT unnamedParam2,
//   UINT_PTR unnamedParam3,
//   DWORD unnamedParam4
// ) {
//     win32_main_loop_body(engine);
// }

LRESULT CALLBACK WindowProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    in_win32_process_event(
        &win32->in_current,
        uMsg,
        wParam
    );
    #if 1
    switch (uMsg) {
    case WM_SETCURSOR: {
        //if(engine->window_flags & ENGINE_WINDOWING_FLAG_HOVER)
        //if (hwnd == win32->main_surface.hwnd)
        #if 1
        Os_Cursor cursor  = (Os_Cursor)win32->cursor;
        HCURSOR   hCursor = win32->cursors[cursor];
        SetCursor(hCursor);
        #else
        SetCursor(win32->cursors[engine->cursor]);
        #endif
    } break;
    case WM_MOUSEMOVE: {
        engine->window_flags |=  ENGINE_WINDOWING_FLAG_HOVER;
    } break;
    // resize
    // case WM_ENTERSIZEMOVE: {
    //     /* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer */
    //     win32->resize_timer = SetTimer(
    //         NULL,
    //         0,
    //         USER_TIMER_MINIMUM,
    //         Timerproc
    //     );
    // } break;
    // case WM_EXITSIZEMOVE: {
    //     win32->resize_timer = KillTimer(NULL, win32->resize_timer);
    // } break;
    case WM_SIZE: {
        int width  = LOWORD(lParam); 
        int height = HIWORD(lParam);
        engine->window_size_in_px.x = width;
        engine->window_size_in_px.y = height;
    } break;
    // window is asked to be displayed
    case WM_SHOWWINDOW: {
        // Immediatly get rid of the hourglass cursor.
        SetCursor(win32->cursors[0]);

//		// There is a bug on Windows, where the act of asking to show
//		// a given window, will fill it's DC with a white brush,
//		// regardless of the hbrBackground submitted on calling RegisterClass().
//		// This causes a white flash whenever the window is shown.
//		// To hack around this we ask Windows to play a blend animation if supported.
//		if (IsWindows8OrGreater()) {
//			if (!GetLayeredWindowAttributes(hwnd, NULL, NULL, NULL)) {
//				AnimateWindow(hwnd, 200, AW_ACTIVATE | AW_BLEND);
//				return TRUE;
//			}
//		}
    } break;
    case WM_SETFOCUS: {
        engine->window_flags |= ENGINE_WINDOWING_FLAG_FOCUS;
    } break;
    case WM_KILLFOCUS: {
        engine->window_flags &= ~ENGINE_WINDOWING_FLAG_FOCUS;
    } break;
    case WM_NCMOUSELEAVE: {
        engine->window_flags &= ~ENGINE_WINDOWING_FLAG_HOVER;
    } break;
    case WM_CLOSE: {
        #ifdef ENGINE_ENABLE_MEMORY_STAT_USAGE_ON_CLOSE
        engine_error(
            "Memory usage stats report", 
            "engine->app_arena->commited == %llu (~%llu MiB)\n"
            "engine->frame_arena->commited == %llu (~%llu MiB)\n"
            ,
            engine->app_arena->commited,
            ALIGN(engine->app_arena->commited, 1ull << 20) >> 20,
            engine->frame_arena->commited,
            ALIGN(engine->frame_arena->commited, 1ull << 20) >> 20
        );
        #endif
        ExitProcess(0);
        return TRUE;
    } break;
    case WM_USER_CREATE_WINDOW: {
        Win32_CreateWindowEx_Command *command = (Win32_CreateWindowEx_Command *)wParam;
        HWND window = CreateWindowEx(
            command->dwExStyle,
            command->lpClassName,
            command->lpWindowName,
            command->dwStyle,
            command->X,
            command->Y,
            command->nWidth,
            command->nHeight,
            command->hWndParent,
            command->hMenu,
            command->hInstance,
            command->lpParam
        );
        //ASSERT(window != NULL, "CreateWindowEx");
        if(window == NULL)
            os_win32_exit_with_error(TEXT("CreateWindowEx"));

        /* Since a DC is bound to the thread that creates it,
         * that is that if that thread get destroyed the DC becomes unusable,
         * we create the DC on the this thread that we know wont't die early. */
        HDC *hdc = (HDC *)lParam;
        *hdc = GetDC(window);
        
        return (LRESULT)window;
    } break;
    case WM_USER_CREATE_OPENGL_CONTEXT: {
        #if 1
        HDC  hdc  = (HDC)wParam;
        #else
        HWND hwnd = (HWND)wParam;
        HDC  hdc  = GetDC(hwnd);
        #endif
        HGLRC opengl_context = engine_backend_win32_opengl_post_window_init(hdc);
        //ReleaseDC(hwnd, hdc);
        return (LRESULT)opengl_context;
    } break;
        
//#ifndef ENGINE_DISABLE_INPUTS
//	case WM_SYSKEYDOWN:
//	case WM_SYSKEYUP:
//	case WM_KEYDOWN:
//	case WM_KEYUP: 
//	{
////		UINT32 VKCode = (UINT32)wParam;
////		BOOL   WasDown = ((lParam & (1 << 30)) != 0);
////		BOOL   IsDown = ((lParam & (1 << 31)) == 0);
////		if(VKCode == 'Z' && IsDown)
////		{
////			++g_i_note;
////		}
//		++g_i_note;
//	} break;
//#endif // ENGINE_DISABLE_INPUTS
    }
    #endif
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/* Surface */
LRESULT CALLBACK SurfaceWndProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Os_Surface *os_surface_create(Os_Surface_Description *description) {
    
    // initialize windowing
    HWND hwnd;
    HDC  hdc;
    {
        const TCHAR WINDOW_CLASS_NAME[] = TEXT("\1");
        const TCHAR WINDOW_TEXT[]       = TEXT("Hello");
        HINSTANCE hInstance = GetModuleHandleW(NULL);
        {
            WNDCLASS window_class = { 0 };
            window_class.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
            window_class.lpfnWndProc   = WindowProc;
            window_class.hInstance     = hInstance;
            window_class.lpszClassName = WINDOW_CLASS_NAME;
            window_class.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+3);
            window_class.hIcon         = LoadIcon(win32->hInstance, MAKEINTRESOURCE(1));
            RegisterClass(&window_class);
        }

        /* Create Window */
        {
            Win32_CreateWindowEx_Command command;
            command.dwExStyle    = 0;
            command.lpClassName  = WINDOW_CLASS_NAME;
            command.lpWindowName = NULL;
            command.dwStyle      = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
            command.X            = CW_USEDEFAULT;
            command.Y            = CW_USEDEFAULT;
            command.nWidth       = CW_USEDEFAULT;
            command.nHeight      = CW_USEDEFAULT;
            command.hWndParent   = NULL;
            command.hMenu        = NULL;
            command.hInstance    = hInstance;
            hwnd = (HWND)SendMessage(
                win32->hwnd,
                WM_USER_CREATE_WINDOW,
                (WPARAM)&command,
                (LPARAM)&hdc
            );
        }
        
        if (hwnd == NULL)
            os_win32_exit_with_error(TEXT("SendMessage not handled."));

        // track mouse leaving client window area
        TRACKMOUSEEVENT tackMouseEvent;
        tackMouseEvent.cbSize  = sizeof(TRACKMOUSEEVENT);
        tackMouseEvent.dwFlags = TME_NONCLIENT | TME_LEAVE;
        tackMouseEvent.hwndTrack = hwnd;
        BOOL TrackMouseEventSuccess = TrackMouseEvent(&tackMouseEvent);
        if (!TrackMouseEvent(&tackMouseEvent)) 
            os_win32_exit_with_error(TEXT("TrackMouseEvent"));
    }

    Os_Win32_Surface *surface = &win32->main_surface; /* HACK */
    surface->hwnd   = hwnd;
    surface->hdc    = hdc;

    // apply user window title
    {
        short *utf16 = alloca((description->title.length + 1) * sizeof(short));
        size_t utf16_length;
        utf8_to_utf16(
            &utf16_length,
            utf16,
            description->title.length,
            description->title.data
        );
        utf16[utf16_length] = 0;
        SetWindowTextW(hwnd, (LPCWSTR)utf16);
    }

    // apply user window size and show window
    {
        // Note: We don't set the window size to user's demanded size on window creation,
        // because we need user to describe it's desired window size through init(). 
        // However OpenGL requires a window to be created before being able to use it's API.
        // Since we want the user to be able to call the graphic api durring init(),
        // we delay init() after window creation and OpenGL's initialization and only after
        // do we set the window size according to user's demanded window size.

        // get non-client window size so it can contain desired client window size
        int window_width, window_height;
        {
            RECT client_to_window_rect = {0};
            client_to_window_rect.right  = description->resolution_x;
            client_to_window_rect.bottom = description->resolution_y;
            AdjustWindowRect(&client_to_window_rect, WS_OVERLAPPEDWINDOW, FALSE);
            window_width  = client_to_window_rect.right  - client_to_window_rect.left;
            window_height = client_to_window_rect.bottom - client_to_window_rect.top;
        }
        SetWindowPos(
            hwnd,
            NULL,
            0, 0,
            window_width, window_height,
            SWP_NOMOVE | SWP_SHOWWINDOW
        );
    }

    return (Os_Surface*)surface;
}

Os_Surface *os_surface_create_child(
    Os_Surface *parent,
    Os_Surface_Description *description
) {
    return 0;
}

V2 os_screen_to_surface_v2(Os_Surface *surface, V2 v) {
    Os_Win32_Surface *win32_surface = (Os_Win32_Surface *)surface;
    POINT p;
    p.x = (LONG)v.x;
    p.y = (LONG)v.y;
    ScreenToClient(win32_surface->hwnd, &p);
    RECT rect;
    GetClientRect(win32_surface->hwnd, &rect);
    LONG y = (rect.bottom - rect.top) - p.y;
    return v2((F32)p.x, (F32)y);
}

V2 os_surface_get_resolution(Os_Surface *surface) {
    Os_Win32_Surface *win32_surface = (Os_Win32_Surface *)surface;
    RECT rect;
    GetClientRect(win32_surface->hwnd, &rect);
    return v2((F32)(rect.right - rect.left), (F32)(rect.bottom - rect.top));
}