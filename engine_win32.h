/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#ifndef ENGINE_WIN32_H
#define ENGINE_WIN32_H
#ifdef __INTELLISENSE__
#include "engine.h"
#endif // __INTELLISENSE__

#define NOGDICAPMASKS    
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES    
//#define NOWINSTYLES      
#define NOSYSMETRICS     
#define NOMENUS          
//#define NOICONS          
#define NOKEYSTATES      
#define NOSYSCOMMANDS    
#define NORASTEROPS      
#define NOSHOWWINDOW     
#define OEMRESOURCE      
#define NOATOM           
#define NOCLIPBOARD      
//#define NOCOLOR          
#define NOCTLMGR         
#define NODRAWTEXT       
//#define NOGDI            
#define NOKERNEL         
//#define NOUSER           
#define NONLS            
//#define NOMB             
#define NOMEMMGR         
#define NOMETAFILE       
#define NOMINMAX         
//#define NOMSG            
#define NOOPENFILE       
#define NOSCROLL         
#define NOSERVICE        
#define NOSOUND          
#define NOTEXTMETRIC     
#define NOWH             
#define NOWINOFFSETS     
#define NOCOMM           
#define NOKANJI          
#define NOHELP           
#define NOPROFILER       
#define NODEFERWINDOWPOS 
#define NOMCX            
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Winuser.h>
#include <VersionHelpers.h> //IsWindows8OrGreater()
#include <strsafe.h> // StringCchPrintf()
#pragma comment(lib, "user32")

#define WIN32_INITIAL_INVISIBLE_WINDOW_CLASS_NAME TEXT("Initial Invisible Window Class")
#define WIN32_WINDOW_CLASS_NAME                   TEXT("Window Class")

/* Custom Window Messages */
#define WM_USER_CREATE_WINDOW         WM_USER + 0
#define WM_USER_DESTROY_WINDOW        WM_USER + 1
#define WM_USER_CREATE_OPENGL_CONTEXT WM_USER + 2

// gpu
void engine_backend_win32_opengl_pre_window_init(HINSTANCE hInstance);
HGLRC engine_backend_win32_opengl_post_window_init(HDC hdc);
void gl_error_guard(char *proc_name);

// threading
#include <processthreadsapi.h>
#include "engine_threading_win32.h"

// memory
#include "engine_arena_win32.h"

// assets
#include "engine_asset_win32.h"

// audio
#ifndef ENGINE_DISABLE_MODULE_AUDIO

// TODO: also add DirectSound for Windows 7 and before...

// wasapi
#define CINTERFACE
#define COBJMACROS
#include <mmdeviceapi.h>
#include <audioclient.h>
#undef CINTERFACE
#undef COBJMACROS
#pragma comment(lib, "ksuser")
#pragma comment(lib, "Ole32")
static const CLSID CLSID_MMDeviceEnumerator  = { 0xbcde0395, 0xe52f, 0x467c, {0x8e, 0x3d, 0xc4, 0x57, 0x92, 0x91, 0x69, 0x2e} };
static const   IID   IID_IMMDeviceEnumerator = { 0xa95664d2, 0x9614, 0x4f35, {0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6} };
static const   IID   IID_IAudioClient        = { 0x1cb9ad4c, 0xdbfa, 0x4c32, {0xb1, 0x78, 0xc2, 0xf5, 0x68, 0xa7, 0x03, 0xb2} };
static const   IID   IID_IAudioRenderClient  = { 0xf294acfc, 0x3146, 0x4483, {0xa7, 0xbf, 0xad, 0xdc, 0xa7, 0xc2, 0x60, 0xe2} };
#endif // ENGINE_DISABLE_MODULE_AUDIO

typedef struct Win32_CreateWindowEx_Command {
    DWORD     dwExStyle;
    LPCTSTR   lpClassName;
    LPCTSTR   lpWindowName;
    DWORD     dwStyle;
    int       X;
    int       Y;
    int       nWidth;
    int       nHeight;
    HWND      hWndParent;
    HMENU     hMenu;
    HINSTANCE hInstance;
    LPVOID    lpParam;
} Win32_CreateWindowEx_Command;

