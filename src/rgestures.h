/**********************************************************************************************
*
*   rgestures - Gestures system, gestures processing based on input events (touch/mouse)
*
*   CONFIGURATION:
*       #define RGESTURES_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RGESTURES_STANDALONE
*           If defined, the library can be used as standalone to process gesture events with
*           no external dependencies.
*
*   CONTRIBUTORS:
*       Marc Palau:         Initial implementation (2014)
*       Albert Martos:      Complete redesign and testing (2015)
*       Ian Eito:           Complete redesign and testing (2015)
*       Ramon Santamaria:   Supervision, review, update and maintenance
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_RGESTURES_H
#define RAYLIB_RGESTURES_H

#ifndef RAYLIB_PI
    #define RAYLIB_PI 3.14159265358979323846
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef RAYLIB_MAX_TOUCH_POINTS
    #define RAYLIB_MAX_TOUCH_POINTS        8        // Maximum number of touch points supported
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Below types are required for standalone usage
//----------------------------------------------------------------------------------
// Boolean type
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool) && !defined(RL_BOOL_TYPE)
    typedef enum bool { false = 0, true = !false } bool;
#endif

#if !defined(RL_VECTOR2_TYPE)
// RaylibVector2 type
typedef struct RaylibVector2 {
    float x;
    float y;
} RaylibVector2;
#endif

#if defined(RGESTURES_STANDALONE)
// Gestures type
// NOTE: It could be used as flags to enable only some gestures
typedef enum {
    RAYLIB_GESTURE_NONE        = 0,
    RAYLIB_GESTURE_TAP         = 1,
    RAYLIB_GESTURE_DOUBLETAP   = 2,
    RAYLIB_GESTURE_HOLD        = 4,
    RAYLIB_GESTURE_DRAG        = 8,
    RAYLIB_GESTURE_SWIPE_RIGHT = 16,
    RAYLIB_GESTURE_SWIPE_LEFT  = 32,
    RAYLIB_GESTURE_SWIPE_UP    = 64,
    RAYLIB_GESTURE_SWIPE_DOWN  = 128,
    RAYLIB_GESTURE_PINCH_IN    = 256,
    RAYLIB_GESTURE_PINCH_OUT   = 512
} RaylibGesture;
#endif

typedef enum {
    RAYLIB_TOUCH_ACTION_UP = 0,
    RAYLIB_TOUCH_ACTION_DOWN,
    RAYLIB_TOUCH_ACTION_MOVE,
    RAYLIB_TOUCH_ACTION_CANCEL
} RaylibTouchAction;

// RaylibGesture event
typedef struct {
    int touchAction;
    int pointCount;
    int pointId[RAYLIB_MAX_TOUCH_POINTS];
    RaylibVector2 position[RAYLIB_MAX_TOUCH_POINTS];
} GestureEvent;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

void ProcessGestureEvent(GestureEvent event);           // Process gesture event and translate it into gestures
void UpdateGestures(void);                              // Update gestures detected (must be called every frame)

#if defined(RGESTURES_STANDALONE)
void RaylibSetGesturesEnabled(unsigned int flags);            // Enable a set of gestures using flags
bool RaylibIsGestureDetected(int gesture);                    // Check if a gesture have been detected
int RaylibGetGestureDetected(void);                           // Get latest detected gesture

float RaylibGetGestureHoldDuration(void);                     // Get gesture hold time in seconds
RaylibVector2 RaylibGetGestureDragVector(void);                     // Get gesture drag vector
float RaylibGetGestureDragAngle(void);                        // Get gesture drag angle
RaylibVector2 RaylibGetGesturePinchVector(void);                    // Get gesture pinch delta
float RaylibGetGesturePinchAngle(void);                       // Get gesture pinch angle
#endif

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_RGESTURES_H

/***********************************************************************************
*
*   RGESTURES IMPLEMENTATION
*
************************************************************************************/

#if defined(RGESTURES_IMPLEMENTATION)

#if defined(RGESTURES_STANDALONE)
#if defined(_WIN32)
    #if defined(__cplusplus)
    extern "C" {        // Prevents name mangling of functions
    #endif
    // Functions required to query time on Windows
    int __stdcall QueryPerformanceCounter(unsigned long long int *lpPerformanceCount);
    int __stdcall QueryPerformanceFrequency(unsigned long long int *lpFrequency);
    #if defined(__cplusplus)
    }
    #endif
