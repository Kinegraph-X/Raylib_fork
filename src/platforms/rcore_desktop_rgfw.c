/**********************************************************************************************
*
*   rcore_desktop_rgfw - Functions to manage window, graphics device and inputs
*
*   PLATFORM: RGFW
*       - Windows (Win32, Win64)
*       - Linux (X11/Wayland desktop mode)
*       - MacOS (Cocoa)
*
*   LIMITATIONS:
*       - TODO
*
*   POSSIBLE IMPROVEMENTS:
*       - TODO
*
*   ADDITIONAL NOTES:
*       - RAYLIB_TRACELOG() function is located in raylib [utils] module
*
*   CONFIGURATION:
*       #define RCORE_PLATFORM_RGFW
*           Custom flag for rcore on target platform RGFW
*
*   DEPENDENCIES:
*       - RGFW.h (main library): Windowing and inputs management
*       - gestures: Gestures system for touch-ready devices (or simulated from mouse inputs)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5), Colleague Riley and contributors
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#if defined(RAYLIB_GRAPHICS_API_OPENGL_ES2)
    #define RGFW_OPENGL_ES2
#endif

void RaylibShowCursor(void);
void RaylibCloseWindow(void);

#if defined(__linux__)
    #define _INPUT_EVENT_CODES_H
#endif

#if defined(__unix__) || defined(__linux__)
    #define _XTYPEDEF_FONT
#endif

#define RGFW_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
	#define RaylibRectangle rectangle_win32
    #define RaylibCloseWindow CloseWindow_win32
    #define RaylibShowCursor __imp_ShowCursor
	#define _APISETSTRING_
	
	#undef MAX_PATH

	__declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int CodePage, unsigned long dwFlags, const char *lpMultiByteStr, int cbMultiByte, wchar_t *lpWideCharStr, int cchWideChar);
#endif

#if defined(__APPLE__)
    #define Point NSPOINT
    #define Size NSSIZE
#endif

#include "../external/RGFW.h"

#if defined(_WIN32) || defined(_WIN64)
    #undef RaylibDrawText
    #undef RaylibShowCursor
    #undef RaylibCloseWindow
    #undef RaylibRectangle

	#undef MAX_PATH
	#define MAX_PATH 1025
#endif

#if defined(__APPLE__)
    #undef Point
    #undef Size
#endif

#include <stdbool.h>
#include <string.h>     // Required for: strcmp()

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct {
    RGFW_window *window;                // Native display device (physical screen connection)
} PlatformData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static PlatformData platform = { NULL }; // Platform specific

static bool RGFW_disableCursor = false;

static const unsigned short keyMappingRGFW[] = {
    [RGFW_KEY_NULL] = RAYLIB_KEY_NULL,
    [RGFW_Quote] = RAYLIB_KEY_APOSTROPHE,
    [RGFW_Comma] = RAYLIB_KEY_COMMA,
    [RGFW_Minus] = RAYLIB_KEY_MINUS,
    [RGFW_Period] = RAYLIB_KEY_PERIOD,
    [RGFW_Slash] = RAYLIB_KEY_SLASH,
    [RGFW_Escape] = RAYLIB_KEY_ESCAPE,
    [RGFW_F1] = RAYLIB_KEY_F1,
    [RGFW_F2] = RAYLIB_KEY_F2,
    [RGFW_F3] = RAYLIB_KEY_F3,
    [RGFW_F4] = RAYLIB_KEY_F4,
    [RGFW_F5] = RAYLIB_KEY_F5,
    [RGFW_F6] = RAYLIB_KEY_F6,
    [RGFW_F7] = RAYLIB_KEY_F7,
    [RGFW_F8] = RAYLIB_KEY_F8,
    [RGFW_F9] = RAYLIB_KEY_F9,
    [RGFW_F10] = RAYLIB_KEY_F10,
    [RGFW_F11] = RAYLIB_KEY_F11,
    [RGFW_F12] = RAYLIB_KEY_F12,
    [RGFW_Backtick] = RAYLIB_KEY_GRAVE,
    [RGFW_0] = RAYLIB_KEY_ZERO,
    [RGFW_1] = RAYLIB_KEY_ONE,
    [RGFW_2] = RAYLIB_KEY_TWO,
    [RGFW_3] = RAYLIB_KEY_THREE,
    [RGFW_4] = RAYLIB_KEY_FOUR,
    [RGFW_5] = RAYLIB_KEY_FIVE,
    [RGFW_6] = RAYLIB_KEY_SIX,
    [RGFW_7] = RAYLIB_KEY_SEVEN,
    [RGFW_8] = RAYLIB_KEY_EIGHT,
    [RGFW_9] = RAYLIB_KEY_NINE,
    [RGFW_Equals] = RAYLIB_KEY_EQUAL,
    [RGFW_BackSpace] = RAYLIB_KEY_BACKSPACE,
    [RGFW_Tab] = RAYLIB_KEY_TAB,
    [RGFW_CapsLock] = RAYLIB_KEY_CAPS_LOCK,
    [RGFW_ShiftL] = RAYLIB_KEY_LEFT_SHIFT,
    [RGFW_ControlL] = RAYLIB_KEY_LEFT_CONTROL,
    [RGFW_AltL] = RAYLIB_KEY_LEFT_ALT,
    [RGFW_SuperL] = RAYLIB_KEY_LEFT_SUPER,
    #ifndef RGFW_MACOS
    [RGFW_ShiftR] = RAYLIB_KEY_RIGHT_SHIFT,

    [RGFW_AltR] = RAYLIB_KEY_RIGHT_ALT,
    #endif
    [RGFW_Space] = RAYLIB_KEY_SPACE,

    [RGFW_a] = RAYLIB_KEY_A,
    [RGFW_b] = RAYLIB_KEY_B,
    [RGFW_c] = RAYLIB_KEY_C,
    [RGFW_d] = RAYLIB_KEY_D,
    [RGFW_e] = RAYLIB_KEY_E,
    [RGFW_f] = RAYLIB_KEY_F,
    [RGFW_g] = RAYLIB_KEY_G,
    [RGFW_h] = RAYLIB_KEY_H,
    [RGFW_i] = RAYLIB_KEY_I,
    [RGFW_j] = RAYLIB_KEY_J,
    [RGFW_k] = RAYLIB_KEY_K,
    [RGFW_l] = RAYLIB_KEY_L,
    [RGFW_m] = RAYLIB_KEY_M,
    [RGFW_n] = RAYLIB_KEY_N,
    [RGFW_o] = RAYLIB_KEY_O,
    [RGFW_p] = RAYLIB_KEY_P,
    [RGFW_q] = RAYLIB_KEY_Q,
    [RGFW_r] = RAYLIB_KEY_R,
    [RGFW_s] = RAYLIB_KEY_S,
    [RGFW_t] = RAYLIB_KEY_T,
    [RGFW_u] = RAYLIB_KEY_U,
    [RGFW_v] = RAYLIB_KEY_V,
    [RGFW_w] = RAYLIB_KEY_W,
    [RGFW_x] = RAYLIB_KEY_X,
    [RGFW_y] = RAYLIB_KEY_Y,
    [RGFW_z] = RAYLIB_KEY_Z,
    [RGFW_Bracket] = RAYLIB_KEY_LEFT_BRACKET,
    [RGFW_BackSlash] = RAYLIB_KEY_BACKSLASH,
    [RGFW_CloseBracket] = RAYLIB_KEY_RIGHT_BRACKET,
    [RGFW_Semicolon] = RAYLIB_KEY_SEMICOLON,
    [RGFW_Insert] = RAYLIB_KEY_INSERT,
    [RGFW_Home] = RAYLIB_KEY_HOME,
    [RGFW_PageUp] = RAYLIB_KEY_PAGE_UP,
    [RGFW_Delete] = RAYLIB_KEY_DELETE,
    [RGFW_End] = RAYLIB_KEY_END,
    [RGFW_PageDown] = RAYLIB_KEY_PAGE_DOWN,
    [RGFW_Right] = RAYLIB_KEY_RIGHT,
    [RGFW_Left] = RAYLIB_KEY_LEFT,
    [RGFW_Down] = RAYLIB_KEY_DOWN,
    [RGFW_Up] = RAYLIB_KEY_UP,
    [RGFW_Numlock] = RAYLIB_KEY_NUM_LOCK,
    [RGFW_KP_Slash] = RAYLIB_KEY_KP_DIVIDE,
    [RGFW_Multiply] = RAYLIB_KEY_KP_MULTIPLY,
    [RGFW_KP_Minus] = RAYLIB_KEY_KP_SUBTRACT,
    [RGFW_KP_Return] = RAYLIB_KEY_KP_ENTER,
    [RGFW_KP_1] = RAYLIB_KEY_KP_1,
    [RGFW_KP_2] = RAYLIB_KEY_KP_2,
    [RGFW_KP_3] = RAYLIB_KEY_KP_3,
    [RGFW_KP_4] = RAYLIB_KEY_KP_4,
    [RGFW_KP_5] = RAYLIB_KEY_KP_5,
    [RGFW_KP_6] = RAYLIB_KEY_KP_6,
    [RGFW_KP_7] = RAYLIB_KEY_KP_7,
    [RGFW_KP_8] = RAYLIB_KEY_KP_8,
    [RGFW_KP_9] = RAYLIB_KEY_KP_9,
    [RGFW_KP_0] = RAYLIB_KEY_KP_0,
    [RGFW_KP_Period] = RAYLIB_KEY_KP_DECIMAL
};

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
int InitPlatform(void);          // Initialize platform (graphics, inputs and more)
bool InitGraphicsDevice(void);   // Initialize graphics device

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
// NOTE: Functions declaration is provided by raylib.h

//----------------------------------------------------------------------------------
// Module Functions Definition: Window and Graphics Device
//----------------------------------------------------------------------------------

// Check if application should close
bool RaylibWindowShouldClose(void)
{
    if (CORE.Window.shouldClose == false)
        CORE.Window.shouldClose = RGFW_window_shouldClose(platform.window);
    if (CORE.Window.ready) return CORE.Window.shouldClose;
    else return true;
}

// Toggle fullscreen mode
void RaylibToggleFullscreen(void)
{   
    RGFW_window_maximize(platform.window);
    RaylibToggleBorderlessWindowed();
}

// Toggle borderless windowed mode
void RaylibToggleBorderlessWindowed(void)
{
    if (platform.window != NULL)
    {
        RGFW_window_setBorder(platform.window, CORE.Window.flags & RAYLIB_FLAG_WINDOW_UNDECORATED);
    }
}

// Set window state: maximized, if resizable
void RaylibMaximizeWindow(void)
{
    RGFW_window_maximize(platform.window);
}

// Set window state: minimized
void RaylibMinimizeWindow(void)
{
    RGFW_window_minimize(platform.window);
}

// Set window state: not minimized/maximized
void RaylibRestoreWindow(void)
{
    RGFW_window_restore(platform.window);
}

// Set window configuration state using flags
void RaylibSetWindowState(unsigned int flags)
{
    CORE.Window.flags |= flags;

    if (flags & RAYLIB_FLAG_VSYNC_HINT)
    {
        RGFW_window_swapInterval(platform.window, 1);
    }
    if (flags & RAYLIB_FLAG_FULLSCREEN_MODE)
    {
        RGFW_window_maximize(platform.window);
        RaylibToggleBorderlessWindowed();
    }
    if (flags & RAYLIB_FLAG_WINDOW_RESIZABLE)
    {
        RGFW_window_setMaxSize(platform.window, RGFW_AREA(platform.window->r.w, platform.window->r.h));
        RGFW_window_setMinSize(platform.window, RGFW_AREA(platform.window->r.w, platform.window->r.h));
    }
    if (flags & RAYLIB_FLAG_WINDOW_UNDECORATED)
    {
        RaylibToggleBorderlessWindowed();
    }
    if (flags & RAYLIB_FLAG_WINDOW_HIDDEN)
    {
        RGFW_window_hide(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_MINIMIZED)
    {
        RGFW_window_minimize(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_MAXIMIZED)
    {
        RGFW_window_maximize(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_UNFOCUSED)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_WINDOW_UNFOCUSED is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_TOPMOST)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_WINDOW_TOPMOST is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_ALWAYS_RUN)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_WINDOW_ALWAYS_RUN is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_TRANSPARENT)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_WINDOW_TRANSPARENT post window creation post window creation is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_HIGHDPI)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_WINDOW_HIGHDPI is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH)
    {
        RGFW_window_setMousePassthrough(platform.window, flags & RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH);
    }
    if (flags & RAYLIB_FLAG_BORDERLESS_WINDOWED_MODE)
    {
        RaylibToggleBorderlessWindowed();
    }
    if (flags & RAYLIB_FLAG_MSAA_4X_HINT)
    {
        RGFW_setGLSamples(4);
    }
    if (flags & RAYLIB_FLAG_INTERLACED_HINT)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowState() - RAYLIB_FLAG_INTERLACED_HINT is not supported on PLATFORM_DESKTOP_RGFW");
    }
}

// Clear window configuration state flags
void RaylibClearWindowState(unsigned int flags)
{
    CORE.Window.flags &= ~flags;

    if (flags & RAYLIB_FLAG_VSYNC_HINT)
    {
        RGFW_window_swapInterval(platform.window, 0);
    }
    if (flags & RAYLIB_FLAG_FULLSCREEN_MODE)
    {
        RaylibToggleBorderlessWindowed();
        RGFW_window_restore(platform.window);
        CORE.Window.fullscreen = false;
    }
    if (flags & RAYLIB_FLAG_WINDOW_RESIZABLE)
    {
        RGFW_window_setMaxSize(platform.window, RGFW_AREA(0, 0));
        RGFW_window_setMinSize(platform.window, RGFW_AREA(0, 0));
    }
    if (flags & RAYLIB_FLAG_WINDOW_UNDECORATED)
    {
        RaylibToggleBorderlessWindowed();
    }
    if (flags & RAYLIB_FLAG_WINDOW_HIDDEN)
    {
        RGFW_window_show(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_MINIMIZED)
    {
        RGFW_window_restore(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_MAXIMIZED)
    {
        RGFW_window_restore(platform.window);
    }
    if (flags & RAYLIB_FLAG_WINDOW_UNFOCUSED)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_UNFOCUSED is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_TOPMOST)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_TOPMOST is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_ALWAYS_RUN)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_ALWAYS_RUN is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_TRANSPARENT)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_TRANSPARENT is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_HIGHDPI)
    {
        // NOTE: There also doesn't seem to be a feature to disable high DPI once enabled
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_HIGHDPI is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH)
    {
        RGFW_window_setMousePassthrough(platform.window, flags & RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH);
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH is not supported on PLATFORM_DESKTOP_RGFW");
    }
    if (flags & RAYLIB_FLAG_BORDERLESS_WINDOWED_MODE)
    {
        RaylibToggleFullscreen();
    }
    if (flags & RAYLIB_FLAG_MSAA_4X_HINT)
    {
        RGFW_setGLSamples(0);
    }
    if (flags & RAYLIB_FLAG_INTERLACED_HINT)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibClearWindowState() - RAYLIB_FLAG_INTERLACED_HINT is not supported on PLATFORM_DESKTOP_RGFW");
    }
}

// Set icon for window
void RaylibSetWindowIcon(RaylibImage image)
{
    i32 channels = 4;

    switch (image.format)
    {
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16:           // 16 bpp (1 channel - half float)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32:           // 32 bpp (1 channel - float)
        {
            channels = 1;
        } break;
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:    // 8*2 bpp (2 channels)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R5G6B5:        // 16 bpp
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R8G8B8:        // 24 bpp
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:      // 16 bpp (1 bit alpha)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:      // 16 bpp (4 bit alpha)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:      // 32 bpp
        {
            channels = 2;
        } break;
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32G32B32:     // 32*3 bpp (3 channels - float)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16G16B16:     // 16*3 bpp (3 channels - half float)
        case RAYLIB_PIXELFORMAT_COMPRESSED_DXT1_RGB:        // 4 bpp (no alpha)
        case RAYLIB_PIXELFORMAT_COMPRESSED_ETC1_RGB:        // 4 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_ETC2_RGB:        // 4 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_PVRT_RGB:        // 4 bpp
        {
            channels = 3;
        } break;
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:  // 32*4 bpp (4 channels - float)
        case RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:  // 16*4 bpp (4 channels - half float)
        case RAYLIB_PIXELFORMAT_COMPRESSED_DXT1_RGBA:       // 4 bpp (1 bit alpha)
        case RAYLIB_PIXELFORMAT_COMPRESSED_DXT3_RGBA:       // 8 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_DXT5_RGBA:       // 8 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA:   // 8 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_PVRT_RGBA:       // 4 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA:   // 8 bpp
        case RAYLIB_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA:   // 2 bpp
        {
            channels = 4;
        } break;
        default: break;
    }

    RGFW_window_setIcon(platform.window, image.data, RGFW_AREA(image.width, image.height), channels);
}

// Set icon for window
void RaylibSetWindowIcons(RaylibImage *images, int count)
{
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowIcons() not available on target platform");
}

// Set title for window
void RaylibSetWindowTitle(const char *title)
{
    RGFW_window_setName(platform.window, (char*)title);
    CORE.Window.title = title;
}

// Set window position on screen (windowed mode)
void RaylibSetWindowPosition(int x, int y)
{
    RGFW_window_move(platform.window, RGFW_POINT(x, y));
}

// Set monitor for the current window
void RaylibSetWindowMonitor(int monitor)
{
    RGFW_window_moveToMonitor(platform.window, RGFW_getMonitors()[monitor]);
}

// Set window minimum dimensions (RAYLIB_FLAG_WINDOW_RESIZABLE)
void RaylibSetWindowMinSize(int width, int height)
{
    RGFW_window_setMinSize(platform.window, RGFW_AREA(width, height));
    CORE.Window.screenMin.width = width;
    CORE.Window.screenMin.height = height;
}

// Set window maximum dimensions (RAYLIB_FLAG_WINDOW_RESIZABLE)
void RaylibSetWindowMaxSize(int width, int height)
{
    RGFW_window_setMaxSize(platform.window, RGFW_AREA(width, height));
    CORE.Window.screenMax.width = width;
    CORE.Window.screenMax.height = height;
}

// Set window dimensions
void RaylibSetWindowSize(int width, int height)
{
    CORE.Window.screen.width = width;
    CORE.Window.screen.height = height;

    RGFW_window_resize(platform.window, RGFW_AREA(width, height));
}

// Set window opacity, value opacity is between 0.0 and 1.0
void RaylibSetWindowOpacity(float opacity)
{
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetWindowOpacity() not available on target platform");
}

// Set window focused
void RaylibSetWindowFocused(void)
{
    RGFW_window_show(platform.window);
}

// Get native window handle
void *RaylibGetWindowHandle(void)
{
#ifdef RGFW_WEBASM
    return (void*)platform.window->src.ctx;
#else
    return (void*)platform.window->src.window;
#endif
}

// Get number of monitors
int RaylibGetMonitorCount(void)
{
    #define MAX_MONITORS_SUPPORTED 6

    int count = MAX_MONITORS_SUPPORTED;
    RGFW_monitor *mons = RGFW_getMonitors();

    for (int i = 0; i < 6; i++)
    {
        if (!mons[i].rect.x && !mons[i].rect.y && !mons[i].rect.w && mons[i].rect.h)
        {
            count = i;
            break;
        }
    }

    return count;
}

// Get number of monitors
int RaylibGetCurrentMonitor(void)
{
    RGFW_monitor *mons = RGFW_getMonitors();
    RGFW_monitor mon = RGFW_window_getMonitor(platform.window);

    for (int i = 0; i < 6; i++)
    {
        if ((mons[i].rect.x ==  mon.rect.x) && (mons[i].rect.y ==  mon.rect.y)) return i;
    }

    return 0;
}

// Get selected monitor position
RaylibVector2 RaylibGetMonitorPosition(int monitor)
{
    RGFW_monitor *mons = RGFW_getMonitors();

    return (RaylibVector2){mons[monitor].rect.x, mons[monitor].rect.y};
}

// Get selected monitor width (currently used by monitor)
int RaylibGetMonitorWidth(int monitor)
{
    RGFW_monitor *mons = RGFW_getMonitors();

    return mons[monitor].rect.w;
}

// Get selected monitor height (currently used by monitor)
int RaylibGetMonitorHeight(int monitor)
{
    RGFW_monitor *mons = RGFW_getMonitors();

    return mons[monitor].rect.h;
}

// Get selected monitor physical width in millimetres
int RaylibGetMonitorPhysicalWidth(int monitor)
{
    RGFW_monitor* mons = RGFW_getMonitors();

    return mons[monitor].physW;
}

// Get selected monitor physical height in millimetres
int RaylibGetMonitorPhysicalHeight(int monitor)
{
    RGFW_monitor *mons = RGFW_getMonitors();

    return mons[monitor].physH;
}

// Get selected monitor refresh rate
int RaylibGetMonitorRefreshRate(int monitor)
{
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibGetMonitorRefreshRate() not implemented on target platform");
    return 0;
}

// Get the human-readable, UTF-8 encoded name of the selected monitor
const char *RaylibGetMonitorName(int monitor)
{
    RGFW_monitor *mons = RGFW_getMonitors();

    return mons[monitor].name;
}

// Get window position XY on monitor
RaylibVector2 RaylibGetWindowPosition(void)
{
    return (RaylibVector2){ platform.window->r.x, platform.window->r.y };
}

// Get window scale DPI factor for current monitor
RaylibVector2 RaylibGetWindowScaleDPI(void)
{
    RGFW_monitor monitor = RGFW_window_getMonitor(platform.window);

    return (RaylibVector2){monitor.scaleX, monitor.scaleX};
}

// Set clipboard text content
void RaylibSetClipboardText(const char *text)
{
    RGFW_writeClipboard(text, strlen(text));
}

// Get clipboard text content
// NOTE: returned string is allocated and freed by GLFW
const char *RaylibGetClipboardText(void)
{
    return RGFW_readClipboard(NULL);
}


#if defined(RAYLIB_SUPPORT_CLIPBOARD_IMAGE)

#ifdef _WIN32
#   define WIN32_CLIPBOARD_IMPLEMENTATION
#   define WINUSER_ALREADY_INCLUDED
#   define WINBASE_ALREADY_INCLUDED
#   define WINGDI_ALREADY_INCLUDED
#   include "../external/win32_clipboard.h"
#endif

// Get clipboard image
RaylibImage RaylibGetClipboardImage(void)
{
    RaylibImage image = {0};
    unsigned long long int dataSize = 0;
    void* fileData = NULL;

#ifdef _WIN32
    int width, height;
    fileData  = (void*)Win32GetClipboardImageData(&width, &height, &dataSize);
#else
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "Clipboard image: PLATFORM_DESKTOP_RGFW doesn't implement `RaylibGetClipboardImage` for this OS");
#endif

    if (fileData == NULL)
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "Clipboard image: Couldn't get clipboard data.");
    }
    else
    {
        image = RaylibLoadImageFromMemory(".bmp", fileData, dataSize);
    }
    return image;
}
#endif // RAYLIB_SUPPORT_CLIPBOARD_IMAGE

// Show mouse cursor
void RaylibShowCursor(void)
{
    RGFW_window_showMouse(platform.window, true);
    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void RaylibHideCursor(void)
{
    RGFW_window_showMouse(platform.window, false);
    CORE.Input.Mouse.cursorHidden = true;
}

// Enables cursor (unlock cursor)
void RaylibEnableCursor(void)
{
    RGFW_disableCursor = false;
    RGFW_window_mouseUnhold(platform.window);

    // Set cursor position in the middle
    RaylibSetMousePosition(CORE.Window.screen.width/2, CORE.Window.screen.height/2);
    RGFW_window_showMouse(platform.window, true);
    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void RaylibDisableCursor(void)
{
    RGFW_disableCursor = true;

    RGFW_window_mouseHold(platform.window, RGFW_AREA(0, 0));

    RaylibHideCursor();
}

// Swap back buffer with front buffer (screen drawing)
void RaylibSwapScreenBuffer(void)
{
    RGFW_window_swapBuffers(platform.window);
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Misc
//----------------------------------------------------------------------------------

// Get elapsed time measure in seconds since InitTimer()
double RaylibGetTime(void)
{
    double time = 0.0;
    unsigned long long int nanoSeconds = RGFW_getTimeNS();
    time = (double)(nanoSeconds - CORE.Time.base)*1e-9;  // Elapsed time since InitTimer()

    return time;
}

// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// Ref: https://github.com/raysan5/raylib/issues/686
void RaylibOpenURL(const char *url)
{
    // Security check to (partially) avoid malicious code on target platform
    if (strchr(url, '\'') != NULL) RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "SYSTEM: Provided URL could be potentially malicious, avoid [\'] character");
    else
    {
        // TODO: Open URL implementation
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition: Inputs
//----------------------------------------------------------------------------------

// Set internal gamepad mappings
int RaylibSetGamepadMappings(const char *mappings)
{
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "RaylibSetGamepadMappings() not implemented on target platform");
    return 0;
}

// Set mouse position XY
void RaylibSetMousePosition(int x, int y)
{
    RGFW_window_moveMouse(platform.window, RGFW_POINT(x, y));
    CORE.Input.Mouse.currentPosition = (RaylibVector2){ (float)x, (float)y };
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

// Set mouse cursor
void RaylibSetMouseCursor(int cursor)
{
    RGFW_window_setMouseStandard(platform.window, cursor);
}

// Get physical key name.
const char *GetKeyName(int key)
{
    RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "GetKeyName() not implemented on target platform");
    return "";
}

static RaylibKeyboardKey ConvertScancodeToKey(u32 keycode);

// TODO: Review function to avoid duplicate with RSGL
char RSGL_keystrToChar(const char *str)
{
    if (str[1] == 0) return str[0];

    static const char *map[] = {
        "asciitilde", "`",
        "grave", "~",
        "exclam", "!",
        "at", "@",
        "numbersign", "#",
        "dollar", "$",
        "percent", "%%",
        "asciicircum", "^",
        "ampersand", "&",
        "asterisk", "*",
        "parenleft", "(",
        "parenright", ")",
        "underscore", "_",
        "minus", "-",
        "plus", "+",
        "equal", "=",
        "braceleft", "{",
        "bracketleft", "[",
        "bracketright", "]",
        "braceright", "}",
        "colon", ":",
        "semicolon", ";",
        "quotedbl", "\"",
        "apostrophe", "'",
        "bar", "|",
        "backslash", "\'",
        "less", "<",
        "comma", ",",
        "greater", ">",
        "period", ".",
        "question", "?",
        "slash", "/",
        "space", " ",
        "Return", "\n",
        "Enter", "\n",
        "enter", "\n",
    };

    for (unsigned char i = 0; i < (sizeof(map)/sizeof(char *)); i += 2)
    {
        if (strcmp(map[i], str) == 0) return *map[i + 1];
    }

    return '\0';
}

// Register all input events
void RaylibPollInputEvents(void)
{
#if defined(RAYLIB_SUPPORT_GESTURES_SYSTEM)
    // NOTE: Gestures update must be called every frame to reset gestures correctly
    // because ProcessGestureEvent() is just called on an event, not every frame
    UpdateGestures();
#endif

    // Reset keys/chars pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;
    CORE.Input.Keyboard.charPressedQueueCount = 0;

    // Reset mouse wheel
    CORE.Input.Mouse.currentWheelMove.x = 0;
    CORE.Input.Mouse.currentWheelMove.y = 0;

    // Register previous mouse position

    // Reset last gamepad button/axis registered state

    for (int i = 0; (i < 4) && (i < RAYLIB_MAX_GAMEPADS); i++)
    {
        // Check if gamepad is available
        if (CORE.Input.Gamepad.ready[i])
        {
            // Register previous gamepad button states
            for (int k = 0; k < RAYLIB_MAX_GAMEPAD_BUTTONS; k++)
            {
                CORE.Input.Gamepad.previousButtonState[i][k] = CORE.Input.Gamepad.currentButtonState[i][k];
            }
        }
    }

    // Register previous touch states
    for (int i = 0; i < RAYLIB_MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];

    // Map touch position to mouse position for convenience
    CORE.Input.Touch.position[0] = CORE.Input.Mouse.currentPosition;

    int touchAction = -1;       // 0-RAYLIB_TOUCH_ACTION_UP, 1-RAYLIB_TOUCH_ACTION_DOWN, 2-RAYLIB_TOUCH_ACTION_MOVE
    bool realTouch = false;     // Flag to differentiate real touch gestures from mouse ones

    // Register previous keys states
    // NOTE: Android supports up to 260 keys
    for (int i = 0; i < RAYLIB_MAX_KEYBOARD_KEYS; i++)
    {
        CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];
        CORE.Input.Keyboard.keyRepeatInFrame[i] = 0;
    }

    // Register previous mouse states
    for (int i = 0; i < RAYLIB_MAX_MOUSE_BUTTONS; i++) CORE.Input.Mouse.previousButtonState[i] = CORE.Input.Mouse.currentButtonState[i];

    // Poll input events for current platform
    //-----------------------------------------------------------------------------
    CORE.Window.resizedLastFrame = false;

    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
    #define RGFW_HOLD_MOUSE     (1L<<2)
    if (platform.window->_winArgs & RGFW_HOLD_MOUSE)
    {
        CORE.Input.Mouse.previousPosition = (RaylibVector2){ 0.0f, 0.0f };
        CORE.Input.Mouse.currentPosition = (RaylibVector2){ 0.0f, 0.0f };
    }
    else
    {
        CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
    }

    while (RGFW_window_checkEvent(platform.window))
    {
        if ((platform.window->event.type >= RGFW_jsButtonPressed) && (platform.window->event.type <= RGFW_jsAxisMove))
        {
            if (!CORE.Input.Gamepad.ready[platform.window->event.joystick])
            {
                CORE.Input.Gamepad.ready[platform.window->event.joystick] = true;
                CORE.Input.Gamepad.axisCount[platform.window->event.joystick] = platform.window->event.axisesCount;
                CORE.Input.Gamepad.name[platform.window->event.joystick][0] = '\0';
                CORE.Input.Gamepad.axisState[platform.window->event.joystick][RAYLIB_GAMEPAD_AXIS_LEFT_TRIGGER] = -1.0f;
                CORE.Input.Gamepad.axisState[platform.window->event.joystick][RAYLIB_GAMEPAD_AXIS_RIGHT_TRIGGER] = -1.0f;
            }
        }

        RGFW_Event *event = &platform.window->event;

        // All input events can be processed after polling
        switch (event->type)
        {
            case RGFW_quit: CORE.Window.shouldClose = true; break;
            case RGFW_dnd:      // Dropped file
            {
                for (int i = 0; i < event->droppedFilesCount; i++)
                {
                    if (CORE.Window.dropFileCount == 0)
                    {
                        // When a new file is dropped, we reserve a fixed number of slots for all possible dropped files
                        // at the moment we limit the number of drops at once to 1024 files but this behaviour should probably be reviewed
                        // TODO: Pointers should probably be reallocated for any new file added...
                        CORE.Window.dropFilepaths = (char **)RL_CALLOC(1024, sizeof(char *));

                        CORE.Window.dropFilepaths[CORE.Window.dropFileCount] = (char *)RL_CALLOC(RAYLIB_MAX_FILEPATH_LENGTH, sizeof(char));
                        strcpy(CORE.Window.dropFilepaths[CORE.Window.dropFileCount], event->droppedFiles[i]);

                        CORE.Window.dropFileCount++;
                    }
                    else if (CORE.Window.dropFileCount < 1024)
                    {
                        CORE.Window.dropFilepaths[CORE.Window.dropFileCount] = (char *)RL_CALLOC(RAYLIB_MAX_FILEPATH_LENGTH, sizeof(char));
                        strcpy(CORE.Window.dropFilepaths[CORE.Window.dropFileCount], event->droppedFiles[i]);

                        CORE.Window.dropFileCount++;
                    }
                    else RAYLIB_TRACELOG(RAYLIB_LOG_WARNING, "FILE: Maximum drag and drop files at once is limited to 1024 files!");
                }
            } break;

            // Window events are also polled (Minimized, maximized, close...)
            case RGFW_windowResized:
            {
                SetupViewport(platform.window->r.w, platform.window->r.h);
                CORE.Window.screen.width = platform.window->r.w;
                CORE.Window.screen.height =  platform.window->r.h;
                CORE.Window.currentFbo.width = platform.window->r.w;
                CORE.Window.currentFbo.height = platform.window->r.h;
                CORE.Window.resizedLastFrame = true;
            } break;
            case RGFW_windowMoved:
            {
                CORE.Window.position.x = platform.window->r.x;
                CORE.Window.position.y = platform.window->r.x;
            } break;

            // Keyboard events
            case RGFW_keyPressed:
            {
                RaylibKeyboardKey key = ConvertScancodeToKey(event->keyCode);

                if (key != RAYLIB_KEY_NULL)
                {
                    // If key was up, add it to the key pressed queue
                    if ((CORE.Input.Keyboard.currentKeyState[key] == 0) && (CORE.Input.Keyboard.keyPressedQueueCount < RAYLIB_MAX_KEY_PRESSED_QUEUE))
                    {
                        CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = key;
                        CORE.Input.Keyboard.keyPressedQueueCount++;
                    }

                    CORE.Input.Keyboard.currentKeyState[key] = 1;
                }

                // TODO: Put exitKey verification outside the switch?
                if (CORE.Input.Keyboard.currentKeyState[CORE.Input.Keyboard.exitKey])
                {
                    CORE.Window.shouldClose = true;
                }

                // NOTE: event.text.text data comes an UTF-8 text sequence but we register codepoints (int)
                // Check if there is space available in the queue
                if (CORE.Input.Keyboard.charPressedQueueCount < RAYLIB_MAX_CHAR_PRESSED_QUEUE)
                {
                    // Add character (codepoint) to the queue
                    CORE.Input.Keyboard.charPressedQueue[CORE.Input.Keyboard.charPressedQueueCount] = RSGL_keystrToChar(event->keyName);
                    CORE.Input.Keyboard.charPressedQueueCount++;
                }
            } break;
            case RGFW_keyReleased:
            {
                RaylibKeyboardKey key = ConvertScancodeToKey(event->keyCode);
                if (key != RAYLIB_KEY_NULL) CORE.Input.Keyboard.currentKeyState[key] = 0;
            } break;

            // Check mouse events
            case RGFW_mouseButtonPressed:
            {
                if ((event->button == RGFW_mouseScrollUp) || (event->button == RGFW_mouseScrollDown))
                {
                    CORE.Input.Mouse.currentWheelMove.y = event->scroll;
                    break;
                }

                int btn = event->button;
                if (btn == RGFW_mouseLeft) btn = 1;
                else if (btn == RGFW_mouseRight) btn = 2;
                else if (btn == RGFW_mouseMiddle) btn = 3;

                CORE.Input.Mouse.currentButtonState[btn - 1] = 1;
                CORE.Input.Touch.currentTouchState[btn - 1] = 1;

                touchAction = 1;
            } break;
            case RGFW_mouseButtonReleased:
            {

                if ((event->button == RGFW_mouseScrollUp) || (event->button == RGFW_mouseScrollDown))
                {
                    CORE.Input.Mouse.currentWheelMove.y = event->scroll;
                    break;
                }

                int btn = event->button;
                if (btn == RGFW_mouseLeft) btn = 1;
                else if (btn == RGFW_mouseRight) btn = 2;
                else if (btn == RGFW_mouseMiddle) btn = 3;

                CORE.Input.Mouse.currentButtonState[btn - 1] = 0;
                CORE.Input.Touch.currentTouchState[btn - 1] = 0;

                touchAction = 0;
            } break;
            case RGFW_mousePosChanged:
            {
                if (platform.window->_winArgs & RGFW_HOLD_MOUSE)
                {
                    CORE.Input.Mouse.currentPosition.x += (float)event->point.x;
                    CORE.Input.Mouse.currentPosition.y += (float)event->point.y;
                }
                else
                {
                    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
                    CORE.Input.Mouse.currentPosition.x = (float)event->point.x;
                    CORE.Input.Mouse.currentPosition.y = (float)event->point.y;
                }

                CORE.Input.Touch.position[0] = CORE.Input.Mouse.currentPosition;
                touchAction = 2;
            } break;
            case RGFW_jsButtonPressed:
            {
                int button = -1;

                switch (event->button)
                {
                    case RGFW_JS_Y: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
                    case RGFW_JS_B: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
                    case RGFW_JS_A: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
                    case RGFW_JS_X: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;

                    case RGFW_JS_L1: button = RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
                    case RGFW_JS_R1: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;

                    case RGFW_JS_L2: button = RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_2; break;
                    case RGFW_JS_R2: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_2; break;

                    case RGFW_JS_SELECT: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE_LEFT; break;
                    case RGFW_JS_HOME: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE; break;
                    case RGFW_JS_START: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE_RIGHT; break;

                    case RGFW_JS_UP: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_UP; break;
                    case RGFW_JS_RIGHT: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
                    case RGFW_JS_DOWN: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
                    case RGFW_JS_LEFT: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;

                    default: break;
                }

                if (button >= 0)
                {
                    CORE.Input.Gamepad.currentButtonState[event->joystick][button] = 1;
                    CORE.Input.Gamepad.lastButtonPressed = button;
                }
            } break;
            case RGFW_jsButtonReleased:
            {
                int button = -1;
                switch (event->button)
                {
                    case RGFW_JS_Y: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
                    case RGFW_JS_B: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
                    case RGFW_JS_A: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
                    case RGFW_JS_X: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;

                    case RGFW_JS_L1: button = RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
                    case RGFW_JS_R1: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;

                    case RGFW_JS_L2: button = RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_2; break;
                    case RGFW_JS_R2: button = RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_2; break;

                    case RGFW_JS_SELECT: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE_LEFT; break;
                    case RGFW_JS_HOME: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE; break;
                    case RGFW_JS_START: button = RAYLIB_GAMEPAD_BUTTON_MIDDLE_RIGHT; break;

                    case RGFW_JS_UP: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_UP; break;
                    case RGFW_JS_RIGHT: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
                    case RGFW_JS_DOWN: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
                    case RGFW_JS_LEFT: button = RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;
                    default: break;
                }

                if (button >= 0)
                {
                    CORE.Input.Gamepad.currentButtonState[event->joystick][button] = 0;
                    if (CORE.Input.Gamepad.lastButtonPressed == button) CORE.Input.Gamepad.lastButtonPressed = 0;
                }
            } break;
            case RGFW_jsAxisMove:
            {
                int axis = -1;
                for (int i = 0; i < event->axisesCount; i++)
                {
                    switch(i)
                    {
                        case 0:
                        {
                            if (abs(event->axis[i].x) > abs(event->axis[i].y))
                            {
                                axis = RAYLIB_GAMEPAD_AXIS_LEFT_X;
                                break;
                            }

                            axis = RAYLIB_GAMEPAD_AXIS_LEFT_Y;
                        } break;
                        case 1:
                        {
                            if (abs(event->axis[i].x) > abs(event->axis[i].y))
                            {
                                axis = RAYLIB_GAMEPAD_AXIS_RIGHT_X;
                                break;
                            }

                            axis = RAYLIB_GAMEPAD_AXIS_RIGHT_Y;
                        } break;
                        case 2: axis = RAYLIB_GAMEPAD_AXIS_LEFT_TRIGGER; break;
                        case 3: axis = RAYLIB_GAMEPAD_AXIS_RIGHT_TRIGGER; break;
                        default: break;
                    }

                    #ifdef __linux__
                    float value = (event->axis[i].x + event->axis[i].y)/(float)32767;
                    #else
                    float value = (event->axis[i].x + -event->axis[i].y)/(float)32767;
                    #endif
                    CORE.Input.Gamepad.axisState[event->joystick][axis] = value;

                    // Register button state for triggers in addition to their axes
                    if ((axis == RAYLIB_GAMEPAD_AXIS_LEFT_TRIGGER) || (axis == RAYLIB_GAMEPAD_AXIS_RIGHT_TRIGGER))
                    {
                        int button = (axis == RAYLIB_GAMEPAD_AXIS_LEFT_TRIGGER)? RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_2 : RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_2;
                        int pressed = (value > 0.1f);
                        CORE.Input.Gamepad.currentButtonState[event->joystick][button] = pressed;
                        
                        if (pressed) CORE.Input.Gamepad.lastButtonPressed = button;
                        else if (CORE.Input.Gamepad.lastButtonPressed == button) CORE.Input.Gamepad.lastButtonPressed = 0;
                    }
                }
            } break;
            default: break;
        }

#if defined(RAYLIB_SUPPORT_GESTURES_SYSTEM)
        if (touchAction > -1)
        {
            // Process mouse events as touches to be able to use mouse-gestures
            GestureEvent gestureEvent = { 0 };

            // Register touch actions
            gestureEvent.touchAction = touchAction;

            // Assign a pointer ID
            gestureEvent.pointId[0] = 0;

            // Register touch points count
            gestureEvent.pointCount = 1;

            // Register touch points position, only one point registered
            if (touchAction == 2 || realTouch) gestureEvent.position[0] = CORE.Input.Touch.position[0];
            else gestureEvent.position[0] = RaylibGetMousePosition();

            // RaylibNormalize gestureEvent.position[0] for CORE.Window.screen.width and CORE.Window.screen.height
            gestureEvent.position[0].x /= (float)RaylibGetScreenWidth();
            gestureEvent.position[0].y /= (float)RaylibGetScreenHeight();

            // RaylibGesture data is sent to gestures-system for processing
            ProcessGestureEvent(gestureEvent);

            touchAction = -1;
        }
#endif
    }
    //-----------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------

// Initialize platform: graphics, inputs and more
int InitPlatform(void)
{
    // Initialize RGFW internal global state, only required systems
    unsigned int flags = RGFW_CENTER | RGFW_ALLOW_DND;

    // Check window creation flags
    if ((CORE.Window.flags & RAYLIB_FLAG_FULLSCREEN_MODE) > 0)
    {
        CORE.Window.fullscreen = true;
        flags |= RGFW_FULLSCREEN;
    }

    if ((CORE.Window.flags & RAYLIB_FLAG_WINDOW_UNDECORATED) > 0) flags |= RGFW_NO_BORDER;
    if ((CORE.Window.flags & RAYLIB_FLAG_WINDOW_RESIZABLE) == 0) flags |= RGFW_NO_RESIZE;

    if ((CORE.Window.flags & RAYLIB_FLAG_WINDOW_TRANSPARENT) > 0) flags |= RGFW_TRANSPARENT_WINDOW;

    if ((CORE.Window.flags & RAYLIB_FLAG_FULLSCREEN_MODE) > 0) flags |= RGFW_FULLSCREEN;

    // NOTE: Some OpenGL context attributes must be set before window creation

    // Check selection OpenGL version
    if (rlGetVersion() == RL_OPENGL_21)
    {
        RGFW_setGLVersion(RGFW_GL_CORE, 2, 1);
    }
    else if (rlGetVersion() == RL_OPENGL_33)
    {
        RGFW_setGLVersion(RGFW_GL_CORE, 3, 3);
    }
    else if (rlGetVersion() == RL_OPENGL_43)
    {
        RGFW_setGLVersion(RGFW_GL_CORE, 4, 1);
    }

    if (CORE.Window.flags & RAYLIB_FLAG_MSAA_4X_HINT)
    {
        RGFW_setGLSamples(4);
    }

    platform.window = RGFW_createWindow(CORE.Window.title, RGFW_RECT(0, 0, CORE.Window.screen.width, CORE.Window.screen.height), flags);

    RGFW_area screenSize = RGFW_getScreenSize();
    CORE.Window.display.width = screenSize.w;
    CORE.Window.display.height = screenSize.h;
    /* 
        I think this is needed by Raylib now ? 
        If so, rcore_destkop_sdl should be updated too
    */
    SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

    if (CORE.Window.flags & RAYLIB_FLAG_VSYNC_HINT) RGFW_window_swapInterval(platform.window, 1);

    RGFW_window_makeCurrent(platform.window);

    // Check surface and context activation
    if (platform.window != NULL)
    {
        CORE.Window.ready = true;

        CORE.Window.render.width = CORE.Window.screen.width;
        CORE.Window.render.height = CORE.Window.screen.height;
        CORE.Window.currentFbo.width = CORE.Window.render.width;
        CORE.Window.currentFbo.height = CORE.Window.render.height;

        RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "DISPLAY: Device initialized successfully");
        RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
        RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
        RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
        RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);
    }
    else
    {
        RAYLIB_TRACELOG(RAYLIB_LOG_FATAL, "PLATFORM: Failed to initialize graphics device");
        return -1;
    }
    //----------------------------------------------------------------------------

    // If everything work as expected, we can continue
    CORE.Window.position.x = platform.window->r.x;
    CORE.Window.position.y = platform.window->r.y;
    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.currentFbo.width = CORE.Window.render.width;
    CORE.Window.currentFbo.height = CORE.Window.render.height;

    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "DISPLAY: Device initialized successfully");
    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);

    // Load OpenGL extensions
    // NOTE: GL procedures address loader is required to load extensions
    //----------------------------------------------------------------------------
    rlLoadExtensions((void*)RGFW_getProcAddress);
    //----------------------------------------------------------------------------

    // TODO: Initialize input events system
    // It could imply keyboard, mouse, gamepad, touch...
    // Depending on the platform libraries/SDK it could use a callback mechanism
    // For system events and inputs evens polling on a per-frame basis, use RaylibPollInputEvents()
    //----------------------------------------------------------------------------
    // ...
    //----------------------------------------------------------------------------

    // Initialize timing system
    //----------------------------------------------------------------------------
    InitTimer();
    //----------------------------------------------------------------------------

    // Initialize storage system
    //----------------------------------------------------------------------------
    CORE.Storage.basePath = RaylibGetWorkingDirectory();
    //----------------------------------------------------------------------------

#ifdef RGFW_X11
    for (int i = 0; (i < 4) && (i < RAYLIB_MAX_GAMEPADS); i++)
    {
        RGFW_registerJoystick(platform.window, i);
    }
#endif

    RAYLIB_TRACELOG(RAYLIB_LOG_INFO, "PLATFORM: CUSTOM: Initialized successfully");

    return 0;
}

// Close platform
void ClosePlatform(void)
{
    RGFW_window_close(platform.window);
}

// Keycode mapping
static RaylibKeyboardKey ConvertScancodeToKey(u32 keycode)
{
    if (keycode > sizeof(keyMappingRGFW)/sizeof(unsigned short)) return 0;

    return keyMappingRGFW[keycode];
}
