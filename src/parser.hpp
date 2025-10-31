#pragma once
#include <string>
#include <vector>

enum class TokenType { Number, Op, LParen, RParen, Ident, Comma };
struct Token {
    TokenType type;
    std::string text;
    double value;
};

std::vector<Token> tokenize(const std::string& s);
std::vector<Token> toRPN(const std::vector<Token>& tokens); 
