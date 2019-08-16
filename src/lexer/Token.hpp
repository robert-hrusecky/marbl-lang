#ifndef LEX_TOKEN_HPP
#define LEX_TOKEN_HPP

#include <cstddef>
#include <string>
#include <unordered_map>

namespace lex {

struct Token {
    enum class Type {
        FUNC,
        CLASS,
        STRUCT,
        MUT,
        CON,
        RET,
        BREAK,
        IF,
        ELSE,
        WHILE,
        FOR,
        SIZEOF,
        TYPE,
        PUB,
        ID,
        CHARACTER,
        STRING,
        INTEGER,
        FLOAT,
        EQUAL,
        ASSIGN,
        ADD_ASSIGN,
        ADD,
        SUB_ASSIGN,
        ARROW,
        SUB,
        MUL_ASSIGN,
        MUL,
        DIV_ASSIGN,
        DIV,
        MOD_ASSIGN,
        MOD,
        NOT_EQUAL,
        NOT,
        AND_ASSIGN,
        AND,
        BIT_AND,
        OR_ASSIGN,
        OR,
        BIT_OR,
        BIT_NOT,
        XOR_ASSIGN,
        XOR,
        LESS_EQUAL,
        LEFT_SHIFT,
        LESS,
        GREATER_EQUAL,
        RIGHT_SHIFT,
        GREATER,
        COLON_COLON,
        COLON,
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        QUESTION,
        END_OF_FILE,
        UNRECOGNIZED,
    };

    Type type;
    union {
        std::string str;
        std::uint64_t integer;
        double floating;
        char character;
    };

    explicit Token(Type);
    explicit Token(Type, const std::string&);
    explicit Token(std::uint64_t);
    explicit Token(double);
    explicit Token(char);

    // rule of five
    Token(const Token&);
    Token& operator=(const Token&);
    Token(Token&&);
    Token& operator=(Token&&);
    ~Token();
};

extern const std::unordered_map<std::string, Token::Type> keywords;

} // namespace lex
// namespace lex

#endif // LEX_TOKEN_HPP
