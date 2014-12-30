#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace dragon
{
  // Represents a token
  class Token
  {
    Token * parent;
  public:
    virtual ~Token() {};
    virtual void levelup() = 0;
    virtual void print(std::wostream &os) { os << L"[-]"; }
  };

  struct Place
  {
    unsigned long row;
    unsigned long col;
    unsigned long len;
  };

  class Identifier : public Token
  {
  public:
    std::wstring text;
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[Identifier: "<<text<<"]"; }
  };

  class Operator : public Token
  {
  public:
    std::wstring text;
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[Operator: "<<text<<"]"; }
  };

  class Literal : public Token
  {
  public:
    std::wstring text;
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[Literal: "<<text<<"]"; }
  };

  class Newline : public Token
  {
  public:
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"\n"; }
  };

  class Indent : public Token
  {
  public:
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[>>]"; }
  };

  class Dedent : public Token
  {
  public:
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};
    virtual void print(std::wostream &os) { os << L"[<<]"; }

  public:
    virtual void levelup() { converter(this); }
  };

  class Sequence : public Token
  {
    std::vector<Token *> tokens;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
  };

  class Repeat : public Token
  {
    std::vector<Token *> tokens;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
  };
}