/* engine_os_win32_types.h*/
typedef struct Os_Win32_Surface {
    HWND hwnd;
    HDC  hdc;
} Os_Win32_Surface;

// backend
typedef struct Win32 {
    HWND hwnd;
    Os_Win32_Surface main_surface;
    HINSTANCE     hInstance;
    //HWND      main_hwnd;
    //HWND      hwnd;
    //HDC       hdc;
    Os_Cursor cursor;
    HCURSOR   cursors[OS_CURSOR_COUNT];
    UINT_PTR  resize_timer;

    /* Inputs */
    POINT  cursor_position;
    POINT  cursor_position_last_frame;
    V2     mouse_wheel;
    U64    buttons[256 / ((sizeof(U64) * 8) / 2)];
    In     ins[2];
    In    *in_current;
	size_t in_lock;

    // audio 
    #ifndef ENGINE_DISABLE_MODULE_AUDIO

        // wasapi
        IAudioClient       *pAudioClient;
        IAudioRenderClient *pAudioRenderClient;
        UINT32              bufferFrameCount;
        REFERENCE_TIME      hnsActualDuration;
        UINT32              nSamplesPerSec;

    #endif // ENGINE_DISABLE_MODULE_AUDIO

    // gpu
    //Emulated_Gpu_Backend gpu;
} Win32;

static Win32  _win32;
static Engine _engine;
static Win32* win32;

/* Inputs */
#include "engine_in_win32.h"

/* Os */
#include "engine_os_win32.h"

/* @DEPRECATED Use os_error instead. */
void engine_error(char* title, char *msg, ...) {
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

// /* @DEPRECATED Use os_exit instead. */
// void engine_crash(int error) {
//     if(IsDebuggerPresent())
//         __debugbreak();
//     ExitProcess(error); 
// }

#ifndef ENGINE_DISABLE_MODULE_AUDIO
void AudioClientErrorExit(HRESULT result, LPTSTR lpszFunction) {
    LPCTSTR AudioClientErrorExitTable[40] = {
        TEXT("The IAudioClient object is already initialized. (AUDCLNT_E_ALREADY_INITIALIZED)"),
        TEXT("The requested buffer size is not aligned. (AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED)"),
        TEXT("The buffer duration value requested by an exclusive-mode client is out of range. (AUDCLNT_E_BUFFER_SIZE_ERROR)"),
        TEXT("The process-pass duration exceeded the maximum CPU usage. (AUDCLNT_E_CPUUSAGE_EXCEEDED)"),
        TEXT("The audio endpoint device has been unplugged, reconfigured or disabled. (AUDCLNT_E_DEVICE_INVALIDATED)"),
        TEXT("The endpoint device is already in use. (AUDCLNT_E_DEVICE_IN_USE)"),
        TEXT("Failed to create the audio endpoint for the render or the capture device. (AUDCLNT_E_ENDPOINT_CREATE_FAILED)"),
        TEXT("The device period requested by an exclusive-mode client is greater than 5000 milliseconds. (AUDCLNT_E_INVALID_DEVICE_PERIOD)"),
        TEXT("The audio engine (shared mode) or audio endpoint device (exclusive mode) does not support the specified format. (AUDCLNT_E_UNSUPPORTED_FORMAT)"),
        TEXT("User has disabled exclusive-mode use of the device. (AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED)"),
        TEXT("Parameters hnsBufferDuration and hnsPeriodicity are not equal. (AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL)"),
        TEXT("The Windows audio service is not running. AUDCLNT_E_SERVICE_NOT_RUNNING")
    };
    LPVOID lpDisplayBuf;
    LPCTSTR pszFormat = AudioClientErrorExitTable[HRESULT_CODE(result)];
    lpDisplayBuf = (LPVOID)LocalAlloc(
        LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)pszFormat) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)
    ); 
    StringCchPrintf(
        (LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, 
        HRESULT_CODE(result), 
        pszFormat
    ); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
    LocalFree(lpDisplayBuf);
    ExitProcess(HRESULT_CODE(result)); 
}