#elif defined(__linux__)
    #if _POSIX_C_SOURCE < 199309L
        #undef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L // Required for CLOCK_MONOTONIC if compiled with c99 without gnu ext.
    #endif
    #include <sys/time.h>               // Required for: timespec
    #include <time.h>                   // Required for: clock_gettime()

    #include <math.h>                   // Required for: sqrtf(), atan2f()
#endif
#if defined(__APPLE__)                  // macOS also defines __MACH__
    #include <mach/clock.h>             // Required for: clock_get_time()
    #include <mach/mach.h>              // Required for: mach_timespec_t
#endif
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define RAYLIB_FORCE_TO_SWIPE      0.2f        // Swipe force, measured in normalized screen units/time
#define RAYLIB_MINIMUM_DRAG        0.015f      // Drag minimum force, measured in normalized screen units (0.0f to 1.0f)
#define RAYLIB_DRAG_TIMEOUT        0.3f        // Drag minimum time for web, measured in seconds
#define RAYLIB_MINIMUM_PINCH       0.005f      // Pinch minimum force, measured in normalized screen units (0.0f to 1.0f)
#define RAYLIB_TAP_TIMEOUT         0.3f        // Tap minimum time, measured in seconds
#define RAYLIB_PINCH_TIMEOUT       0.3f        // Pinch minimum time, measured in seconds
#define RAYLIB_DOUBLETAP_RANGE     0.03f       // DoubleTap range, measured in normalized screen units (0.0f to 1.0f)

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Gestures module state context [136 bytes]
typedef struct {
    unsigned int current;               // Current detected gesture
    unsigned int enabledFlags;          // Enabled gestures flags
    struct {
        int firstId;                    // Touch id for first touch point
        int pointCount;                 // Touch points counter
        double eventTime;               // Time stamp when an event happened
        RaylibVector2 upPosition;             // Touch up position
        RaylibVector2 downPositionA;          // First touch down position
        RaylibVector2 downPositionB;          // Second touch down position
        RaylibVector2 downDragPosition;       // Touch drag position
        RaylibVector2 moveDownPositionA;      // First touch down position on move
        RaylibVector2 moveDownPositionB;      // Second touch down position on move
        RaylibVector2 previousPositionA;      // Previous position A to compare for pinch gestures
        RaylibVector2 previousPositionB;      // Previous position B to compare for pinch gestures
        int tapCounter;                 // TAP counter (one tap implies RAYLIB_TOUCH_ACTION_DOWN and RAYLIB_TOUCH_ACTION_UP actions)
    } Touch;
    struct {
        bool resetRequired;             // HOLD reset to get first touch point again
        double timeDuration;            // HOLD duration in seconds
    } Hold;
    struct {
        RaylibVector2 vector;                 // DRAG vector (between initial and current position)
        float angle;                    // DRAG angle (relative to x-axis)
        float distance;                 // DRAG distance (from initial touch point to final) (normalized [0..1])
        float intensity;                // DRAG intensity, how far why did the DRAG (pixels per frame)
    } Drag;
    struct {
        double startTime;               // SWIPE start time to calculate drag intensity
    } Swipe;
    struct {
        RaylibVector2 vector;                 // PINCH vector (between first and second touch points)
        float angle;                    // PINCH angle (relative to x-axis)
        float distance;                 // PINCH displacement distance (normalized [0..1])
    } Pinch;
} GesturesData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static GesturesData GESTURES = {
    .Touch.firstId = -1,
    .current = RAYLIB_GESTURE_NONE,        // No current gesture detected
    .enabledFlags = 0b0000001111111111  // All gestures supported by default
};

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
static float rgVector2Angle(RaylibVector2 initialPosition, RaylibVector2 finalPosition);
static float rgVector2Distance(RaylibVector2 v1, RaylibVector2 v2);
static double rgGetCurrentTime(void);

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Enable only desired gestures to be detected
void RaylibSetGesturesEnabled(unsigned int flags)
{
    GESTURES.enabledFlags = flags;
}

// Check if a gesture have been detected
bool RaylibIsGestureDetected(unsigned int gesture)
{
    if ((GESTURES.enabledFlags & GESTURES.current) == gesture) return true;
    else return false;
}

