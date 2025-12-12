#include "evaluator.hpp"
#include "function_registry.hpp"
#include <stack>
#include <cmath>
#include <stdexcept>
#include <limits>

EvalResult evaluateRPN(const std::vector<Token>& rpn, const FunctionRegistry& registry) {
    std::stack<double> st;

    try {
        for (const auto& tok : rpn) {
            switch (tok.type) {
                case TokenType::Number:
                    if (std::isnan(tok.value)) {
                        return EvalResult{ 0.0, false, "Некорректное число: " + tok.text };
                    }
                    st.push(tok.value);
                    break;

                case TokenType::Operator: {
                    if (tok.text == "u-") {
                        if (st.empty()) {
                            return EvalResult{ 0.0, false, "Не хватает операнда для унарного минуса" };
                        }
                        double a = st.top(); st.pop();
                        st.push(-a);
                        break;
                    }

                    if (st.size() < 2) {
                        return EvalResult{ 0.0, false, "Не достаточно операндов для оператора " + tok.text };
                    }
                    
                    double b = st.top(); st.pop();
                    double a = st.top(); st.pop();

                    if (tok.text == "+") {
                        st.push(a + b);
                    } else if (tok.text == "-") {
                        st.push(a - b);
                    } else if (tok.text == "*") {
                        st.push(a * b);
                    } else if (tok.text == "/") {
                        if (b == 0.0) {
                            return EvalResult{ 0.0, false, "Деление на ноль" };
                        }
                        st.push(a / b);
                    } else if (tok.text == "^") {
                        st.push(std::pow(a, b));
                    } else {
                        return EvalResult{ 0.0, false, "Неизвестный оператор: " + tok.text };
                    }
                    break;
                }

                case TokenType::Identifier: {
                    if (st.empty()) {
                        return EvalResult{ 0.0, false, "Не хватает аргумента для функции: " + tok.text };
                    }
                    double arg = st.top(); st.pop();

                    if (!registry.hasFunction(tok.text)) {
                        return EvalResult{ 0.0, false, "Неизвестная функция: " + tok.text };
                    }
                    double res = registry.callFunction(tok.text, arg);

                    if (std::isnan(res)) {
                        return EvalResult{ 0.0, false, "Функция " + tok.text + " вернула NaN" };
                    }
                    if (std::isinf(res)) {
                        return EvalResult{ 0.0, false, "Функция " + tok.text + " вернула бесконечность" };
                    }
                    
                    st.push(res);
                    break;
                }

                case TokenType::LParenthesis:
                case TokenType::RParenthesis:
                case TokenType::Comma:
                    return EvalResult{ 0.0, false, "Неожиданный токен в обратной польской нотации: " + tok.text };
                default:
                    return EvalResult{ 0.0, false, "Неожиданный тип токена" };
            }
        }

        if (st.size() != 1) {
            return EvalResult{ 0.0, false, "У стека " + std::to_string(st.size()) + " элементов в конце (ожидается один)"};
        }

        double finalValue = st.top();
        return EvalResult{ finalValue, true, "" };
    } catch (const std::exception& ex) {
        return EvalResult{ 0.0, false, ex.what() };
    } catch (...) {
        return EvalResult{ 0.0, false, "Неизвестная вычислительная ошибка" };
    }
}
