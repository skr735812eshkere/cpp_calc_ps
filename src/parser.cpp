#include "parser.hpp"
#include <cctype>
#include <stdexcept>
#include <stack>
#include <unordered_map>
#include <cmath>

static bool isIdentChar(char c) {
    return std::isalpha((unsigned char)c) || c == '_';
}

std::vector<Token> tokenize(const std::string& s) {
    std::vector<Token> out;
    size_t i = 0;
    while (i < s.size()) {
        char c = s[i];
        if (std::isspace((unsigned char)c)) { ++i; continue; }
        if (std::isdigit((unsigned char)c) || (c == '.' && i+1 < s.size() && std::isdigit((unsigned char)s[i+1]))) {
            size_t j = i;
            while (j < s.size() && (std::isdigit((unsigned char)s[j]) || s[j]=='.' || s[j]=='e' || s[j]=='E' || s[j]=='+' || s[j]=='-')) {
                if (s[j]=='+' || s[j]=='-') {
                    if (j==i) break;
                    char prev = s[j-1];
                    if (prev!='e' && prev!='E') break;
                }
                ++j;
            }
            std::string tok = s.substr(i, j-i);
            Token t; t.type = TokenType::Number; t.text = tok;
            try { t.value = std::stod(tok); } catch(...) { t.value = std::numeric_limits<double>::quiet_NaN(); }
            out.push_back(t);
            i = j;
            continue;
        }
        if (isIdentChar(c)) {
            size_t j = i;
            while (j < s.size() && (isIdentChar(s[j]) || std::isdigit((unsigned char)s[j]))) ++j;
            std::string id = s.substr(i, j-i);
            out.push_back(Token{TokenType::Ident, id, 0.0});
            i = j;
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            out.push_back(Token{TokenType::Op, std::string(1,c), 0.0});
            ++i; continue;
        }
        if (c == '(') { out.push_back(Token{TokenType::LParen,"(",0.0}); ++i; continue; }
        if (c == ')') { out.push_back(Token{TokenType::RParen,")",0.0}); ++i; continue; }
        if (c == ',') { out.push_back(Token{TokenType::Comma, ",", 0.0}); ++i; continue; }

        throw std::runtime_error(std::string("Unknown char: ") + c);
    }
    return out;
}

static int precedence(const std::string& op) {
    if (op == "+" || op == "-") return 2;
    if (op == "*" || op == "/") return 3;
    if (op == "^") return 4;
    return 0;
}
static bool isRightAssoc(const std::string& op) {
    if (op == "^") return true;
    return false;
}

std::vector<Token> toRPN(const std::vector<Token>& tokens) {
    std::vector<Token> out;
    std::stack<Token> ops;
    bool mayUnary = true;
    for (size_t i=0;i<tokens.size();++i) {
        const Token& t = tokens[i];
        if (t.type == TokenType::Number) {
            out.push_back(t);
            mayUnary = false;
        } else if (t.type == TokenType::Ident) {
            ops.push(t);
            mayUnary = true;
        } else if (t.type == TokenType::Comma) {
            while (!ops.empty() && ops.top().type != TokenType::LParen) {
                out.push_back(ops.top()); ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("Misplaced comma or parentheses");
            mayUnary = true;
        } else if (t.type == TokenType::Op) {
            std::string op = t.text;
            if (mayUnary && op == "-") {
                Token unary;
                unary.type = TokenType::Op;
                unary.text = "u-";
                ops.push(unary);
            } else {
                while (!ops.empty() && ops.top().type == TokenType::Op) {
                    std::string topop = ops.top().text;
                    int p1 = precedence(topop);
                    int p2 = precedence(op);
                    if ((isRightAssoc(op) && p2 < p1) || (!isRightAssoc(op) && p2 <= p1)) {
                        out.push_back(ops.top()); ops.pop();
                    } else break;
                }
                ops.push(t);
            }
            mayUnary = true;
        } else if (t.type == TokenType::LParen) {
            ops.push(t);
            mayUnary = true;
        } else if (t.type == TokenType::RParen) {
            while (!ops.empty() && ops.top().type != TokenType::LParen) {
                out.push_back(ops.top()); ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
            ops.pop();

            if (!ops.empty() && ops.top().type == TokenType::Ident) {
                out.push_back(ops.top()); ops.pop();
            }
            mayUnary = false;
        } else {
            throw std::runtime_error("Unknown token type");
        }
    }

    while (!ops.empty()) {
        if (ops.top().type == TokenType::LParen || ops.top().type == TokenType::RParen)
            throw std::runtime_error("Mismatched parentheses");
        out.push_back(ops.top()); ops.pop();
    }
    return out;
}
