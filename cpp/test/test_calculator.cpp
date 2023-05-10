#include "catch2/catch_amalgamated.hpp"
#include "jumble/calculator.hpp"

using jumble::Token;
using jumble::Lexer;
using jumble::Parser;
using jumble::Calculator;

TEST_CASE("Lexer1") {
    std::istringstream iss(" (-10) + +23");

    std::vector<Token> actual_toks = {
        Token::oper("("),
        Token::oper("-"),
        Token::num("10"),
        Token::oper(")"),
        Token::oper("+"),
        Token::oper("+"),
        Token::num("23"),
    };

    Lexer lexer(iss);
    std::vector<Token> expect_toks;
    while (true) {
        auto tok = lexer.scan();
        if (tok.type() == Token::END_INPUT) {
            break;
        }
        expect_toks.push_back(tok);
    }

    REQUIRE(actual_toks == expect_toks);
}

TEST_CASE("Lexer2") {
    std::istringstream iss(" \n4* -\v(567/8)");

    std::vector<Token> actual_toks = {
        Token::num("4"),
        Token::oper("*"),
        Token::oper("-"),
        Token::oper("("),
        Token::num("567"),
        Token::oper("/"),
        Token::num("8"),
        Token::oper(")"),
    };

    Lexer lexer(iss);
    std::vector<Token> expect_toks;
    while (true) {
        auto tok = lexer.scan();
        if (tok.type() == Token::END_INPUT) {
            break;
        }
        expect_toks.push_back(tok);
    }

    REQUIRE(actual_toks == expect_toks);
}

TEST_CASE("Lexer3") {
    std::istringstream iss("90*(- 345/ (1+2))\n");
 
    std::vector<Token> actual_toks = {
        Token::num("90"),
        Token::oper("*"),
        Token::oper("("),
        Token::oper("-"),
        Token::num("345"),
        Token::oper("/"),
        Token::oper("("),
        Token::num("1"),
        Token::oper("+"),
        Token::num("2"),
        Token::oper(")"),
        Token::oper(")"),
    };

    Lexer lexer(iss);
    std::vector<Token> expect_toks;
    while (true) {
        auto tok = lexer.scan();
        if (tok.type() == Token::END_INPUT) {
            break;
        }
        expect_toks.push_back(tok);
    }

    REQUIRE(actual_toks == expect_toks);
}

TEST_CASE("Parser1") {
    std::istringstream iss("+ 3 *( -1+(-(1 -2)) ) /  2");

    std::vector<Token> actual_toks = {
        Token::num("3"),
        Token::oper("@"),
        Token::num("1"),
        Token::oper("#"),
        Token::num("1"),
        Token::num("2"),
        Token::oper("-"),
        Token::oper("#"),
        Token::oper("+"),
        Token::oper("*"),
        Token::num("2"),
        Token::oper("/"),
    };

    Parser parser(iss);
    const auto &expect_toks = parser.parse();
    REQUIRE(actual_toks == expect_toks);
}

TEST_CASE("Parser2") {
    std::istringstream iss("(-3) *(- 20+(-(2*(3+5))- (8+7)) )");

    std::vector<Token> actual_toks = {
        Token::num("3"),
        Token::oper("#"),
        Token::num("20"),
        Token::oper("#"),
        Token::num("2"),
        Token::num("3"),
        Token::num("5"),
        Token::oper("+"),
        Token::oper("*"),
        Token::oper("#"),
        Token::num("8"),
        Token::num("7"),
        Token::oper("+"),
        Token::oper("-"),
        Token::oper("+"),
        Token::oper("*"),
    };

    Parser parser(iss);
    const auto &expect_toks = parser.parse();
    REQUIRE(actual_toks == expect_toks);
}

TEST_CASE("ParserException") {
    std::string what;
    try {
        std::istringstream iss("9 ) 8");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "cannot parse ) 8");

    what = "";
    try {
        std::istringstream iss;
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "expr meets $");

    what = "";
    try {
        std::istringstream iss("*");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "expr meets operator *");

    what = "";
    try {
        std::istringstream iss("0-");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "A meets $");

    what = "";
    try {
        std::istringstream iss("1+/");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "A meets operator /");

    what = "";
    try {
        std::istringstream iss("+");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "B meets $");

    what = "";
    try {
        std::istringstream iss("2/*");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "B meets operator *");

    what = "";
    try {
        std::istringstream iss("3 3");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "tmpB meets num 3");

    what = "";
    try {
        std::istringstream iss("4(");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "tmpB meets operator (");

    what = "";
    try {
        std::istringstream iss("( 56");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error &e) {
        what = e.what();
    }
    REQUIRE(what == "brackets mismatch");
}

TEST_CASE("Calculate") {
    std::istringstream iss("+ 3 *( -1+(-(1 -2)) ) / 2");
    REQUIRE(Calculator(iss).calculate() == 0);

    iss.str("(6*((5   +((2+3)  *8))+ 3))");
    iss.clear();
    REQUIRE(Calculator(iss).calculate() == 288);

    iss.str("(-3) *(- 20+(-(2*(3+5))- (8+7)))");
    iss.clear();
    REQUIRE(Calculator(iss).calculate() == 153);

    iss.str("- 8 -(20/4+(3 - 5) *20)+3+ 4+88 /8-1+3");
    iss.clear();
    REQUIRE(Calculator(iss).calculate() == 47);

    iss.str("( 60 /(2*(3+4)+(5- 2)/ 3)-1-1) * 8-(20/4+(3-5) *20+3+ (- 4) +88/8-(2-3))");
    iss.clear();
    REQUIRE(Calculator(iss).calculate() == 40);
}
