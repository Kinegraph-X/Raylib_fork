/*******************************************************************************************
*
*   rcamera - Basic camera system with support for multiple camera modes
*
*   CONFIGURATION:
*       #define RCAMERA_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RCAMERA_STANDALONE
*           If defined, the library can be used as standalone as a camera system but some
*           functions must be redefined to manage inputs accordingly.
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Supervision, review, update and maintenance
*       Christoph Wagner:   Complete redesign, using raymath (2022)
*       Marc Palau:         Initial implementation (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2022-2024 Christoph Wagner (@Crydsch) & Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_RCAMERA_H
#define RAYLIB_RCAMERA_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Function specifiers definition

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
#if defined(BUILD_LIBTYPE_SHARED)
#if defined(__TINYC__)
#define __declspec(x) __attribute__((x))
#endif
#define RAYLIB_RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
#define RAYLIB_RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#endif
#endif

#ifndef RAYLIB_RLAPI
    #define RAYLIB_RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#if defined(RCAMERA_STANDALONE)
    #define RAYLIB_CAMERA_CULL_DISTANCE_NEAR      0.01
    #define RAYLIB_CAMERA_CULL_DISTANCE_FAR    1000.0
#else
    #define RAYLIB_CAMERA_CULL_DISTANCE_NEAR   RL_CULL_DISTANCE_NEAR
    #define RAYLIB_CAMERA_CULL_DISTANCE_FAR    RL_CULL_DISTANCE_FAR
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Below types are required for standalone usage
//----------------------------------------------------------------------------------
#if defined(RCAMERA_STANDALONE)
    // RaylibVector2, 2 components
    typedef struct RaylibVector2 {
        float x;                // Vector x component
        float y;                // Vector y component
    } RaylibVector2;

    // RaylibVector3, 3 components
    typedef struct RaylibVector3 {
        float x;                // Vector x component
        float y;                // Vector y component
        float z;                // Vector z component
    } RaylibVector3;

    // RaylibMatrix, 4x4 components, column major, OpenGL style, right-handed
    typedef struct RaylibMatrix {
        float m0, m4, m8, m12;  // RaylibMatrix first row (4 components)
        float m1, m5, m9, m13;  // RaylibMatrix second row (4 components)
        float m2, m6, m10, m14; // RaylibMatrix third row (4 components)
        float m3, m7, m11, m15; // RaylibMatrix fourth row (4 components)
    } RaylibMatrix;

    // Camera type, defines a camera position/orientation in 3d space
    typedef struct RaylibCamera3D {
        RaylibVector3 position;       // Camera position
        RaylibVector3 target;         // Camera target it looks-at
        RaylibVector3 up;             // Camera up vector (rotation over its axis)
        float fovy;             // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
        int projection;         // Camera projection type: RAYLIB_CAMERA_PERSPECTIVE or RAYLIB_CAMERA_ORTHOGRAPHIC
    } RaylibCamera3D;

    typedef RaylibCamera3D Camera;    // Camera type fallback, defaults to RaylibCamera3D

    // Camera projection
    typedef enum {
        RAYLIB_CAMERA_PERSPECTIVE = 0, // Perspective projection
        RAYLIB_CAMERA_ORTHOGRAPHIC     // Orthographic projection
    } RaylibCameraProjection;

    // Camera system modes
    typedef enum {
        RAYLIB_CAMERA_CUSTOM = 0,      // Camera custom, controlled by user (RaylibUpdateCamera() does nothing)
        RAYLIB_CAMERA_FREE,            // Camera free mode
        RAYLIB_CAMERA_ORBITAL,         // Camera orbital, around target, zoom supported
        RAYLIB_CAMERA_FIRST_PERSON,    // Camera first person
        RAYLIB_CAMERA_THIRD_PERSON     // Camera third person
    } RaylibCameraMode;
#endif

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

RAYLIB_RLAPI RaylibVector3 RaylibGetCameraForward(Camera *camera);
RAYLIB_RLAPI RaylibVector3 RaylibGetCameraUp(Camera *camera);
RAYLIB_RLAPI RaylibVector3 RaylibGetCameraRight(Camera *camera);

// Camera movement
RAYLIB_RLAPI void RaylibCameraMoveForward(Camera *camera, float distance, bool moveInWorldPlane);
RAYLIB_RLAPI void RaylibCameraMoveUp(Camera *camera, float distance);
RAYLIB_RLAPI void RaylibCameraMoveRight(Camera *camera, float distance, bool moveInWorldPlane);
RAYLIB_RLAPI void RaylibCameraMoveToTarget(Camera *camera, float delta);

// Camera rotation
RAYLIB_RLAPI void RaylibCameraYaw(Camera *camera, float angle, bool rotateAroundTarget);
RAYLIB_RLAPI void RaylibCameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
RAYLIB_RLAPI void RaylibCameraRoll(Camera *camera, float angle);

RAYLIB_RLAPI RaylibMatrix RaylibGetCameraViewMatrix(Camera *camera);
RAYLIB_RLAPI RaylibMatrix RaylibGetCameraProjectionMatrix(Camera* camera, float aspect);

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_RCAMERA_H

/***********************************************************************************
*
*   CAMERA IMPLEMENTATION
*
************************************************************************************/

