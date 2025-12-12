#include "function_registry.hpp"
#include "plugin_manager.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include <iostream>
#include <string>
#include <cmath>

int main() {
    FunctionRegistry registry;
    Parser parser;

    registry.registerFunction("abs", [](double x){ return std::abs(x); });
    registry.registerFunction("sqrt", [](double x){ if (x<0) return std::numeric_limits<double>::quiet_NaN(); return std::sqrt(x); });
    registry.registerFunction("ln", [](double x){ if (x<=0) return std::numeric_limits<double>::quiet_NaN(); return std::log(x); });

    PluginManager pm(registry);
    pm.loadPlugins("./plugins");
    parser.setAvailableFunctions(registry.getFunctionInfo());

    std::cout << "Введите выразжение или 'quit'\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "quit" || line == "exit") break;

        try {
            auto toks = parser.tokenize(line);
            auto rpn = parser.toRPN(toks);
            auto res = evaluateRPN(rpn, registry);
            if (!res.ok) {
                std::cout << "Ашипка: " << res.err << "\n";
            } else {
                std::cout << res.value << "\n";
            }
        } catch (const std::exception& ex) {
            std::cout << "Ошибка парсинга: " << ex.what() << "\n";
        } catch (...) {
            std::cout << "Неизвестная ошибка\n";
        }
    }

    return 0;
}
