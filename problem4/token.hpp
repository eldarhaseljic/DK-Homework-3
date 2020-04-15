#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <iostream>
#include <string>

const int TAGBEGIN = 1;
const int TAGEND = 2;
const int TAGCLOSE = 3;
const int TAGENDANDCLOSE = 4;
const int ATTRIBUTENAME = 5;
const int EQUAL = 6;
const int ATTRIBUTEVALUE = 7;
const int CONTENT = 8;

std::string TagToString(int tag)
{
  switch(tag)
  {
    case TAGBEGIN:
      return "TAGBEGIN";
    case TAGEND:
      return "TAGEND";
    case TAGCLOSE:
      return "TAGCLOSE";
    case TAGENDANDCLOSE:
      return "TAGENDANDCLOSE";
    case ATTRIBUTEVALUE:
      return "ATTRIBUTEVALUE";
    case ATTRIBUTENAME:
      return "ATTRIBUTENAME";
    case EQUAL:
      return "EQUAL";
    case CONTENT:
      return "CONTENT";
  }
  return "";
}
struct Token {
  int tag = 0;
  std::string lexeme;

  Token() = default;
  Token(int i, std::string const& s) : tag(i), lexeme(s) {}

  Token& operator=(const Token& t) {
    //return *this = Token(t);
    tag = t.tag;
    lexeme = t.lexeme;
    return *this;
  }

  bool operator!() { return tag != 0; }

  explicit operator bool() { return tag; }

  void print( int line, int column ) 
  {
    std::cout << "<" << TagToString(tag) << ", " << lexeme << ">:";
    std::cout << "line " << line << ", column " << column + 1 << std::endl;
  }
};

#endif  // _TOKEN_HPP_
