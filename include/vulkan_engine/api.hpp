#pragma once

// Only apply visibility macros if building a shared library
#if defined(VKENGINE_SHARED)
    #if defined(_WIN32)
        #if defined(VKENGINE_BUILD_DLL)
            #define VKENGINE_API __declspec(dllexport) // Engine is building
        #else
            #define VKENGINE_API __declspec(dllimport) // Client is consuming
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(VKENGINE_BUILD_DLL)
            #define VKENGINE_API __attribute__((visibility("default")))
        #else
            #define VKENGINE_API
        #endif
    #else
        #define VKENGINE_API
    #endif
#else
    // Static library fallback
    #define VKENGINE_API
#endif