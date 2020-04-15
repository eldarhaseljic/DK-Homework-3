%{
#include "token.hpp"
%}

identifier   [a-zA-Z]([a-zA-Z]|[0-9])*
number       [0-9]*
assign       [=]
openBracket  [(]
closeBracket [)]
multiply     [*]
divide       [/]
plus         [+]
minus        [-]

%%

{identifier}   { return ID; }
{openBracket}  { return OPENING_BRACKET; }
{closeBracket} { return CLOSING_BRACKET; }
{multiply}     { return MULTIPLY; }
{divide}       { return DIVIDE; }
{plus}         { return PLUS; }
{minus}        { return MINUS; }
{assign}       { return ASSIGN; }
{number}       { return NUMBER; }
[ \t\n]       ;

%%

int main(void) {
  int tag;
  while(tag = yylex())
  {
  Token t = Token (tag , yytext);
  t.print();
  }
  return 0;
}

