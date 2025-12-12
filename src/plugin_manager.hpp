#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "function_registry.hpp"

class DynamicLibrary {
public:
    DynamicLibrary() = default;
    ~DynamicLibrary();
    bool load(const std::string& path);
    void unload();
    void* getSymbol(const std::string& name);
    bool isLoaded() const { return handle_ != nullptr; }
private:
    void* handle_ = nullptr;
};

class PluginManager {
public:
    explicit PluginManager(FunctionRegistry& registry);
    ~PluginManager() = default;
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    void loadPlugins(const std::string& pluginsDir);
private:
    FunctionRegistry& registry_;
    std::vector<std::unique_ptr<DynamicLibrary>> loadedLibs_;
    
    void loadSinglePlugin(const std::string& filepath);
};
