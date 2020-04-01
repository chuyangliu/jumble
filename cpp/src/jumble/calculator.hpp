#ifndef JUMBLE_CALCULATOR_HPP_INCLUDED
#define JUMBLE_CALCULATOR_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <string>
#include <iostream>
#include <vector>

JUMBLE_NAMESPACE_BEGIN

class Token {
    friend bool operator==(const Token& lhs, const Token& rhs) noexcept {
        return lhs.type_ == rhs.type_ && lhs.val_ == rhs.val_;
    }

    friend bool operator!=(const Token& lhs, const Token& rhs) noexcept {
        return !(lhs == rhs);
    }

public:
    enum Type {
        END_INPUT,
        NUMBER,
        OPERATOR
    };

public:
    static Token end() noexcept {
        return Token(Type::END_INPUT, "$");
    }

    static Token num(const std::string& v) noexcept {
        return Token(Type::NUMBER, v);
    }

    static Token oper(const std::string& v) noexcept {
        return Token(Type::OPERATOR, v);
    }

    Type type() const noexcept {
        return type_;
    }

    const std::string& val() const noexcept {
        return val_;
    }

private:
    Token(const Type t, const std::string& v) noexcept
        : type_(t), val_(v) {}

private:
    Type type_;
    std::string val_;
};


class Lexer {
public:
    Lexer(std::istream& i) noexcept : cur(' '), is(i) {}

    // Read and parser one token from input stream
    Token scan();

private:
    char cur;
    std::istream &is;
};


class Parser {
public:
    Parser(std::istream& i) noexcept : cur(Token::end()), lexer(i) {}

    /*
    Parse an infix arithmetic expression and return its postfix tokens.

    @throw std::runtime_error if parse failed
    */
    const std::vector<Token>& parse();

private:
    void read() {
        cur = lexer.scan();
    }
    
    // Recursive descent of nonterminals
    void expr();
    void A();
    void tmpA();
    void B();
    void tmpB();
    void factor();

private:
    Token cur;
    Lexer lexer;
    std::vector<Token> postfix;
};


class Calculator {
public:
    Calculator(std::istream& i) noexcept : parser(i) {}

    /*
    Return the result of an infix arithmetic expression.

    @throw std::runtime_error if the expression is invalid
    */
    long calculate();

private:
    Parser parser;
};

JUMBLE_NAMESPACE_END

#endif
