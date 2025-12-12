#include "parser.hpp"
#include <cctype>
#include <stack>
#include <stdexcept>
#include <cmath>

Parser::Parser() {
    availableFunctions_ = {
        {"abs", 1}, {"sqrt", 1}, {"log", 1}, {"ln", 1}
    };
}

bool Parser::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Parser::isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool Parser::isIdentifierChar(char c) {
    return isLetter(c) || isDigit(c) || c == '_';
}

void Parser::setAvailableFunctions(const std::map<std::string, int>& functions) {
    availableFunctions_ = functions;
}

bool Parser::isValidIdentifier(const std::string& ident) const {
    if (ident.empty()) return false;
    if (!isLetter(ident[0])) return false;
    for (char c : ident) {
        if (!isIdentifierChar(c)) return false;
    }
    return true;
}

std::vector<Token> Parser::tokenize(const std::string& expression) const {
    std::vector<Token> tokens;
    size_t i = 0;
    const size_t n = expression.length();
    auto addToken = [&tokens](TokenType type, const std::string& text, double value = 0.0) {
        tokens.emplace_back(type, text, value);
    };
    
    while (i < n) {
        char c = expression[i];
        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }
        if (isDigit(c) || (c == '.' && i + 1 < n && isDigit(expression[i + 1]))) {
            size_t start = i;
            bool hasDot = false;
            bool hasExponent = false;
            while (i < n) {
                c = expression[i];
                if (isDigit(c)) {
                    ++i;
                }
                else if (c == '.' && !hasDot && !hasExponent) {
                    hasDot = true;
                    ++i;
                }
                else if ((c == 'e' || c == 'E') && !hasExponent) {
                    hasExponent = true;
                    ++i;
                    if (i < n && (expression[i] == '+' || expression[i] == '-')) {
                        ++i;
                    }
                }
                else {
                    break;
                }
            }
            
            std::string numStr = expression.substr(start, i - start);
            double value;
            try {
                value = std::stod(numStr);
            }
            catch (...) {
                throw std::runtime_error("Неправильный номер: " + numStr);
            }
            
            addToken(TokenType::Number, numStr, value);
            continue;
        }

        if (isLetter(c) || c == '_') {
            size_t start = i;
            while (i < n && isIdentifierChar(expression[i])) {
                ++i;
            }
            
            std::string ident = expression.substr(start, i - start);

            if (!isValidIdentifier(ident)) {
                throw std::runtime_error("Неправильный идентификатор: " + ident);
            }
            
            addToken(TokenType::Identifier, ident);
            continue;
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            addToken(TokenType::Operator, std::string(1, c));
            ++i;
            continue;
        }
        
        if (c == '(') {
            addToken(TokenType::LParenthesis, "(");
            ++i;
            continue;
        }
        
        if (c == ')') {
            addToken(TokenType::RParenthesis, ")");
            ++i;
            continue;
        }
        
        if (c == ',') {
            addToken(TokenType::Comma, ",");
            ++i;
            continue;
        }
        throw std::runtime_error(std::string("Неизвестный символ: '") + c + "'");
    }
    
    return tokens;
}

int Parser::getOperatorPrecedence(const std::string& op) {
    if (op == "^") return 4;
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    if (op == "u-") return 4;
    return 0;
}

bool Parser::isRightAssociative(const std::string& op) {
    return op == "^";
}

bool Parser::isUnaryOperator(const Token& token, bool wasValue) {
    return token.type == TokenType::Operator && token.text == "-" && !wasValue;
}

std::vector<Token> Parser::toRPN(const std::vector<Token>& tokens) const {
    std::vector<Token> output;
    std::stack<Token> operators;
    bool lastWasValue = false;
    
    for (const auto& token : tokens) {
        switch (token.type) {
            case TokenType::Number:
                output.push_back(token);
                lastWasValue = true;
                break;
                
            case TokenType::Identifier:
                operators.push(token);
                lastWasValue = false;
                break;
                
            case TokenType::Operator: {
                Token currentToken = token;
                if (isUnaryOperator(token, lastWasValue)) {
                    currentToken.text = "u-";
                }
                
                while (!operators.empty()) {
                    const Token& top = operators.top();
                    
                    if (top.type != TokenType::Operator && top.type != TokenType::Identifier) {
                        break;
                    }
                    
                    bool isTopOperator = top.type == TokenType::Operator;
                    bool isTopFunction = top.type == TokenType::Identifier;
                    
                    if (isTopOperator) {
                        int topPrec = getOperatorPrecedence(top.text);
                        int currPrec = getOperatorPrecedence(currentToken.text);
                        
                        bool isRightAssoc = isRightAssociative(currentToken.text);
                        
                        if ((!isRightAssoc && currPrec <= topPrec) ||
                            (isRightAssoc && currPrec < topPrec)) {
                            output.push_back(top);
                            operators.pop();
                        }
                        else {
                            break;
                        }
                    }
                    else if (isTopFunction) {
                        output.push_back(top);
                        operators.pop();
                    }
                    else {
                        break;
                    }
                }
                
                operators.push(currentToken);
                lastWasValue = false;
                break;
            }
                
            case TokenType::LParenthesis:
                operators.push(token);
                lastWasValue = false;
                break;
                
            case TokenType::RParenthesis:
                while (!operators.empty() && operators.top().type != TokenType::LParenthesis) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                
                if (operators.empty()) {
                    throw std::runtime_error("Не хватает скобки");
                }
                
                operators.pop();
                
                if (!operators.empty() && operators.top().type == TokenType::Identifier) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                
                lastWasValue = true;
                break;
                
            case TokenType::Comma:
                while (!operators.empty() && operators.top().type != TokenType::LParenthesis) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                
                if (operators.empty()) {
                    throw std::runtime_error("Не хватает запятой");
                }
                lastWasValue = false;
                break;
        }
    }
    
    while (!operators.empty()) {
        if (operators.top().type == TokenType::LParenthesis) {
            throw std::runtime_error("Не хватает скобок");
        }
        output.push_back(operators.top());
        operators.pop();
    }
    
    return output;
}