#if defined(RCAMERA_IMPLEMENTATION)

#include "raymath.h"        // Required for vector maths:
                            // RaylibVector3Add()
                            // RaylibVector3Subtract()
                            // RaylibVector3Scale()
                            // RaylibVector3Normalize()
                            // RaylibVector3Distance()
                            // RaylibVector3CrossProduct()
                            // RaylibVector3RotateByAxisAngle()
                            // RaylibVector3Angle()
                            // RaylibVector3Negate()
                            // RaylibMatrixLookAt()
                            // RaylibMatrixPerspective()
                            // RaylibMatrixOrtho()
                            // RaylibMatrixIdentity()

// raylib required functionality:
                            // RaylibGetMouseDelta()
                            // RaylibGetMouseWheelMove()
                            // RaylibIsKeyDown()
                            // RaylibIsKeyPressed()
                            // RaylibGetFrameTime()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define RAYLIB_CAMERA_MOVE_SPEED                               5.4f       // Units per second
#define RAYLIB_CAMERA_ROTATION_SPEED                           0.03f
#define RAYLIB_CAMERA_PAN_SPEED                                0.2f

// Camera mouse movement sensitivity
#define RAYLIB_CAMERA_MOUSE_MOVE_SENSITIVITY                   0.003f

// Camera orbital speed in RAYLIB_CAMERA_ORBITAL mode
#define RAYLIB_CAMERA_ORBITAL_SPEED                            0.5f       // Radians per second

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Returns the cameras forward vector (normalized)
RaylibVector3 RaylibGetCameraForward(Camera *camera)
{
    return RaylibVector3Normalize(RaylibVector3Subtract(camera->target, camera->position));
}

// Returns the cameras up vector (normalized)
// Note: The up vector might not be perpendicular to the forward vector
RaylibVector3 RaylibGetCameraUp(Camera *camera)
{
    return RaylibVector3Normalize(camera->up);
}

// Returns the cameras right vector (normalized)
RaylibVector3 RaylibGetCameraRight(Camera *camera)
{
    RaylibVector3 forward = RaylibGetCameraForward(camera);
    RaylibVector3 up = RaylibGetCameraUp(camera);

    return RaylibVector3Normalize(RaylibVector3CrossProduct(forward, up));
}

// Moves the camera in its forward direction
void RaylibCameraMoveForward(Camera *camera, float distance, bool moveInWorldPlane)
{
    RaylibVector3 forward = RaylibGetCameraForward(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        forward.y = 0;
        forward = RaylibVector3Normalize(forward);
    }

    // Scale by distance
    forward = RaylibVector3Scale(forward, distance);

    // Move position and target
    camera->position = RaylibVector3Add(camera->position, forward);
    camera->target = RaylibVector3Add(camera->target, forward);
}

// Moves the camera in its up direction
void RaylibCameraMoveUp(Camera *camera, float distance)
{
    RaylibVector3 up = RaylibGetCameraUp(camera);

    // Scale by distance
    up = RaylibVector3Scale(up, distance);

    // Move position and target
    camera->position = RaylibVector3Add(camera->position, up);
    camera->target = RaylibVector3Add(camera->target, up);
}

