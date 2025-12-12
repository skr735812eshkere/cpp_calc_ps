#include "plugin_manager.hpp"
#include "plugin_interface.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace fs = std::filesystem;

DynamicLibrary::~DynamicLibrary() {
    unload();
}

bool DynamicLibrary::load(const std::string& path) {
    if (handle_) {
        unload();
    }
    
#if defined(_WIN32)
    handle_ = LoadLibraryA(path.c_str());
    if (!handle_) {
        DWORD error = GetLastError();
        std::cerr << "Ошибка загрузки библиотеки " << path << ", ошибка: " << error << std::endl;
        return false;
    }
#else
    handle_ = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!handle_) {
        std::cerr << "Ошибка загрузки библиотеки " << path << ": " << dlerror() << std::endl;
        return false;
    }
#endif
    return true;
}

void DynamicLibrary::unload() {
    if (handle_) {
#if defined(_WIN32)
        FreeLibrary(static_cast<HMODULE>(handle_));
#else
        dlclose(handle_);
#endif
        handle_ = nullptr;
    }
}

void* DynamicLibrary::getSymbol(const std::string& name) {
    if (!handle_) return nullptr;
    
#if defined(_WIN32)
    return reinterpret_cast<void*>(
        GetProcAddress(static_cast<HMODULE>(handle_), name.c_str())
    );
#else
    return dlsym(handle_, name.c_str());
#endif
}

PluginManager::PluginManager(FunctionRegistry& registry): registry_(registry) {}

void PluginManager::loadPlugins(const std::string& pluginsDir) {
    if (!fs::exists(pluginsDir)) {
        std::cerr << "Папка с плагинами отсутствует: " << pluginsDir << std::endl;
        return;
    }
    
    if (!fs::is_directory(pluginsDir)) {
        std::cerr << "Путь - не папка: " << pluginsDir << std::endl;
        return;
    }
    
    std::cout << "Загрузка плагинов с: " << pluginsDir << std::endl;
    
    try {
        for (const auto& entry : fs::directory_iterator(pluginsDir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                
#if defined(_WIN32)
                bool isPlugin = (ext == ".dll");
#elif defined(__APPLE__)
                bool isPlugin = (ext == ".dylib");
#else
                bool isPlugin = (ext == ".so");
#endif
                
                if (isPlugin) {
                    loadSinglePlugin(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения папки с плагинами: " << e.what() << std::endl;
    }
}

void PluginManager::loadSinglePlugin(const std::string& filepath) {
    auto lib = std::make_unique<DynamicLibrary>();
    
    if (!lib->load(filepath)) {
        std::cerr << "Ошибка загрузки плагинов: " << filepath << std::endl;
        return;
    }
    
    using GetNameFunc = const char*(*)();
    using ExecuteFunc = double(*)(double);
    
    auto getName = reinterpret_cast<GetNameFunc>(
        lib->getSymbol("getFunctionName"));
    auto execute = reinterpret_cast<ExecuteFunc>(
        lib->getSymbol("calcFunction"));
    
    if (!getName || !execute) {
        std::cerr << "Плагин " << filepath << " не экспортирует необходимые функции" << std::endl;
        return;
    }
    
    const char* functionName = nullptr;
    try {
        functionName = getName();
    } catch (const std::exception& e) {
        std::cerr << "Плагин getFunctionName() бросил исключение: " << e.what() << std::endl;
        return;
    } catch (...) {
        std::cerr << "Плагин getFunctionName() бросил неизвестное исключение" << std::endl;
        return;
    }
    
    if (!functionName || functionName[0] == '\0') {
        std::cerr << "Плагин не возвратил имя: " << filepath << std::endl;
        return;
    }
    
    std::string nameStr(functionName);
    
    if (registry_.hasFunction(nameStr)) {
        std::cerr << "Функция '" << nameStr << "' уже зарегистрирован: " << filepath << std::endl;
        return;
    }
    
    std::function<double(double)> wrapper = [execute, nameStr](double x) -> double {
            try {
                return execute(x);
            } catch (const std::exception& e) {
                throw std::runtime_error(
                    std::string("Функция плагина '") + nameStr + "' ошибка: " + e.what());
            } catch (...) {
                throw std::runtime_error(
                    std::string("Неизвестная ошибка в функции плагина '") +
                    nameStr + "'");
            }
        };
    
    registry_.registerFunction(nameStr, wrapper);
    
    loadedLibs_.push_back(std::move(lib));
    
    std::cout << "Загружен плагин: " << nameStr << " из " << fs::path(filepath).filename().string() << std::endl;
}
