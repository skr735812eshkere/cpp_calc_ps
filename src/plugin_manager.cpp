#include "plugin_manager.hpp"
#include "plugin_interface.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

struct PluginManager::Impl {
    std::vector<void*> handles;
};

PluginManager::PluginManager(FunctionRegistry& r) : registry(r) {
    impl = new Impl();
}

PluginManager::~PluginManager() {
    for (void* h : impl->handles) {
#ifdef _WIN32
        FreeLibrary((HMODULE)h);
#else
        dlclose(h);
#endif
    }
    delete impl;
}

static bool isSharedLib(const std::filesystem::path& p) {
#ifdef _WIN32
    return p.extension() == ".dll";
#elif __APPLE__
    return p.extension() == ".dylib" || p.extension() == ".so";
#else
    return p.extension() == ".so";
#endif
}

void PluginManager::loadFromDirectory(const std::string& path) {
    namespace fs = std::filesystem;
    fs::path dir(path);
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        std::cerr << "Plugins dir not found: " << path << "\n";
        return;
    }

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        if (!isSharedLib(entry.path())) continue;

        const auto filePath = entry.path().string();

#ifdef _WIN32
        HMODULE h = LoadLibraryA(filePath.c_str());
        if (!h) {
            std::cerr << "Failed to load plugin: " << filePath << "\n";
            continue;
        }
        auto nameFn = (const char*(*)())GetProcAddress(h, "plugin_name");
        auto funcFn = (double(*)(double))GetProcAddress(h, "plugin_func");
#else
        void* h = dlopen(filePath.c_str(), RTLD_NOW);
        if (!h) {
            std::cerr << "Failed to load plugin: " << filePath << " - " << dlerror() << "\n";
            continue;
        }
        auto nameFn = (const char*(*)())dlsym(h, "plugin_name");
        auto funcFn = (double(*)(double))dlsym(h, "plugin_func");
#endif

        if (!nameFn || !funcFn) {
            std::cerr << "Invalid plugin (missing symbols): " << filePath << "\n";
#ifdef _WIN32
            FreeLibrary((HMODULE)h);
#else
            dlclose(h);
#endif
            continue;
        }

        const char* nameC = nullptr;
        try {
            nameC = nameFn();
        } catch (...) {
            std::cerr << "plugin_name() threw in " << filePath << "\n";
#ifdef _WIN32
            FreeLibrary((HMODULE)h);
#else
            dlclose(h);
#endif
            continue;
        }
        if (!nameC) {
            std::cerr << "plugin_name() returned null in " << filePath << "\n";
#ifdef _WIN32
            FreeLibrary((HMODULE)h);
#else
            dlclose(h);
#endif
            continue;
        }

        std::string fname = nameC;
        std::function<double(double)> wrapper = [funcFn, filePath](double x)->double {
            try {
                double r = funcFn(x);
                return r;
            } catch (const std::exception& ex) {
                std::cerr << "Plugin function threw exception in " << filePath << ": " << ex.what() << "\n";
                return std::numeric_limits<double>::quiet_NaN();
            } catch (...) {
                std::cerr << "Plugin function threw unknown exception in " << filePath << "\n";
                return std::numeric_limits<double>::quiet_NaN();
            }
        };

        registry.registerFunction(fname, wrapper);
        std::cerr << "Loaded plugin: " << filePath << " as function '" << fname << "'\n";

        impl->handles.push_back(h);
    }
}
