#pragma once
#include "function_registry.hpp"
#include <string>
#include <vector>

class PluginManager {
public:
    PluginManager(FunctionRegistry& registry);
    ~PluginManager();
    void loadFromDirectory(const std::string& path);
private:
    FunctionRegistry& registry;
    struct Impl;
    Impl* impl = nullptr;
};
