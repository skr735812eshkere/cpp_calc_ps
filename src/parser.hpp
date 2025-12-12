#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

enum class TokenType {Number, Operator, LParenthesis, RParenthesis, Identifier, Comma};

struct Token {
    TokenType type;
    std::string text;
    double value;
};

class Parser {
public:
    Parser();
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    Parser(Parser&&) noexcept = default;
    Parser& operator=(Parser&&) noexcept = default;
    std::vector<Token> tokenize(const std::string& expression) const;
    std::vector<Token> toRPN(const std::vector<Token>& tokens) const;
    void setAvailableFunctions(const std::map<std::string, int>& functions);
    bool isValidIdentifier(const std::string& ident) const;
    
private:
    std::map<std::string, int> availableFunctions_;
    static bool isDigit(char c);
    static bool isLetter(char c);
    static bool isIdentifierChar(char c);
    static int getOperatorPrecedence(const std::string& op);
    static bool isRightAssociative(const std::string& op);
    static bool isUnaryOperator(const Token& token, bool wasValue);
};
