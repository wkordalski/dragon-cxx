#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace dragon
{
  // Represents a token
  class Token
  {
    int h;    // simple handle for itself :P
  public:
    virtual ~Token() {};
    virtual void levelup() = 0;
    virtual void print(std::wostream &os) { os << L"[-]"; }

    // Token replaces this token.
    void replace(Token *);

    friend class Handle;
  };

  class Handle
  {
  public:
    typedef std::shared_ptr<Token> pointer;

    Handle();
    Handle(Token *);
    Handle(pointer);
    Handle(const Handle &);
    ~Handle();

    Handle & operator = (const Handle &);

    pointer operator -> () const;
    pointer operator *  () const;

    pointer get() const;
    const Handle & set(pointer) const;
    Handle & set(pointer);

    bool valid() const;

  protected:
    int h;

    friend class Token;
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
    virtual void print(std::wostream &os) { os << L"[# "<<text<<"]"; }
  };

  class Operator : public Token
  {
  public:
    std::wstring text;
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[@ "<<text<<"]"; }
  };

  class Literal : public Token
  {
  public:
    std::wstring text;
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[! "<<text<<"]"; }
  };

  class Newline : public Token
  {
  public:
    Place place;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os << L"[=]"; }
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
  public:
    std::vector<Handle> tokens;

    std::function<void(Token*)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os<<L"(";for(auto h : tokens) h->print(os); os<<L")"; }
  };

  class Alternative : public Token
  {
  public:
    Handle token;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os<<L"{"; token->print(os); os<<L"}"; }
  };

  class Repeat : public Token
  {
  public:
    std::vector<Handle> tokens;

    std::function<void(Token *)> converter = [](Token *t){};

  public:
    virtual void levelup() { converter(this); }
    virtual void print(std::wostream &os) { os<<L"(";for(auto h : tokens) h->print(os); os<<L")"; }
  };
}
