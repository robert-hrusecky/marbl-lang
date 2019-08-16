#include "lexer/Lexer.hpp"
#include <catch2/catch.hpp>

using namespace lex;

TEST_CASE("keywords", "[lex]") {
    {
        Lexer lexer("func");
        REQUIRE(lexer.getPos() == 0);
        Token func = lexer.get();
        REQUIRE(lexer.getPos() == 4);
        REQUIRE(func.type == Token::Type::FUNC);
    }
    {
        Lexer lexer(" \n mut ");
        Token tok = lexer.get();
        REQUIRE(tok.type == Token::Type::MUT);
        REQUIRE(lexer.getPos() == 6);
    }
    {
        Lexer lexer("birthdays suck");
        Token tok = lexer.get();
        REQUIRE(lexer.getPos() == 9);
        REQUIRE(tok.type == Token::Type::ID);
        REQUIRE(tok.str == std::string("birthdays"));
    }
    {
        Lexer lexer("break_");
        Token tok = lexer.get();
        REQUIRE(tok.type == Token::Type::ID);
        REQUIRE(tok.str == "break_");
        REQUIRE(lexer.getPos() == 6);
    }
    {
        Lexer lexer(" _underscore_");
        Token tok = lexer.get();
        REQUIRE(tok.type == Token::Type::ID);
        REQUIRE(tok.str == "_underscore_");
        REQUIRE(lexer.getPos() == 13);
    }
}

TEST_CASE("numbers", "[lex]") {
    {
        Lexer lexer("123_456_789");
        Token num = lexer.get();
        REQUIRE(num.type == Token::Type::INTEGER);
        REQUIRE(num.integer == 123456789);
        REQUIRE(lexer.getPos() == 11);
    }
    {
        Lexer lexer("123.456.789");
        Token num = lexer.get();
        REQUIRE(num.type == Token::Type::FLOAT);
        REQUIRE(num.floating == 123.456);
        REQUIRE(lexer.getPos() == 7);
    }
    {
        Lexer lexer("123e45");
        Token num = lexer.get();
        REQUIRE(num.type == Token::Type::FLOAT);
        REQUIRE(num.floating == 123e45);
        REQUIRE(lexer.getPos() == 6);
    }
    {
        Lexer lexer("0x123_456_789");
        Token num = lexer.get();
        REQUIRE(num.type == Token::Type::INTEGER);
        REQUIRE(num.integer == 0x123456789);
        REQUIRE(lexer.getPos() == 13);
    }
    {
        Lexer lexer("1x123_456_789");
        Token num = lexer.get();
        REQUIRE(num.type == Token::Type::INTEGER);
        REQUIRE(num.integer == 1);
        REQUIRE(lexer.getPos() == 1);
    }
}

TEST_CASE("specials", "[lex]") {
    const char* ops = "== = + += - -= -> * *= / /= % %= ! != & && &= | || |= ~ "
                      "^ ^= < <= << > >= >> : :: ( ) [ ] { } , ?";
    Lexer lex(ops);
    REQUIRE(lex.get().type == Token::Type::EQUAL);
    REQUIRE(lex.get().type == Token::Type::ASSIGN);
    REQUIRE(lex.get().type == Token::Type::ADD);
    REQUIRE(lex.get().type == Token::Type::ADD_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::SUB);
    REQUIRE(lex.get().type == Token::Type::SUB_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::ARROW);
    REQUIRE(lex.get().type == Token::Type::MUL);
    REQUIRE(lex.get().type == Token::Type::MUL_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::DIV);
    REQUIRE(lex.get().type == Token::Type::DIV_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::MOD);
    REQUIRE(lex.get().type == Token::Type::MOD_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::NOT);
    REQUIRE(lex.get().type == Token::Type::NOT_EQUAL);
    REQUIRE(lex.get().type == Token::Type::BIT_AND);
    REQUIRE(lex.get().type == Token::Type::AND);
    REQUIRE(lex.get().type == Token::Type::AND_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::BIT_OR);
    REQUIRE(lex.get().type == Token::Type::OR);
    REQUIRE(lex.get().type == Token::Type::OR_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::BIT_NOT);
    REQUIRE(lex.get().type == Token::Type::XOR);
    REQUIRE(lex.get().type == Token::Type::XOR_ASSIGN);
    REQUIRE(lex.get().type == Token::Type::LESS);
    REQUIRE(lex.get().type == Token::Type::LESS_EQUAL);
    REQUIRE(lex.get().type == Token::Type::LEFT_SHIFT);
    REQUIRE(lex.get().type == Token::Type::GREATER);
    REQUIRE(lex.get().type == Token::Type::GREATER_EQUAL);
    REQUIRE(lex.get().type == Token::Type::RIGHT_SHIFT);
    REQUIRE(lex.get().type == Token::Type::COLON);
    REQUIRE(lex.get().type == Token::Type::COLON_COLON);
    REQUIRE(lex.get().type == Token::Type::LEFT_PAREN);
    REQUIRE(lex.get().type == Token::Type::RIGHT_PAREN);
    REQUIRE(lex.get().type == Token::Type::LEFT_BRACKET);
    REQUIRE(lex.get().type == Token::Type::RIGHT_BRACKET);
    REQUIRE(lex.get().type == Token::Type::LEFT_BRACE);
    REQUIRE(lex.get().type == Token::Type::RIGHT_BRACE);
    REQUIRE(lex.get().type == Token::Type::COMMA);
    REQUIRE(lex.get().type == Token::Type::QUESTION);
}


TEST_CASE("character", "[lex]") {
    Lexer lex("'''");
    Token result = lex.get();
    REQUIRE(result.type == Token::Type::CHARACTER);
    REQUIRE(result.character == '\'');
    REQUIRE(lex.getPos() == 3);
}


TEST_CASE("string", "[lex]") {
    Lexer lex("\"marbl\"");
    Token result = lex.get();
    REQUIRE(result.type == Token::Type::STRING);
    REQUIRE(result.str == "marbl");
    REQUIRE(lex.getPos() == 7);
}