// Process gesture event and translate it into gestures
void ProcessGestureEvent(GestureEvent event)
{
    // Reset required variables
    GESTURES.Touch.pointCount = event.pointCount;      // Required on UpdateGestures()

    if (GESTURES.Touch.pointCount == 1)     // One touch point
    {
        if (event.touchAction == RAYLIB_TOUCH_ACTION_DOWN)
        {
            GESTURES.Touch.tapCounter++;    // Tap counter

            // Detect GESTURE_DOUBLE_TAP
            if ((GESTURES.current == RAYLIB_GESTURE_NONE) && (GESTURES.Touch.tapCounter >= 2) && ((rgGetCurrentTime() - GESTURES.Touch.eventTime) < RAYLIB_TAP_TIMEOUT) && (rgVector2Distance(GESTURES.Touch.downPositionA, event.position[0]) < RAYLIB_DOUBLETAP_RANGE))
            {
                GESTURES.current = RAYLIB_GESTURE_DOUBLETAP;
                GESTURES.Touch.tapCounter = 0;
            }
            else    // Detect RAYLIB_GESTURE_TAP
            {
                GESTURES.Touch.tapCounter = 1;
                GESTURES.current = RAYLIB_GESTURE_TAP;
            }

            GESTURES.Touch.downPositionA = event.position[0];
            GESTURES.Touch.downDragPosition = event.position[0];

            GESTURES.Touch.upPosition = GESTURES.Touch.downPositionA;
            GESTURES.Touch.eventTime = rgGetCurrentTime();

            GESTURES.Swipe.startTime = rgGetCurrentTime();

            GESTURES.Drag.vector = (RaylibVector2){ 0.0f, 0.0f };
        }
        else if (event.touchAction == RAYLIB_TOUCH_ACTION_UP)
        {
            // A swipe can happen while the current gesture is drag, but (specially for web) also hold, so set upPosition for both cases
            if (GESTURES.current == RAYLIB_GESTURE_DRAG || GESTURES.current == RAYLIB_GESTURE_HOLD) GESTURES.Touch.upPosition = event.position[0];

            // NOTE: GESTURES.Drag.intensity dependent on the resolution of the screen
            GESTURES.Drag.distance = rgVector2Distance(GESTURES.Touch.downPositionA, GESTURES.Touch.upPosition);
            GESTURES.Drag.intensity = GESTURES.Drag.distance/(float)((rgGetCurrentTime() - GESTURES.Swipe.startTime));

            // Detect GESTURE_SWIPE
            if ((GESTURES.Drag.intensity > RAYLIB_FORCE_TO_SWIPE) && (GESTURES.current != RAYLIB_GESTURE_DRAG))
            {
                // NOTE: Angle should be inverted in Y
                GESTURES.Drag.angle = 360.0f - rgVector2Angle(GESTURES.Touch.downPositionA, GESTURES.Touch.upPosition);

                if ((GESTURES.Drag.angle < 30) || (GESTURES.Drag.angle > 330)) GESTURES.current = RAYLIB_GESTURE_SWIPE_RIGHT;          // Right
                else if ((GESTURES.Drag.angle >= 30) && (GESTURES.Drag.angle <= 150)) GESTURES.current = RAYLIB_GESTURE_SWIPE_UP;      // Up
                else if ((GESTURES.Drag.angle > 150) && (GESTURES.Drag.angle < 210)) GESTURES.current = RAYLIB_GESTURE_SWIPE_LEFT;     // Left
                else if ((GESTURES.Drag.angle >= 210) && (GESTURES.Drag.angle <= 330)) GESTURES.current = RAYLIB_GESTURE_SWIPE_DOWN;   // Down
                else GESTURES.current = RAYLIB_GESTURE_NONE;
            }
            else
            {
                GESTURES.Drag.distance = 0.0f;
                GESTURES.Drag.intensity = 0.0f;
                GESTURES.Drag.angle = 0.0f;

                GESTURES.current = RAYLIB_GESTURE_NONE;
            }

            GESTURES.Touch.downDragPosition = (RaylibVector2){ 0.0f, 0.0f };
            GESTURES.Touch.pointCount = 0;
        }
        else if (event.touchAction == RAYLIB_TOUCH_ACTION_MOVE)
        {
            GESTURES.Touch.moveDownPositionA = event.position[0];

            if (GESTURES.current == RAYLIB_GESTURE_HOLD)
            {
                if (GESTURES.Hold.resetRequired) GESTURES.Touch.downPositionA = event.position[0];

                GESTURES.Hold.resetRequired = false;

                // Detect RAYLIB_GESTURE_DRAG
                if ((rgGetCurrentTime() - GESTURES.Touch.eventTime) > RAYLIB_DRAG_TIMEOUT)
                {
                    GESTURES.Touch.eventTime = rgGetCurrentTime();
                    GESTURES.current = RAYLIB_GESTURE_DRAG;
                }
            }

            GESTURES.Drag.vector.x = GESTURES.Touch.moveDownPositionA.x - GESTURES.Touch.downDragPosition.x;
            GESTURES.Drag.vector.y = GESTURES.Touch.moveDownPositionA.y - GESTURES.Touch.downDragPosition.y;
        }
    }
    else if (GESTURES.Touch.pointCount == 2)    // Two touch points
    {
        if (event.touchAction == RAYLIB_TOUCH_ACTION_DOWN)
        {
            GESTURES.Touch.downPositionA = event.position[0];
            GESTURES.Touch.downPositionB = event.position[1];

            GESTURES.Touch.previousPositionA = GESTURES.Touch.downPositionA;
            GESTURES.Touch.previousPositionB = GESTURES.Touch.downPositionB;

            //GESTURES.Pinch.distance = rgVector2Distance(GESTURES.Touch.downPositionA, GESTURES.Touch.downPositionB);

            GESTURES.Pinch.vector.x = GESTURES.Touch.downPositionB.x - GESTURES.Touch.downPositionA.x;
            GESTURES.Pinch.vector.y = GESTURES.Touch.downPositionB.y - GESTURES.Touch.downPositionA.y;

            GESTURES.current = RAYLIB_GESTURE_HOLD;
            GESTURES.Hold.timeDuration = rgGetCurrentTime();
        }
        else if (event.touchAction == RAYLIB_TOUCH_ACTION_MOVE)
        {
            GESTURES.Pinch.distance = rgVector2Distance(GESTURES.Touch.moveDownPositionA, GESTURES.Touch.moveDownPositionB);

            GESTURES.Touch.moveDownPositionA = event.position[0];
            GESTURES.Touch.moveDownPositionB = event.position[1];

            GESTURES.Pinch.vector.x = GESTURES.Touch.moveDownPositionB.x - GESTURES.Touch.moveDownPositionA.x;
            GESTURES.Pinch.vector.y = GESTURES.Touch.moveDownPositionB.y - GESTURES.Touch.moveDownPositionA.y;

            if ((rgVector2Distance(GESTURES.Touch.previousPositionA, GESTURES.Touch.moveDownPositionA) >= RAYLIB_MINIMUM_PINCH) || (rgVector2Distance(GESTURES.Touch.previousPositionB, GESTURES.Touch.moveDownPositionB) >= RAYLIB_MINIMUM_PINCH))
            {
                if ( rgVector2Distance(GESTURES.Touch.previousPositionA, GESTURES.Touch.previousPositionB) > rgVector2Distance(GESTURES.Touch.moveDownPositionA, GESTURES.Touch.moveDownPositionB) ) GESTURES.current = RAYLIB_GESTURE_PINCH_IN;
                else GESTURES.current = RAYLIB_GESTURE_PINCH_OUT;
            }
            else
            {
                GESTURES.current = RAYLIB_GESTURE_HOLD;
                GESTURES.Hold.timeDuration = rgGetCurrentTime();
            }

            // NOTE: Angle should be inverted in Y
            GESTURES.Pinch.angle = 360.0f - rgVector2Angle(GESTURES.Touch.moveDownPositionA, GESTURES.Touch.moveDownPositionB);
        }
        else if (event.touchAction == RAYLIB_TOUCH_ACTION_UP)
        {
            GESTURES.Pinch.distance = 0.0f;
            GESTURES.Pinch.angle = 0.0f;
            GESTURES.Pinch.vector = (RaylibVector2){ 0.0f, 0.0f };
            GESTURES.Touch.pointCount = 0;

            GESTURES.current = RAYLIB_GESTURE_NONE;
        }
    }
    else if (GESTURES.Touch.pointCount > 2)     // More than two touch points
    {
        // TODO: Process gesture events for more than two points
    }
}

