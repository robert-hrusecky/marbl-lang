/**@file	/home/robert/workspace/occ/src/lexer/Lexer.cpp
 * @author	robert
 * @version	800
 * @date
 * 	Created:	04th Jul 2019
 * 	Last Update:	04th Jul 2019
 */

#include "lexer/Lexer.hpp"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace lex {

const std::string Lexer::specialChars = "+-*/%><^!&|~=.{}[](),;:?";

Lexer::Branch* Lexer::initBranchTable() {
    constexpr std::size_t size = std::numeric_limits<unsigned char>::max() + 1;
    static Branch branchTable[size];
    for (size_t i = 0; i < size; i++) {
        branchTable[i] = Branch::UNRECOGNIZED;
    }

    for (unsigned char c = 'a'; c <= 'z'; c++) {
        branchTable[c] = Branch::KEYWORD_OR_ID;
    }

    for (unsigned char c = 'A'; c <= 'Z'; c++) {
        branchTable[c] = Branch::KEYWORD_OR_ID;
    }

    branchTable[static_cast<unsigned char>('_')] = Branch::KEYWORD_OR_ID;

    for (unsigned char c = '0'; c <= '9'; c++) {
        branchTable[c] = Branch::NUMBER;
    }

    for (const char& c : Lexer::specialChars) {
        branchTable[static_cast<unsigned char>(c)] = Branch::SPECIAL;
    }


    branchTable[static_cast<unsigned char>('\'')] = Branch::CHARACTER;
    branchTable[static_cast<unsigned char>('"')] = Branch::STRING;

    return &branchTable[0];
}

const Lexer::Branch* Lexer::branchTable = initBranchTable();

Lexer::Lexer(std::string file) : file(file), pos(0) {}

std::size_t Lexer::charsLeft() {
    return pos < file.size() ? file.size() - pos : 0;
}

bool Lexer::charIs(char c) {
    return charsLeft() > 0 && file[pos] == c;
}

bool isIdChar(char c) { return std::isalpha(c) || std::isdigit(c) || c == '_'; }

Token Lexer::parseKeywordOrId() {
    for (const auto& it : keywords) {
        if (it.first.size() <= charsLeft() &&
            (it.first.size() == charsLeft() ||
             !isIdChar(file[pos + it.first.size()])) &&
            file.compare(pos, it.first.size(), it.first) == 0) {

            pos += it.first.size();
            return Token(it.second);
        }
    }
    std::size_t len = 0;
    while (len < charsLeft() &&
           (std::isalpha(file[pos + len]) || file[pos + len] == '_')) {
        len++;
    }

    Token result(Token::Type::ID, file.substr(pos, len));
    pos += len;
    return result;
}

bool isDecimalChar(char c) { return std::isdigit(c) || c == '_'; }

