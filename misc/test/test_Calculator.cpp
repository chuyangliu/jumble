#include "gtest/gtest.h"
#include "tastylib/Calculator.h"

using tastylib::Token;
using tastylib::Lexer;
using tastylib::Parser;
using tastylib::Calculator;


TEST(CalculatorTest, Lexer1) {
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

    EXPECT_EQ(actual_toks, expect_toks);
}

TEST(CalculatorTest, Lexer2) {
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

    EXPECT_EQ(actual_toks, expect_toks);
}

TEST(CalculatorTest, Lexer3) {
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

    EXPECT_EQ(actual_toks, expect_toks);
}

TEST(CalculatorTest, Parser1) {
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
    EXPECT_EQ(actual_toks, expect_toks);
}

TEST(CalculatorTest, Parser2) {
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
    EXPECT_EQ(actual_toks, expect_toks);
}

TEST(CalculatorTest, ParserException) {
    std::string what;
    try {
        std::istringstream iss("9 ) 8");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "cannot parse ) 8");

    what = "";
    try {
        std::istringstream iss;
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "expr meets $");

    what = "";
    try {
        std::istringstream iss("*");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "expr meets operator *");

    what = "";
    try {
        std::istringstream iss("0-");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "A meets $");

    what = "";
    try {
        std::istringstream iss("1+/");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "A meets operator /");

    what = "";
    try {
        std::istringstream iss("+");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "B meets $");

    what = "";
    try {
        std::istringstream iss("2/*");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "B meets operator *");

    what = "";
    try {
        std::istringstream iss("3 3");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "tmpB meets num 3");

    what = "";
    try {
        std::istringstream iss("4(");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "tmpB meets operator (");

    what = "";
    try {
        std::istringstream iss("( 56");
        Parser parser(iss);
        parser.parse();
    } catch (std::runtime_error e) {
        what = e.what();
    }
    EXPECT_EQ(what, "brackets mismatch");
}

TEST(CalculatorTest, Calculate) {
    std::istringstream iss("+ 3 *( -1+(-(1 -2)) ) / 2");
    EXPECT_EQ(Calculator(iss).calculate(), 0);

    iss.str("(6*((5   +((2+3)  *8))+ 3))");
    iss.clear();
    EXPECT_EQ(Calculator(iss).calculate(), 288);

    iss.str("(-3) *(- 20+(-(2*(3+5))- (8+7)))");
    iss.clear();
    EXPECT_EQ(Calculator(iss).calculate(), 153);

    iss.str("- 8 -(20/4+(3 - 5) *20)+3+ 4+88 /8-1+3");
    iss.clear();
    EXPECT_EQ(Calculator(iss).calculate(), 47);

    iss.str("( 60 /(2*(3+4)+(5- 2)/ 3)-1-1) * 8-(20/4+(3-5) *20+3+ (- 4) +88/8-(2-3))");
    iss.clear();
    EXPECT_EQ(Calculator(iss).calculate(), 40);
}
