#include "function_registry.hpp"
#include "plugin_manager.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include <iostream>
#include <string>
#include <cmath>

int main() {
    FunctionRegistry registry;

    registry.registerFunction("abs", [](double x){ return std::abs(x); });
    registry.registerFunction("sqrt", [](double x){ if (x<0) return std::numeric_limits<double>::quiet_NaN(); return std::sqrt(x); });
    registry.registerFunction("ln", [](double x){ if (x<=0) return std::numeric_limits<double>::quiet_NaN(); return std::log(x); });

    PluginManager pm(registry);
    pm.loadFromDirectory("./plugins");

    std::cout << "Calc REPL. Enter expression or 'quit'.\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "quit" || line == "exit") break;

        try {
            auto toks = tokenize(line);
            auto rpn = toRPN(toks);
            auto res = evaluateRPN(rpn, registry);
            if (!res.ok) {
                std::cout << "Error: " << res.err << "\n";
            } else {
                std::cout << res.value << "\n";
            }
        } catch (const std::exception& ex) {
            std::cout << "Parse/Error: " << ex.what() << "\n";
        } catch (...) {
            std::cout << "Unknown error\n";
        }
    }

    return 0;
}
