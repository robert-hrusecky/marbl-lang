#ifndef LEX_LEXER_HPP
#define LEX_LEXER_HPP

#include "lexer/Token.hpp"
#include <deque>
#include <limits>
#include <string>
#include <vector>

namespace lex {

class Lexer {
  public:
    explicit Lexer(std::string);
    Token get();
    Token expect(Token::Type);
    const Token& peek(std::size_t count);
    std::size_t getPos();

  private:
    std::deque<Token> queue;
    std::vector<std::string> parsed_file;
    std::string file;
    std::size_t pos;

    std::size_t charsLeft();
    bool charIs(char);

    Token parseKeywordOrId();
    Token parseDecimal();
    Token parseHex();
    Token parseFloat();
    Token parseNumber();
    Token parseSpecial();
    Token parseCharacter();
    Token parseString();
    Token parse();

    const static std::string specialChars;

    enum class Branch {
        KEYWORD_OR_ID,
        NUMBER,
        SPECIAL,
        CHARACTER,
        STRING,
        UNRECOGNIZED,
    };

    const static Branch* branchTable;
    static Branch* initBranchTable();
};

} // namespace lex

#endif // LEX_LEXER_HPP