// Update gestures detected (must be called every frame)
void UpdateGestures(void)
{
    // NOTE: Gestures are processed through system callbacks on touch events

    // Detect RAYLIB_GESTURE_HOLD
    if (((GESTURES.current == RAYLIB_GESTURE_TAP) || (GESTURES.current == RAYLIB_GESTURE_DOUBLETAP)) && (GESTURES.Touch.pointCount < 2))
    {
        GESTURES.current = RAYLIB_GESTURE_HOLD;
        GESTURES.Hold.timeDuration = rgGetCurrentTime();
    }

    // Detect RAYLIB_GESTURE_NONE
    if ((GESTURES.current == RAYLIB_GESTURE_SWIPE_RIGHT) || (GESTURES.current == RAYLIB_GESTURE_SWIPE_UP) || (GESTURES.current == RAYLIB_GESTURE_SWIPE_LEFT) || (GESTURES.current == RAYLIB_GESTURE_SWIPE_DOWN))
    {
        GESTURES.current = RAYLIB_GESTURE_NONE;
    }
}

// Get latest detected gesture
int RaylibGetGestureDetected(void)
{
    // Get current gesture only if enabled
    return (GESTURES.enabledFlags & GESTURES.current);
}

// Hold time measured in seconds
float RaylibGetGestureHoldDuration(void)
{
    // NOTE: time is calculated on current gesture HOLD

    double time = 0.0;

    if (GESTURES.current == RAYLIB_GESTURE_HOLD) time = rgGetCurrentTime() - GESTURES.Hold.timeDuration;

    return (float)time;
}

