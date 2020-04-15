#ifndef _LEKSER_
#define _LEKSER_HPP_ value
#include "token.hpp"
#include <queue>

std::queue<Token> tokens;

struct InputVal 
{
  std::string text;
  int lexemeBegin = 0;
  int currentPosition = 0;
  
  InputVal ( std::string const& input ) : text( input ) {  }

  char peekNext  () { return text [ currentPosition   ] ; }
  char getNext   () { return text [ currentPosition++ ] ; }
  void backtrack () { currentPosition = lexemeBegin ; }
  void getReady  () { lexemeBegin = currentPosition ; }

  void backtrack ( int n ) { currentPosition -= n ; }
  std::string getLex () 
  {
    return std::string(text, lexemeBegin, currentPosition - lexemeBegin);
  }  
};

Token isID ( InputVal & val )
{
  val.getReady();
  char c = val.getNext();
  if ( isalpha ( c ) )
  {
    while ( isalpha ( c ) || isdigit ( c ))
    {
      c = val.getNext();
    }
    val.backtrack(1);
    return Token (ID , val.getLex());
  }
  val.backtrack(1);
  return Token();
}

Token isInteger ( InputVal & val )
{
  val.getReady();
  char c = val.getNext();
  if( isdigit( c ) )
  {
    while ( isdigit ( c ) )
    {
      c = val.getNext();
    }
    val.backtrack(1);
    return Token ( NUMBER , val.getLex());
  }
  val.backtrack(1);
  return Token();
}

Token isOperation ( InputVal & val )
{
  val.getReady();
  char c = val.getNext();
  if( c == '*' )
    return Token ( MULTIPLY, val.getLex() );
  if( c == '/' )
    return Token ( DIVIDE, val.getLex() );
  if( c == '+' )
    return Token ( PLUS, val.getLex() );
  if( c == '-' )
    return Token ( MINUS, val.getLex() );

  val.backtrack(1);
  return Token();
}

Token isBracket ( InputVal & val )
{
  val.getReady();
  char c = val.getNext();
 
  if ( c == '(')
    return Token (OPENING_BRACKET , val.getLex());
 
  if ( c == ')' )
    return Token (CLOSING_BRACKET , val.getLex());
   
  val.backtrack(1);
  return Token (); 
}

Token isAssign ( InputVal & val )
{
  val.getReady();
  char c = val.getNext();
  if ( c == '=' )
    return Token (ASSIGN, val.getLex());
  val.backtrack(1);
  return Token();
}

Token getToken()
{
  Token t;
  std::string line;
  while(std::getline(std::cin, line))
  {
    InputVal elements (line);
    while(elements.lexemeBegin != line.size())
    {
      if((t = isID(elements)).tag != 0)
      {
        tokens.push(t);
      }
      else if((t = isBracket(elements)).tag != 0)
      {
        tokens.push(t);
      }
      else if((t = isInteger(elements)).tag != 0)
      {
        tokens.push(t);
      }
      else if((t = isAssign(elements)).tag != 0)
      {
        tokens.push(t);
      }
      else if((t = isOperation(elements)).tag != 0)
      {
        tokens.push(t);
      }
      else
      {
        ++elements.currentPosition;
      }
    }
  }
  
  if(tokens.empty())
  {
    return t;
  }
  else
  { 
    t = tokens.front();
    tokens.pop();
    return t;
  }
}

#endif /* ifndef _LEKSER_HPP_ */
