#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <iostream>
#include <string>

const int ID = 1;
const int OPENING_BRACKET = 2;
const int CLOSING_BRACKET = 3;
const int MULTIPLY = 4;
const int DIVIDE = 5;
const int PLUS = 6;
const int MINUS = 7;
const int ASSIGN = 8;
const int NUMBER = 9;

struct Token {
  int tag = 0;
  std::string lexeme;

  Token() = default;
  Token(int i, std::string const& s) : tag(i), lexeme(s) {}

  Token& operator= (const Token& t) {
    //return *this = Token(t);
    tag = t.tag;
    lexeme = t.lexeme;
    return *this;
  }

  bool operator!() { return tag != 0; }

  explicit operator bool() { return tag; }

  void print() {
    std::cout << "<" << tag << ", " << lexeme << ">" << std::endl;
  }
};

#endif  // _TOKEN_HPP_
