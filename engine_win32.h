#ifdef __INTELLISENSE__
#include "engine.h"
#endif // __INTELLISENSE__

#define NOGDICAPMASKS    
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES    
//#define NOWINSTYLES      
#define NOSYSMETRICS     
#define NOMENUS          
#define NOICONS          
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

// gpu
void engine_backend_win32_opengl_pre_window_init(HINSTANCE hInstance);
void engine_backend_win32_opengl_post_window_init();

// threading
#include <processthreadsapi.h>

// memory
#include "engine_win32_arena.h"

// assets
#include "engine_win32_asset.h"

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

// backend
typedef struct Win32 {
	HCURSOR cursor;
	HWND    hwnd;
    HDC     hdc;

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

void ErrorExit(LPTSTR lpszFunction) { 
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
	ExitProcess(dw); 
}

void GeneralErrorExit(char* title, char* msg) { 
    MessageBoxA(NULL, msg, title, MB_OK);
    ExitProcess(1); 
}

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

LRESULT CALLBACK WindowProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (uMsg) {
    // inputs
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    //case WM_MOUSEWHEEL:
    {
        UINT down = 0;
        //if (
        //    uMsg == WM_MBUTTONDOWN ||
        //    uMsg == WM_MBUTTONUP   
        //) {
        //    down |= (wParam & MK_MBUTTON) != 0;
        //    wParam = VK_MBUTTON;
        //}

        down |=
            uMsg == WM_LBUTTONDOWN ||
            uMsg == WM_RBUTTONDOWN ||
            uMsg == WM_MBUTTONDOWN ||
            uMsg == WM_SYSKEYDOWN  ||
            uMsg == WM_KEYDOWN;

             if (uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN) wParam = VK_LBUTTON;
        else if (uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDOWN) wParam = VK_RBUTTON;
        else if (uMsg == WM_MBUTTONUP || uMsg == WM_MBUTTONDOWN) 
                 wParam = VK_MBUTTON;

        U64 button_batch_index  = (U64)wParam >> 5;
        U64 button_bit_in_batch = (U64)1 << ((wParam & (32 - 1)) << 1);
        if (engine->buttons[button_batch_index] & (button_bit_in_batch << 1)) {
            if (!down) engine->buttons[button_batch_index] &= ~button_bit_in_batch;
        } else {
            if (down) engine->buttons[button_batch_index] |= button_bit_in_batch;
        }
    } break;
    case WM_MOUSEWHEEL: {
        engine->mouse_wheel.y = (F32)((signed short)HIWORD(wParam) / WHEEL_DELTA);
    } break;
    case WM_MOUSEMOVE: {
        SetCursor(win32->cursor);
        engine->window_flags |=  ENGINE_WINDOWING_FLAG_HOVER;
    } break;
    // resize
    case WM_SIZE: {
        int width  = LOWORD(lParam); 
        int height = HIWORD(lParam);
        engine->window_size_in_px.x = width;
        engine->window_size_in_px.y = height;
    } break;
    // window is asked to be displayed
    case WM_SHOWWINDOW: {
        // Immediatly get rid of the hourglass cursor.
        SetCursor(win32->cursor);

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
        ExitProcess(0);
        return TRUE;
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
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

	//// initialize emulated gpu module
	//gpu_module_init_emulated_backend(&win32->gpu);
	
	engine = &_engine;
	
	// initialize gpu module
	engine_backend_win32_opengl_pre_window_init(hInstance);

	// initialize windowing
    HWND hwnd;
    {
		const TCHAR WINDOW_CLASS_NAME[] = TEXT("\1");
		const TCHAR WINDOW_TEXT[]       = TEXT("Hello");
        {
            WNDCLASS window_class = { 0 };
            window_class.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
            window_class.lpfnWndProc   = WindowProc;
            window_class.hInstance     = hInstance;
            window_class.lpszClassName = WINDOW_CLASS_NAME;
            window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
            window_class.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(1));
            RegisterClass(&window_class);
        }

        hwnd = CreateWindowEx(
            0,                                   // Optional window styles.
            WINDOW_CLASS_NAME,                   // Window class
            NULL,                                // Window text (set later)
            WS_OVERLAPPEDWINDOW,// | WS_VISIBLE, // Window style
            CW_USEDEFAULT, CW_USEDEFAULT,        // Window position 
			0, 0,                                // Window size (set later)

            NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
        );
		if (hwnd == NULL) ErrorExit(TEXT("CreateWindowEx"));

		// track mouse leaving client window area
		TRACKMOUSEEVENT tackMouseEvent;
		tackMouseEvent.cbSize  = sizeof(TRACKMOUSEEVENT);
		tackMouseEvent.dwFlags = TME_NONCLIENT | TME_LEAVE;
		tackMouseEvent.hwndTrack = hwnd;
		if (!TrackMouseEvent(&tackMouseEvent)) 
			ErrorExit(TEXT("TrackMouseEvent"));

		// default cursor to arrows
		win32->hwnd   = hwnd;
        win32->hdc    = GetDC(hwnd);
        win32->cursor = LoadCursor(NULL, IDC_ARROW);
    }
	
	engine_backend_win32_opengl_post_window_init();
	
	// initialize engine
	#ifndef ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION
	engine_pre_init(engine); // engine.h
	#endif // ENGINE_DISABLE_ENGINE_DEFAULT_INITIALIZATION

	// initialize user
	_init();

    // apply user window title
    {
        SetWindowTextW(hwnd, (LPCWSTR)engine->window_title.data);
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
			client_to_window_rect.right  = engine->window_size_in_px.x;
			client_to_window_rect.bottom = engine->window_size_in_px.y;
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

    // begin main loop
    MSG msg;
	for(;;) {

        // inputs
        {
            // clear mouse wheek
            engine->mouse_wheel = (V2){ 0, 0 };

            // update buttons
            int num_batches = sizeof(engine->buttons) / sizeof(engine->buttons[0]);
            for(int i_batch = 0; i_batch < num_batches; ++i_batch) {
                // copy last frame's input bits to adjacent bits
                engine->buttons[i_batch] &= 0x5555555555555555;             // clear very old bits
                engine->buttons[i_batch] |= engine->buttons[i_batch] << 1;  // copy last frame's bit
            }
        }
		
		// handle Windows messaging
		MSG msg;
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) ExitProcess(0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// engine pre update
        #ifndef ENGINE_DISABLE_ENGINE_PRE_UPDATE
        engine_pre_update(engine);
        #endif // ENGINE_DISABLE_ENGINE_PRE_UPDATE

        // inputs
        // updat mouse position
        {
            POINT cursor_position;
            GetCursorPos(&cursor_position);
            ScreenToClient(hwnd, &cursor_position);
            engine->mouse_position_at_last_frame = engine->mouse_position;
            engine->mouse_position               = v2(cursor_position.x, engine->window_size_in_px.y - cursor_position.y);
            engine->mouse_delta.x = engine->mouse_position.x - engine->mouse_position_at_last_frame.x;
            engine->mouse_delta.y = engine->mouse_position.y - engine->mouse_position_at_last_frame.y;
            //if(engine.delta_time > 0)
            //    engine.mouse.velocity           = lerp(engine.mouse.velocity, engine.mouse.delta / engine.delta_time, 0.75f);
        }

		// user update
		_update();
		
		// engine post update
        #ifndef ENGINE_DISABLE_ENGINE_POST_UPDATE
        engine_post_update(engine);
        #endif // ENGINE_DISABLE_ENGINE_POST_UPDATE
    }

    return 0;
}