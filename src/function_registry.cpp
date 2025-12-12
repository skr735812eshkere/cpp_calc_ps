#include "function_registry.hpp"
#include <algorithm>

void FunctionRegistry::registerFunction(const std::string& name, std::function<double(double)> fn) {
    functions[name] = std::move(fn);
}

bool FunctionRegistry::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

double FunctionRegistry::callFunction(const std::string& name, double arg) const {
    auto it = functions.find(name);
    if (it == functions.end())
        throw std::runtime_error("Неизвестный идентификатор: " + name);
    return it->second(arg);
}

std::vector<std::string> FunctionRegistry::getFunctionNames() const {
    std::vector<std::string> names;
    names.reserve(functions.size());
    
    for (const auto& pair : functions) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::map<std::string, int> FunctionRegistry::getFunctionInfo() const {
    std::map<std::string, int> info;
    
    for (const auto& pair : functions) {
        info[pair.first] = 1;
    }
    
    return info;
}
