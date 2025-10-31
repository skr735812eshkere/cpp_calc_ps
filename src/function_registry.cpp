#include "function_registry.hpp"

void FunctionRegistry::registerFunction(const std::string& name, std::function<double(double)> fn) {
    functions[name] = std::move(fn);
}

bool FunctionRegistry::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

double FunctionRegistry::callFunction(const std::string& name, double arg) const {
    auto it = functions.find(name);
    if (it == functions.end())
        throw std::runtime_error("Unknown identifier: " + name);
    return it->second(arg);
}