#define AUDIO_FRAMES_TO_MILLISECONDS(numAudioFrames, nSamplesPerSec) \
    ((numAudioFrames * 1000) / nSamplesPerSec)
#define MILLISECONDS_TO_AUDIO_FRAMES(numMs, nSamplesPerSec) \
    (((numMs * nSamplesPerSec) + (1000 - 1)) / 1000)
//USER_API _audio
DWORD WINAPI AudioThreadProc(LPVOID lpParam) {		
    // initalize WASAPI
    HRESULT result;

    CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
    
    // get an instance to a device enumerator
    IMMDeviceEnumerator *device_enumerator = NULL;
    result = CoCreateInstance(
        &CLSID_MMDeviceEnumerator, 
        NULL,
        CLSCTX_ALL,
        &IID_IMMDeviceEnumerator,
        (void**)&device_enumerator
    );
    if (FAILED(result)) return result;

    // get the default audio device
    IMMDevice* device;
    result = IMMDeviceEnumerator_GetDefaultAudioEndpoint(
        device_enumerator, 
        eRender, 
        eConsole, 
        &device
    );
    if (FAILED(result)) return result;

    // activate the audio device
    IAudioClient* pAudioClient;
    result = IMMDevice_Activate(device, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
    if (FAILED(result)) return result;
    win32->pAudioClient = pAudioClient;

    // initalize an audio client for our application
    WAVEFORMATEX * pWaveFormat;
    result = IAudioClient_GetMixFormat(pAudioClient, &pWaveFormat);
    if (FAILED(result)) AudioClientErrorExit(result, TEXT("IAudioClient_GetMixFormat"));
    DWORD nSamplesPerSec = pWaveFormat->nSamplesPerSec;
    nSamplesPerSec = 44100;

    CoTaskMemFree(pWaveFormat);
    
    WAVEFORMATEXTENSIBLE waveFormat;
    waveFormat.Format.cbSize               = sizeof(waveFormat);
    waveFormat.Format.wFormatTag           = WAVE_FORMAT_EXTENSIBLE;
    waveFormat.Format.wBitsPerSample       = 16;
    waveFormat.Format.nChannels            = 2;
    waveFormat.Format.nSamplesPerSec       = nSamplesPerSec;
    waveFormat.Format.nBlockAlign          = (WORD)(waveFormat.Format.nChannels * waveFormat.Format.wBitsPerSample / 8);
    waveFormat.Format.nAvgBytesPerSec      = waveFormat.Format.nSamplesPerSec * waveFormat.Format.nBlockAlign;
    waveFormat.Samples.wValidBitsPerSample = 16;
    waveFormat.dwChannelMask               = KSAUDIO_SPEAKER_STEREO;
    waveFormat.SubFormat                   = KSDATAFORMAT_SUBTYPE_PCM;

    REFERENCE_TIME hnsRequestedDuration = 10000000ULL * 60;

    result = IAudioClient_Initialize(
        pAudioClient, 
        AUDCLNT_SHAREMODE_SHARED, 
        AUDCLNT_STREAMFLAGS_NOPERSIST | AUDCLNT_STREAMFLAGS_RATEADJUST,
        hnsRequestedDuration, 
        0, 
        &waveFormat.Format, 
        NULL
    );
    if (FAILED(result)) AudioClientErrorExit(result, TEXT("IAudioClient_Initialize"));
    
    INT32 bufferFrameCount;
    result = IAudioClient_GetBufferSize(pAudioClient, &bufferFrameCount);
    if (FAILED(result)) AudioClientErrorExit(result, TEXT("IAudioClient_GetBufferSize"));
    REFERENCE_TIME hnsActualDuration = 10000000 * bufferFrameCount / nSamplesPerSec;

    IAudioRenderClient* pAudioRenderClient;
    result = IAudioClient_GetService(
        pAudioClient, 
        &IID_IAudioRenderClient, 
        (void **)&pAudioRenderClient
    );
    if (FAILED(result)) return result;
    win32->pAudioRenderClient = pAudioRenderClient;

    // make the audio client start playing
    IAudioClient_Start(pAudioClient);

    win32->hnsActualDuration = hnsActualDuration;
    win32->bufferFrameCount  = bufferFrameCount;
    win32->nSamplesPerSec    = nSamplesPerSec;

    DWORD msLastTick = GetTickCount();
    for (;;) {
        DWORD msCurrentTick = GetTickCount();
        DWORD msDeltaTick = msCurrentTick - msLastTick;
        msLastTick = msCurrentTick;
        // TODO: Handle device swap
        HRESULT result;
        UINT32 numPaddingAudioFrames;
        result = IAudioClient_GetCurrentPadding(
            win32->pAudioClient, 
            &numPaddingAudioFrames
        );
        UINT32 numAvailableAudioFrames = 
            win32->bufferFrameCount - numPaddingAudioFrames;
        INT32 numDesiredAudioFrames = MILLISECONDS_TO_AUDIO_FRAMES(
            msDeltaTick,
            win32->nSamplesPerSec
        );
        numDesiredAudioFrames = max(numDesiredAudioFrames, 0);
        UINT32 numRequestedAudioFrames = min(
            numAvailableAudioFrames, 
            numDesiredAudioFrames
        );
        BYTE* pData;
        result = IAudioRenderClient_GetBuffer(
            win32->pAudioRenderClient,
            numRequestedAudioFrames,
            &pData
        );
        if (SUCCEEDED(result)) {
            _audio(numRequestedAudioFrames, (short*)pData);
            IAudioRenderClient_ReleaseBuffer(
                win32->pAudioRenderClient,
                numRequestedAudioFrames,
                0
            );
        }
        Sleep(AUDIO_FRAMES_TO_MILLISECONDS(
            engine->audio_latency_in_audio_frames,
            win32->nSamplesPerSec
        ));
    }
}
#endif // ENGINE_DISABLE_MODULE_AUDIO

void engine_main_loop_body(void *data) {

    // engine pre update
    #ifndef ENGINE_DISABLE_ENGINE_PRE_UPDATE
    engine_pre_update(engine);
    #endif // ENGINE_DISABLE_ENGINE_PRE_UPDATE

    // inputs
    #if 0
    // updat mouse position
    {
        POINT cursor_position;
        GetCursorPos(&cursor_position);
        ScreenToClient(win32->hwnd, &cursor_position);
        engine->mouse_position_at_last_frame = engine->mouse_position;
        engine->mouse_position               = v2(cursor_position.x, engine->window_size_in_px.y - cursor_position.y);
        engine->mouse_delta.x = engine->mouse_position.x - engine->mouse_position_at_last_frame.x;
        engine->mouse_delta.y = engine->mouse_position.y - engine->mouse_position_at_last_frame.y;
        //if(engine.delta_time > 0)
        //    engine.mouse.velocity           = lerp(engine.mouse.velocity, engine.mouse.delta / engine.delta_time, 0.75f);
    }
    #endif

    // call user update
    if(engine->update)
        engine->update();

    // if(engine->cursor != win32->cursor) {
    //     PostMessage(0, WM_SETCURSOR, 0, 0);
    //     POINT p = {0};
    //     GetCursorPos(&p);
    //     SetCursorPos(p.x, p.y);
    // }

    //if(engine->window_flags & ENGINE_WINDOWING_FLAG_HOVER) {
    //    SetCursor(win32->cursors[engine->cursor]);
    //}
    
    // deleteme
    // // engine post update
    // #ifndef ENGINE_DISABLE_ENGINE_POST_UPDATE
    // engine_post_update(engine);
    // #endif // ENGINE_DISABLE_ENGINE_POST_UPDATE
}

void win32_main_loop_body() {
    // for(
    //     Job *graphic_job = job_ring_try_remove_one(&engine->job_ring_graphics);
    //     graphic_job;
    //     graphic_job = job_ring_try_remove_one(&engine->job_ring_graphics)
    // ) {
    //     graphic_job->proc(graphic_job->data);
    //     job_end(graphic_job);
    // }
    
    for(
        Job *job = job_ring64_try_remove_one(&engine->job_ring);
        job;
        job = job_ring64_try_remove_one(&engine->job_ring)
    ) {
        job->proc(job->data);
        job_end(job);
    }
}

void win32_worker_proc(void *data) {
    if (engine->worker_init_proc)
        engine->worker_init_proc(data);
    for(size_t itteration_index = 0;;) {
        Job *job = job_ring64_try_remove_one(&engine->job_ring);
        if (job) {
            job->proc(job->data);
            job_end(job);
            itteration_index = 0;
        }
        ++itteration_index;
    }
}

void win32_graphics_worker_proc(Os_Win32_Surface *data) {
    /* Init Opengl */
    {
        HDC hdc = data->hdc;
        HGLRC context = (HGLRC)SendMessage(
            win32->hwnd,
            WM_USER_CREATE_OPENGL_CONTEXT,
            (WPARAM)hdc,
            0
        );
        wglMakeCurrent(hdc, context);
        gl_error_guard("wglMakeCurrent");
    }
    if (engine->worker_init_proc)
        engine->worker_init_proc(data);
    if (engine->worker_graphic_init_proc)
        engine->worker_graphic_init_proc(data);
    for(size_t itteration_index = 0;;) {
        Job *job;
        job = job_ring64_try_remove_one(&engine->job_ring_graphics);
        if (!job)
            job = job_ring64_try_remove_one(&engine->job_ring);
        if (job) {
            job->proc(job->data);
            job_end(job);
            itteration_index = 0;
            continue;
        }
        ++itteration_index;
        if (itteration_index > 8) {
            
        }
    }
}

static const Job WIN32_STARTUP_JOB = {
    _STARTUP, /* _STARTUP_JOB */
    &_engine, 
    0,
    0,
    0,
    0
};

void win32_startup_thread_proc(void *data) {
    _STARTUP(engine);
    
    ASSERT(engine->update, "No update function provided. Please set engine->update to something.");
    
    /* Start Scheduler */
    {
        size_t num_logical_core = os_num_logical_core();
        num_logical_core = 1;
        //num_logical_core = num_logical_core > 1 ? 1 : num_logical_core;
        size_t logical_core_index = 0;
        /* Graphics Worker */
        for (
            ;
            logical_core_index < num_logical_core;
            ++logical_core_index
        ) {
            HANDLE hWorkerThread = CreateThread(
                NULL,
                0,
                win32_graphics_worker_proc,
                &win32->main_surface, /* HACK */
                0,
                NULL // lpThreadId
            );

            {
                DWORD_PTR result;
                SetThreadAffinityMask(
                    hWorkerThread,
                    (1ull << logical_core_index)
                );
            }
        }
        /* Regular Worker */
        for (
            ;
            logical_core_index < num_logical_core;
            ++logical_core_index
        ) {
            HANDLE hWorkerThread = CreateThread(
                NULL,
                0,
                win32_worker_proc,
                &win32->main_surface, /* HACK */
                0,
                NULL // lpThreadId
            );

            {
                DWORD_PTR result;
                SetThreadAffinityMask(
                    hWorkerThread,
                    (1ull << logical_core_index)
                );
            }
        }
    }
}

// TODO change to wWinMain
int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR     lpCmdLine, 
    int       nCmdShow
) {
    // initialize backend
    win32 = (Win32*)&_win32;

    engine = &_engine;
    
    #ifndef ENGINE_DISABLE_GPU
    // initialize gpu module
    engine_backend_win32_opengl_pre_window_init(hInstance);
    #endif /* !ENGINE_DISABLE_GPU */
    
    /* Load cursors */
    {
        #define FOREACH(o, w) win32->cursors[o] = LoadCursor(NULL, w);
            OS_CURSOR_FOREACH(FOREACH)
        #undef FOREACH
        
        BYTE ANDmaskCursor[] = 
        { 
            0xFF, 0xFC, 0x3F, 0xFF,   // line 1 
            0xFF, 0xC0, 0x1F, 0xFF,   // line 2 
            0xFF, 0x00, 0x3F, 0xFF,   // line 3 
            0xFE, 0x00, 0xFF, 0xFF,   // line 4 
        
            0xF7, 0x01, 0xFF, 0xFF,   // line 5 
            0xF0, 0x03, 0xFF, 0xFF,   // line 6 
            0xF0, 0x03, 0xFF, 0xFF,   // line 7 
            0xE0, 0x07, 0xFF, 0xFF,   // line 8 
        
            0xC0, 0x07, 0xFF, 0xFF,   // line 9 
            0xC0, 0x0F, 0xFF, 0xFF,   // line 10 
            0x80, 0x0F, 0xFF, 0xFF,   // line 11 
            0x80, 0x0F, 0xFF, 0xFF,   // line 12 
        
            0x80, 0x07, 0xFF, 0xFF,   // line 13 
            0x00, 0x07, 0xFF, 0xFF,   // line 14 
            0x00, 0x03, 0xFF, 0xFF,   // line 15 
            0x00, 0x00, 0xFF, 0xFF,   // line 16 
        
            0x00, 0x00, 0x7F, 0xFF,   // line 17 
            0x00, 0x00, 0x1F, 0xFF,   // line 18 
            0x00, 0x00, 0x0F, 0xFF,   // line 19 
            0x80, 0x00, 0x0F, 0xFF,   // line 20 
        
            0x80, 0x00, 0x07, 0xFF,   // line 21 
            0x80, 0x00, 0x07, 0xFF,   // line 22 
            0xC0, 0x00, 0x07, 0xFF,   // line 23 
            0xC0, 0x00, 0x0F, 0xFF,   // line 24 
        
            0xE0, 0x00, 0x0F, 0xFF,   // line 25 
            0xF0, 0x00, 0x1F, 0xFF,   // line 26 
            0xF0, 0x00, 0x1F, 0xFF,   // line 27 
            0xF8, 0x00, 0x3F, 0xFF,   // line 28 
        
            0xFE, 0x00, 0x7F, 0xFF,   // line 29 
            0xFF, 0x00, 0xFF, 0xFF,   // line 30 
            0xFF, 0xC3, 0xFF, 0xFF,   // line 31 
            0xFF, 0xFF, 0xFF, 0xFF    // line 32 
        };
        
        // Yin-shaped cursor XOR mask 
        
        BYTE XORmaskCursor[] = 
        { 
            0x00, 0x00, 0x00, 0x00,   // line 1 
            0x00, 0x03, 0xC0, 0x00,   // line 2 
            0x00, 0x3F, 0x00, 0x00,   // line 3 
            0x00, 0xFE, 0x00, 0x00,   // line 4 
        
            0x0E, 0xFC, 0x00, 0x00,   // line 5 
            0x07, 0xF8, 0x00, 0x00,   // line 6 
            0x07, 0xF8, 0x00, 0x00,   // line 7 
            0x0F, 0xF0, 0x00, 0x00,   // line 8 
        
            0x1F, 0xF0, 0x00, 0x00,   // line 9 
            0x1F, 0xE0, 0x00, 0x00,   // line 10 
            0x3F, 0xE0, 0x00, 0x00,   // line 11 
            0x3F, 0xE0, 0x00, 0x00,   // line 12 
        
            0x3F, 0xF0, 0x00, 0x00,   // line 13 
            0x7F, 0xF0, 0x00, 0x00,   // line 14 
            0x7F, 0xF8, 0x00, 0x00,   // line 15 
            0x7F, 0xFC, 0x00, 0x00,   // line 16 
        
            0x7F, 0xFF, 0x00, 0x00,   // line 17 
            0x7F, 0xFF, 0x80, 0x00,   // line 18 
            0x7F, 0xFF, 0xE0, 0x00,   // line 19 
            0x3F, 0xFF, 0xE0, 0x00,   // line 20 
        
            0x3F, 0xC7, 0xF0, 0x00,   // line 21 
            0x3F, 0x83, 0xF0, 0x00,   // line 22 
            0x1F, 0x83, 0xF0, 0x00,   // line 23 
            0x1F, 0x83, 0xE0, 0x00,   // line 24 
        
            0x0F, 0xC7, 0xE0, 0x00,   // line 25 
            0x07, 0xFF, 0xC0, 0x00,   // line 26 
            0x07, 0xFF, 0xC0, 0x00,   // line 27 
            0x01, 0xFF, 0x80, 0x00,   // line 28 
        
            0x00, 0xFF, 0x00, 0x00,   // line 29 
            0x00, 0x3C, 0x00, 0x00,   // line 30 
            0x00, 0x00, 0x00, 0x00,   // line 31 
            0x00, 0x00, 0x00, 0x00    // line 32 
        };

        // Create a custom cursor at run time. 
        
        win32->cursors[3] = CreateCursor(
            hInstance,   // app. instance 
            19,                // horizontal position of hot spot 
            2,                 // vertical position of hot spot 
            32,                // cursor width 
            32,                // cursor height 
            ANDmaskCursor,     // AND mask 
            XORmaskCursor      // XOR mask 
        );
    }

    /* Invisible window:
     *   Because of the way an application on Windows
     *   must handle messages, the thread creating the window,
     *   also responsible for handling it's messages, will stall
     *   every time the user resizes that window.
     *   To prevent the application from stalling, we dedicate
     *   the initial thread to be responsible for creating and
     *   destroying windows.
     *   At startup we create a hidden window  on the initial
     *   thread, responsible of handling our custom create
     *   and destroy window events.
     *   (We also profit from that hidden window to query the supported
     *   OpenGL featureset on the executing machine. As the API requires
     *   a window to do such.)
     * 
     *   This idea comes from Casey Muratori's Dangerous Threads Crew:
     *   > https://github.com/cmuratori/dtc */
    HWND hwnd;
    {
        {
            WNDCLASSEX window_class = {0};
            window_class.cbSize        = sizeof(WNDCLASSEX);
            window_class.style         = 0;
            window_class.lpfnWndProc   = WindowProc;
            window_class.hInstance     = hInstance;
            window_class.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
            window_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
            window_class.lpszClassName = WIN32_INITIAL_INVISIBLE_WINDOW_CLASS_NAME;
            //window_class.hbrBackground = NULL;
            window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            //window_class.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(1));
            ATOM result = RegisterClassEx(&window_class);
            if (result == 0)
                os_win32_exit_with_error(TEXT("RegisterClassEx"));
        }

        hwnd = CreateWindowEx(
            0,                                         // Optional window styles.
            WIN32_INITIAL_INVISIBLE_WINDOW_CLASS_NAME, // Window class
            NULL,                                      // Window text (set later)
            0,                                         // Window style
            CW_USEDEFAULT, CW_USEDEFAULT,              // Window position 
            CW_USEDEFAULT, CW_USEDEFAULT,              // Window size (set later)

            HWND_MESSAGE, //NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
        );
        if (hwnd == NULL) {
            DWORD error = GetLastError();
            os_win32_exit_with_error(TEXT("CreateWindowEx"));
        }
    }
    win32->hwnd = hwnd;

    // initialize engine
    #ifndef ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION
    //engine_pre_init(engine); // engine.h
    #endif // ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION

    /* Inputs */
    win32->in_current = &win32->ins[0];
    in_on_dll_reload(&win32->ins[1]);

    HANDLE startup_thread = CreateThread(
        NULL,
        0,
        win32_startup_thread_proc,
        0,
        0,
        NULL
    );

    // initialize audio
    #ifndef ENGINE_DISABLE_MODULE_AUDIO
    // create and start the audio thread
    HANDLE hAudioThread = CreateThread(
        NULL,
        0,
        AudioThreadProc,
        NULL,
        0,
        NULL // lpThreadId
    );
    #endif // ENGINE_DISABLE_MODULE_AUDIO

    MSG msg;
    for(;;) {
        MSG msg;
        GetMessage(&msg, NULL, 0, 0);
        if (msg.message == WM_QUIT)
            ExitProcess(0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
#endif /* ENGINE_WIN32_H */