// Moves the camera target in its current right direction
void RaylibCameraMoveRight(Camera *camera, float distance, bool moveInWorldPlane)
{
    RaylibVector3 right = RaylibGetCameraRight(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        right.y = 0;
        right = RaylibVector3Normalize(right);
    }

    // Scale by distance
    right = RaylibVector3Scale(right, distance);

    // Move position and target
    camera->position = RaylibVector3Add(camera->position, right);
    camera->target = RaylibVector3Add(camera->target, right);
}

// Moves the camera position closer/farther to/from the camera target
void RaylibCameraMoveToTarget(Camera *camera, float delta)
{
    float distance = RaylibVector3Distance(camera->position, camera->target);

    // Apply delta
    distance += delta;

    // Distance must be greater than 0
    if (distance <= 0) distance = 0.001f;

    // Set new distance by moving the position along the forward vector
    RaylibVector3 forward = RaylibGetCameraForward(camera);
    camera->position = RaylibVector3Add(camera->target, RaylibVector3Scale(forward, -distance));
}

// Rotates the camera around its up vector
// Yaw is "looking left and right"
// If rotateAroundTarget is false, the camera rotates around its position
// Note: angle must be provided in radians
void RaylibCameraYaw(Camera *camera, float angle, bool rotateAroundTarget)
{
    // Rotation axis
    RaylibVector3 up = RaylibGetCameraUp(camera);

    // View vector
    RaylibVector3 targetPosition = RaylibVector3Subtract(camera->target, camera->position);

    // Rotate view vector around up axis
    targetPosition = RaylibVector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = RaylibVector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = RaylibVector3Add(camera->position, targetPosition);
    }
}

// Rotates the camera around its right vector, pitch is "looking up and down"
//  - lockView prevents camera overrotation (aka "somersaults")
//  - rotateAroundTarget defines if rotation is around target or around its position
//  - rotateUp rotates the up direction as well (typically only usefull in RAYLIB_CAMERA_FREE)
// NOTE: angle must be provided in radians
void RaylibCameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp)
{
    // Up direction
    RaylibVector3 up = RaylibGetCameraUp(camera);

    // View vector
    RaylibVector3 targetPosition = RaylibVector3Subtract(camera->target, camera->position);

    if (lockView)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // RaylibClamp view up
        float maxAngleUp = RaylibVector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // RaylibClamp view down
        float maxAngleDown = RaylibVector3Angle(RaylibVector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    RaylibVector3 right = RaylibGetCameraRight(camera);

    // Rotate view vector around right axis
    targetPosition = RaylibVector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = RaylibVector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = RaylibVector3Add(camera->position, targetPosition);
    }

    if (rotateUp)
    {
        // Rotate up direction around right axis
        camera->up = RaylibVector3RotateByAxisAngle(camera->up, right, angle);
    }
}

// Rotates the camera around its forward vector
// Roll is "turning your head sideways to the left or right"
// Note: angle must be provided in radians
void RaylibCameraRoll(Camera *camera, float angle)
{
    // Rotation axis
    RaylibVector3 forward = RaylibGetCameraForward(camera);

    // Rotate up direction around forward axis
    camera->up = RaylibVector3RotateByAxisAngle(camera->up, forward, angle);
}

// Returns the camera view matrix
RaylibMatrix RaylibGetCameraViewMatrix(Camera *camera)
{
    return RaylibMatrixLookAt(camera->position, camera->target, camera->up);
}

// Returns the camera projection matrix
RaylibMatrix RaylibGetCameraProjectionMatrix(Camera *camera, float aspect)
{
    if (camera->projection == RAYLIB_CAMERA_PERSPECTIVE)
    {
        return RaylibMatrixPerspective(camera->fovy*RAYLIB_DEG2RAD, aspect, RAYLIB_CAMERA_CULL_DISTANCE_NEAR, RAYLIB_CAMERA_CULL_DISTANCE_FAR);
    }
    else if (camera->projection == RAYLIB_CAMERA_ORTHOGRAPHIC)
    {
        double top = camera->fovy/2.0;
        double right = top*aspect;

        return RaylibMatrixOrtho(-right, right, -top, top, RAYLIB_CAMERA_CULL_DISTANCE_NEAR, RAYLIB_CAMERA_CULL_DISTANCE_FAR);
    }

    return RaylibMatrixIdentity();
}

