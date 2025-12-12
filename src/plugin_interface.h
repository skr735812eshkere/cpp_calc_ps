#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#ifdef _WIN32
    #ifdef BUILDING_PLUGIN
        #define PLUGIN_API extern "C" __declspec(dllexport)
    #else
        #define PLUGIN_API extern "C" __declspec(dllimport)
    #endif
#else
    #define PLUGIN_API extern "C" __attribute__((visibility("default")))
#endif

PLUGIN_API const char* getFunctionName();
PLUGIN_API double calcFunction(double x);

#endif
