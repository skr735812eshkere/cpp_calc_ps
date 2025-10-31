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
                    try {
                        st.push(std::stod(tok.text));
                    } catch (...) {
                        st.push(std::numeric_limits<double>::quiet_NaN());
                    }
                    break;

                case TokenType::Op: {
                    if (tok.text == "u-") {
                        if (st.empty()) throw std::runtime_error("Missing operand for unary -");
                        double a = st.top(); st.pop();
                        st.push(-a);
                        break;
                    }

                    if (st.size() < 2)
                        throw std::runtime_error("Not enough operands for operator " + tok.text);
                    double b = st.top(); st.pop();
                    double a = st.top(); st.pop();

                    if (tok.text == "+") st.push(a + b);
                    else if (tok.text == "-") st.push(a - b);
                    else if (tok.text == "*") st.push(a * b);
                    else if (tok.text == "/") {
                        if (b == 0.0) st.push(std::numeric_limits<double>::quiet_NaN());
                        else st.push(a / b);
                    }
                    else if (tok.text == "^") st.push(std::pow(a, b));
                    else
                        throw std::runtime_error("Unknown operator: " + tok.text);
                    break;
                }

                case TokenType::Ident: {
                    if (st.empty())
                        throw std::runtime_error("Missing argument for function: " + tok.text);

                    double arg = st.top(); st.pop();

                    if (!registry.hasFunction(tok.text))
                        throw std::runtime_error("Unknown identifier: " + tok.text);

                    double res = registry.callFunction(tok.text, arg);

                    if (std::isnan(res) || std::isinf(res)) {
                        st.push(res);
                    } else {
                        st.push(res);
                    }
                    break;
                }

                case TokenType::LParen:
                case TokenType::RParen:
                case TokenType::Comma:
                    throw std::runtime_error("Unexpected token in RPN: " + tok.text);

                default:
                    throw std::runtime_error("Unexpected token type");
            }
        }

        if (st.size() != 1)
            throw std::runtime_error("Stack has " + std::to_string(st.size()) + " elements at end (expected 1)");

        double finalValue = st.top();
        return EvalResult{ finalValue, true, "" };
    } catch (const std::exception& ex) {
        return EvalResult{ 0.0, false, ex.what() };
    } catch (...) {
        return EvalResult{ 0.0, false, "Unknown evaluation error" };
    }
}
