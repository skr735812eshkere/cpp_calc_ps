#pragma once
#include <functional>
#include <map>
#include <string>
#include <stdexcept>

class FunctionRegistry {
public:
    void registerFunction(const std::string& name, std::function<double(double)> fn);
    bool hasFunction(const std::string& name) const;
    double callFunction(const std::string& name, double arg) const;

private:
    std::map<std::string, std::function<double(double)>> functions;
};
