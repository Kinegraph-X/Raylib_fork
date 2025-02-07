/**********************************************************************************************
*
*   raylib v5.5 - A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
*
*   FEATURES:
*       - NO external dependencies, all required libraries included with raylib
*       - Multiplatform: Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly,
*                        MacOS, Haiku, Android, Raspberry Pi, DRM native, HTML5.
*       - Written in plain C code (C99) in PascalCase/camelCase notation
*       - Hardware accelerated with OpenGL (1.1, 2.1, 3.3, 4.3, ES2, ES3 - choose at compile)
*       - Unique OpenGL abstraction layer (usable as standalone module): [rlgl]
*       - Multiple Fonts formats supported (TTF, OTF, FNT, BDF, Sprite fonts)
*       - Outstanding texture formats support, including compressed formats (DXT, ETC, ASTC)
*       - Full 3d support for 3d Shapes, Models, Billboards, Heightmaps and more!
*       - Flexible Materials system, supporting classic maps and PBR maps
*       - Animated 3D models supported (skeletal bones animation) (IQM, M3D, GLTF)
*       - Shaders support, including RaylibModel shaders and Postprocessing shaders
*       - Powerful math module for Vector, RaylibMatrix and Quaternion operations: [raymath]
*       - Audio loading and playing with streaming support (WAV, OGG, MP3, FLAC, QOA, XM, MOD)
*       - VR stereo rendering with configurable HMD device parameters
*       - Bindings to multiple programming languages available!
*
*   NOTES:
*       - One default RaylibFont is loaded on RaylibInitWindow()->LoadFontDefault() [core, text]
*       - One default Texture2D is loaded on rlglInit(), 1x1 white pixel R8G8B8A8 [rlgl] (OpenGL 3.3 or ES2)
*       - One default RaylibShader is loaded on rlglInit()->rlLoadShaderDefault() [rlgl] (OpenGL 3.3 or ES2)
*       - One default RenderBatch is loaded on rlglInit()->rlLoadRenderBatch() [rlgl] (OpenGL 3.3 or ES2)
*
*   DEPENDENCIES (included):
*       [rcore][GLFW] rglfw (Camilla Löwy - github.com/glfw/glfw) for window/context management and input
*       [rcore][RGFW] rgfw (ColleagueRiley - github.com/ColleagueRiley/RGFW) for window/context management and input
*       [rlgl] glad/glad_gles2 (David Herberth - github.com/Dav1dde/glad) for OpenGL 3.3 extensions loading
*       [raudio] miniaudio (David Reid - github.com/mackron/miniaudio) for audio device/context management
*
*   OPTIONAL DEPENDENCIES (included):
*       [rcore] msf_gif (Miles Fogle) for GIF recording
*       [rcore] sinfl (Micha Mettke) for DEFLATE decompression algorithm
*       [rcore] sdefl (Micha Mettke) for DEFLATE compression algorithm
*       [rcore] rprand (Ramon Snatamaria) for pseudo-random numbers generation
*       [rtextures] qoi (Dominic Szablewski - https://phoboslab.org) for QOI image manage
*       [rtextures] stb_image (Sean Barret) for images loading (BMP, TGA, PNG, JPEG, HDR...)
*       [rtextures] stb_image_write (Sean Barret) for image writing (BMP, TGA, PNG, JPG)
*       [rtextures] stb_image_resize2 (Sean Barret) for image resizing algorithms
*       [rtextures] stb_perlin (Sean Barret) for Perlin Noise image generation
*       [rtext] stb_truetype (Sean Barret) for ttf fonts loading
*       [rtext] stb_rect_pack (Sean Barret) for rectangles packing
*       [rmodels] par_shapes (Philip Rideout) for parametric 3d shapes generation
*       [rmodels] tinyobj_loader_c (Syoyo Fujita) for models loading (OBJ, MTL)
*       [rmodels] cgltf (Johannes Kuhlmann) for models loading (glTF)
*       [rmodels] m3d (bzt) for models loading (M3D, https://bztsrc.gitlab.io/model3d)
*       [rmodels] vox_loader (Johann Nadalutti) for models loading (VOX)
*       [raudio] dr_wav (David Reid) for WAV audio file loading
*       [raudio] dr_flac (David Reid) for FLAC audio file loading
*       [raudio] dr_mp3 (David Reid) for MP3 audio file loading
*       [raudio] stb_vorbis (Sean Barret) for OGG audio loading
*       [raudio] jar_xm (Joshua Reisenauer) for XM audio module loading
*       [raudio] jar_mod (Joshua Reisenauer) for MOD audio module loading
*       [raudio] qoa (Dominic Szablewski - https://phoboslab.org) for QOA audio manage
*
*
*   LICENSE: zlib/libpng
*
*   raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdarg.h>     // Required for: va_list - Only used by RaylibTraceLogCallback

#define RAYLIB_VERSION_MAJOR 5
#define RAYLIB_VERSION_MINOR 5
#define RAYLIB_VERSION_PATCH 0
#define RAYLIB_VERSION  "5.5"

// Function specifiers in case library is build/used as a shared library
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
// NOTE: visibility("default") attribute makes symbols "visible" when compiled with -fvisibility=hidden
#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RAYLIB_RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RAYLIB_RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#else
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RAYLIB_RLAPI __attribute__((visibility("default"))) // We are building as a Unix shared library (.so/.dylib)
    #endif
#endif

#ifndef RAYLIB_RLAPI
    #define RAYLIB_RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#ifndef RAYLIB_PI
    #define RAYLIB_PI 3.14159265358979323846f
#endif
#ifndef RAYLIB_DEG2RAD
    #define RAYLIB_DEG2RAD (RAYLIB_PI/180.0f)
#endif
#ifndef RAYLIB_RAD2DEG
    #define RAYLIB_RAD2DEG (180.0f/RAYLIB_PI)
#endif

// Allow custom memory allocators
// NOTE: Require recompiling raylib sources
#ifndef RL_MALLOC
    #define RL_MALLOC(sz)       malloc(sz)
#endif
#ifndef RL_CALLOC
    #define RL_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RL_REALLOC
    #define RL_REALLOC(ptr,sz)  realloc(ptr,sz)
#endif
#ifndef RL_FREE
    #define RL_FREE(ptr)        free(ptr)
#endif

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
// This is called aggregate initialization (C++11 feature)
#if defined(__cplusplus)
    #define RAYLIB_CLITERAL(type)      type
#else
    #define RAYLIB_CLITERAL(type)      (type)
#endif

// Some compilers (mostly macos clang) default to C++98,
// where aggregate initialization can't be used
// So, give a more clear error stating how to fix this
#if !defined(_MSC_VER) && (defined(__cplusplus) && __cplusplus < 201103L)
    #error "C++11 or later is required. Add -std=c++11"
#endif

// NOTE: We set some defines with some data types declared by raylib
// Other modules (raymath, rlgl) also require some of those types, so,
// to be able to use those other modules as standalone (not depending on raylib)
// this defines are very useful for internal check and avoid type (re)definitions
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on RAYLIB_WHITE background
#define RAYLIB_LIGHTGRAY  RAYLIB_CLITERAL(RaylibColor){ 200, 200, 200, 255 }   // Light Gray
#define RAYLIB_GRAY       RAYLIB_CLITERAL(RaylibColor){ 130, 130, 130, 255 }   // Gray
#define RAYLIB_DARKGRAY   RAYLIB_CLITERAL(RaylibColor){ 80, 80, 80, 255 }      // Dark Gray
#define RAYLIB_YELLOW     RAYLIB_CLITERAL(RaylibColor){ 253, 249, 0, 255 }     // Yellow
#define RAYLIB_GOLD       RAYLIB_CLITERAL(RaylibColor){ 255, 203, 0, 255 }     // Gold
#define RAYLIB_ORANGE     RAYLIB_CLITERAL(RaylibColor){ 255, 161, 0, 255 }     // Orange
#define RAYLIB_PINK       RAYLIB_CLITERAL(RaylibColor){ 255, 109, 194, 255 }   // Pink
#define RAYLIB_RED        RAYLIB_CLITERAL(RaylibColor){ 230, 41, 55, 255 }     // Red
#define RAYLIB_MAROON     RAYLIB_CLITERAL(RaylibColor){ 190, 33, 55, 255 }     // Maroon
#define RAYLIB_GREEN      RAYLIB_CLITERAL(RaylibColor){ 0, 228, 48, 255 }      // Green
#define RAYLIB_LIME       RAYLIB_CLITERAL(RaylibColor){ 0, 158, 47, 255 }      // Lime
#define RAYLIB_DARKGREEN  RAYLIB_CLITERAL(RaylibColor){ 0, 117, 44, 255 }      // Dark Green
#define RAYLIB_SKYBLUE    RAYLIB_CLITERAL(RaylibColor){ 102, 191, 255, 255 }   // Sky Blue
#define RAYLIB_BLUE       RAYLIB_CLITERAL(RaylibColor){ 0, 121, 241, 255 }     // Blue
#define RAYLIB_DARKBLUE   RAYLIB_CLITERAL(RaylibColor){ 0, 82, 172, 255 }      // Dark Blue
#define RAYLIB_PURPLE     RAYLIB_CLITERAL(RaylibColor){ 200, 122, 255, 255 }   // Purple
#define RAYLIB_VIOLET     RAYLIB_CLITERAL(RaylibColor){ 135, 60, 190, 255 }    // Violet
#define RAYLIB_DARKPURPLE RAYLIB_CLITERAL(RaylibColor){ 112, 31, 126, 255 }    // Dark Purple
#define RAYLIB_BEIGE      RAYLIB_CLITERAL(RaylibColor){ 211, 176, 131, 255 }   // Beige
#define RAYLIB_BROWN      RAYLIB_CLITERAL(RaylibColor){ 127, 106, 79, 255 }    // Brown
#define RAYLIB_DARKBROWN  RAYLIB_CLITERAL(RaylibColor){ 76, 63, 47, 255 }      // Dark Brown

#define RAYLIB_WHITE      RAYLIB_CLITERAL(RaylibColor){ 255, 255, 255, 255 }   // White
#define RAYLIB_BLACK      RAYLIB_CLITERAL(RaylibColor){ 0, 0, 0, 255 }         // Black
#define RAYLIB_BLANK      RAYLIB_CLITERAL(RaylibColor){ 0, 0, 0, 0 }           // Blank (Transparent)
#define RAYLIB_MAGENTA    RAYLIB_CLITERAL(RaylibColor){ 255, 0, 255, 255 }     // Magenta
#define RAYLIB_RAYWHITE   RAYLIB_CLITERAL(RaylibColor){ 245, 245, 245, 255 }   // My own White (raylib logo)

//----------------------------------------------------------------------------------
// Structures Definition
//----------------------------------------------------------------------------------
// Boolean type
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

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

// RaylibVector4, 4 components
typedef struct RaylibVector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} RaylibVector4;

// Quaternion, 4 components (RaylibVector4 alias)
typedef RaylibVector4 Quaternion;

// RaylibMatrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct RaylibMatrix {
    float m0, m4, m8, m12;  // RaylibMatrix first row (4 components)
    float m1, m5, m9, m13;  // RaylibMatrix second row (4 components)
    float m2, m6, m10, m14; // RaylibMatrix third row (4 components)
    float m3, m7, m11, m15; // RaylibMatrix fourth row (4 components)
} RaylibMatrix;

// RaylibColor, 4 components, R8G8B8A8 (32bit)
typedef struct RaylibColor {
    unsigned char r;        // RaylibColor red value
    unsigned char g;        // RaylibColor green value
    unsigned char b;        // RaylibColor blue value
    unsigned char a;        // RaylibColor alpha value
} RaylibColor;

// RaylibRectangle, 4 components
typedef struct RaylibRectangle {
    float x;                // RaylibRectangle top-left corner position x
    float y;                // RaylibRectangle top-left corner position y
    float width;            // RaylibRectangle width
    float height;           // RaylibRectangle height
} RaylibRectangle;

// RaylibImage, pixel data stored in CPU memory (RAM)
typedef struct RaylibImage {
    void *data;             // RaylibImage raw data
    int width;              // RaylibImage base width
    int height;             // RaylibImage base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RaylibPixelFormat type)
} RaylibImage;

// RaylibTexture, tex data stored in GPU memory (VRAM)
typedef struct RaylibTexture {
    unsigned int id;        // OpenGL texture id
    int width;              // RaylibTexture base width
    int height;             // RaylibTexture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RaylibPixelFormat type)
} RaylibTexture;

// Texture2D, same as RaylibTexture
typedef RaylibTexture Texture2D;

// TextureCubemap, same as RaylibTexture
typedef RaylibTexture TextureCubemap;

// RaylibRenderTexture, fbo for texture rendering
typedef struct RaylibRenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    RaylibTexture texture;        // RaylibColor buffer attachment texture
    RaylibTexture depth;          // Depth buffer attachment texture
} RaylibRenderTexture;

// RenderTexture2D, same as RaylibRenderTexture
typedef RaylibRenderTexture RenderTexture2D;

// RaylibNPatchInfo, n-patch layout info
typedef struct RaylibNPatchInfo {
    RaylibRectangle source;       // RaylibTexture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} RaylibNPatchInfo;

// RaylibGlyphInfo, font characters glyphs info
typedef struct RaylibGlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    RaylibImage image;            // Character image data
} RaylibGlyphInfo;

// RaylibFont, font texture and RaylibGlyphInfo array data
typedef struct RaylibFont {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    Texture2D texture;      // RaylibTexture atlas containing the glyphs
    RaylibRectangle *recs;        // Rectangles in texture for the glyphs
    RaylibGlyphInfo *glyphs;      // Glyphs info data
} RaylibFont;

// Camera, defines position/orientation in 3d space
typedef struct RaylibCamera3D {
    RaylibVector3 position;       // Camera position
    RaylibVector3 target;         // Camera target it looks-at
    RaylibVector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: RAYLIB_CAMERA_PERSPECTIVE or RAYLIB_CAMERA_ORTHOGRAPHIC
} RaylibCamera3D;

typedef RaylibCamera3D Camera;    // Camera type fallback, defaults to RaylibCamera3D

// RaylibCamera2D, defines position/orientation in 2d space
typedef struct RaylibCamera2D {
    RaylibVector2 offset;         // Camera offset (displacement from target)
    RaylibVector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} RaylibCamera2D;

// RaylibMesh, vertex data and vao/vbo
typedef struct RaylibMesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float *animVertices;    // Animated vertex positions (after bones transformations)
    float *animNormals;     // Animated normals (after bones transformations)
    unsigned char *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning) (shader-location = 6)
    float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning) (shader-location = 7)
    RaylibMatrix *boneMatrices;   // Bones animated transformation matrices
    int boneCount;          // Number of bones

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} RaylibMesh;

// RaylibShader
typedef struct RaylibShader {
    unsigned int id;        // RaylibShader program id
    int *locs;              // RaylibShader locations array (RL_MAX_SHADER_LOCATIONS)
} RaylibShader;

// RaylibMaterialMap
typedef struct RaylibMaterialMap {
    Texture2D texture;      // RaylibMaterial map texture
    RaylibColor color;            // RaylibMaterial map color
    float value;            // RaylibMaterial map value
} RaylibMaterialMap;

// RaylibMaterial, includes shader and maps
typedef struct RaylibMaterial {
    RaylibShader shader;          // RaylibMaterial shader
    RaylibMaterialMap *maps;      // RaylibMaterial maps array (RAYLIB_MAX_MATERIAL_MAPS)
    float params[4];        // RaylibMaterial generic parameters (if required)
} RaylibMaterial;

// RaylibTransform, vertex transformation data
typedef struct RaylibTransform {
    RaylibVector3 translation;    // Translation
    Quaternion rotation;    // Rotation
    RaylibVector3 scale;          // Scale
} RaylibTransform;

// Bone, skeletal animation bone
typedef struct RaylibBoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} RaylibBoneInfo;

// RaylibModel, meshes, materials and animation data
typedef struct RaylibModel {
    RaylibMatrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    RaylibMesh *meshes;           // Meshes array
    RaylibMaterial *materials;    // Materials array
    int *meshMaterial;      // RaylibMesh material number

    // Animation data
    int boneCount;          // Number of bones
    RaylibBoneInfo *bones;        // Bones information (skeleton)
    RaylibTransform *bindPose;    // Bones base transformation (pose)
} RaylibModel;

// RaylibModelAnimation
typedef struct RaylibModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    RaylibBoneInfo *bones;        // Bones information (skeleton)
    RaylibTransform **framePoses; // Poses array by frame
    char name[32];          // Animation name
} RaylibModelAnimation;

// RaylibRay, ray for raycasting
typedef struct RaylibRay {
    RaylibVector3 position;       // RaylibRay position (origin)
    RaylibVector3 direction;      // RaylibRay direction (normalized)
} RaylibRay;

// RaylibRayCollision, ray hit information
typedef struct RaylibRayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    RaylibVector3 point;          // Point of the nearest hit
    RaylibVector3 normal;         // Surface normal of hit
} RaylibRayCollision;

// RaylibBoundingBox
typedef struct RaylibBoundingBox {
    RaylibVector3 min;            // Minimum vertex box-corner
    RaylibVector3 max;            // Maximum vertex box-corner
} RaylibBoundingBox;

// RaylibWave, audio wave data
typedef struct RaylibWave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} RaylibWave;

// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct RaylibrAudioBuffer RaylibrAudioBuffer;
typedef struct RaylibrAudioProcessor RaylibrAudioProcessor;

// RaylibAudioStream, custom audio stream
typedef struct RaylibAudioStream {
    RaylibrAudioBuffer *buffer;       // Pointer to internal data used by the audio system
    RaylibrAudioProcessor *processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} RaylibAudioStream;

// RaylibSound
typedef struct RaylibSound {
    RaylibAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} RaylibSound;

// RaylibMusic, audio stream, anything longer than ~10 seconds should be streamed
typedef struct RaylibMusic {
    RaylibAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // RaylibMusic looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} RaylibMusic;

// RaylibVrDeviceInfo, Head-Mounted-Display device parameters
typedef struct RaylibVrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} RaylibVrDeviceInfo;

// RaylibVrStereoConfig, VR stereo rendering configuration for simulator
typedef struct RaylibVrStereoConfig {
    RaylibMatrix projection[2];           // VR projection matrices (per eye)
    RaylibMatrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} RaylibVrStereoConfig;

// File path list
typedef struct RaylibFilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char **paths;                   // Filepaths entries
} RaylibFilePathList;

// Automation event
typedef struct RaylibAutomationEvent {
    unsigned int frame;             // Event frame
    unsigned int type;              // Event type (AutomationEventType)
    int params[4];                  // Event parameters (if required)
} RaylibAutomationEvent;

// Automation event list
typedef struct RaylibAutomationEventList {
    unsigned int capacity;          // Events max entries (RAYLIB_MAX_AUTOMATION_EVENTS)
    unsigned int count;             // Events entries count
    RaylibAutomationEvent *events;        // Events entries
} RaylibAutomationEventList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    RAYLIB_FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    RAYLIB_FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    RAYLIB_FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    RAYLIB_FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    RAYLIB_FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    RAYLIB_FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    RAYLIB_FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    RAYLIB_FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    RAYLIB_FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    RAYLIB_FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    RAYLIB_FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    RAYLIB_FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    RAYLIB_FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when RAYLIB_FLAG_WINDOW_UNDECORATED
    RAYLIB_FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000, // Set to run program in borderless windowed mode
    RAYLIB_FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    RAYLIB_FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} RaylibConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    RAYLIB_LOG_ALL = 0,        // Display all logs
    RAYLIB_LOG_TRACE,          // Trace logging, intended for internal use only
    RAYLIB_LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    RAYLIB_LOG_INFO,           // Info logging, used for program execution info
    RAYLIB_LOG_WARNING,        // Warning logging, used on recoverable failures
    RAYLIB_LOG_ERROR,          // Error logging, used on unrecoverable failures
    RAYLIB_LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    RAYLIB_LOG_NONE            // Disable logging
} RaylibTraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use RaylibGetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
    RAYLIB_KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    RAYLIB_KEY_APOSTROPHE      = 39,       // Key: '
    RAYLIB_KEY_COMMA           = 44,       // Key: ,
    RAYLIB_KEY_MINUS           = 45,       // Key: -
    RAYLIB_KEY_PERIOD          = 46,       // Key: .
    RAYLIB_KEY_SLASH           = 47,       // Key: /
    RAYLIB_KEY_ZERO            = 48,       // Key: 0
    RAYLIB_KEY_ONE             = 49,       // Key: 1
    RAYLIB_KEY_TWO             = 50,       // Key: 2
    RAYLIB_KEY_THREE           = 51,       // Key: 3
    RAYLIB_KEY_FOUR            = 52,       // Key: 4
    RAYLIB_KEY_FIVE            = 53,       // Key: 5
    RAYLIB_KEY_SIX             = 54,       // Key: 6
    RAYLIB_KEY_SEVEN           = 55,       // Key: 7
    RAYLIB_KEY_EIGHT           = 56,       // Key: 8
    RAYLIB_KEY_NINE            = 57,       // Key: 9
    RAYLIB_KEY_SEMICOLON       = 59,       // Key: ;
    RAYLIB_KEY_EQUAL           = 61,       // Key: =
    RAYLIB_KEY_A               = 65,       // Key: A | a
    RAYLIB_KEY_B               = 66,       // Key: B | b
    RAYLIB_KEY_C               = 67,       // Key: C | c
    RAYLIB_KEY_D               = 68,       // Key: D | d
    RAYLIB_KEY_E               = 69,       // Key: E | e
    RAYLIB_KEY_F               = 70,       // Key: F | f
    RAYLIB_KEY_G               = 71,       // Key: G | g
    RAYLIB_KEY_H               = 72,       // Key: H | h
    RAYLIB_KEY_I               = 73,       // Key: I | i
    RAYLIB_KEY_J               = 74,       // Key: J | j
    RAYLIB_KEY_K               = 75,       // Key: K | k
    RAYLIB_KEY_L               = 76,       // Key: L | l
    RAYLIB_KEY_M               = 77,       // Key: M | m
    RAYLIB_KEY_N               = 78,       // Key: N | n
    RAYLIB_KEY_O               = 79,       // Key: O | o
    RAYLIB_KEY_P               = 80,       // Key: P | p
    RAYLIB_KEY_Q               = 81,       // Key: Q | q
    RAYLIB_KEY_R               = 82,       // Key: R | r
    RAYLIB_KEY_S               = 83,       // Key: S | s
    RAYLIB_KEY_T               = 84,       // Key: T | t
    RAYLIB_KEY_U               = 85,       // Key: U | u
    RAYLIB_KEY_V               = 86,       // Key: V | v
    RAYLIB_KEY_W               = 87,       // Key: W | w
    RAYLIB_KEY_X               = 88,       // Key: X | x
    RAYLIB_KEY_Y               = 89,       // Key: Y | y
    RAYLIB_KEY_Z               = 90,       // Key: Z | z
    RAYLIB_KEY_LEFT_BRACKET    = 91,       // Key: [
    RAYLIB_KEY_BACKSLASH       = 92,       // Key: '\'
    RAYLIB_KEY_RIGHT_BRACKET   = 93,       // Key: ]
    RAYLIB_KEY_GRAVE           = 96,       // Key: `
    // Function keys
    RAYLIB_KEY_SPACE           = 32,       // Key: Space
    RAYLIB_KEY_ESCAPE          = 256,      // Key: Esc
    RAYLIB_KEY_ENTER           = 257,      // Key: Enter
    RAYLIB_KEY_TAB             = 258,      // Key: Tab
    RAYLIB_KEY_BACKSPACE       = 259,      // Key: Backspace
    RAYLIB_KEY_INSERT          = 260,      // Key: Ins
    RAYLIB_KEY_DELETE          = 261,      // Key: Del
    RAYLIB_KEY_RIGHT           = 262,      // Key: Cursor right
    RAYLIB_KEY_LEFT            = 263,      // Key: Cursor left
    RAYLIB_KEY_DOWN            = 264,      // Key: Cursor down
    RAYLIB_KEY_UP              = 265,      // Key: Cursor up
    RAYLIB_KEY_PAGE_UP         = 266,      // Key: Page up
    RAYLIB_KEY_PAGE_DOWN       = 267,      // Key: Page down
    RAYLIB_KEY_HOME            = 268,      // Key: Home
    RAYLIB_KEY_END             = 269,      // Key: End
    RAYLIB_KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    RAYLIB_KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    RAYLIB_KEY_NUM_LOCK        = 282,      // Key: Num lock
    RAYLIB_KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    RAYLIB_KEY_PAUSE           = 284,      // Key: Pause
    RAYLIB_KEY_F1              = 290,      // Key: F1
    RAYLIB_KEY_F2              = 291,      // Key: F2
    RAYLIB_KEY_F3              = 292,      // Key: F3
    RAYLIB_KEY_F4              = 293,      // Key: F4
    RAYLIB_KEY_F5              = 294,      // Key: F5
    RAYLIB_KEY_F6              = 295,      // Key: F6
    RAYLIB_KEY_F7              = 296,      // Key: F7
    RAYLIB_KEY_F8              = 297,      // Key: F8
    RAYLIB_KEY_F9              = 298,      // Key: F9
    RAYLIB_KEY_F10             = 299,      // Key: F10
    RAYLIB_KEY_F11             = 300,      // Key: F11
    RAYLIB_KEY_F12             = 301,      // Key: F12
    RAYLIB_KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    RAYLIB_KEY_LEFT_CONTROL    = 341,      // Key: Control left
    RAYLIB_KEY_LEFT_ALT        = 342,      // Key: Alt left
    RAYLIB_KEY_LEFT_SUPER      = 343,      // Key: Super left
    RAYLIB_KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    RAYLIB_KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    RAYLIB_KEY_RIGHT_ALT       = 346,      // Key: Alt right
    RAYLIB_KEY_RIGHT_SUPER     = 347,      // Key: Super right
    RAYLIB_KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    RAYLIB_KEY_KP_0            = 320,      // Key: Keypad 0
    RAYLIB_KEY_KP_1            = 321,      // Key: Keypad 1
    RAYLIB_KEY_KP_2            = 322,      // Key: Keypad 2
    RAYLIB_KEY_KP_3            = 323,      // Key: Keypad 3
    RAYLIB_KEY_KP_4            = 324,      // Key: Keypad 4
    RAYLIB_KEY_KP_5            = 325,      // Key: Keypad 5
    RAYLIB_KEY_KP_6            = 326,      // Key: Keypad 6
    RAYLIB_KEY_KP_7            = 327,      // Key: Keypad 7
    RAYLIB_KEY_KP_8            = 328,      // Key: Keypad 8
    RAYLIB_KEY_KP_9            = 329,      // Key: Keypad 9
    RAYLIB_KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    RAYLIB_KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    RAYLIB_KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    RAYLIB_KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    RAYLIB_KEY_KP_ADD          = 334,      // Key: Keypad +
    RAYLIB_KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    RAYLIB_KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    RAYLIB_KEY_BACK            = 4,        // Key: Android back button
    RAYLIB_KEY_MENU            = 5,        // Key: Android menu button
    RAYLIB_KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    RAYLIB_KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} RaylibKeyboardKey;

// Add backwards compatibility support for deprecated names
#define RAYLIB_MOUSE_LEFT_BUTTON   RAYLIB_MOUSE_BUTTON_LEFT
#define RAYLIB_MOUSE_RIGHT_BUTTON  RAYLIB_MOUSE_BUTTON_RIGHT
#define RAYLIB_MOUSE_MIDDLE_BUTTON RAYLIB_MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    RAYLIB_MOUSE_BUTTON_LEFT    = 0,       // Mouse button left
    RAYLIB_MOUSE_BUTTON_RIGHT   = 1,       // Mouse button right
    RAYLIB_MOUSE_BUTTON_MIDDLE  = 2,       // Mouse button middle (pressed wheel)
    RAYLIB_MOUSE_BUTTON_SIDE    = 3,       // Mouse button side (advanced mouse device)
    RAYLIB_MOUSE_BUTTON_EXTRA   = 4,       // Mouse button extra (advanced mouse device)
    RAYLIB_MOUSE_BUTTON_FORWARD = 5,       // Mouse button forward (advanced mouse device)
    RAYLIB_MOUSE_BUTTON_BACK    = 6,       // Mouse button back (advanced mouse device)
} RaylibMouseButton;

// Mouse cursor
typedef enum {
    RAYLIB_MOUSE_CURSOR_DEFAULT       = 0,     // Default pointer shape
    RAYLIB_MOUSE_CURSOR_ARROW         = 1,     // Arrow shape
    RAYLIB_MOUSE_CURSOR_IBEAM         = 2,     // Text writing cursor shape
    RAYLIB_MOUSE_CURSOR_CROSSHAIR     = 3,     // Cross shape
    RAYLIB_MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    RAYLIB_MOUSE_CURSOR_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    RAYLIB_MOUSE_CURSOR_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    RAYLIB_MOUSE_CURSOR_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    RAYLIB_MOUSE_CURSOR_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    RAYLIB_MOUSE_CURSOR_RESIZE_ALL    = 9,     // The omnidirectional resize/move cursor shape
    RAYLIB_MOUSE_CURSOR_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} RaylibMouseCursor;

// Gamepad buttons
typedef enum {
    RAYLIB_GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    RAYLIB_GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
    RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    RAYLIB_GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Square, Xbox: X)
    RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    RAYLIB_GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (first), it could be a trailing button
    RAYLIB_GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    RAYLIB_GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    RAYLIB_GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    RAYLIB_GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    RAYLIB_GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    RAYLIB_GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} RaylibGamepadButton;

// Gamepad axis
typedef enum {
    RAYLIB_GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    RAYLIB_GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    RAYLIB_GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    RAYLIB_GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    RAYLIB_GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    RAYLIB_GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} RaylibGamepadAxis;

// RaylibMaterial map index
typedef enum {
    RAYLIB_MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: RAYLIB_MATERIAL_MAP_DIFFUSE)
    RAYLIB_MATERIAL_MAP_METALNESS,         // Metalness material (same as: RAYLIB_MATERIAL_MAP_SPECULAR)
    RAYLIB_MATERIAL_MAP_NORMAL,            // Normal material
    RAYLIB_MATERIAL_MAP_ROUGHNESS,         // Roughness material
    RAYLIB_MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    RAYLIB_MATERIAL_MAP_EMISSION,          // Emission material
    RAYLIB_MATERIAL_MAP_HEIGHT,            // Heightmap material
    RAYLIB_MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RAYLIB_MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RAYLIB_MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RAYLIB_MATERIAL_MAP_BRDF               // Brdf material
} RaylibMaterialMapIndex;

#define RAYLIB_MATERIAL_MAP_DIFFUSE      RAYLIB_MATERIAL_MAP_ALBEDO
#define RAYLIB_MATERIAL_MAP_SPECULAR     RAYLIB_MATERIAL_MAP_METALNESS

// RaylibShader location index
typedef enum {
    RAYLIB_SHADER_LOC_VERTEX_POSITION = 0, // RaylibShader location: vertex attribute: position
    RAYLIB_SHADER_LOC_VERTEX_TEXCOORD01,   // RaylibShader location: vertex attribute: texcoord01
    RAYLIB_SHADER_LOC_VERTEX_TEXCOORD02,   // RaylibShader location: vertex attribute: texcoord02
    RAYLIB_SHADER_LOC_VERTEX_NORMAL,       // RaylibShader location: vertex attribute: normal
    RAYLIB_SHADER_LOC_VERTEX_TANGENT,      // RaylibShader location: vertex attribute: tangent
    RAYLIB_SHADER_LOC_VERTEX_COLOR,        // RaylibShader location: vertex attribute: color
    RAYLIB_SHADER_LOC_MATRIX_MVP,          // RaylibShader location: matrix uniform: model-view-projection
    RAYLIB_SHADER_LOC_MATRIX_VIEW,         // RaylibShader location: matrix uniform: view (camera transform)
    RAYLIB_SHADER_LOC_MATRIX_PROJECTION,   // RaylibShader location: matrix uniform: projection
    RAYLIB_SHADER_LOC_MATRIX_MODEL,        // RaylibShader location: matrix uniform: model (transform)
    RAYLIB_SHADER_LOC_MATRIX_NORMAL,       // RaylibShader location: matrix uniform: normal
    RAYLIB_SHADER_LOC_VECTOR_VIEW,         // RaylibShader location: vector uniform: view
    RAYLIB_SHADER_LOC_COLOR_DIFFUSE,       // RaylibShader location: vector uniform: diffuse color
    RAYLIB_SHADER_LOC_COLOR_SPECULAR,      // RaylibShader location: vector uniform: specular color
    RAYLIB_SHADER_LOC_COLOR_AMBIENT,       // RaylibShader location: vector uniform: ambient color
    RAYLIB_SHADER_LOC_MAP_ALBEDO,          // RaylibShader location: sampler2d texture: albedo (same as: RAYLIB_SHADER_LOC_MAP_DIFFUSE)
    RAYLIB_SHADER_LOC_MAP_METALNESS,       // RaylibShader location: sampler2d texture: metalness (same as: RAYLIB_SHADER_LOC_MAP_SPECULAR)
    RAYLIB_SHADER_LOC_MAP_NORMAL,          // RaylibShader location: sampler2d texture: normal
    RAYLIB_SHADER_LOC_MAP_ROUGHNESS,       // RaylibShader location: sampler2d texture: roughness
    RAYLIB_SHADER_LOC_MAP_OCCLUSION,       // RaylibShader location: sampler2d texture: occlusion
    RAYLIB_SHADER_LOC_MAP_EMISSION,        // RaylibShader location: sampler2d texture: emission
    RAYLIB_SHADER_LOC_MAP_HEIGHT,          // RaylibShader location: sampler2d texture: height
    RAYLIB_SHADER_LOC_MAP_CUBEMAP,         // RaylibShader location: samplerCube texture: cubemap
    RAYLIB_SHADER_LOC_MAP_IRRADIANCE,      // RaylibShader location: samplerCube texture: irradiance
    RAYLIB_SHADER_LOC_MAP_PREFILTER,       // RaylibShader location: samplerCube texture: prefilter
    RAYLIB_SHADER_LOC_MAP_BRDF,            // RaylibShader location: sampler2d texture: brdf
    RAYLIB_SHADER_LOC_VERTEX_BONEIDS,      // RaylibShader location: vertex attribute: boneIds
    RAYLIB_SHADER_LOC_VERTEX_BONEWEIGHTS,  // RaylibShader location: vertex attribute: boneWeights
    RAYLIB_SHADER_LOC_BONE_MATRICES        // RaylibShader location: array of matrices uniform: boneMatrices
} RaylibShaderLocationIndex;

#define RAYLIB_SHADER_LOC_MAP_DIFFUSE      RAYLIB_SHADER_LOC_MAP_ALBEDO
#define RAYLIB_SHADER_LOC_MAP_SPECULAR     RAYLIB_SHADER_LOC_MAP_METALNESS

// RaylibShader uniform data type
typedef enum {
    RAYLIB_SHADER_UNIFORM_FLOAT = 0,       // RaylibShader uniform type: float
    RAYLIB_SHADER_UNIFORM_VEC2,            // RaylibShader uniform type: vec2 (2 float)
    RAYLIB_SHADER_UNIFORM_VEC3,            // RaylibShader uniform type: vec3 (3 float)
    RAYLIB_SHADER_UNIFORM_VEC4,            // RaylibShader uniform type: vec4 (4 float)
    RAYLIB_SHADER_UNIFORM_INT,             // RaylibShader uniform type: int
    RAYLIB_SHADER_UNIFORM_IVEC2,           // RaylibShader uniform type: ivec2 (2 int)
    RAYLIB_SHADER_UNIFORM_IVEC3,           // RaylibShader uniform type: ivec3 (3 int)
    RAYLIB_SHADER_UNIFORM_IVEC4,           // RaylibShader uniform type: ivec4 (4 int)
    RAYLIB_SHADER_UNIFORM_SAMPLER2D        // RaylibShader uniform type: sampler2d
} RaylibShaderUniformDataType;

// RaylibShader attribute data types
typedef enum {
    RAYLIB_SHADER_ATTRIB_FLOAT = 0,        // RaylibShader attribute type: float
    RAYLIB_SHADER_ATTRIB_VEC2,             // RaylibShader attribute type: vec2 (2 float)
    RAYLIB_SHADER_ATTRIB_VEC3,             // RaylibShader attribute type: vec3 (3 float)
    RAYLIB_SHADER_ATTRIB_VEC4              // RaylibShader attribute type: vec4 (4 float)
} RaylibShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    RAYLIB_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    RAYLIB_PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    RAYLIB_PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    RAYLIB_PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    RAYLIB_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} RaylibPixelFormat;

// RaylibTexture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    RAYLIB_TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    RAYLIB_TEXTURE_FILTER_BILINEAR,                // Linear filtering
    RAYLIB_TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    RAYLIB_TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    RAYLIB_TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    RAYLIB_TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} RaylibTextureFilter;

// RaylibTexture parameters: wrap mode
typedef enum {
    RAYLIB_TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    RAYLIB_TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    RAYLIB_TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    RAYLIB_TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} RaylibTextureWrap;

// Cubemap layouts
typedef enum {
    RAYLIB_CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    RAYLIB_CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    RAYLIB_CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    RAYLIB_CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    RAYLIB_CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE     // Layout is defined by a 4x3 cross with cubemap faces
} RaylibCubemapLayout;

// RaylibFont type, defines generation method
typedef enum {
    RAYLIB_FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    RAYLIB_FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    RAYLIB_FONT_SDF                        // SDF font generation, requires external shader
} RaylibFontType;

// RaylibColor blending modes (pre-defined)
typedef enum {
    RAYLIB_BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    RAYLIB_BLEND_ADDITIVE,                 // Blend textures adding colors
    RAYLIB_BLEND_MULTIPLIED,               // Blend textures multiplying colors
    RAYLIB_BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    RAYLIB_BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    RAYLIB_BLEND_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    RAYLIB_BLEND_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    RAYLIB_BLEND_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} RaylibBlendMode;

// RaylibGesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    RAYLIB_GESTURE_NONE        = 0,        // No gesture
    RAYLIB_GESTURE_TAP         = 1,        // Tap gesture
    RAYLIB_GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    RAYLIB_GESTURE_HOLD        = 4,        // Hold gesture
    RAYLIB_GESTURE_DRAG        = 8,        // Drag gesture
    RAYLIB_GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    RAYLIB_GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    RAYLIB_GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    RAYLIB_GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    RAYLIB_GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    RAYLIB_GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} RaylibGesture;

// Camera system modes
typedef enum {
    RAYLIB_CAMERA_CUSTOM = 0,              // Camera custom, controlled by user (RaylibUpdateCamera() does nothing)
    RAYLIB_CAMERA_FREE,                    // Camera free mode
    RAYLIB_CAMERA_ORBITAL,                 // Camera orbital, around target, zoom supported
    RAYLIB_CAMERA_FIRST_PERSON,            // Camera first person
    RAYLIB_CAMERA_THIRD_PERSON             // Camera third person
} RaylibCameraMode;

// Camera projection
typedef enum {
    RAYLIB_CAMERA_PERSPECTIVE = 0,         // Perspective projection
    RAYLIB_CAMERA_ORTHOGRAPHIC             // Orthographic projection
} RaylibCameraProjection;

// N-patch layout
typedef enum {
    RAYLIB_NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    RAYLIB_NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    RAYLIB_NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} RaylibNPatchLayout;

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advanced users
typedef void (*RaylibTraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*RaylibLoadFileDataCallback)(const char *fileName, int *dataSize);    // FileIO: Load binary data
typedef bool (*RaylibSaveFileDataCallback)(const char *fileName, void *data, int dataSize);   // FileIO: Save binary data
typedef char *(*RaylibLoadFileTextCallback)(const char *fileName);            // FileIO: Load text data
typedef bool (*RaylibSaveFileTextCallback)(const char *fileName, char *text); // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Window-related functions
RAYLIB_RLAPI void RaylibInitWindow(int width, int height, const char *title);  // Initialize window and OpenGL context
RAYLIB_RLAPI void RaylibCloseWindow(void);                                     // Close window and unload OpenGL context
RAYLIB_RLAPI bool RaylibWindowShouldClose(void);                               // Check if application should close (RAYLIB_KEY_ESCAPE pressed or windows close icon clicked)
RAYLIB_RLAPI bool RaylibIsWindowReady(void);                                   // Check if window has been initialized successfully
RAYLIB_RLAPI bool RaylibIsWindowFullscreen(void);                              // Check if window is currently fullscreen
RAYLIB_RLAPI bool RaylibIsWindowHidden(void);                                  // Check if window is currently hidden
RAYLIB_RLAPI bool RaylibIsWindowMinimized(void);                               // Check if window is currently minimized
RAYLIB_RLAPI bool RaylibIsWindowMaximized(void);                               // Check if window is currently maximized
RAYLIB_RLAPI bool RaylibIsWindowFocused(void);                                 // Check if window is currently focused
RAYLIB_RLAPI bool RaylibIsWindowResized(void);                                 // Check if window has been resized last frame
RAYLIB_RLAPI bool RaylibIsWindowState(unsigned int flag);                      // Check if one specific window flag is enabled
RAYLIB_RLAPI void RaylibSetWindowState(unsigned int flags);                    // Set window configuration state using flags
RAYLIB_RLAPI void RaylibClearWindowState(unsigned int flags);                  // Clear window configuration state flags
RAYLIB_RLAPI void RaylibToggleFullscreen(void);                                // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
RAYLIB_RLAPI void RaylibToggleBorderlessWindowed(void);                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
RAYLIB_RLAPI void RaylibMaximizeWindow(void);                                  // Set window state: maximized, if resizable
RAYLIB_RLAPI void RaylibMinimizeWindow(void);                                  // Set window state: minimized, if resizable
RAYLIB_RLAPI void RaylibRestoreWindow(void);                                   // Set window state: not minimized/maximized
RAYLIB_RLAPI void RaylibSetWindowIcon(RaylibImage image);                            // Set icon for window (single image, RGBA 32bit)
RAYLIB_RLAPI void RaylibSetWindowIcons(RaylibImage *images, int count);              // Set icon for window (multiple images, RGBA 32bit)
RAYLIB_RLAPI void RaylibSetWindowTitle(const char *title);                     // Set title for window
RAYLIB_RLAPI void RaylibSetWindowPosition(int x, int y);                       // Set window position on screen
RAYLIB_RLAPI void RaylibSetWindowMonitor(int monitor);                         // Set monitor for the current window
RAYLIB_RLAPI void RaylibSetWindowMinSize(int width, int height);               // Set window minimum dimensions (for RAYLIB_FLAG_WINDOW_RESIZABLE)
RAYLIB_RLAPI void RaylibSetWindowMaxSize(int width, int height);               // Set window maximum dimensions (for RAYLIB_FLAG_WINDOW_RESIZABLE)
RAYLIB_RLAPI void RaylibSetWindowSize(int width, int height);                  // Set window dimensions
RAYLIB_RLAPI void RaylibSetWindowOpacity(float opacity);                       // Set window opacity [0.0f..1.0f]
RAYLIB_RLAPI void RaylibSetWindowFocused(void);                                // Set window focused
RAYLIB_RLAPI void *RaylibGetWindowHandle(void);                                // Get native window handle
RAYLIB_RLAPI int RaylibGetScreenWidth(void);                                   // Get current screen width
RAYLIB_RLAPI int RaylibGetScreenHeight(void);                                  // Get current screen height
RAYLIB_RLAPI int RaylibGetRenderWidth(void);                                   // Get current render width (it considers HiDPI)
RAYLIB_RLAPI int RaylibGetRenderHeight(void);                                  // Get current render height (it considers HiDPI)
RAYLIB_RLAPI int RaylibGetMonitorCount(void);                                  // Get number of connected monitors
RAYLIB_RLAPI int RaylibGetCurrentMonitor(void);                                // Get current monitor where window is placed
RAYLIB_RLAPI RaylibVector2 RaylibGetMonitorPosition(int monitor);                    // Get specified monitor position
RAYLIB_RLAPI int RaylibGetMonitorWidth(int monitor);                           // Get specified monitor width (current video mode used by monitor)
RAYLIB_RLAPI int RaylibGetMonitorHeight(int monitor);                          // Get specified monitor height (current video mode used by monitor)
RAYLIB_RLAPI int RaylibGetMonitorPhysicalWidth(int monitor);                   // Get specified monitor physical width in millimetres
RAYLIB_RLAPI int RaylibGetMonitorPhysicalHeight(int monitor);                  // Get specified monitor physical height in millimetres
RAYLIB_RLAPI int RaylibGetMonitorRefreshRate(int monitor);                     // Get specified monitor refresh rate
RAYLIB_RLAPI RaylibVector2 RaylibGetWindowPosition(void);                            // Get window position XY on monitor
RAYLIB_RLAPI RaylibVector2 RaylibGetWindowScaleDPI(void);                            // Get window scale DPI factor
RAYLIB_RLAPI const char *RaylibGetMonitorName(int monitor);                    // Get the human-readable, UTF-8 encoded name of the specified monitor
RAYLIB_RLAPI void RaylibSetClipboardText(const char *text);                    // Set clipboard text content
RAYLIB_RLAPI const char *RaylibGetClipboardText(void);                         // Get clipboard text content
RAYLIB_RLAPI RaylibImage RaylibGetClipboardImage(void);                              // Get clipboard image content
RAYLIB_RLAPI void RaylibEnableEventWaiting(void);                              // Enable waiting for events on RaylibEndDrawing(), no automatic event polling
RAYLIB_RLAPI void RaylibDisableEventWaiting(void);                             // Disable waiting for events on RaylibEndDrawing(), automatic events polling

// Cursor-related functions
RAYLIB_RLAPI void RaylibShowCursor(void);                                      // Shows cursor
RAYLIB_RLAPI void RaylibHideCursor(void);                                      // Hides cursor
RAYLIB_RLAPI bool RaylibIsCursorHidden(void);                                  // Check if cursor is not visible
RAYLIB_RLAPI void RaylibEnableCursor(void);                                    // Enables cursor (unlock cursor)
RAYLIB_RLAPI void RaylibDisableCursor(void);                                   // Disables cursor (lock cursor)
RAYLIB_RLAPI bool RaylibIsCursorOnScreen(void);                                // Check if cursor is on the screen

// Drawing-related functions
RAYLIB_RLAPI void RaylibClearBackground(RaylibColor color);                          // Set background color (framebuffer clear color)
RAYLIB_RLAPI void RaylibBeginDrawing(void);                                    // Setup canvas (framebuffer) to start drawing
RAYLIB_RLAPI void RaylibEndDrawing(void);                                      // End canvas drawing and swap buffers (double buffering)
RAYLIB_RLAPI void RaylibBeginMode2D(RaylibCamera2D camera);                          // Begin 2D mode with custom camera (2D)
RAYLIB_RLAPI void RaylibEndMode2D(void);                                       // Ends 2D mode with custom camera
RAYLIB_RLAPI void RaylibBeginMode3D(RaylibCamera3D camera);                          // Begin 3D mode with custom camera (3D)
RAYLIB_RLAPI void RaylibEndMode3D(void);                                       // Ends 3D mode and returns to default 2D orthographic mode
RAYLIB_RLAPI void RaylibBeginTextureMode(RenderTexture2D target);              // Begin drawing to render texture
RAYLIB_RLAPI void RaylibEndTextureMode(void);                                  // Ends drawing to render texture
RAYLIB_RLAPI void RaylibBeginShaderMode(RaylibShader shader);                        // Begin custom shader drawing
RAYLIB_RLAPI void RaylibEndShaderMode(void);                                   // End custom shader drawing (use default shader)
RAYLIB_RLAPI void RaylibBeginBlendMode(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
RAYLIB_RLAPI void RaylibEndBlendMode(void);                                    // End blending mode (reset to default: alpha blending)
RAYLIB_RLAPI void RaylibBeginScissorMode(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
RAYLIB_RLAPI void RaylibEndScissorMode(void);                                  // End scissor mode
RAYLIB_RLAPI void RaylibBeginVrStereoMode(RaylibVrStereoConfig config);              // Begin stereo rendering (requires VR simulator)
RAYLIB_RLAPI void RaylibEndVrStereoMode(void);                                 // End stereo rendering (requires VR simulator)

// VR stereo config functions for VR simulator
RAYLIB_RLAPI RaylibVrStereoConfig RaylibLoadVrStereoConfig(RaylibVrDeviceInfo device);     // Load VR stereo config for VR simulator device parameters
RAYLIB_RLAPI void RaylibUnloadVrStereoConfig(RaylibVrStereoConfig config);           // Unload VR stereo config

// RaylibShader management functions
// NOTE: RaylibShader functionality is not available on OpenGL 1.1
RAYLIB_RLAPI RaylibShader RaylibLoadShader(const char *vsFileName, const char *fsFileName);   // Load shader from files and bind default locations
RAYLIB_RLAPI RaylibShader RaylibLoadShaderFromMemory(const char *vsCode, const char *fsCode); // Load shader from code strings and bind default locations
RAYLIB_RLAPI bool RaylibIsShaderValid(RaylibShader shader);                                   // Check if a shader is valid (loaded on GPU)
RAYLIB_RLAPI int RaylibGetShaderLocation(RaylibShader shader, const char *uniformName);       // Get shader uniform location
RAYLIB_RLAPI int RaylibGetShaderLocationAttrib(RaylibShader shader, const char *attribName);  // Get shader attribute location
RAYLIB_RLAPI void RaylibSetShaderValue(RaylibShader shader, int locIndex, const void *value, int uniformType);               // Set shader uniform value
RAYLIB_RLAPI void RaylibSetShaderValueV(RaylibShader shader, int locIndex, const void *value, int uniformType, int count);   // Set shader uniform value vector
RAYLIB_RLAPI void RaylibSetShaderValueMatrix(RaylibShader shader, int locIndex, RaylibMatrix mat);         // Set shader uniform value (matrix 4x4)
RAYLIB_RLAPI void RaylibSetShaderValueTexture(RaylibShader shader, int locIndex, Texture2D texture); // Set shader uniform value for texture (sampler2d)
RAYLIB_RLAPI void RaylibUnloadShader(RaylibShader shader);                                    // Unload shader from GPU memory (VRAM)

// Screen-space-related functions
#define RAYLIB_GetMouseRay RaylibGetScreenToWorldRay     // Compatibility hack for previous raylib versions
RAYLIB_RLAPI RaylibRay RaylibGetScreenToWorldRay(RaylibVector2 position, Camera camera);         // Get a ray trace from screen position (i.e mouse)
RAYLIB_RLAPI RaylibRay RaylibGetScreenToWorldRayEx(RaylibVector2 position, Camera camera, int width, int height); // Get a ray trace from screen position (i.e mouse) in a viewport
RAYLIB_RLAPI RaylibVector2 RaylibGetWorldToScreen(RaylibVector3 position, Camera camera);        // Get the screen space position for a 3d world space position
RAYLIB_RLAPI RaylibVector2 RaylibGetWorldToScreenEx(RaylibVector3 position, Camera camera, int width, int height); // Get size position for a 3d world space position
RAYLIB_RLAPI RaylibVector2 RaylibGetWorldToScreen2D(RaylibVector2 position, RaylibCamera2D camera);    // Get the screen space position for a 2d camera world space position
RAYLIB_RLAPI RaylibVector2 RaylibGetScreenToWorld2D(RaylibVector2 position, RaylibCamera2D camera);    // Get the world space position for a 2d camera screen space position
RAYLIB_RLAPI RaylibMatrix RaylibGetCameraMatrix(Camera camera);                            // Get camera transform matrix (view matrix)
RAYLIB_RLAPI RaylibMatrix RaylibGetCameraMatrix2D(RaylibCamera2D camera);                        // Get camera 2d transform matrix

// Timing-related functions
RAYLIB_RLAPI void RaylibSetTargetFPS(int fps);                                 // Set target FPS (maximum)
RAYLIB_RLAPI float RaylibGetFrameTime(void);                                   // Get time in seconds for last frame drawn (delta time)
RAYLIB_RLAPI double RaylibGetTime(void);                                       // Get elapsed time in seconds since RaylibInitWindow()
RAYLIB_RLAPI int RaylibGetFPS(void);                                           // Get current FPS

// Custom frame control functions
// NOTE: Those functions are intended for advanced users that want full control over the frame processing
// By default RaylibEndDrawing() does this job: draws everything + RaylibSwapScreenBuffer() + manage frame timing + RaylibPollInputEvents()
// To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
RAYLIB_RLAPI void RaylibSwapScreenBuffer(void);                                // Swap back buffer with front buffer (screen drawing)
RAYLIB_RLAPI void RaylibPollInputEvents(void);                                 // Register all input events
RAYLIB_RLAPI void RaylibWaitTime(double seconds);                              // Wait for some time (halt program execution)

// Random values generation functions
RAYLIB_RLAPI void RaylibSetRandomSeed(unsigned int seed);                      // Set the seed for the random number generator
RAYLIB_RLAPI int RaylibGetRandomValue(int min, int max);                       // Get a random value between min and max (both included)
RAYLIB_RLAPI int *RaylibLoadRandomSequence(unsigned int count, int min, int max); // Load random values sequence, no values repeated
RAYLIB_RLAPI void RaylibUnloadRandomSequence(int *sequence);                   // Unload random values sequence

// Misc. functions
RAYLIB_RLAPI void RaylibTakeScreenshot(const char *fileName);                  // Takes a screenshot of current screen (filename extension defines format)
RAYLIB_RLAPI void RaylibSetConfigFlags(unsigned int flags);                    // Setup init configuration flags (view FLAGS)
RAYLIB_RLAPI void RaylibOpenURL(const char *url);                              // Open URL with default system browser (if available)

// NOTE: Following functions implemented in module [utils]
//------------------------------------------------------------------
RAYLIB_RLAPI void RaylibTraceLog(int logLevel, const char *text, ...);         // Show trace log messages (RAYLIB_LOG_DEBUG, RAYLIB_LOG_INFO, RAYLIB_LOG_WARNING, RAYLIB_LOG_ERROR...)
RAYLIB_RLAPI void RaylibSetTraceLogLevel(int logLevel);                        // Set the current threshold (minimum) log level
RAYLIB_RLAPI void *RaylibMemAlloc(unsigned int size);                          // Internal memory allocator
RAYLIB_RLAPI void *RaylibMemRealloc(void *ptr, unsigned int size);             // Internal memory reallocator
RAYLIB_RLAPI void RaylibMemFree(void *ptr);                                    // Internal memory free

// Set custom callbacks
// WARNING: Callbacks setup is intended for advanced users
RAYLIB_RLAPI void RaylibSetTraceLogCallback(RaylibTraceLogCallback callback);         // Set custom trace log
RAYLIB_RLAPI void RaylibSetLoadFileDataCallback(RaylibLoadFileDataCallback callback); // Set custom file binary data loader
RAYLIB_RLAPI void RaylibSetSaveFileDataCallback(RaylibSaveFileDataCallback callback); // Set custom file binary data saver
RAYLIB_RLAPI void RaylibSetLoadFileTextCallback(RaylibLoadFileTextCallback callback); // Set custom file text data loader
RAYLIB_RLAPI void RaylibSetSaveFileTextCallback(RaylibSaveFileTextCallback callback); // Set custom file text data saver

// Files management functions
RAYLIB_RLAPI unsigned char *RaylibLoadFileData(const char *fileName, int *dataSize); // Load file data as byte array (read)
RAYLIB_RLAPI void RaylibUnloadFileData(unsigned char *data);                   // Unload file data allocated by RaylibLoadFileData()
RAYLIB_RLAPI bool RaylibSaveFileData(const char *fileName, void *data, int dataSize); // Save data to file from byte array (write), returns true on success
RAYLIB_RLAPI bool RaylibExportDataAsCode(const unsigned char *data, int dataSize, const char *fileName); // Export data to code (.h), returns true on success
RAYLIB_RLAPI char *RaylibLoadFileText(const char *fileName);                   // Load text data from file (read), returns a '\0' terminated string
RAYLIB_RLAPI void RaylibUnloadFileText(char *text);                            // Unload file text data allocated by RaylibLoadFileText()
RAYLIB_RLAPI bool RaylibSaveFileText(const char *fileName, char *text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
//------------------------------------------------------------------

// File system functions
RAYLIB_RLAPI bool RaylibFileExists(const char *fileName);                      // Check if file exists
RAYLIB_RLAPI bool RaylibDirectoryExists(const char *dirPath);                  // Check if a directory path exists
RAYLIB_RLAPI bool RaylibIsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
RAYLIB_RLAPI int RaylibGetFileLength(const char *fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
RAYLIB_RLAPI const char *RaylibGetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
RAYLIB_RLAPI const char *RaylibGetFileName(const char *filePath);              // Get pointer to filename for a path string
RAYLIB_RLAPI const char *RaylibGetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
RAYLIB_RLAPI const char *RaylibGetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)
RAYLIB_RLAPI const char *RaylibGetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses static string)
RAYLIB_RLAPI const char *RaylibGetWorkingDirectory(void);                      // Get current working directory (uses static string)
RAYLIB_RLAPI const char *RaylibGetApplicationDirectory(void);                  // Get the directory of the running application (uses static string)
RAYLIB_RLAPI int RaylibMakeDirectory(const char *dirPath);                     // Create directories (including full path requested), returns 0 on success
RAYLIB_RLAPI bool RaylibChangeDirectory(const char *dir);                      // Change working directory, return true on success
RAYLIB_RLAPI bool RaylibIsPathFile(const char *path);                          // Check if a given path is a file or a directory
RAYLIB_RLAPI bool RaylibIsFileNameValid(const char *fileName);                 // Check if fileName is valid for the platform/OS
RAYLIB_RLAPI RaylibFilePathList RaylibLoadDirectoryFiles(const char *dirPath);       // Load directory filepaths
RAYLIB_RLAPI RaylibFilePathList RaylibLoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan. Use 'DIR' in the filter string to include directories in the result
RAYLIB_RLAPI void RaylibUnloadDirectoryFiles(RaylibFilePathList files);              // Unload filepaths
RAYLIB_RLAPI bool RaylibIsFileDropped(void);                                   // Check if a file has been dropped into window
RAYLIB_RLAPI RaylibFilePathList RaylibLoadDroppedFiles(void);                        // Load dropped filepaths
RAYLIB_RLAPI void RaylibUnloadDroppedFiles(RaylibFilePathList files);                // Unload dropped filepaths
RAYLIB_RLAPI long RaylibGetFileModTime(const char *fileName);                  // Get file modification time (last write time)

// Compression/Encoding functionality
RAYLIB_RLAPI unsigned char *RaylibCompressData(const unsigned char *data, int dataSize, int *compDataSize);        // Compress data (DEFLATE algorithm), memory must be RaylibMemFree()
RAYLIB_RLAPI unsigned char *RaylibDecompressData(const unsigned char *compData, int compDataSize, int *dataSize);  // Decompress data (DEFLATE algorithm), memory must be RaylibMemFree()
RAYLIB_RLAPI char *RaylibEncodeDataBase64(const unsigned char *data, int dataSize, int *outputSize);               // Encode data to Base64 string, memory must be RaylibMemFree()
RAYLIB_RLAPI unsigned char *RaylibDecodeDataBase64(const unsigned char *data, int *outputSize);                    // Decode Base64 string data, memory must be RaylibMemFree()
RAYLIB_RLAPI unsigned int RaylibComputeCRC32(unsigned char *data, int dataSize);     // Compute CRC32 hash code
RAYLIB_RLAPI unsigned int *RaylibComputeMD5(unsigned char *data, int dataSize);      // Compute MD5 hash code, returns static int[4] (16 bytes)
RAYLIB_RLAPI unsigned int *RaylibComputeSHA1(unsigned char *data, int dataSize);      // Compute SHA1 hash code, returns static int[5] (20 bytes)


// Automation events functionality
RAYLIB_RLAPI RaylibAutomationEventList RaylibLoadAutomationEventList(const char *fileName);                // Load automation events list from file, NULL for empty list, capacity = RAYLIB_MAX_AUTOMATION_EVENTS
RAYLIB_RLAPI void RaylibUnloadAutomationEventList(RaylibAutomationEventList list);                         // Unload automation events list from file
RAYLIB_RLAPI bool RaylibExportAutomationEventList(RaylibAutomationEventList list, const char *fileName);   // Export automation events list as text file
RAYLIB_RLAPI void RaylibSetAutomationEventList(RaylibAutomationEventList *list);                           // Set automation event list to record to
RAYLIB_RLAPI void RaylibSetAutomationEventBaseFrame(int frame);                                      // Set automation event internal base frame to start recording
RAYLIB_RLAPI void RaylibStartAutomationEventRecording(void);                                         // Start recording automation events (RaylibAutomationEventList must be set)
RAYLIB_RLAPI void RaylibStopAutomationEventRecording(void);                                          // Stop recording automation events
RAYLIB_RLAPI void RaylibPlayAutomationEvent(RaylibAutomationEvent event);                                  // Play a recorded automation event

//------------------------------------------------------------------------------------
// Input Handling Functions (Module: core)
//------------------------------------------------------------------------------------

// Input-related functions: keyboard
RAYLIB_RLAPI bool RaylibIsKeyPressed(int key);                             // Check if a key has been pressed once
RAYLIB_RLAPI bool RaylibIsKeyPressedRepeat(int key);                       // Check if a key has been pressed again
RAYLIB_RLAPI bool RaylibIsKeyDown(int key);                                // Check if a key is being pressed
RAYLIB_RLAPI bool RaylibIsKeyReleased(int key);                            // Check if a key has been released once
RAYLIB_RLAPI bool RaylibIsKeyUp(int key);                                  // Check if a key is NOT being pressed
RAYLIB_RLAPI int RaylibGetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
RAYLIB_RLAPI int RaylibGetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
RAYLIB_RLAPI void RaylibSetExitKey(int key);                               // Set a custom key to exit program (default is ESC)

// Input-related functions: gamepads
RAYLIB_RLAPI bool RaylibIsGamepadAvailable(int gamepad);                                        // Check if a gamepad is available
RAYLIB_RLAPI const char *RaylibGetGamepadName(int gamepad);                                     // Get gamepad internal name id
RAYLIB_RLAPI bool RaylibIsGamepadButtonPressed(int gamepad, int button);                        // Check if a gamepad button has been pressed once
RAYLIB_RLAPI bool RaylibIsGamepadButtonDown(int gamepad, int button);                           // Check if a gamepad button is being pressed
RAYLIB_RLAPI bool RaylibIsGamepadButtonReleased(int gamepad, int button);                       // Check if a gamepad button has been released once
RAYLIB_RLAPI bool RaylibIsGamepadButtonUp(int gamepad, int button);                             // Check if a gamepad button is NOT being pressed
RAYLIB_RLAPI int RaylibGetGamepadButtonPressed(void);                                           // Get the last gamepad button pressed
RAYLIB_RLAPI int RaylibGetGamepadAxisCount(int gamepad);                                        // Get gamepad axis count for a gamepad
RAYLIB_RLAPI float RaylibGetGamepadAxisMovement(int gamepad, int axis);                         // Get axis movement value for a gamepad axis
RAYLIB_RLAPI int RaylibSetGamepadMappings(const char *mappings);                                // Set internal gamepad mappings (SDL_GameControllerDB)
RAYLIB_RLAPI void RaylibSetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration); // Set gamepad vibration for both motors (duration in seconds)

// Input-related functions: mouse
RAYLIB_RLAPI bool RaylibIsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
RAYLIB_RLAPI bool RaylibHasMouseButtonBeenDown(int button);	// Improved version using a queue to catch events happening between two calls to PollInputEvents()
RAYLIB_RLAPI void RaylibClearEventQueue();					// Needed as the previous method doesn't clears the queue
RAYLIB_RLAPI bool RaylibIsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
RAYLIB_RLAPI bool RaylibIsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
RAYLIB_RLAPI bool RaylibIsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
RAYLIB_RLAPI int RaylibGetMouseX(void);                                    // Get mouse position X
RAYLIB_RLAPI int RaylibGetMouseY(void);                                    // Get mouse position Y
RAYLIB_RLAPI RaylibVector2 RaylibGetMousePosition(void);                         // Get mouse position XY
RAYLIB_RLAPI RaylibVector2 RaylibGetMouseDelta(void);                            // Get mouse delta between frames
RAYLIB_RLAPI void RaylibSetMousePosition(int x, int y);                    // Set mouse position XY
RAYLIB_RLAPI void RaylibSetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
RAYLIB_RLAPI void RaylibSetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
RAYLIB_RLAPI float RaylibGetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger
RAYLIB_RLAPI RaylibVector2 RaylibGetMouseWheelMoveV(void);                       // Get mouse wheel movement for both X and Y
RAYLIB_RLAPI void RaylibSetMouseCursor(int cursor);                        // Set mouse cursor

// Input-related functions: touch
RAYLIB_RLAPI int RaylibGetTouchX(void);                                    // Get touch position X for touch point 0 (relative to screen size)
RAYLIB_RLAPI int RaylibGetTouchY(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
RAYLIB_RLAPI RaylibVector2 RaylibGetTouchPosition(int index);                    // Get touch position XY for a touch point index (relative to screen size)
RAYLIB_RLAPI int RaylibGetTouchPointId(int index);                         // Get touch point identifier for given index
RAYLIB_RLAPI int RaylibGetTouchPointCount(void);                           // Get number of touch points

//------------------------------------------------------------------------------------
// Gestures and Touch Handling Functions (Module: rgestures)
//------------------------------------------------------------------------------------
RAYLIB_RLAPI void RaylibSetGesturesEnabled(unsigned int flags);      // Enable a set of gestures using flags
RAYLIB_RLAPI bool RaylibIsGestureDetected(unsigned int gesture);     // Check if a gesture have been detected
RAYLIB_RLAPI int RaylibGetGestureDetected(void);                     // Get latest detected gesture
RAYLIB_RLAPI float RaylibGetGestureHoldDuration(void);               // Get gesture hold time in seconds
RAYLIB_RLAPI RaylibVector2 RaylibGetGestureDragVector(void);               // Get gesture drag vector
RAYLIB_RLAPI float RaylibGetGestureDragAngle(void);                  // Get gesture drag angle
RAYLIB_RLAPI RaylibVector2 RaylibGetGesturePinchVector(void);              // Get gesture pinch delta
RAYLIB_RLAPI float RaylibGetGesturePinchAngle(void);                 // Get gesture pinch angle

//------------------------------------------------------------------------------------
// Camera System Functions (Module: rcamera)
//------------------------------------------------------------------------------------
RAYLIB_RLAPI void RaylibUpdateCamera(Camera *camera, int mode);      // Update camera position for selected mode
RAYLIB_RLAPI void RaylibUpdateCameraPro(Camera *camera, RaylibVector3 movement, RaylibVector3 rotation, float zoom); // Update camera movement/rotation

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
RAYLIB_RLAPI void RaylibSetShapesTexture(Texture2D texture, RaylibRectangle source);       // Set texture and rectangle to be used on shapes drawing
RAYLIB_RLAPI Texture2D RaylibGetShapesTexture(void);                                 // Get texture that is used for shapes drawing
RAYLIB_RLAPI RaylibRectangle RaylibGetShapesTextureRectangle(void);                        // Get texture source rectangle that is used for shapes drawing

// Basic shapes drawing functions
RAYLIB_RLAPI void RaylibDrawPixel(int posX, int posY, RaylibColor color);                                                   // Draw a pixel using geometry [Can be slow, use with care]
RAYLIB_RLAPI void RaylibDrawPixelV(RaylibVector2 position, RaylibColor color);                                                    // Draw a pixel using geometry (Vector version) [Can be slow, use with care]
RAYLIB_RLAPI void RaylibDrawLine(int startPosX, int startPosY, int endPosX, int endPosY, RaylibColor color);                // Draw a line
RAYLIB_RLAPI void RaylibDrawLineV(RaylibVector2 startPos, RaylibVector2 endPos, RaylibColor color);                                     // Draw a line (using gl lines)
RAYLIB_RLAPI void RaylibDrawLineEx(RaylibVector2 startPos, RaylibVector2 endPos, float thick, RaylibColor color);                       // Draw a line (using triangles/quads)
RAYLIB_RLAPI void RaylibDrawLineStrip(const RaylibVector2 *points, int pointCount, RaylibColor color);                            // Draw lines sequence (using gl lines)
RAYLIB_RLAPI void RaylibDrawLineBezier(RaylibVector2 startPos, RaylibVector2 endPos, float thick, RaylibColor color);                   // Draw line segment cubic-bezier in-out interpolation
RAYLIB_RLAPI void RaylibDrawCircle(int centerX, int centerY, float radius, RaylibColor color);                              // Draw a color-filled circle
RAYLIB_RLAPI void RaylibDrawCircleSector(RaylibVector2 center, float radius, float startAngle, float endAngle, int segments, RaylibColor color);      // Draw a piece of a circle
RAYLIB_RLAPI void RaylibDrawCircleSectorLines(RaylibVector2 center, float radius, float startAngle, float endAngle, int segments, RaylibColor color); // Draw circle sector outline
RAYLIB_RLAPI void RaylibDrawCircleGradient(int centerX, int centerY, float radius, RaylibColor inner, RaylibColor outer);         // Draw a gradient-filled circle
RAYLIB_RLAPI void RaylibDrawCircleV(RaylibVector2 center, float radius, RaylibColor color);                                       // Draw a color-filled circle (Vector version)
RAYLIB_RLAPI void RaylibDrawCircleLines(int centerX, int centerY, float radius, RaylibColor color);                         // Draw circle outline
RAYLIB_RLAPI void RaylibDrawCircleLinesV(RaylibVector2 center, float radius, RaylibColor color);                                  // Draw circle outline (Vector version)
RAYLIB_RLAPI void RaylibDrawEllipse(int centerX, int centerY, float radiusH, float radiusV, RaylibColor color);             // Draw ellipse
RAYLIB_RLAPI void RaylibDrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, RaylibColor color);        // Draw ellipse outline
RAYLIB_RLAPI void RaylibDrawRing(RaylibVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RaylibColor color); // Draw ring
RAYLIB_RLAPI void RaylibDrawRingLines(RaylibVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RaylibColor color);    // Draw ring outline
RAYLIB_RLAPI void RaylibDrawRectangle(int posX, int posY, int width, int height, RaylibColor color);                        // Draw a color-filled rectangle
RAYLIB_RLAPI void RaylibDrawRectangleV(RaylibVector2 position, RaylibVector2 size, RaylibColor color);                                  // Draw a color-filled rectangle (Vector version)
RAYLIB_RLAPI void RaylibDrawRectangleRec(RaylibRectangle rec, RaylibColor color);                                                 // Draw a color-filled rectangle
RAYLIB_RLAPI void RaylibDrawRectanglePro(RaylibRectangle rec, RaylibVector2 origin, float rotation, RaylibColor color);                 // Draw a color-filled rectangle with pro parameters
RAYLIB_RLAPI void RaylibDrawRectangleGradientV(int posX, int posY, int width, int height, RaylibColor top, RaylibColor bottom);   // Draw a vertical-gradient-filled rectangle
RAYLIB_RLAPI void RaylibDrawRectangleGradientH(int posX, int posY, int width, int height, RaylibColor left, RaylibColor right);   // Draw a horizontal-gradient-filled rectangle
RAYLIB_RLAPI void RaylibDrawRectangleGradientEx(RaylibRectangle rec, RaylibColor topLeft, RaylibColor bottomLeft, RaylibColor topRight, RaylibColor bottomRight); // Draw a gradient-filled rectangle with custom vertex colors
RAYLIB_RLAPI void RaylibDrawRectangleLines(int posX, int posY, int width, int height, RaylibColor color);                   // Draw rectangle outline
RAYLIB_RLAPI void RaylibDrawRectangleLinesEx(RaylibRectangle rec, float lineThick, RaylibColor color);                            // Draw rectangle outline with extended parameters
RAYLIB_RLAPI void RaylibDrawRectangleRounded(RaylibRectangle rec, float roundness, int segments, RaylibColor color);              // Draw rectangle with rounded edges
RAYLIB_RLAPI void RaylibDrawRectangleRoundedLines(RaylibRectangle rec, float roundness, int segments, RaylibColor color);         // Draw rectangle lines with rounded edges
RAYLIB_RLAPI void RaylibDrawRectangleRoundedLinesEx(RaylibRectangle rec, float roundness, int segments, float lineThick, RaylibColor color); // Draw rectangle with rounded edges outline
RAYLIB_RLAPI void RaylibDrawTriangle(RaylibVector2 v1, RaylibVector2 v2, RaylibVector2 v3, RaylibColor color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
RAYLIB_RLAPI void RaylibDrawTriangleLines(RaylibVector2 v1, RaylibVector2 v2, RaylibVector2 v3, RaylibColor color);                           // Draw triangle outline (vertex in counter-clockwise order!)
RAYLIB_RLAPI void RaylibDrawTriangleFan(const RaylibVector2 *points, int pointCount, RaylibColor color);                          // Draw a triangle fan defined by points (first vertex is the center)
RAYLIB_RLAPI void RaylibDrawTriangleStrip(const RaylibVector2 *points, int pointCount, RaylibColor color);                        // Draw a triangle strip defined by points
RAYLIB_RLAPI void RaylibDrawPoly(RaylibVector2 center, int sides, float radius, float rotation, RaylibColor color);               // Draw a regular polygon (Vector version)
RAYLIB_RLAPI void RaylibDrawPolyLines(RaylibVector2 center, int sides, float radius, float rotation, RaylibColor color);          // Draw a polygon outline of n sides
RAYLIB_RLAPI void RaylibDrawPolyLinesEx(RaylibVector2 center, int sides, float radius, float rotation, float lineThick, RaylibColor color); // Draw a polygon outline of n sides with extended parameters

// Splines drawing functions
RAYLIB_RLAPI void RaylibDrawSplineLinear(const RaylibVector2 *points, int pointCount, float thick, RaylibColor color);                  // Draw spline: Linear, minimum 2 points
RAYLIB_RLAPI void RaylibDrawSplineBasis(const RaylibVector2 *points, int pointCount, float thick, RaylibColor color);                   // Draw spline: B-Spline, minimum 4 points
RAYLIB_RLAPI void RaylibDrawSplineCatmullRom(const RaylibVector2 *points, int pointCount, float thick, RaylibColor color);              // Draw spline: Catmull-Rom, minimum 4 points
RAYLIB_RLAPI void RaylibDrawSplineBezierQuadratic(const RaylibVector2 *points, int pointCount, float thick, RaylibColor color);         // Draw spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
RAYLIB_RLAPI void RaylibDrawSplineBezierCubic(const RaylibVector2 *points, int pointCount, float thick, RaylibColor color);             // Draw spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
RAYLIB_RLAPI void RaylibDrawSplineSegmentLinear(RaylibVector2 p1, RaylibVector2 p2, float thick, RaylibColor color);                    // Draw spline segment: Linear, 2 points
RAYLIB_RLAPI void RaylibDrawSplineSegmentBasis(RaylibVector2 p1, RaylibVector2 p2, RaylibVector2 p3, RaylibVector2 p4, float thick, RaylibColor color); // Draw spline segment: B-Spline, 4 points
RAYLIB_RLAPI void RaylibDrawSplineSegmentCatmullRom(RaylibVector2 p1, RaylibVector2 p2, RaylibVector2 p3, RaylibVector2 p4, float thick, RaylibColor color); // Draw spline segment: Catmull-Rom, 4 points
RAYLIB_RLAPI void RaylibDrawSplineSegmentBezierQuadratic(RaylibVector2 p1, RaylibVector2 c2, RaylibVector2 p3, float thick, RaylibColor color); // Draw spline segment: Quadratic Bezier, 2 points, 1 control point
RAYLIB_RLAPI void RaylibDrawSplineSegmentBezierCubic(RaylibVector2 p1, RaylibVector2 c2, RaylibVector2 c3, RaylibVector2 p4, float thick, RaylibColor color); // Draw spline segment: Cubic Bezier, 2 points, 2 control points

// Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
RAYLIB_RLAPI RaylibVector2 RaylibGetSplinePointLinear(RaylibVector2 startPos, RaylibVector2 endPos, float t);                           // Get (evaluate) spline point: Linear
RAYLIB_RLAPI RaylibVector2 RaylibGetSplinePointBasis(RaylibVector2 p1, RaylibVector2 p2, RaylibVector2 p3, RaylibVector2 p4, float t);              // Get (evaluate) spline point: B-Spline
RAYLIB_RLAPI RaylibVector2 RaylibGetSplinePointCatmullRom(RaylibVector2 p1, RaylibVector2 p2, RaylibVector2 p3, RaylibVector2 p4, float t);         // Get (evaluate) spline point: Catmull-Rom
RAYLIB_RLAPI RaylibVector2 RaylibGetSplinePointBezierQuad(RaylibVector2 p1, RaylibVector2 c2, RaylibVector2 p3, float t);                     // Get (evaluate) spline point: Quadratic Bezier
RAYLIB_RLAPI RaylibVector2 RaylibGetSplinePointBezierCubic(RaylibVector2 p1, RaylibVector2 c2, RaylibVector2 c3, RaylibVector2 p4, float t);        // Get (evaluate) spline point: Cubic Bezier

// Basic shapes collision detection functions
RAYLIB_RLAPI bool RaylibCheckCollisionRecs(RaylibRectangle rec1, RaylibRectangle rec2);                                           // Check collision between two rectangles
RAYLIB_RLAPI bool RaylibCheckCollisionCircles(RaylibVector2 center1, float radius1, RaylibVector2 center2, float radius2);        // Check collision between two circles
RAYLIB_RLAPI bool RaylibCheckCollisionCircleRec(RaylibVector2 center, float radius, RaylibRectangle rec);                         // Check collision between circle and rectangle
RAYLIB_RLAPI bool RaylibCheckCollisionCircleLine(RaylibVector2 center, float radius, RaylibVector2 p1, RaylibVector2 p2);               // Check if circle collides with a line created betweeen two points [p1] and [p2]
RAYLIB_RLAPI bool RaylibCheckCollisionPointRec(RaylibVector2 point, RaylibRectangle rec);                                         // Check if point is inside rectangle
RAYLIB_RLAPI bool RaylibCheckCollisionPointCircle(RaylibVector2 point, RaylibVector2 center, float radius);                       // Check if point is inside circle
RAYLIB_RLAPI bool RaylibCheckCollisionPointTriangle(RaylibVector2 point, RaylibVector2 p1, RaylibVector2 p2, RaylibVector2 p3);               // Check if point is inside a triangle
RAYLIB_RLAPI bool RaylibCheckCollisionPointLine(RaylibVector2 point, RaylibVector2 p1, RaylibVector2 p2, int threshold);                // Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
RAYLIB_RLAPI bool RaylibCheckCollisionPointPoly(RaylibVector2 point, const RaylibVector2 *points, int pointCount);                // Check if point is within a polygon described by array of vertices
RAYLIB_RLAPI bool RaylibCheckCollisionLines(RaylibVector2 startPos1, RaylibVector2 endPos1, RaylibVector2 startPos2, RaylibVector2 endPos2, RaylibVector2 *collisionPoint); // Check the collision between two lines defined by two points each, returns collision point by reference
RAYLIB_RLAPI RaylibRectangle RaylibGetCollisionRec(RaylibRectangle rec1, RaylibRectangle rec2);                                         // Get collision rectangle for two rectangles collision

//------------------------------------------------------------------------------------
// RaylibTexture Loading and Drawing Functions (Module: textures)
//------------------------------------------------------------------------------------

// RaylibImage loading functions
// NOTE: These functions do not require GPU access
RAYLIB_RLAPI RaylibImage RaylibLoadImage(const char *fileName);                                                             // Load image from file into CPU memory (RAM)
RAYLIB_RLAPI RaylibImage RaylibLoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
RAYLIB_RLAPI RaylibImage RaylibLoadImageAnim(const char *fileName, int *frames);                                            // Load image sequence from file (frames appended to image.data)
RAYLIB_RLAPI RaylibImage RaylibLoadImageAnimFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int *frames); // Load image sequence from memory buffer
RAYLIB_RLAPI RaylibImage RaylibLoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
RAYLIB_RLAPI RaylibImage RaylibLoadImageFromTexture(Texture2D texture);                                                     // Load image from GPU texture data
RAYLIB_RLAPI RaylibImage RaylibLoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
RAYLIB_RLAPI bool RaylibIsImageValid(RaylibImage image);                                                                    // Check if an image is valid (data and parameters)
RAYLIB_RLAPI void RaylibUnloadImage(RaylibImage image);                                                                     // Unload image from CPU memory (RAM)
RAYLIB_RLAPI bool RaylibExportImage(RaylibImage image, const char *fileName);                                               // Export image data to file, returns true on success
RAYLIB_RLAPI unsigned char *RaylibExportImageToMemory(RaylibImage image, const char *fileType, int *fileSize);              // Export image to memory buffer
RAYLIB_RLAPI bool RaylibExportImageAsCode(RaylibImage image, const char *fileName);                                         // Export image as code file defining an array of bytes, returns true on success

// RaylibImage generation functions
RAYLIB_RLAPI RaylibImage RaylibGenImageColor(int width, int height, RaylibColor color);                                           // Generate image: plain color
RAYLIB_RLAPI RaylibImage RaylibGenImageGradientLinear(int width, int height, int direction, RaylibColor start, RaylibColor end);        // Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient
RAYLIB_RLAPI RaylibImage RaylibGenImageGradientRadial(int width, int height, float density, RaylibColor inner, RaylibColor outer);      // Generate image: radial gradient
RAYLIB_RLAPI RaylibImage RaylibGenImageGradientSquare(int width, int height, float density, RaylibColor inner, RaylibColor outer);      // Generate image: square gradient
RAYLIB_RLAPI RaylibImage RaylibGenImageChecked(int width, int height, int checksX, int checksY, RaylibColor col1, RaylibColor col2);    // Generate image: checked
RAYLIB_RLAPI RaylibImage RaylibGenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
RAYLIB_RLAPI RaylibImage RaylibGenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
RAYLIB_RLAPI RaylibImage RaylibGenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
RAYLIB_RLAPI RaylibImage RaylibGenImageText(int width, int height, const char *text);                                       // Generate image: grayscale image from text data

// RaylibImage manipulation functions
RAYLIB_RLAPI RaylibImage RaylibImageCopy(RaylibImage image);                                                                      // Create an image duplicate (useful for transformations)
RAYLIB_RLAPI RaylibImage RaylibImageFromImage(RaylibImage image, RaylibRectangle rec);                                                  // Create an image from another image piece
RAYLIB_RLAPI RaylibImage RaylibImageFromChannel(RaylibImage image, int selectedChannel);                                          // Create an image from a selected channel of another image (GRAYSCALE)
RAYLIB_RLAPI RaylibImage RaylibImageText(const char *text, int fontSize, RaylibColor color);                                      // Create an image from text (default font)
RAYLIB_RLAPI RaylibImage RaylibImageTextEx(RaylibFont font, const char *text, float fontSize, float spacing, RaylibColor tint);         // Create an image from text (custom sprite font)
RAYLIB_RLAPI void RaylibImageFormat(RaylibImage *image, int newFormat);                                                     // Convert image data to desired format
RAYLIB_RLAPI void RaylibImageToPOT(RaylibImage *image, RaylibColor fill);                                                         // Convert image to POT (power-of-two)
RAYLIB_RLAPI void RaylibImageCrop(RaylibImage *image, RaylibRectangle crop);                                                      // Crop an image to a defined rectangle
RAYLIB_RLAPI void RaylibImageAlphaCrop(RaylibImage *image, float threshold);                                                // Crop image depending on alpha value
RAYLIB_RLAPI void RaylibImageAlphaClear(RaylibImage *image, RaylibColor color, float threshold);                                  // Clear alpha channel to desired color
RAYLIB_RLAPI void RaylibImageAlphaMask(RaylibImage *image, RaylibImage alphaMask);                                                // Apply alpha mask to image
RAYLIB_RLAPI void RaylibImageAlphaPremultiply(RaylibImage *image);                                                          // Premultiply alpha channel
RAYLIB_RLAPI void RaylibImageBlurGaussian(RaylibImage *image, int blurSize);                                                // Apply Gaussian blur using a box blur approximation
RAYLIB_RLAPI void RaylibImageKernelConvolution(RaylibImage *image, const float *kernel, int kernelSize);                    // Apply custom square convolution kernel to image
RAYLIB_RLAPI void RaylibImageResize(RaylibImage *image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
RAYLIB_RLAPI void RaylibImageResizeNN(RaylibImage *image, int newWidth,int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
RAYLIB_RLAPI void RaylibImageResizeCanvas(RaylibImage *image, int newWidth, int newHeight, int offsetX, int offsetY, RaylibColor fill); // Resize canvas and fill with color
RAYLIB_RLAPI void RaylibImageMipmaps(RaylibImage *image);                                                                   // Compute all mipmap levels for a provided image
RAYLIB_RLAPI void RaylibImageDither(RaylibImage *image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
RAYLIB_RLAPI void RaylibImageFlipVertical(RaylibImage *image);                                                              // Flip image vertically
RAYLIB_RLAPI void RaylibImageFlipHorizontal(RaylibImage *image);                                                            // Flip image horizontally
RAYLIB_RLAPI void RaylibImageRotate(RaylibImage *image, int degrees);                                                       // Rotate image by input angle in degrees (-359 to 359)
RAYLIB_RLAPI void RaylibImageRotateCW(RaylibImage *image);                                                                  // Rotate image clockwise 90deg
RAYLIB_RLAPI void RaylibImageRotateCCW(RaylibImage *image);                                                                 // Rotate image counter-clockwise 90deg
RAYLIB_RLAPI void RaylibImageColorTint(RaylibImage *image, RaylibColor color);                                                    // Modify image color: tint
RAYLIB_RLAPI void RaylibImageColorInvert(RaylibImage *image);                                                               // Modify image color: invert
RAYLIB_RLAPI void RaylibImageColorGrayscale(RaylibImage *image);                                                            // Modify image color: grayscale
RAYLIB_RLAPI void RaylibImageColorContrast(RaylibImage *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
RAYLIB_RLAPI void RaylibImageColorBrightness(RaylibImage *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
RAYLIB_RLAPI void RaylibImageColorReplace(RaylibImage *image, RaylibColor color, RaylibColor replace);                                  // Modify image color: replace color
RAYLIB_RLAPI RaylibColor *RaylibLoadImageColors(RaylibImage image);                                                               // Load color data from image as a RaylibColor array (RGBA - 32bit)
RAYLIB_RLAPI RaylibColor *RaylibLoadImagePalette(RaylibImage image, int maxPaletteSize, int *colorCount);                         // Load colors palette from image as a RaylibColor array (RGBA - 32bit)
RAYLIB_RLAPI void RaylibUnloadImageColors(RaylibColor *colors);                                                             // Unload color data loaded with RaylibLoadImageColors()
RAYLIB_RLAPI void RaylibUnloadImagePalette(RaylibColor *colors);                                                            // Unload colors palette loaded with RaylibLoadImagePalette()
RAYLIB_RLAPI RaylibRectangle RaylibGetImageAlphaBorder(RaylibImage image, float threshold);                                       // Get image alpha border rectangle
RAYLIB_RLAPI RaylibColor RaylibGetImageColor(RaylibImage image, int x, int y);                                                    // Get image pixel color at (x, y) position

// RaylibImage drawing functions
// NOTE: RaylibImage software-rendering functions (CPU)
RAYLIB_RLAPI void RaylibImageClearBackground(RaylibImage *dst, RaylibColor color);                                                // Clear image background with given color
RAYLIB_RLAPI void RaylibImageDrawPixel(RaylibImage *dst, int posX, int posY, RaylibColor color);                                  // Draw pixel within an image
RAYLIB_RLAPI void RaylibImageDrawPixelV(RaylibImage *dst, RaylibVector2 position, RaylibColor color);                                   // Draw pixel within an image (Vector version)
RAYLIB_RLAPI void RaylibImageDrawLine(RaylibImage *dst, int startPosX, int startPosY, int endPosX, int endPosY, RaylibColor color); // Draw line within an image
RAYLIB_RLAPI void RaylibImageDrawLineV(RaylibImage *dst, RaylibVector2 start, RaylibVector2 end, RaylibColor color);                          // Draw line within an image (Vector version)
RAYLIB_RLAPI void RaylibImageDrawLineEx(RaylibImage *dst, RaylibVector2 start, RaylibVector2 end, int thick, RaylibColor color);              // Draw a line defining thickness within an image
RAYLIB_RLAPI void RaylibImageDrawCircle(RaylibImage *dst, int centerX, int centerY, int radius, RaylibColor color);               // Draw a filled circle within an image
RAYLIB_RLAPI void RaylibImageDrawCircleV(RaylibImage *dst, RaylibVector2 center, int radius, RaylibColor color);                        // Draw a filled circle within an image (Vector version)
RAYLIB_RLAPI void RaylibImageDrawCircleLines(RaylibImage *dst, int centerX, int centerY, int radius, RaylibColor color);          // Draw circle outline within an image
RAYLIB_RLAPI void RaylibImageDrawCircleLinesV(RaylibImage *dst, RaylibVector2 center, int radius, RaylibColor color);                   // Draw circle outline within an image (Vector version)
RAYLIB_RLAPI void RaylibImageDrawRectangle(RaylibImage *dst, int posX, int posY, int width, int height, RaylibColor color);       // Draw rectangle within an image
RAYLIB_RLAPI void RaylibImageDrawRectangleV(RaylibImage *dst, RaylibVector2 position, RaylibVector2 size, RaylibColor color);                 // Draw rectangle within an image (Vector version)
RAYLIB_RLAPI void RaylibImageDrawRectangleRec(RaylibImage *dst, RaylibRectangle rec, RaylibColor color);                                // Draw rectangle within an image
RAYLIB_RLAPI void RaylibImageDrawRectangleLines(RaylibImage *dst, RaylibRectangle rec, int thick, RaylibColor color);                   // Draw rectangle lines within an image
RAYLIB_RLAPI void RaylibImageDrawTriangle(RaylibImage *dst, RaylibVector2 v1, RaylibVector2 v2, RaylibVector2 v3, RaylibColor color);               // Draw triangle within an image
RAYLIB_RLAPI void RaylibImageDrawTriangleEx(RaylibImage *dst, RaylibVector2 v1, RaylibVector2 v2, RaylibVector2 v3, RaylibColor c1, RaylibColor c2, RaylibColor c3); // Draw triangle with interpolated colors within an image
RAYLIB_RLAPI void RaylibImageDrawTriangleLines(RaylibImage *dst, RaylibVector2 v1, RaylibVector2 v2, RaylibVector2 v3, RaylibColor color);          // Draw triangle outline within an image
RAYLIB_RLAPI void RaylibImageDrawTriangleFan(RaylibImage *dst, RaylibVector2 *points, int pointCount, RaylibColor color);               // Draw a triangle fan defined by points within an image (first vertex is the center)
RAYLIB_RLAPI void RaylibImageDrawTriangleStrip(RaylibImage *dst, RaylibVector2 *points, int pointCount, RaylibColor color);             // Draw a triangle strip defined by points within an image
RAYLIB_RLAPI void RaylibImageDraw(RaylibImage *dst, RaylibImage src, RaylibRectangle srcRec, RaylibRectangle dstRec, RaylibColor tint);             // Draw a source image within a destination image (tint applied to source)
RAYLIB_RLAPI void RaylibImageDrawText(RaylibImage *dst, const char *text, int posX, int posY, int fontSize, RaylibColor color);   // Draw text (using default font) within an image (destination)
RAYLIB_RLAPI void RaylibImageDrawTextEx(RaylibImage *dst, RaylibFont font, const char *text, RaylibVector2 position, float fontSize, float spacing, RaylibColor tint); // Draw text (custom sprite font) within an image (destination)

// RaylibTexture loading functions
// NOTE: These functions require GPU access
RAYLIB_RLAPI Texture2D RaylibLoadTexture(const char *fileName);                                                       // Load texture from file into GPU memory (VRAM)
RAYLIB_RLAPI Texture2D RaylibLoadTextureFromImage(RaylibImage image);                                                       // Load texture from image data
RAYLIB_RLAPI TextureCubemap RaylibLoadTextureCubemap(RaylibImage image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
RAYLIB_RLAPI RenderTexture2D RaylibLoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
RAYLIB_RLAPI bool RaylibIsTextureValid(Texture2D texture);                                                            // Check if a texture is valid (loaded in GPU)
RAYLIB_RLAPI void RaylibUnloadTexture(Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
RAYLIB_RLAPI bool RaylibIsRenderTextureValid(RenderTexture2D target);                                                 // Check if a render texture is valid (loaded in GPU)
RAYLIB_RLAPI void RaylibUnloadRenderTexture(RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
RAYLIB_RLAPI void RaylibUpdateTexture(Texture2D texture, const void *pixels);                                         // Update GPU texture with new data
RAYLIB_RLAPI void RaylibUpdateTextureRec(Texture2D texture, RaylibRectangle rec, const void *pixels);                       // Update GPU texture rectangle with new data

// RaylibTexture configuration functions
RAYLIB_RLAPI void RaylibGenTextureMipmaps(Texture2D *texture);                                                        // Generate GPU mipmaps for a texture
RAYLIB_RLAPI void RaylibSetTextureFilter(Texture2D texture, int filter);                                              // Set texture scaling filter mode
RAYLIB_RLAPI void RaylibSetTextureWrap(Texture2D texture, int wrap);                                                  // Set texture wrapping mode

// RaylibTexture drawing functions
RAYLIB_RLAPI void RaylibDrawTexture(Texture2D texture, int posX, int posY, RaylibColor tint);                               // Draw a Texture2D
RAYLIB_RLAPI void RaylibDrawTextureV(Texture2D texture, RaylibVector2 position, RaylibColor tint);                                // Draw a Texture2D with position defined as RaylibVector2
RAYLIB_RLAPI void RaylibDrawTextureEx(Texture2D texture, RaylibVector2 position, float rotation, float scale, RaylibColor tint);  // Draw a Texture2D with extended parameters
RAYLIB_RLAPI void RaylibDrawTextureRec(Texture2D texture, RaylibRectangle source, RaylibVector2 position, RaylibColor tint);            // Draw a part of a texture defined by a rectangle
RAYLIB_RLAPI void RaylibDrawTexturePro(Texture2D texture, RaylibRectangle source, RaylibRectangle dest, RaylibVector2 origin, float rotation, RaylibColor tint); // Draw a part of a texture defined by a rectangle with 'pro' parameters
RAYLIB_RLAPI void RaylibDrawTextureNPatch(Texture2D texture, RaylibNPatchInfo nPatchInfo, RaylibRectangle dest, RaylibVector2 origin, float rotation, RaylibColor tint); // Draws a texture (or part of it) that stretches or shrinks nicely

// RaylibColor/pixel related functions
RAYLIB_RLAPI bool RaylibColorIsEqual(RaylibColor col1, RaylibColor col2);                            // Check if two colors are equal
RAYLIB_RLAPI RaylibColor RaylibFade(RaylibColor color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RAYLIB_RLAPI int RaylibColorToInt(RaylibColor color);                                          // Get hexadecimal value for a RaylibColor (0xRRGGBBAA)
RAYLIB_RLAPI RaylibVector4 RaylibColorNormalize(RaylibColor color);                                  // Get RaylibColor normalized as float [0..1]
RAYLIB_RLAPI RaylibColor RaylibColorFromNormalized(RaylibVector4 normalized);                        // Get RaylibColor from normalized values [0..1]
RAYLIB_RLAPI RaylibVector3 RaylibColorToHSV(RaylibColor color);                                      // Get HSV values for a RaylibColor, hue [0..360], saturation/value [0..1]
RAYLIB_RLAPI RaylibColor RaylibColorFromHSV(float hue, float saturation, float value);         // Get a RaylibColor from HSV values, hue [0..360], saturation/value [0..1]
RAYLIB_RLAPI RaylibColor RaylibColorTint(RaylibColor color, RaylibColor tint);                             // Get color multiplied with another color
RAYLIB_RLAPI RaylibColor RaylibColorBrightness(RaylibColor color, float factor);                     // Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
RAYLIB_RLAPI RaylibColor RaylibColorContrast(RaylibColor color, float contrast);                     // Get color with contrast correction, contrast values between -1.0f and 1.0f
RAYLIB_RLAPI RaylibColor RaylibColorAlpha(RaylibColor color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RAYLIB_RLAPI RaylibColor RaylibColorAlphaBlend(RaylibColor dst, RaylibColor src, RaylibColor tint);              // Get src alpha-blended into dst color with tint
RAYLIB_RLAPI RaylibColor RaylibColorLerp(RaylibColor color1, RaylibColor color2, float factor);            // Get color lerp interpolation between two colors, factor [0.0f..1.0f]
RAYLIB_RLAPI RaylibColor RaylibGetColor(unsigned int hexValue);                                // Get RaylibColor structure from hexadecimal value
RAYLIB_RLAPI RaylibColor RaylibGetPixelColor(void *srcPtr, int format);                        // Get RaylibColor from a source pixel pointer of certain format
RAYLIB_RLAPI void RaylibSetPixelColor(void *dstPtr, RaylibColor color, int format);            // Set color formatted into destination pixel pointer
RAYLIB_RLAPI int RaylibGetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format

//------------------------------------------------------------------------------------
// RaylibFont Loading and Text Drawing Functions (Module: text)
//------------------------------------------------------------------------------------

// RaylibFont loading/unloading functions
RAYLIB_RLAPI RaylibFont RaylibGetFontDefault(void);                                                            // Get the default RaylibFont
RAYLIB_RLAPI RaylibFont RaylibLoadFont(const char *fileName);                                                  // Load font from file into GPU memory (VRAM)
RAYLIB_RLAPI RaylibFont RaylibLoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount); // Load font from file with extended parameters, use NULL for codepoints and 0 for codepointCount to load the default character set, font size is provided in pixels height
RAYLIB_RLAPI RaylibFont RaylibLoadFontFromImage(RaylibImage image, RaylibColor key, int firstChar);                        // Load font from RaylibImage (XNA style)
RAYLIB_RLAPI RaylibFont RaylibLoadFontFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
RAYLIB_RLAPI bool RaylibIsFontValid(RaylibFont font);                                                          // Check if a font is valid (font data loaded, WARNING: GPU texture not checked)
RAYLIB_RLAPI RaylibGlyphInfo *RaylibLoadFontData(const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount, int type); // Load font data for further use
RAYLIB_RLAPI RaylibImage RaylibGenImageFontAtlas(const RaylibGlyphInfo *glyphs, RaylibRectangle **glyphRecs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
RAYLIB_RLAPI void RaylibUnloadFontData(RaylibGlyphInfo *glyphs, int glyphCount);                               // Unload font chars info data (RAM)
RAYLIB_RLAPI void RaylibUnloadFont(RaylibFont font);                                                           // Unload font from GPU memory (VRAM)
RAYLIB_RLAPI bool RaylibExportFontAsCode(RaylibFont font, const char *fileName);                               // Export font as code file, returns true on success

// Text drawing functions
RAYLIB_RLAPI void RaylibDrawFPS(int posX, int posY);                                                     // Draw current FPS
RAYLIB_RLAPI void RaylibDrawText(const char *text, int posX, int posY, int fontSize, RaylibColor color);       // Draw text (using default font)
RAYLIB_RLAPI void RaylibDrawTextEx(RaylibFont font, const char *text, RaylibVector2 position, float fontSize, float spacing, RaylibColor tint); // Draw text using font and additional parameters
RAYLIB_RLAPI void RaylibDrawTextPro(RaylibFont font, const char *text, RaylibVector2 position, RaylibVector2 origin, float rotation, float fontSize, float spacing, RaylibColor tint); // Draw text using RaylibFont and pro parameters (rotation)
RAYLIB_RLAPI void RaylibDrawTextCodepoint(RaylibFont font, int codepoint, RaylibVector2 position, float fontSize, RaylibColor tint); // Draw one character (codepoint)
RAYLIB_RLAPI void RaylibDrawTextCodepoints(RaylibFont font, const int *codepoints, int codepointCount, RaylibVector2 position, float fontSize, float spacing, RaylibColor tint); // Draw multiple character (codepoint)

// Text font info functions
RAYLIB_RLAPI void RaylibSetTextLineSpacing(int spacing);                                                 // Set vertical line spacing when drawing with line-breaks
RAYLIB_RLAPI int RaylibMeasureText(const char *text, int fontSize);                                      // Measure string width for default font
RAYLIB_RLAPI RaylibVector2 RaylibMeasureTextEx(RaylibFont font, const char *text, float fontSize, float spacing);    // Measure string size for RaylibFont
RAYLIB_RLAPI int RaylibGetGlyphIndex(RaylibFont font, int codepoint);                                          // Get glyph index position in font for a codepoint (unicode character), fallback to '?' if not found
RAYLIB_RLAPI RaylibGlyphInfo RaylibGetGlyphInfo(RaylibFont font, int codepoint);                                     // Get glyph font info data for a codepoint (unicode character), fallback to '?' if not found
RAYLIB_RLAPI RaylibRectangle RaylibGetGlyphAtlasRec(RaylibFont font, int codepoint);                                 // Get glyph rectangle in font atlas for a codepoint (unicode character), fallback to '?' if not found

// Text codepoints management functions (unicode characters)
RAYLIB_RLAPI char *RaylibLoadUTF8(const int *codepoints, int length);                // Load UTF-8 text encoded from codepoints array
RAYLIB_RLAPI void RaylibUnloadUTF8(char *text);                                      // Unload UTF-8 text encoded from codepoints array
RAYLIB_RLAPI int *RaylibLoadCodepoints(const char *text, int *count);                // Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
RAYLIB_RLAPI void RaylibUnloadCodepoints(int *codepoints);                           // Unload codepoints data from memory
RAYLIB_RLAPI int RaylibGetCodepointCount(const char *text);                          // Get total number of codepoints in a UTF-8 encoded string
RAYLIB_RLAPI int RaylibGetCodepoint(const char *text, int *codepointSize);           // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RAYLIB_RLAPI int RaylibGetCodepointNext(const char *text, int *codepointSize);       // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RAYLIB_RLAPI int RaylibGetCodepointPrevious(const char *text, int *codepointSize);   // Get previous codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RAYLIB_RLAPI const char *RaylibCodepointToUTF8(int codepoint, int *utf8Size);        // Encode one codepoint into UTF-8 byte array (array length returned as parameter)

// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
RAYLIB_RLAPI int RaylibTextCopy(char *dst, const char *src);                                             // Copy one string to another, returns bytes copied
RAYLIB_RLAPI bool RaylibTextIsEqual(const char *text1, const char *text2);                               // Check if two text string are equal
RAYLIB_RLAPI unsigned int RaylibTextLength(const char *text);                                            // Get text length, checks for '\0' ending
RAYLIB_RLAPI const char *RaylibTextFormat(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
RAYLIB_RLAPI const char *RaylibTextSubtext(const char *text, int position, int length);                  // Get a piece of a text string
RAYLIB_RLAPI char *RaylibTextReplace(const char *text, const char *replace, const char *by);             // Replace text string (WARNING: memory must be freed!)
RAYLIB_RLAPI char *RaylibTextInsert(const char *text, const char *insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
RAYLIB_RLAPI const char *RaylibTextJoin(const char **textList, int count, const char *delimiter);        // Join text strings with delimiter
RAYLIB_RLAPI const char **RaylibTextSplit(const char *text, char delimiter, int *count);                 // Split text into multiple strings
RAYLIB_RLAPI void RaylibTextAppend(char *text, const char *append, int *position);                       // Append text at specific position and move cursor!
RAYLIB_RLAPI int RaylibTextFindIndex(const char *text, const char *find);                                // Find first text occurrence within a string
RAYLIB_RLAPI const char *RaylibTextToUpper(const char *text);                      // Get upper case version of provided string
RAYLIB_RLAPI const char *RaylibTextToLower(const char *text);                      // Get lower case version of provided string
RAYLIB_RLAPI const char *RaylibTextToPascal(const char *text);                     // Get Pascal case notation version of provided string
RAYLIB_RLAPI const char *RaylibTextToSnake(const char *text);                      // Get Snake case notation version of provided string
RAYLIB_RLAPI const char *RaylibTextToCamel(const char *text);                      // Get Camel case notation version of provided string

RAYLIB_RLAPI int RaylibTextToInteger(const char *text);                            // Get integer value from text (negative values not supported)
RAYLIB_RLAPI float RaylibTextToFloat(const char *text);                            // Get float value from text (negative values not supported)

//------------------------------------------------------------------------------------
// Basic 3d Shapes Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// Basic geometric 3D shapes drawing functions
RAYLIB_RLAPI void RaylibDrawLine3D(RaylibVector3 startPos, RaylibVector3 endPos, RaylibColor color);                                    // Draw a line in 3D world space
RAYLIB_RLAPI void RaylibDrawPoint3D(RaylibVector3 position, RaylibColor color);                                                   // Draw a point in 3D space, actually a small line
RAYLIB_RLAPI void RaylibDrawCircle3D(RaylibVector3 center, float radius, RaylibVector3 rotationAxis, float rotationAngle, RaylibColor color); // Draw a circle in 3D world space
RAYLIB_RLAPI void RaylibDrawTriangle3D(RaylibVector3 v1, RaylibVector3 v2, RaylibVector3 v3, RaylibColor color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
RAYLIB_RLAPI void RaylibDrawTriangleStrip3D(const RaylibVector3 *points, int pointCount, RaylibColor color);                      // Draw a triangle strip defined by points
RAYLIB_RLAPI void RaylibDrawCube(RaylibVector3 position, float width, float height, float length, RaylibColor color);             // Draw cube
RAYLIB_RLAPI void RaylibDrawCubeV(RaylibVector3 position, RaylibVector3 size, RaylibColor color);                                       // Draw cube (Vector version)
RAYLIB_RLAPI void RaylibDrawCubeWires(RaylibVector3 position, float width, float height, float length, RaylibColor color);        // Draw cube wires
RAYLIB_RLAPI void RaylibDrawCubeWiresV(RaylibVector3 position, RaylibVector3 size, RaylibColor color);                                  // Draw cube wires (Vector version)
RAYLIB_RLAPI void RaylibDrawSphere(RaylibVector3 centerPos, float radius, RaylibColor color);                                     // Draw sphere
RAYLIB_RLAPI void RaylibDrawSphereEx(RaylibVector3 centerPos, float radius, int rings, int slices, RaylibColor color);            // Draw sphere with extended parameters
RAYLIB_RLAPI void RaylibDrawSphereWires(RaylibVector3 centerPos, float radius, int rings, int slices, RaylibColor color);         // Draw sphere wires
RAYLIB_RLAPI void RaylibDrawCylinder(RaylibVector3 position, float radiusTop, float radiusBottom, float height, int slices, RaylibColor color); // Draw a cylinder/cone
RAYLIB_RLAPI void RaylibDrawCylinderEx(RaylibVector3 startPos, RaylibVector3 endPos, float startRadius, float endRadius, int sides, RaylibColor color); // Draw a cylinder with base at startPos and top at endPos
RAYLIB_RLAPI void RaylibDrawCylinderWires(RaylibVector3 position, float radiusTop, float radiusBottom, float height, int slices, RaylibColor color); // Draw a cylinder/cone wires
RAYLIB_RLAPI void RaylibDrawCylinderWiresEx(RaylibVector3 startPos, RaylibVector3 endPos, float startRadius, float endRadius, int sides, RaylibColor color); // Draw a cylinder wires with base at startPos and top at endPos
RAYLIB_RLAPI void RaylibDrawCapsule(RaylibVector3 startPos, RaylibVector3 endPos, float radius, int slices, int rings, RaylibColor color); // Draw a capsule with the center of its sphere caps at startPos and endPos
RAYLIB_RLAPI void RaylibDrawCapsuleWires(RaylibVector3 startPos, RaylibVector3 endPos, float radius, int slices, int rings, RaylibColor color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
RAYLIB_RLAPI void RaylibDrawPlane(RaylibVector3 centerPos, RaylibVector2 size, RaylibColor color);                                      // Draw a plane XZ
RAYLIB_RLAPI void RaylibDrawRay(RaylibRay ray, RaylibColor color);                                                                // Draw a ray line
RAYLIB_RLAPI void RaylibDrawGrid(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

//------------------------------------------------------------------------------------
// RaylibModel 3d Loading and Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// RaylibModel management functions
RAYLIB_RLAPI RaylibModel RaylibLoadModel(const char *fileName);                                                // Load model from files (meshes and materials)
RAYLIB_RLAPI RaylibModel RaylibLoadModelFromMesh(RaylibMesh mesh);                                                   // Load model from generated mesh (default material)
RAYLIB_RLAPI bool RaylibIsModelValid(RaylibModel model);                                                       // Check if a model is valid (loaded in GPU, VAO/VBOs)
RAYLIB_RLAPI void RaylibUnloadModel(RaylibModel model);                                                        // Unload model (including meshes) from memory (RAM and/or VRAM)
RAYLIB_RLAPI RaylibBoundingBox RaylibGetModelBoundingBox(RaylibModel model);                                         // Compute model bounding box limits (considers all meshes)

// RaylibModel drawing functions
RAYLIB_RLAPI void RaylibDrawModel(RaylibModel model, RaylibVector3 position, float scale, RaylibColor tint);               // Draw a model (with texture if set)
RAYLIB_RLAPI void RaylibDrawModelEx(RaylibModel model, RaylibVector3 position, RaylibVector3 rotationAxis, float rotationAngle, RaylibVector3 scale, RaylibColor tint); // Draw a model with extended parameters
RAYLIB_RLAPI void RaylibDrawModelWires(RaylibModel model, RaylibVector3 position, float scale, RaylibColor tint);          // Draw a model wires (with texture if set)
RAYLIB_RLAPI void RaylibDrawModelWiresEx(RaylibModel model, RaylibVector3 position, RaylibVector3 rotationAxis, float rotationAngle, RaylibVector3 scale, RaylibColor tint); // Draw a model wires (with texture if set) with extended parameters
RAYLIB_RLAPI void RaylibDrawModelPoints(RaylibModel model, RaylibVector3 position, float scale, RaylibColor tint); // Draw a model as points
RAYLIB_RLAPI void RaylibDrawModelPointsEx(RaylibModel model, RaylibVector3 position, RaylibVector3 rotationAxis, float rotationAngle, RaylibVector3 scale, RaylibColor tint); // Draw a model as points with extended parameters
RAYLIB_RLAPI void RaylibDrawBoundingBox(RaylibBoundingBox box, RaylibColor color);                                   // Draw bounding box (wires)
RAYLIB_RLAPI void RaylibDrawBillboard(Camera camera, Texture2D texture, RaylibVector3 position, float scale, RaylibColor tint);   // Draw a billboard texture
RAYLIB_RLAPI void RaylibDrawBillboardRec(Camera camera, Texture2D texture, RaylibRectangle source, RaylibVector3 position, RaylibVector2 size, RaylibColor tint); // Draw a billboard texture defined by source
RAYLIB_RLAPI void RaylibDrawBillboardPro(Camera camera, Texture2D texture, RaylibRectangle source, RaylibVector3 position, RaylibVector3 up, RaylibVector2 size, RaylibVector2 origin, float rotation, RaylibColor tint); // Draw a billboard texture defined by source and rotation

// RaylibMesh management functions
RAYLIB_RLAPI void RaylibUploadMesh(RaylibMesh *mesh, bool dynamic);                                            // Upload mesh vertex data in GPU and provide VAO/VBO ids
RAYLIB_RLAPI void RaylibUpdateMeshBuffer(RaylibMesh mesh, int index, const void *data, int dataSize, int offset); // Update mesh vertex data in GPU for a specific buffer index
RAYLIB_RLAPI void RaylibUnloadMesh(RaylibMesh mesh);                                                           // Unload mesh data from CPU and GPU
RAYLIB_RLAPI void RaylibDrawMesh(RaylibMesh mesh, RaylibMaterial material, RaylibMatrix transform);                        // Draw a 3d mesh with material and transform
RAYLIB_RLAPI void RaylibDrawMeshInstanced(RaylibMesh mesh, RaylibMaterial material, const RaylibMatrix *transforms, int instances); // Draw multiple mesh instances with material and different transforms
RAYLIB_RLAPI RaylibBoundingBox RaylibGetMeshBoundingBox(RaylibMesh mesh);                                            // Compute mesh bounding box limits
RAYLIB_RLAPI void RaylibGenMeshTangents(RaylibMesh *mesh);                                                     // Compute mesh tangents
RAYLIB_RLAPI bool RaylibExportMesh(RaylibMesh mesh, const char *fileName);                                     // Export mesh data to file, returns true on success
RAYLIB_RLAPI bool RaylibExportMeshAsCode(RaylibMesh mesh, const char *fileName);                               // Export mesh as code file (.h) defining multiple arrays of vertex attributes

// RaylibMesh generation functions
RAYLIB_RLAPI RaylibMesh RaylibGenMeshPoly(int sides, float radius);                                            // Generate polygonal mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshPlane(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
RAYLIB_RLAPI RaylibMesh RaylibGenMeshCube(float width, float height, float length);                            // Generate cuboid mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshSphere(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
RAYLIB_RLAPI RaylibMesh RaylibGenMeshHemiSphere(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
RAYLIB_RLAPI RaylibMesh RaylibGenMeshCylinder(float radius, float height, int slices);                         // Generate cylinder mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshCone(float radius, float height, int slices);                             // Generate cone/pyramid mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshTorus(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshKnot(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
RAYLIB_RLAPI RaylibMesh RaylibGenMeshHeightmap(RaylibImage heightmap, RaylibVector3 size);                                 // Generate heightmap mesh from image data
RAYLIB_RLAPI RaylibMesh RaylibGenMeshCubicmap(RaylibImage cubicmap, RaylibVector3 cubeSize);                               // Generate cubes-based map mesh from image data

// RaylibMaterial loading/unloading functions
RAYLIB_RLAPI RaylibMaterial *RaylibLoadMaterials(const char *fileName, int *materialCount);                    // Load materials from model file
RAYLIB_RLAPI RaylibMaterial RaylibLoadMaterialDefault(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
RAYLIB_RLAPI bool RaylibIsMaterialValid(RaylibMaterial material);                                              // Check if a material is valid (shader assigned, map textures loaded in GPU)
RAYLIB_RLAPI void RaylibUnloadMaterial(RaylibMaterial material);                                               // Unload material from GPU memory (VRAM)
RAYLIB_RLAPI void RaylibSetMaterialTexture(RaylibMaterial *material, int mapType, Texture2D texture);          // Set texture for a material map type (RAYLIB_MATERIAL_MAP_DIFFUSE, RAYLIB_MATERIAL_MAP_SPECULAR...)
RAYLIB_RLAPI void RaylibSetModelMeshMaterial(RaylibModel *model, int meshId, int materialId);                  // Set material for a mesh

// RaylibModel animations loading/unloading functions
RAYLIB_RLAPI RaylibModelAnimation *RaylibLoadModelAnimations(const char *fileName, int *animCount);            // Load model animations from file
RAYLIB_RLAPI void RaylibUpdateModelAnimation(RaylibModel model, RaylibModelAnimation anim, int frame);               // Update model animation pose (CPU)
RAYLIB_RLAPI void RaylibUpdateModelAnimationBones(RaylibModel model, RaylibModelAnimation anim, int frame);          // Update model animation mesh bone matrices (GPU skinning)
RAYLIB_RLAPI void RaylibUnloadModelAnimation(RaylibModelAnimation anim);                                       // Unload animation data
RAYLIB_RLAPI void RaylibUnloadModelAnimations(RaylibModelAnimation *animations, int animCount);                // Unload animation array data
RAYLIB_RLAPI bool RaylibIsModelAnimationValid(RaylibModel model, RaylibModelAnimation anim);                         // Check model animation skeleton match

// Collision detection functions
RAYLIB_RLAPI bool RaylibCheckCollisionSpheres(RaylibVector3 center1, float radius1, RaylibVector3 center2, float radius2);   // Check collision between two spheres
RAYLIB_RLAPI bool RaylibCheckCollisionBoxes(RaylibBoundingBox box1, RaylibBoundingBox box2);                                 // Check collision between two bounding boxes
RAYLIB_RLAPI bool RaylibCheckCollisionBoxSphere(RaylibBoundingBox box, RaylibVector3 center, float radius);                  // Check collision between box and sphere
RAYLIB_RLAPI RaylibRayCollision RaylibGetRayCollisionSphere(RaylibRay ray, RaylibVector3 center, float radius);                    // Get collision info between ray and sphere
RAYLIB_RLAPI RaylibRayCollision RaylibGetRayCollisionBox(RaylibRay ray, RaylibBoundingBox box);                                    // Get collision info between ray and box
RAYLIB_RLAPI RaylibRayCollision RaylibGetRayCollisionMesh(RaylibRay ray, RaylibMesh mesh, RaylibMatrix transform);                       // Get collision info between ray and mesh
RAYLIB_RLAPI RaylibRayCollision RaylibGetRayCollisionTriangle(RaylibRay ray, RaylibVector3 p1, RaylibVector3 p2, RaylibVector3 p3);            // Get collision info between ray and triangle
RAYLIB_RLAPI RaylibRayCollision RaylibGetRayCollisionQuad(RaylibRay ray, RaylibVector3 p1, RaylibVector3 p2, RaylibVector3 p3, RaylibVector3 p4);    // Get collision info between ray and quad

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------
typedef void (*RaylibAudioCallback)(void *bufferData, unsigned int frames);

// Audio device management functions
RAYLIB_RLAPI void RaylibInitAudioDevice(void);                                     // Initialize audio device and context
RAYLIB_RLAPI void RaylibCloseAudioDevice(void);                                    // Close the audio device and context
RAYLIB_RLAPI bool RaylibIsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
RAYLIB_RLAPI void RaylibSetMasterVolume(float volume);                             // Set master volume (listener)
RAYLIB_RLAPI float RaylibGetMasterVolume(void);                                    // Get master volume (listener)

// RaylibWave/RaylibSound loading/unloading functions
RAYLIB_RLAPI RaylibWave RaylibLoadWave(const char *fileName);                            // Load wave data from file
RAYLIB_RLAPI RaylibWave RaylibLoadWaveFromMemory(const char *fileType, const unsigned char *fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
RAYLIB_RLAPI bool RaylibIsWaveValid(RaylibWave wave);                                    // Checks if wave data is valid (data loaded and parameters)
RAYLIB_RLAPI RaylibSound RaylibLoadSound(const char *fileName);                          // Load sound from file
RAYLIB_RLAPI RaylibSound RaylibLoadSoundFromWave(RaylibWave wave);                             // Load sound from wave data
RAYLIB_RLAPI RaylibSound RaylibLoadSoundAlias(RaylibSound source);                             // Create a new sound that shares the same sample data as the source sound, does not own the sound data
RAYLIB_RLAPI bool RaylibIsSoundValid(RaylibSound sound);                                 // Checks if a sound is valid (data loaded and buffers initialized)
RAYLIB_RLAPI void RaylibUpdateSound(RaylibSound sound, const void *data, int sampleCount); // Update sound buffer with new data
RAYLIB_RLAPI void RaylibUnloadWave(RaylibWave wave);                                     // Unload wave data
RAYLIB_RLAPI void RaylibUnloadSound(RaylibSound sound);                                  // Unload sound
RAYLIB_RLAPI void RaylibUnloadSoundAlias(RaylibSound alias);                             // Unload a sound alias (does not deallocate sample data)
RAYLIB_RLAPI bool RaylibExportWave(RaylibWave wave, const char *fileName);               // Export wave data to file, returns true on success
RAYLIB_RLAPI bool RaylibExportWaveAsCode(RaylibWave wave, const char *fileName);         // Export wave sample data to code (.h), returns true on success

// RaylibWave/RaylibSound management functions
RAYLIB_RLAPI void RaylibPlaySound(RaylibSound sound);                                    // Play a sound
RAYLIB_RLAPI void RaylibStopSound(RaylibSound sound);                                    // Stop playing a sound
RAYLIB_RLAPI void RaylibPauseSound(RaylibSound sound);                                   // Pause a sound
RAYLIB_RLAPI void RaylibResumeSound(RaylibSound sound);                                  // Resume a paused sound
RAYLIB_RLAPI bool RaylibIsSoundPlaying(RaylibSound sound);                               // Check if a sound is currently playing
RAYLIB_RLAPI void RaylibSetSoundVolume(RaylibSound sound, float volume);                 // Set volume for a sound (1.0 is max level)
RAYLIB_RLAPI void RaylibSetSoundPitch(RaylibSound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
RAYLIB_RLAPI void RaylibSetSoundPan(RaylibSound sound, float pan);                       // Set pan for a sound (0.5 is center)
RAYLIB_RLAPI RaylibWave RaylibWaveCopy(RaylibWave wave);                                       // Copy a wave to a new wave
RAYLIB_RLAPI void RaylibWaveCrop(RaylibWave *wave, int initFrame, int finalFrame);       // Crop a wave to defined frames range
RAYLIB_RLAPI void RaylibWaveFormat(RaylibWave *wave, int sampleRate, int sampleSize, int channels); // Convert wave data to desired format
RAYLIB_RLAPI float *RaylibLoadWaveSamples(RaylibWave wave);                              // Load samples data from wave as a 32bit float data array
RAYLIB_RLAPI void RaylibUnloadWaveSamples(float *samples);                         // Unload samples data loaded with RaylibLoadWaveSamples()

// RaylibMusic management functions
RAYLIB_RLAPI RaylibMusic RaylibLoadMusicStream(const char *fileName);                    // Load music stream from file
RAYLIB_RLAPI RaylibMusic RaylibLoadMusicStreamFromMemory(const char *fileType, const unsigned char *data, int dataSize); // Load music stream from data
RAYLIB_RLAPI bool RaylibIsMusicValid(RaylibMusic music);                                 // Checks if a music stream is valid (context and buffers initialized)
RAYLIB_RLAPI void RaylibUnloadMusicStream(RaylibMusic music);                            // Unload music stream
RAYLIB_RLAPI void RaylibPlayMusicStream(RaylibMusic music);                              // Start music playing
RAYLIB_RLAPI bool RaylibIsMusicStreamPlaying(RaylibMusic music);                         // Check if music is playing
RAYLIB_RLAPI void RaylibUpdateMusicStream(RaylibMusic music);                            // Updates buffers for music streaming
RAYLIB_RLAPI void RaylibStopMusicStream(RaylibMusic music);                              // Stop music playing
RAYLIB_RLAPI void RaylibPauseMusicStream(RaylibMusic music);                             // Pause music playing
RAYLIB_RLAPI void RaylibResumeMusicStream(RaylibMusic music);                            // Resume playing paused music
RAYLIB_RLAPI void RaylibSeekMusicStream(RaylibMusic music, float position);              // Seek music to a position (in seconds)
RAYLIB_RLAPI void RaylibSetMusicVolume(RaylibMusic music, float volume);                 // Set volume for music (1.0 is max level)
RAYLIB_RLAPI void RaylibSetMusicPitch(RaylibMusic music, float pitch);                   // Set pitch for a music (1.0 is base level)
RAYLIB_RLAPI void RaylibSetMusicPan(RaylibMusic music, float pan);                       // Set pan for a music (0.5 is center)
RAYLIB_RLAPI float RaylibGetMusicTimeLength(RaylibMusic music);                          // Get music time length (in seconds)
RAYLIB_RLAPI float RaylibGetMusicTimePlayed(RaylibMusic music);                          // Get current music time played (in seconds)

// RaylibAudioStream management functions
RAYLIB_RLAPI RaylibAudioStream RaylibLoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
RAYLIB_RLAPI bool RaylibIsAudioStreamValid(RaylibAudioStream stream);                    // Checks if an audio stream is valid (buffers initialized)
RAYLIB_RLAPI void RaylibUnloadAudioStream(RaylibAudioStream stream);                     // Unload audio stream and free memory
RAYLIB_RLAPI void RaylibUpdateAudioStream(RaylibAudioStream stream, const void *data, int frameCount); // Update audio stream buffers with data
RAYLIB_RLAPI bool RaylibIsAudioStreamProcessed(RaylibAudioStream stream);                // Check if any audio stream buffers requires refill
RAYLIB_RLAPI void RaylibPlayAudioStream(RaylibAudioStream stream);                       // Play audio stream
RAYLIB_RLAPI void RaylibPauseAudioStream(RaylibAudioStream stream);                      // Pause audio stream
RAYLIB_RLAPI void RaylibResumeAudioStream(RaylibAudioStream stream);                     // Resume audio stream
RAYLIB_RLAPI bool RaylibIsAudioStreamPlaying(RaylibAudioStream stream);                  // Check if audio stream is playing
RAYLIB_RLAPI void RaylibStopAudioStream(RaylibAudioStream stream);                       // Stop audio stream
RAYLIB_RLAPI void RaylibSetAudioStreamVolume(RaylibAudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
RAYLIB_RLAPI void RaylibSetAudioStreamPitch(RaylibAudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
RAYLIB_RLAPI void RaylibSetAudioStreamPan(RaylibAudioStream stream, float pan);          // Set pan for audio stream (0.5 is centered)
RAYLIB_RLAPI void RaylibSetAudioStreamBufferSizeDefault(int size);                 // Default size for new audio streams
RAYLIB_RLAPI void RaylibSetAudioStreamCallback(RaylibAudioStream stream, RaylibAudioCallback callback); // Audio thread callback to request new data

RAYLIB_RLAPI void RaylibAttachAudioStreamProcessor(RaylibAudioStream stream, RaylibAudioCallback processor); // Attach audio stream processor to stream, receives the samples as 'float'
RAYLIB_RLAPI void RaylibDetachAudioStreamProcessor(RaylibAudioStream stream, RaylibAudioCallback processor); // Detach audio stream processor from stream

RAYLIB_RLAPI void RaylibAttachAudioMixedProcessor(RaylibAudioCallback processor); // Attach audio stream processor to the entire audio pipeline, receives the samples as 'float'
RAYLIB_RLAPI void RaylibDetachAudioMixedProcessor(RaylibAudioCallback processor); // Detach audio stream processor from the entire audio pipeline

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_H
