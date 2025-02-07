/**********************************************************************************************
*
*   raylib configuration flags
*
*   This file defines all the configuration flags for the different raylib modules
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2018-2024 Ahmad Fatoum & Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_CONFIG_H
#define RAYLIB_CONFIG_H

//------------------------------------------------------------------------------------
// Module selection - Some modules could be avoided
// Mandatory modules: rcore, rlgl, utils
//------------------------------------------------------------------------------------
#define RAYLIB_SUPPORT_MODULE_RSHAPES          1
#define RAYLIB_SUPPORT_MODULE_RTEXTURES        1
#define RAYLIB_SUPPORT_MODULE_RTEXT            1       // WARNING: It requires RAYLIB_SUPPORT_MODULE_RTEXTURES to load sprite font textures
#define RAYLIB_SUPPORT_MODULE_RMODELS          1
#define RAYLIB_SUPPORT_MODULE_RAUDIO           1

//------------------------------------------------------------------------------------
// Module: rcore - Configuration Flags
//------------------------------------------------------------------------------------
// Camera module is included (rcamera.h) and multiple predefined cameras are available: free, 1st/3rd person, orbital
#define RAYLIB_SUPPORT_CAMERA_SYSTEM           1
// Gestures module is included (rgestures.h) to support gestures detection: tap, hold, swipe, drag
#define RAYLIB_SUPPORT_GESTURES_SYSTEM         1
// Include pseudo-random numbers generator (rprand.h), based on Xoshiro128** and SplitMix64
#define RAYLIB_SUPPORT_RPRAND_GENERATOR        1
// Mouse gestures are directly mapped like touches and processed by gestures system
#define RAYLIB_SUPPORT_MOUSE_GESTURES          1
// Reconfigure standard input to receive key inputs, works with SSH connection.
#define RAYLIB_SUPPORT_SSH_KEYBOARD_RPI        1
// Setting a higher resolution can improve the accuracy of time-out intervals in wait functions.
// However, it can also reduce overall system performance, because the thread scheduler switches tasks more often.
#define RAYLIB_SUPPORT_WINMM_HIGHRES_TIMER     1
// Use busy wait loop for timing sync, if not defined, a high-resolution timer is set up and used
//#define RAYLIB_SUPPORT_BUSY_WAIT_LOOP          1
// Use a partial-busy wait loop, in this case frame sleeps for most of the time, but then runs a busy loop at the end for accuracy
#define RAYLIB_SUPPORT_PARTIALBUSY_WAIT_LOOP    1
// Allow automatic screen capture of current screen pressing F12, defined in KeyCallback()
#define RAYLIB_SUPPORT_SCREEN_CAPTURE          1
// Allow automatic gif recording of current screen pressing CTRL+F12, defined in KeyCallback()
#define RAYLIB_SUPPORT_GIF_RECORDING           1
// Support RaylibCompressData() and RaylibDecompressData() functions
#define RAYLIB_SUPPORT_COMPRESSION_API         1
// Support automatic generated events, loading and recording of those events when required
#define RAYLIB_SUPPORT_AUTOMATION_EVENTS       1
// Support custom frame control, only for advanced users
// By default RaylibEndDrawing() does this job: draws everything + RaylibSwapScreenBuffer() + manage frame timing + RaylibPollInputEvents()
// Enabling this flag allows manual control of the frame processes, use at your own risk
//#define SUPPORT_CUSTOM_FRAME_CONTROL    1


// rcore: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_MAX_FILEPATH_CAPACITY        8192       // Maximum file paths capacity
#define RAYLIB_MAX_FILEPATH_LENGTH          4096       // Maximum length for filepaths (Linux PATH_MAX default value)

#define RAYLIB_MAX_KEYBOARD_KEYS             512       // Maximum number of keyboard keys supported
#define RAYLIB_MAX_MOUSE_BUTTONS               8       // Maximum number of mouse buttons supported
#define RAYLIB_MAX_GAMEPADS                    4       // Maximum number of gamepads supported
#define RAYLIB_MAX_GAMEPAD_AXIS                8       // Maximum number of axis supported (per gamepad)
#define RAYLIB_MAX_GAMEPAD_BUTTONS            32       // Maximum number of buttons supported (per gamepad)
#define RAYLIB_MAX_GAMEPAD_VIBRATION_TIME      2.0f    // Maximum vibration time in seconds
#define RAYLIB_MAX_TOUCH_POINTS                8       // Maximum number of touch points supported
#define RAYLIB_MAX_KEY_PRESSED_QUEUE          16       // Maximum number of keys in the key input queue
#define RAYLIB_MAX_CHAR_PRESSED_QUEUE         16       // Maximum number of characters in the char input queue

#define RAYLIB_MAX_DECOMPRESSION_SIZE         64       // Max size allocated for decompression in MB

#define RAYLIB_MAX_AUTOMATION_EVENTS       16384       // Maximum number of automation events to record

//------------------------------------------------------------------------------------
// Module: rlgl - Configuration values
//------------------------------------------------------------------------------------

// Enable OpenGL Debug Context (only available on OpenGL 4.3)
//#define RLGL_ENABLE_OPENGL_DEBUG_CONTEXT       1

// Show OpenGL extensions and capabilities detailed logs on init
//#define RLGL_SHOW_GL_DETAILS_INFO              1

#define RL_SUPPORT_MESH_GPU_SKINNING           1      // GPU skinning, comment if your GPU does not support more than 8 VBOs

//#define RL_DEFAULT_BATCH_BUFFER_ELEMENTS    4096    // Default internal render batch elements limits
#define RL_DEFAULT_BATCH_BUFFERS               1      // Default number of batch buffers (multi-buffering)
#define RL_DEFAULT_BATCH_DRAWCALLS           256      // Default number of batch draw calls (by state changes: mode, texture)
#define RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS     4      // Maximum number of textures units that can be activated on batch drawing (RaylibSetShaderValueTexture())

#define RL_MAX_MATRIX_STACK_SIZE              32      // Maximum size of internal RaylibMatrix stack

#define RL_MAX_SHADER_LOCATIONS               32      // Maximum number of shader locations supported

#define RL_CULL_DISTANCE_NEAR               0.01      // Default projection matrix near cull distance
#define RL_CULL_DISTANCE_FAR              1000.0      // Default projection matrix far cull distance

// Default shader vertex attribute locations
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION    0
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD    1
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL      2
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR       3
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT     4
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2   5
#define RL_DEFAULT_SHADER_ATTRIB_LOCATION_INDICES     6
#if defined(RL_SUPPORT_MESH_GPU_SKINNING)
    #define RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEIDS     7
    #define RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS 8
#endif

// Default shader vertex attribute names to set location points
// NOTE: When a new shader is loaded, the following locations are tried to be set for convenience
#define RL_DEFAULT_SHADER_ATTRIB_NAME_POSITION     "vertexPosition"    // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD     "vertexTexCoord"    // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD
#define RL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL       "vertexNormal"      // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL
#define RL_DEFAULT_SHADER_ATTRIB_NAME_COLOR        "vertexColor"       // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT      "vertexTangent"     // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT
#define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2    "vertexTexCoord2"   // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2

#define RL_DEFAULT_SHADER_UNIFORM_NAME_MVP         "mvp"               // model-view-projection matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_VIEW        "matView"           // view matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION  "matProjection"     // projection matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_MODEL       "matModel"          // model matrix
#define RL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL      "matNormal"         // normal matrix (transpose(inverse(matModelView))
#define RL_DEFAULT_SHADER_UNIFORM_NAME_COLOR       "colDiffuse"        // color diffuse (base tint color, multiplied by texture color)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0  "texture0"          // texture0 (texture slot active 0)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1  "texture1"          // texture1 (texture slot active 1)
#define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2  "texture2"          // texture2 (texture slot active 2)


//------------------------------------------------------------------------------------
// Module: rshapes - Configuration Flags
//------------------------------------------------------------------------------------
// Use QUADS instead of TRIANGLES for drawing when possible
// Some lines-based shapes could still use lines
#define RAYLIB_SUPPORT_QUADS_DRAW_MODE         1

// rshapes: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_SPLINE_SEGMENT_DIVISIONS       24       // Spline segments subdivisions


//------------------------------------------------------------------------------------
// Module: rtextures - Configuration Flags
//------------------------------------------------------------------------------------
// Selecte desired fileformats to be supported for image data loading
#define RAYLIB_SUPPORT_FILEFORMAT_PNG      1
//#define RAYLIB_SUPPORT_FILEFORMAT_BMP      1
//#define SUPPORT_FILEFORMAT_TGA      1
//#define RAYLIB_SUPPORT_FILEFORMAT_JPG      1
#define RAYLIB_SUPPORT_FILEFORMAT_GIF      1
#define RAYLIB_SUPPORT_FILEFORMAT_QOI      1
//#define SUPPORT_FILEFORMAT_PSD      1
#define RAYLIB_SUPPORT_FILEFORMAT_DDS      1
//#define SUPPORT_FILEFORMAT_HDR      1
//#define SUPPORT_FILEFORMAT_PIC          1
//#define SUPPORT_FILEFORMAT_KTX      1
//#define SUPPORT_FILEFORMAT_ASTC     1
//#define SUPPORT_FILEFORMAT_PKM      1
//#define SUPPORT_FILEFORMAT_PVR      1

// Support image export functionality (.png, .bmp, .tga, .jpg, .qoi)
#define RAYLIB_SUPPORT_IMAGE_EXPORT            1
// Support procedural image generation functionality (gradient, spot, perlin-noise, cellular)
#define RAYLIB_SUPPORT_IMAGE_GENERATION        1
// Support multiple image editing functions to scale, adjust colors, flip, draw on images, crop...
// If not defined, still some functions are supported: RaylibImageFormat(), RaylibImageCrop(), RaylibImageToPOT()
#define RAYLIB_SUPPORT_IMAGE_MANIPULATION      1


//------------------------------------------------------------------------------------
// Module: rtext - Configuration Flags
//------------------------------------------------------------------------------------
// Default font is loaded on window initialization to be available for the user to render simple text
// NOTE: If enabled, uses external module functions to load default raylib font
#define RAYLIB_SUPPORT_DEFAULT_FONT            1
// Selected desired font fileformats to be supported for loading
#define RAYLIB_SUPPORT_FILEFORMAT_TTF          1
#define RAYLIB_SUPPORT_FILEFORMAT_FNT          1
//#define SUPPORT_FILEFORMAT_BDF          1

// Support text management functions
// If not defined, still some functions are supported: RaylibTextLength(), RaylibTextFormat()
#define RAYLIB_SUPPORT_TEXT_MANIPULATION       1

// On font atlas image generation [RaylibGenImageFontAtlas()], add a 3x3 pixels white rectangle
// at the bottom-right corner of the atlas. It can be useful to for shapes drawing, to allow
// drawing text and shapes with a single draw call [RaylibSetShapesTexture()].
#define RAYLIB_SUPPORT_FONT_ATLAS_WHITE_REC    1

// rtext: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_MAX_TEXT_BUFFER_LENGTH       1024       // Size of internal static buffers used on some functions:
                                                // RaylibTextFormat(), RaylibTextSubtext(), RaylibTextToUpper(), RaylibTextToLower(), RaylibTextToPascal(), RaylibTextSplit()
#define RAYLIB_MAX_TEXTSPLIT_COUNT           128       // Maximum number of substrings to split: RaylibTextSplit()


//------------------------------------------------------------------------------------
// Module: rmodels - Configuration Flags
//------------------------------------------------------------------------------------
// Selected desired model fileformats to be supported for loading
#define RAYLIB_SUPPORT_FILEFORMAT_OBJ          1
#define RAYLIB_SUPPORT_FILEFORMAT_MTL          1
#define RAYLIB_SUPPORT_FILEFORMAT_IQM          1
#define RAYLIB_SUPPORT_FILEFORMAT_GLTF         1
#define RAYLIB_SUPPORT_FILEFORMAT_VOX          1
#define RAYLIB_SUPPORT_FILEFORMAT_M3D          1
// Support procedural mesh generation functions, uses external par_shapes.h library
// NOTE: Some generated meshes DO NOT include generated texture coordinates
#define RAYLIB_SUPPORT_MESH_GENERATION         1

// rmodels: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_MAX_MATERIAL_MAPS              12       // Maximum number of shader maps supported

#ifdef RL_SUPPORT_MESH_GPU_SKINNING
#define RAYLIB_MAX_MESH_VERTEX_BUFFERS         9       // Maximum vertex buffers (VBO) per mesh
#else
#define RAYLIB_MAX_MESH_VERTEX_BUFFERS         7       // Maximum vertex buffers (VBO) per mesh
#endif

//------------------------------------------------------------------------------------
// Module: raudio - Configuration Flags
//------------------------------------------------------------------------------------
// Desired audio fileformats to be supported for loading
#define RAYLIB_SUPPORT_FILEFORMAT_WAV          1
#define RAYLIB_SUPPORT_FILEFORMAT_OGG          1
#define RAYLIB_SUPPORT_FILEFORMAT_MP3          1
#define RAYLIB_SUPPORT_FILEFORMAT_QOA          1
//#define SUPPORT_FILEFORMAT_FLAC         1
#define RAYLIB_SUPPORT_FILEFORMAT_XM           1
#define RAYLIB_SUPPORT_FILEFORMAT_MOD          1

// raudio: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_AUDIO_DEVICE_FORMAT    ma_format_f32    // Device output format (miniaudio: float-32bit)
#define RAYLIB_AUDIO_DEVICE_CHANNELS              2    // Device output channels: stereo
#define RAYLIB_AUDIO_DEVICE_SAMPLE_RATE           0    // Device sample rate (device default)

#define RAYLIB_MAX_AUDIO_BUFFER_POOL_CHANNELS    16    // Maximum number of audio pool channels

//------------------------------------------------------------------------------------
// Module: utils - Configuration Flags
//------------------------------------------------------------------------------------
// Standard file io library (stdio.h) included
#define RAYLIB_SUPPORT_STANDARD_FILEIO         1
// Show RAYLIB_TRACELOG() output messages
// NOTE: By default RAYLIB_LOG_DEBUG traces not shown
#define RAYLIB_SUPPORT_TRACELOG                1
//#define SUPPORT_TRACELOG_DEBUG          1

// utils: Configuration values
//------------------------------------------------------------------------------------
#define RAYLIB_MAX_TRACELOG_MSG_LENGTH       256       // Max length of one trace-log message


// Enable partial support for clipboard image, only working on SDL3 or
// being on both Windows OS + GLFW or Windows OS + RGFW
#define RAYLIB_SUPPORT_CLIPBOARD_IMAGE    1

#if defined(RAYLIB_SUPPORT_CLIPBOARD_IMAGE)
    #ifndef STBI_REQUIRED
        #define STBI_REQUIRED
    #endif

    #ifndef RAYLIB_SUPPORT_FILEFORMAT_BMP // For clipboard image on Windows
        #define RAYLIB_SUPPORT_FILEFORMAT_BMP 1
    #endif

    #ifndef RAYLIB_SUPPORT_FILEFORMAT_PNG // Wayland uses png for prints, at least it was on 22 LTS ubuntu
        #define RAYLIB_SUPPORT_FILEFORMAT_PNG 1
    #endif

    #ifndef RAYLIB_SUPPORT_FILEFORMAT_JPG
        #define RAYLIB_SUPPORT_FILEFORMAT_JPG 1
    #endif

    #ifndef RAYLIB_SUPPORT_MODULE_RTEXTURES
        #define RAYLIB_SUPPORT_MODULE_RTEXTURES 1
    #endif
#endif

#endif // RAYLIB_CONFIG_H