#if !defined(RCAMERA_STANDALONE)
// Update camera position for selected mode
// Camera mode: RAYLIB_CAMERA_FREE, RAYLIB_CAMERA_FIRST_PERSON, RAYLIB_CAMERA_THIRD_PERSON, RAYLIB_CAMERA_ORBITAL or CUSTOM
void RaylibUpdateCamera(Camera *camera, int mode)
{
    RaylibVector2 mousePositionDelta = RaylibGetMouseDelta();

    bool moveInWorldPlane = ((mode == RAYLIB_CAMERA_FIRST_PERSON) || (mode == RAYLIB_CAMERA_THIRD_PERSON));
    bool rotateAroundTarget = ((mode == RAYLIB_CAMERA_THIRD_PERSON) || (mode == RAYLIB_CAMERA_ORBITAL));
    bool lockView = ((mode == RAYLIB_CAMERA_FREE) || (mode == RAYLIB_CAMERA_FIRST_PERSON) || (mode == RAYLIB_CAMERA_THIRD_PERSON) || (mode == RAYLIB_CAMERA_ORBITAL));
    bool rotateUp = false;

    // Camera speeds based on frame time
    float cameraMoveSpeed = RAYLIB_CAMERA_MOVE_SPEED*RaylibGetFrameTime();
    float cameraRotationSpeed = RAYLIB_CAMERA_ROTATION_SPEED*RaylibGetFrameTime();
    float cameraPanSpeed = RAYLIB_CAMERA_PAN_SPEED*RaylibGetFrameTime();
    float cameraOrbitalSpeed = RAYLIB_CAMERA_ORBITAL_SPEED*RaylibGetFrameTime();

    if (mode == RAYLIB_CAMERA_CUSTOM) {}
    else if (mode == RAYLIB_CAMERA_ORBITAL)
    {
        // Orbital can just orbit
        RaylibMatrix rotation = RaylibMatrixRotate(RaylibGetCameraUp(camera), cameraOrbitalSpeed);
        RaylibVector3 view = RaylibVector3Subtract(camera->position, camera->target);
        view = RaylibVector3Transform(view, rotation);
        camera->position = RaylibVector3Add(camera->target, view);
    }
    else
    {
        // Camera rotation
        if (RaylibIsKeyDown(RAYLIB_KEY_DOWN)) RaylibCameraPitch(camera, -cameraRotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (RaylibIsKeyDown(RAYLIB_KEY_UP)) RaylibCameraPitch(camera, cameraRotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (RaylibIsKeyDown(RAYLIB_KEY_RIGHT)) RaylibCameraYaw(camera, -cameraRotationSpeed, rotateAroundTarget);
        if (RaylibIsKeyDown(RAYLIB_KEY_LEFT)) RaylibCameraYaw(camera, cameraRotationSpeed, rotateAroundTarget);
        if (RaylibIsKeyDown(RAYLIB_KEY_Q)) RaylibCameraRoll(camera, -cameraRotationSpeed);
        if (RaylibIsKeyDown(RAYLIB_KEY_E)) RaylibCameraRoll(camera, cameraRotationSpeed);

        // Camera movement
        // Camera pan (for RAYLIB_CAMERA_FREE)
        if ((mode == RAYLIB_CAMERA_FREE) && (RaylibIsMouseButtonDown(RAYLIB_MOUSE_BUTTON_MIDDLE)))
        {
            const RaylibVector2 mouseDelta = RaylibGetMouseDelta();
            if (mouseDelta.x > 0.0f) RaylibCameraMoveRight(camera, cameraPanSpeed, moveInWorldPlane);
            if (mouseDelta.x < 0.0f) RaylibCameraMoveRight(camera, -cameraPanSpeed, moveInWorldPlane);
            if (mouseDelta.y > 0.0f) RaylibCameraMoveUp(camera, -cameraPanSpeed);
            if (mouseDelta.y < 0.0f) RaylibCameraMoveUp(camera, cameraPanSpeed);
        }
        else
        {
            // Mouse support
            RaylibCameraYaw(camera, -mousePositionDelta.x*RAYLIB_CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            RaylibCameraPitch(camera, -mousePositionDelta.y*RAYLIB_CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);
        }

        // Keyboard support
        if (RaylibIsKeyDown(RAYLIB_KEY_W)) RaylibCameraMoveForward(camera, cameraMoveSpeed, moveInWorldPlane);
        if (RaylibIsKeyDown(RAYLIB_KEY_A)) RaylibCameraMoveRight(camera, -cameraMoveSpeed, moveInWorldPlane);
        if (RaylibIsKeyDown(RAYLIB_KEY_S)) RaylibCameraMoveForward(camera, -cameraMoveSpeed, moveInWorldPlane);
        if (RaylibIsKeyDown(RAYLIB_KEY_D)) RaylibCameraMoveRight(camera, cameraMoveSpeed, moveInWorldPlane);

        // Gamepad movement
        if (RaylibIsGamepadAvailable(0))
        {
            // Gamepad controller support
            RaylibCameraYaw(camera, -(RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_RIGHT_X)*2)*RAYLIB_CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            RaylibCameraPitch(camera, -(RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_RIGHT_Y)*2)*RAYLIB_CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

            if (RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_LEFT_Y) <= -0.25f) RaylibCameraMoveForward(camera, cameraMoveSpeed, moveInWorldPlane);
            if (RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_LEFT_X) <= -0.25f) RaylibCameraMoveRight(camera, -cameraMoveSpeed, moveInWorldPlane);
            if (RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_LEFT_Y) >= 0.25f) RaylibCameraMoveForward(camera, -cameraMoveSpeed, moveInWorldPlane);
            if (RaylibGetGamepadAxisMovement(0, RAYLIB_GAMEPAD_AXIS_LEFT_X) >= 0.25f) RaylibCameraMoveRight(camera, cameraMoveSpeed, moveInWorldPlane);
        }

        if (mode == RAYLIB_CAMERA_FREE)
        {
            if (RaylibIsKeyDown(RAYLIB_KEY_SPACE)) RaylibCameraMoveUp(camera, cameraMoveSpeed);
            if (RaylibIsKeyDown(RAYLIB_KEY_LEFT_CONTROL)) RaylibCameraMoveUp(camera, -cameraMoveSpeed);
        }
    }

    if ((mode == RAYLIB_CAMERA_THIRD_PERSON) || (mode == RAYLIB_CAMERA_ORBITAL) || (mode == RAYLIB_CAMERA_FREE))
    {
        // Zoom target distance
        RaylibCameraMoveToTarget(camera, -RaylibGetMouseWheelMove());
        if (RaylibIsKeyPressed(RAYLIB_KEY_KP_SUBTRACT)) RaylibCameraMoveToTarget(camera, 2.0f);
        if (RaylibIsKeyPressed(RAYLIB_KEY_KP_ADD)) RaylibCameraMoveToTarget(camera, -2.0f);
    }
}
#endif // !RCAMERA_STANDALONE

