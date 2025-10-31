#pragma once
#include "parser.hpp"
#include "function_registry.hpp"
#include <string>
#include <vector>

struct EvalResult {
    double value;
    bool ok;
    std::string err;
};

EvalResult evaluateRPN(const std::vector<Token>& rpn, const FunctionRegistry& registry);