// Get drag vector (between initial touch point to current)
RaylibVector2 RaylibGetGestureDragVector(void)
{
    // NOTE: drag vector is calculated on one touch points RAYLIB_TOUCH_ACTION_MOVE

    return GESTURES.Drag.vector;
}

// Get drag angle
// NOTE: Angle in degrees, horizontal-right is 0, counterclockwise
float RaylibGetGestureDragAngle(void)
{
    // NOTE: drag angle is calculated on one touch points RAYLIB_TOUCH_ACTION_UP

    return GESTURES.Drag.angle;
}

// Get distance between two pinch points
RaylibVector2 RaylibGetGesturePinchVector(void)
{
    // NOTE: Pinch distance is calculated on two touch points RAYLIB_TOUCH_ACTION_MOVE

    return GESTURES.Pinch.vector;
}

// Get angle between two pinch points
// NOTE: Angle in degrees, horizontal-right is 0, counterclockwise
float RaylibGetGesturePinchAngle(void)
{
    // NOTE: pinch angle is calculated on two touch points RAYLIB_TOUCH_ACTION_MOVE

    return GESTURES.Pinch.angle;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Get angle from two-points vector with X-axis
static float rgVector2Angle(RaylibVector2 v1, RaylibVector2 v2)
{
    float angle = atan2f(v2.y - v1.y, v2.x - v1.x)*(180.0f/RAYLIB_PI);

    if (angle < 0) angle += 360.0f;

    return angle;
}

// Calculate distance between two RaylibVector2
static float rgVector2Distance(RaylibVector2 v1, RaylibVector2 v2)
{
    float result;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;

    result = (float)sqrt(dx*dx + dy*dy);

    return result;
}

// Time measure returned are seconds
static double rgGetCurrentTime(void)
{
    double time = 0;

#if !defined(RGESTURES_STANDALONE)
    time = RaylibGetTime();
#else
#if defined(_WIN32)
    unsigned long long int clockFrequency, currentTime;

    QueryPerformanceFrequency(&clockFrequency); // BE CAREFUL: Costly operation!
    QueryPerformanceCounter(&currentTime);

    time = (double)currentTime/clockFrequency;  // Time in seconds
#endif

#if defined(__linux__)
    // NOTE: Only for Linux-based systems
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    unsigned long long int nowTime = (unsigned long long int)now.tv_sec*1000000000LLU + (unsigned long long int)now.tv_nsec;     // Time in nanoseconds

    time = ((double)nowTime*1e-9);     // Time in seconds
#endif

#if defined(__APPLE__)
    //#define CLOCK_REALTIME  CALENDAR_CLOCK    // returns UTC time since 1970-01-01
    //#define CLOCK_MONOTONIC SYSTEM_CLOCK      // returns the time since boot time

    clock_serv_t cclock;
    mach_timespec_t now;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);

    // NOTE: OS X does not have clock_gettime(), using clock_get_time()
    clock_get_time(cclock, &now);
    mach_port_deallocate(mach_task_self(), cclock);
    unsigned long long int nowTime = (unsigned long long int)now.tv_sec*1000000000LLU + (unsigned long long int)now.tv_nsec;     // Time in nanoseconds

    time = ((double)nowTime*1e-9);     // Time in seconds
#endif
#endif

    return time;
}

#endif // RGESTURES_IMPLEMENTATION
