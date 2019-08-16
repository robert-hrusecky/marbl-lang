/**@file	/home/robert/workspace/occ/src/lexer/Token.cpp
 * @author	robert
 * @version	800
 * @date
 * 	Created:	07th Jul 2019
 * 	Last Update:	07th Jul 2019
 */

#include "lexer/Token.hpp"
#include <utility>
#include <cassert>

namespace lex {

const std::unordered_map<std::string, Token::Type> keywords = {
    {"func", Token::Type::FUNC},     {"class", Token::Type::CLASS},
    {"struct", Token::Type::STRUCT}, {"mut", Token::Type::MUT},
    {"con", Token::Type::CON},       {"ret", Token::Type::RET},
    {"break", Token::Type::BREAK},   {"if", Token::Type::IF},
    {"else", Token::Type::ELSE},     {"while", Token::Type::WHILE},
    {"for", Token::Type::FOR},       {"sizeof", Token::Type::SIZEOF},
    {"type", Token::Type::TYPE},     {"pub", Token::Type::PUB},
};

Token::Token(Token::Type type) : type(type) {}

Token::Token(Token::Type type, const std::string& str) : type(type), str(str) {
    assert(type == Type::ID || type == Type::STRING);
}

Token::Token(std::uint64_t integer) : type(Type::INTEGER), integer(integer) {}

Token::Token(double floating) : type(Type::FLOAT), floating(floating) {}

Token::Token(char character) : type(Type::CHARACTER), character(character) {}

Token::Token(const Token& rhs) : type(rhs.type) {
    switch (type) {
    case Type::ID:
    case Type::STRING:
        str = rhs.str;
        break;
    case Type::INTEGER:
        integer = rhs.integer;
        break;
    case Type::FLOAT:
        floating = rhs.floating;
        break;
    case Type::CHARACTER:
        character = rhs.character;
        break;
    default:
        break;
    }
}

Token& Token::operator=(const Token& rhs) {
    type = rhs.type;
    switch (type) {
    case Type::ID:
    case Type::STRING:
        str = rhs.str;
        break;
    case Type::INTEGER:
        integer = rhs.integer;
        break;
    case Type::FLOAT:
        floating = rhs.floating;
        break;
    case Type::CHARACTER:
        character = rhs.character;
        break;
    default:
        break;
    }
    return *this;
}

Token::Token(Token&& rhs) : type(std::move(rhs.type)) {
    switch (type) {
    case Type::ID:
    case Type::STRING:
        new (&str) std::string(std::move(rhs.str));
        break;
    case Type::INTEGER:
        new (&integer) std::uint64_t(std::move(rhs.integer));
        break;
    case Type::FLOAT:
        new (&floating) double(std::move(rhs.floating));
        break;
    case Type::CHARACTER:
        new (&character) char(std::move(rhs.character));
        break;
    default:
        break;
    }
}

Token& Token::operator=(Token&& rhs) {
    type = std::move(rhs.type);
    switch (type) {
    case Type::ID:
    case Type::STRING:
        new (&str) std::string(std::move(rhs.str));
        break;
    case Type::INTEGER:
        new (&integer) std::uint64_t(std::move(rhs.integer));
        break;
    case Type::FLOAT:
        new (&floating) double(std::move(rhs.floating));
        break;
    case Type::CHARACTER:
        new (&character) char(std::move(rhs.character));
        break;
    default:
        break;
    }
    return *this;
}

Token::~Token() {
    if (type == Type::ID || type == Type::STRING) {
        using std::string;
        // manual destructor call necessary as the compiler
        // doesn't know which union member is active.
        str.string::~string();
    }
}

} // namespace lex
// namespace lex
