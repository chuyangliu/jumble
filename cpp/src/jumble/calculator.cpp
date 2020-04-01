#include "jumble/calculator.hpp"
#include <stdexcept>
#include <stack>
#include <cstddef>
#include <cctype>

JUMBLE_NAMESPACE_BEGIN

// Lexer

Token Lexer::scan() {
    if (!is) {
        return Token::end();
    }

    while (isspace(cur) || iscntrl(cur)) {
        if (!is.get(cur)) {
            return Token::end();
        }
    }

    if (isdigit(cur)) {
        std::size_t val = 0;
        do {
            val = 10 * val + (cur - '0');
        } while (is.get(cur) && isdigit(cur));
        return Token::num(std::to_string(val));
    }

    auto tok = Token::oper(std::string(1, cur));
    cur = ' ';
    return tok;
}

// Parser

const std::vector<Token>& Parser::parse() {
    read();
    expr();

    if (cur.type() != Token::END_INPUT) {
        auto val = cur.val();
        while (true) {
            read();
            if (cur.type() == Token::END_INPUT) {
                break;
            }
            val +=  " " + cur.val();
        }
        throw std::runtime_error("cannot parse " + val);
    }

    return postfix;
}

void Parser::expr() {
    switch (cur.type()) {
        case Token::END_INPUT:
            throw std::runtime_error("expr meets " + cur.val());
            break;
        case Token::OPERATOR: {
            auto val = cur.val();
            if (val == "*" || val == "/" || val == ")") {
                throw std::runtime_error("expr meets operator " + val);
            }
            break;
        }
        default:
            break;
    }
    A();
    tmpA();
}

void Parser::A() {
    switch (cur.type()) {
        case Token::END_INPUT:
            throw std::runtime_error("A meets " + cur.val());
            break;
        case Token::OPERATOR: {
            auto val = cur.val();
            if (val == "*" || val == "/" || val == ")") {
                throw std::runtime_error("A meets operator " + val);
            }
            break;
        }
        default:
            break;
    }
    B();
    tmpB();;
}

void Parser::tmpA() {
    switch (cur.type()) {
        case Token::NUMBER:
            throw std::runtime_error("tmpA meets num " + cur.val());
            break;
        case Token::OPERATOR: {
            auto val = cur.val();
            if (val == "+" || val == "-") {
                read();
                A();
                postfix.push_back(Token::oper(val));
                tmpA();
            } else if (val == ")") {
                break;
            } else {
                throw std::runtime_error("tmpA meets operator " + val);
            }
            break;
        }
        default:
            break;
    }
}

void Parser::B() {
    switch (cur.type()) {
        case Token::END_INPUT:
            throw std::runtime_error("B meets " + cur.val());
            break;
        case Token::NUMBER:
            factor();
            break;
        case Token::OPERATOR: {
            auto val = cur.val();
            if (val == "+") {
                read();
                B();
                postfix.push_back(Token::oper("@"));
            } else if (val == "-") {
                read();
                B();
                postfix.push_back(Token::oper("#"));
            } else if (val == "(") {
                factor();
            } else {
                throw std::runtime_error("B meets operator " + val);
            }
            break;
        }
        default:
            break;
    }
}

void Parser::tmpB() {
    switch (cur.type()) {
        case Token::NUMBER:
            throw std::runtime_error("tmpB meets num " + cur.val());
            break;
        case Token::OPERATOR: {
            auto val = cur.val();
            if (val == "*" || val == "/") {
                read();
                B();
                postfix.push_back(Token::oper(val));
                tmpB();
            } else if (val == "(") {
                throw std::runtime_error("tmpB meets operator " + val);
            }
            break;
        }
        default:
            break;
    }
}

void Parser::factor() {
    switch (cur.type()) {
        case Token::END_INPUT:
            throw std::runtime_error("factor meets " + cur.val());
            break;
        case Token::NUMBER:
            postfix.push_back(cur);
            read();
            break;
        case Token::OPERATOR:
            if (cur.val() == "(") {
                read();
                expr();
                if (cur != Token::oper(")")) {
                    throw std::runtime_error("brackets mismatch");
                }
                read();
            } else {
                throw std::runtime_error("factor meets operator " + cur.val());
            }
            break;
        default:
            break;
    }
}

// Calculator

long Calculator::calculate() {
    const auto &toks = parser.parse();  // Postfix tokens

    std::stack<std::string> s;
    for (const auto &tok : toks) {
        if (tok.type() == Token::NUMBER) {
            s.push(tok.val());
        } else if (tok.type() == Token::OPERATOR) {
            auto val = tok.val();
            long lhs = 0, rhs = 0;
            if (val == "@" || val == "#") {
                if (s.size() < 1) {
                    throw std::runtime_error("not enough operands for operator " + val);
                }
                if (val == "#") {
                    rhs = std::stol(s.top()); s.pop();
                    s.push(std::to_string(-rhs));
                }
            } else {
                if (s.size() < 2) {
                    throw std::runtime_error("not enough operands for operator " + val);
                }
                rhs = std::stol(s.top()); s.pop();
                lhs = std::stol(s.top()); s.pop();
                if (val == "+") {
                    s.push(std::to_string(lhs + rhs));
                } else if (val == "-") {
                    s.push(std::to_string(lhs - rhs));
                } else if (val == "*") {
                    s.push(std::to_string(lhs * rhs));
                } else if (val == "/") {
                    s.push(std::to_string(lhs / rhs));
                } else {
                    throw std::runtime_error("unknown operator " + val);
                }
            }
        }
    }

    if (s.size() > 1) {
        throw std::runtime_error("invalid postfix expression");
    }

    return std::stol(s.top());
}

JUMBLE_NAMESPACE_END
