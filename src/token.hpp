#pragma once

#include <string>

namespace dragon
{
  // Represents a token
  class Token
  {
    Token * parent;
  public:
    virtual ~Token() = 0;
    virtual levelup() = 0;
  };

  struct Place
  {
    int row;
    int col;
  };

  class Identifier : public Token
  {
    std::wstring text;
    Place place;
  };

  class Operator : public Token
  {
    std::wstring text;
    Place place;
  };

  class Literal : public Token
  {
    std::wstring text;
    Place place;
  };

  class Newline : public Token
  {
    Place place;
  };

  class Indent : public Token
  {
    Place place;
  };

  class Dedent : public Token
  {
    Place place;
  };

  class Sequence : public Token
  {
    std::vector<Token *> tokens;
  };

  class Repeat : public Token
  {
    std::vector<Token *> tokens;
  };
}
