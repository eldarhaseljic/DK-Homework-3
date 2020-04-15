%{
#include "token.hpp"

int num_LINE = 1 , counter = 0, equal = 0;
int begin_tag = 0, end_tag = 0, end_and_close_tag = 0, close_tag = 0;
int name_attribute = 0 , value_attribute = 0, content = 0, ws = 0;

%}

special     [ \t]
letter      [a-zA-Z]
digit       [0-9]
tagBegin    \<{letter}+
tagEnd      \>
eq          [=]
tagClo      \<\/{letter}+\>
tagEndClose \/>
attname     [a-zA-Z0-9]+
attVal      \"[a-z0-9]*\"
cont        [a-zA-Z0-9]+([  ]|[a-zA-Z0-9])*

%%

\n            { num_LINE++; counter = 0;}
{special}     { counter++; };
{tagBegin}    { begin_tag++; counter += yyleng; return TAGBEGIN;}
{tagClo}      { close_tag++; counter += yyleng; return TAGCLOSE; }
{tagEndClose} { end_and_close_tag++; counter += yyleng; return TAGENDANDCLOSE;} 
{tagEnd}      { end_tag++; counter += yyleng; return TAGEND; }
{attname}     { name_attribute++; counter += yyleng; return ATTRIBUTENAME; }
{eq}          { equal++; counter++; return EQUAL; }
{attVal}      { value_attribute++; counter += (yyleng+1); return ATTRIBUTEVALUE; }
{cont}        { content++; counter += yyleng; return CONTENT; }

%%

int main(void)
{
  int tok;
  while(tok = yylex())
  {
    Token t(tok, yytext);
    t.print(num_LINE, counter-t.lexeme.size());
  }
  printf("TAGBEGIN: %d\n", begin_tag);
  printf("TAGEND: %d\n", end_tag);
  printf("TAGCLOSE: %d\n", close_tag);
  printf("TAGENDANDCLOSE: %d\n", end_and_close_tag);
  printf("ATTRIBUTENAME: %d\n", name_attribute);
  printf("EQUAL: %d\n", equal);
  printf("ATTRIBUTEVALUE: %d\n", value_attribute);
  printf("CONTENT: %d\n", content);

  return 0;
}
