#include <iostream>
#include <string>
#include <cctype>

#define TAGBEGIN 1
#define TAGEND   2
#define TAGCLOSE 3
#define TAGENDANDCLOSE 4
#define ATTRIBUTENAME 5
#define EQUAL 6
#define ATTRIBUTEVALUE 7
#define CONTENT 8

int status [8] = { 0, 0, 0, 0, 0, 0, 0, 0};

void TokensStatus()
{
  for(auto i = 0; i<sizeof(status)/sizeof(int); ++i)
  {
    switch(i+1)
    {
      case TAGBEGIN:
        std::cout << "TAGBEGIN: " << status[i]  << std::endl;
        break;
      case TAGEND:
        std::cout << "TAGEND: " << status[i]  << std::endl;
        break;
      case TAGCLOSE:
        std::cout << "TAGCLOSE: " << status[i] << std::endl;
        break;
      case EQUAL:
        std::cout << "EQUAL: " << status[i] << std::endl;
        break;
      case CONTENT:
        std::cout << "CONTENT: " << status[i] << std::endl;
        break;
      case TAGENDANDCLOSE:
        std::cout << "TAGENDANDCLOSE: " << status[i] << std::endl;
        break;
      case ATTRIBUTENAME:
        std::cout << "ATTRIBUTENAME: " << status[i] << std::endl;
        break;
      case ATTRIBUTEVALUE:
        std::cout <<  "ATTRIBUTEVALUE: " << status[i] << std::endl;
        break;
    }
  }
}

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

struct Token 
{
  int tag = 0; 
  std::string lexeme;
  
  Token() = default;
  Token(int t, std::string input) : tag{t}, lexeme{input} {}
  Token& operator= (const Token& t)
  {
    tag = t.tag;
    lexeme = t.lexeme;
    return *this;
  }

  bool operator!() { return tag != 0; }
  explicit operator bool()  { return tag; }

};

struct InputBuffer
{
  std::string text;
  int lexemeBegin = 0;
  int currentPosition = 0;

  InputBuffer ( std::string line ) : text{ line } {};

  char peekNext  () { return text[ currentPosition   ]; }
  char getNext   () { return text[ currentPosition++ ]; }
  void backtrack () { currentPosition = lexemeBegin   ; }
  void getReady  () { lexemeBegin = currentPosition   ; }

  void backtrack ( int n ) { currentPosition -= n; }
  std::string getLex ()
  {
    return std::string(text,lexemeBegin, currentPosition - lexemeBegin);
  }
};

void print(Token & t, InputBuffer & elements, int num)
{
  std::cout << "<" << TagToString(t.tag) << ", " << t.lexeme << ">: ";
  std::cout << "line " << num << ", column ";
  std::cout << elements.lexemeBegin  + 1  << std::endl;
}

Token isTagBegin( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if(c == '<' && isalpha(elements.peekNext()))
  {
    while( isalpha(elements.peekNext()) )
    {
      c = elements.getNext();
    }
    ++status[TAGBEGIN-1];
    return Token (TAGBEGIN, elements.getLex());
  }
  elements.backtrack(1);
  return Token();
}

Token isTagEnd ( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if( c == '/' && ( elements.peekNext() == '>' ) )
  {
    c = elements.getNext();
    if( isspace( elements.peekNext() )
        || ( elements.currentPosition == elements.text.size() )
        || ( elements.peekNext() == '<' ) )
    {
      ++status[TAGENDANDCLOSE-1];
      return Token (TAGENDANDCLOSE, elements.getLex()); 
    }
    elements.backtrack(1);
  }
  
  if( c == '>' && ( isspace( elements.peekNext() )
               || ( elements.currentPosition == elements.text.size() )
               || ( elements.peekNext() == '<' ) ) )
  {
    ++status[TAGEND-1];
    return Token (TAGEND, elements.getLex()); 
  }
  
  elements.backtrack(1);
  return Token();
}

Token isTagClose ( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if( c == '<' && elements.peekNext() == '/' )
  {
    while(!isspace(elements.peekNext()) && 
          ( elements.currentPosition != elements.text.size() ) )
    {
      c = elements.getNext();
    }
    ++status[TAGCLOSE-1];
    return Token (TAGCLOSE, elements.getLex());
  }
  elements.backtrack(1);
  return Token();
}

Token isAttributeValue ( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if( c == '\"' )
  {
    elements.getReady();
    while( elements.peekNext() != '\"')
    {
      c = elements.getNext();
    }
    std::string temp = elements.getLex();
    c = elements.getNext();
    ++status[ATTRIBUTEVALUE-1];
    return Token(ATTRIBUTEVALUE,temp);
  }
  elements.backtrack(1);
  return Token();
}

Token isAttributeName ( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if( status[TAGBEGIN-1] > status[TAGEND-1]  )
      {
        if( isalpha(c) )
        {
          while( isalpha(elements.peekNext()) || isdigit(elements.peekNext()) )
          {
            c = elements.getNext();
          }
        ++status[ATTRIBUTENAME-1];
        return Token (ATTRIBUTENAME , elements.getLex()); 
        }
      }
  elements.backtrack();
  return Token ();
}

Token isEqual ( InputBuffer & elements )
{
  elements.getReady();
  char c = elements.getNext();
  if( c == '=' && (status[ATTRIBUTENAME-1] > status[ATTRIBUTEVALUE-1]) )
  {
    ++status[EQUAL-1];
    return Token (EQUAL , elements.getLex());
  }
  elements.backtrack(1);
  return Token();
}

Token isContent ( InputBuffer & elements )
{
  if(elements.currentPosition!= elements.text.size())
  {
    elements.getReady();
    if(!isspace(elements.peekNext()))
    {
      char c = elements.getNext();
      while((elements.currentPosition != elements.text.size()))
      {
        c = elements.getNext();
      }
      ++status[CONTENT-1];
      return Token (CONTENT, elements.getLex());
    }
  }
  return Token ();
}

int main(int argc, char *argv[])
{
  std::string line;
  int num_line = 0;
  while(std::getline(std::cin,line))
  {
    ++num_line;
    InputBuffer elements (line);
    Token t;
    while(elements.lexemeBegin != line.size())
    {
      if((t = isTagBegin(elements)).tag != 0)
      { 
        print(t,elements,num_line);
      }
      else if ((t = isTagEnd(elements)).tag != 0)
      { 
        print(t,elements,num_line);
      }
      else if ((t = isTagClose(elements)).tag != 0)
      { 
        print(t,elements,num_line);
      }
      else if ((t = isAttributeName(elements)).tag != 0)
       { 
        print(t,elements,num_line);
      }
      else if ((t = isEqual(elements)).tag != 0)
       { 
        print(t,elements,num_line);
      }
      else if ((t = isAttributeValue(elements)).tag != 0)
       { 
        print(t,elements,num_line);
      }
      else if ((t = isContent(elements)).tag != 0)
      { 
        print(t,elements,num_line);
      }
      else
      {
        ++elements.currentPosition;
      }
    }
  }
  TokensStatus();
  return 0;
}