// Update camera movement, movement/rotation values should be provided by user
void RaylibUpdateCameraPro(Camera *camera, RaylibVector3 movement, RaylibVector3 rotation, float zoom)
{
    // Required values
    // movement.x - Move forward/backward
    // movement.y - Move right/left
    // movement.z - Move up/down
    // rotation.x - yaw
    // rotation.y - pitch
    // rotation.z - roll
    // zoom - Move towards target

    bool lockView = true;
    bool rotateAroundTarget = false;
    bool rotateUp = false;
    bool moveInWorldPlane = true;

    // Camera rotation
    RaylibCameraPitch(camera, -rotation.y*RAYLIB_DEG2RAD, lockView, rotateAroundTarget, rotateUp);
    RaylibCameraYaw(camera, -rotation.x*RAYLIB_DEG2RAD, rotateAroundTarget);
    RaylibCameraRoll(camera, rotation.z*RAYLIB_DEG2RAD);

    // Camera movement
    RaylibCameraMoveForward(camera, movement.x, moveInWorldPlane);
    RaylibCameraMoveRight(camera, movement.y, moveInWorldPlane);
    RaylibCameraMoveUp(camera, movement.z);

    // Zoom target distance
    RaylibCameraMoveToTarget(camera, zoom);
}

#endif // RCAMERA_IMPLEMENTATION
