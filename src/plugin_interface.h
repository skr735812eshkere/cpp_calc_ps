#pragma once
#ifdef _WIN32
  #define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
  #define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

PLUGIN_EXPORT const char* plugin_name();

PLUGIN_EXPORT double plugin_func(double x);