bool isHexChar(char c) {
    return std::isdigit(c) || (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f') || c == '_';
}

Token Lexer::parseDecimal() {
    std::uint64_t value = 0;
    for (; pos < file.size() && isDecimalChar(file[pos]); pos++) {
        if (file[pos] != '_') {
            std::uint64_t prev = value;
            value *= 10;
            value += file[pos] - '0';
            if (value < prev) {
                // Overflow; stop parsing here
                return Token(prev);
            }
        }
    }

    return Token(value);
}

Token Lexer::parseHex() {
    // Move past 0x
    pos += 2;

    std::uint64_t value = 0;
    for (; pos < file.size() && isHexChar(file[pos]); pos++) {
        char c = file[pos];
        if (c == '_') {
            continue;
        }

        std::uint64_t prev = value;
        value *= 0x10;
        if (std::isdigit(c)) {
            value += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            value += 10 + (c - 'A');
        } else if (c >= 'a' && c <= 'f') {
            value += 10 + (c - 'a');
        }
        if (value < prev) {
            // Overflow; stop parsing here
            return Token(prev);
        }
    }

    return Token(value);
}

Token Lexer::parseFloat() {
    const char* str = &file.c_str()[pos];
    char* str_end;
    double floating = std::strtod(str, &str_end);
    pos += (str_end - str) / sizeof(char);
    return Token(floating);
}

Token Lexer::parseNumber() {
    if (charsLeft() >= 2 && file[pos] == '0' && file[pos + 1] == 'x') {
        return parseHex();
    }

    std::size_t peek = pos;
    while (peek < file.size() && std::isdigit(file[peek])) {
        peek++;
    }
    if (peek < file.size() &&
        (file[peek] == '.' || file[peek] == 'e' || file[peek] == 'E')) {
        return parseFloat();
    }

    // otherwise, it's a decimal number
    return parseDecimal();
}

Token Lexer::parseSpecial() {
    switch (file[pos]) {
    case '=':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::EQUAL);
        }
        return Token(Token::Type::ASSIGN);
    case '+':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::ADD_ASSIGN);
        }
        return Token(Token::Type::ADD);
    case '-':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::SUB_ASSIGN);
        }
        if (charIs('>')) {
            pos++;
            return Token(Token::Type::ARROW);
        }
        return Token(Token::Type::SUB);
    case '*':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::MUL_ASSIGN);
        }
        return Token(Token::Type::MUL);
    case '/':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::DIV_ASSIGN);
        }
        return Token(Token::Type::DIV);
    case '%':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::MOD_ASSIGN);
        }
        return Token(Token::Type::MOD);
    case '!':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::NOT_EQUAL);
        }
        return Token(Token::Type::NOT);
    case '&':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::AND_ASSIGN);
        }
        if (charIs('&')) {
            pos++;
            return Token(Token::Type::AND);
        }
        return Token(Token::Type::BIT_AND);
    case '|':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::OR_ASSIGN);
        }
        if (charIs('|')) {
            pos++;
            return Token(Token::Type::OR);
        }
        return Token(Token::Type::BIT_OR);
    case '~':
        pos++;
        return Token(Token::Type::BIT_NOT);
    case '^':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::XOR_ASSIGN);
        }
        return Token(Token::Type::XOR);
    case '<':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::LESS_EQUAL);
        }
        if (charIs('<')) {
            pos++;
            return Token(Token::Type::LEFT_SHIFT);
        }
        return Token(Token::Type::LESS);
    case '>':
        pos++;
        if (charIs('=')) {
            pos++;
            return Token(Token::Type::GREATER_EQUAL);
        }
        if (charIs('>')) {
            pos++;
            return Token(Token::Type::RIGHT_SHIFT);
        }
        return Token(Token::Type::GREATER);
    case ':':
        pos++;
        if (charIs(':')) {
            pos++;
            return Token(Token::Type::COLON_COLON);
        }
        return Token(Token::Type::COLON);
    case '(':
        pos++;
        return Token(Token::Type::LEFT_PAREN);
    case ')':
        pos++;
        return Token(Token::Type::RIGHT_PAREN);
    case '[':
        pos++;
        return Token(Token::Type::LEFT_BRACKET);
    case ']':
        pos++;
        return Token(Token::Type::RIGHT_BRACKET);
    case '{':
        pos++;
        return Token(Token::Type::LEFT_BRACE);
    case '}':
        pos++;
        return Token(Token::Type::RIGHT_BRACE);
    case ',':
        pos++;
        return Token(Token::Type::COMMA);
    case '?':
        pos++;
        return Token(Token::Type::QUESTION);
    default:
        return Token(Token::Type::UNRECOGNIZED);
    }
}

Token Lexer::parseCharacter() {
    if (charsLeft() < 3 || file[pos + 2] != '\'') {
        return Token(Token::Type::UNRECOGNIZED);
    }
    pos += 3;
    return Token(file[pos - 2]);
}

Token Lexer::parseString() {
    // Move past "
    pos++;
    std::size_t len = 0;
    while (len < charsLeft() && file[pos + len] != '"') {
        len++;
    }
    if (charsLeft() == 0) return Token(Token::Type::UNRECOGNIZED);
    Token result(Token::Type::STRING, file.substr(pos, len));
    pos += len + 1;
    return result;
}

Token Lexer::parse() {
    if (pos >= file.size()) {
        return Token(Token::Type::END_OF_FILE);
    }

    while (pos < file.size() && std::isspace(file[pos])) {
        pos++;
    }

    switch (branchTable[static_cast<unsigned char>(file[pos])]) {
    case Branch::KEYWORD_OR_ID:
        return parseKeywordOrId();
    case Branch::NUMBER:
        return parseNumber();
    case Branch::SPECIAL:
        return parseSpecial();
    case Branch::CHARACTER:
        return parseCharacter();
    case Branch::STRING:
        return parseString();
    default:
        return Token(Token::Type::UNRECOGNIZED);
    }
}

Token Lexer::get() {
    if (!queue.empty()) {
        Token result = queue.front();
        queue.pop_front();
        return result;
    }
    return parse();
}

Token Lexer::expect(Token::Type type) {
    Token result = get();
    if (result.type != type) {
        // TODO: Failure condition
    }
    return result;
}

const Token& Lexer::peek(std::size_t count) {
    while (queue.size() <= count) {
        queue.push_back(get());
    }
    return queue[count];
}

std::size_t Lexer::getPos() { return pos; }

} // namespace lex
