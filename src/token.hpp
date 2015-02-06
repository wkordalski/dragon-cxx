#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

#include "utils/hash.hpp"

#include "location.hh"

namespace dragon
{
  // Represents a token
  class Token
  {
    int h;    // simple handle for itself :P
  public:
    virtual ~Token() {};
    virtual void print(std::wostream &os) { os << L"Token ["<< h <<"]" << std::endl; }
    virtual bool equal(Token *t) { assert(false && "Unimplemented comparison between tokens"); }
    virtual size_t hash() const { assert(false && "Unimplemented hash operation"); }

    // Token replaces this token.
    void replace(Token *);

    friend class Handle;

  protected:
    int handle() { return h; }
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
    operator bool() { return valid(); }
    bool operator !() { return !valid(); }

    explicit operator int() { return h; }

    Handle operator % (Handle h) const
    {
      if(!valid() or !h.valid()) return Handle();
      if(get()->equal(h.get().get())) return h;
      else return Handle();
    }

    template<class T>
    T * as()
    {
      return dynamic_cast<T*>(this->get().get());
    }

    template<class T>
    T * is()
    {
      if(auto r = as<T>()) return r;
      assert("Wrong cast is here!");
      return nullptr;
    }

    template<class T, class... Args>
    static Handle make(Args... args)
    {
      return Handle(new T(args...));
    }

    static void cleanup();

    // DEBUGGING PURPOSES ONLY!!!
    explicit Handle(int h);
    static bool exists(int h);

  protected:
    int h;

    friend class Token;
  };

  struct Place
  {
    unsigned long row;
    unsigned long col;
    unsigned long len;

    dragon::location location(std::string &f)
    {
      dragon::location c; c.initialize(&f, row+1, col+1); c.columns(len);
      return c;
    }
  };

  class Identifier : public Token
  {
  public:
    std::wstring text;
    Place place;

    Identifier() = default;
    Identifier(std::wstring s) : text(s) {}

  public:
    virtual void print(std::wostream &os) { os << L"Identifier ["<<handle()<<"] \""<<text<<"\"" << std::endl; }
    virtual bool equal(Token *t) { if(auto tt = dynamic_cast<Identifier*>(t)) return (text == tt->text); else return false; }
    virtual std::size_t hash() const
    {
      return hash_args< std::hash<std::type_index>, std::hash<std::wstring> >( std::type_index(typeid(*this)), text );
    }
  };

  class Operator : public Token
  {
  public:
    std::wstring text;
    Place place;

  public:
    virtual void print(std::wostream &os) { os << L"Operator ["<<handle()<<"] <nonprintable>" << std::endl; }
  };

  class Literal : public Token
  {
  public:
    std::wstring text;
    Place place;

  public:
    virtual void print(std::wostream &os) { os << L"[! "<<text<<"]"; }

    void parse_literal();
  };

  class Newline : public Token
  {
  public:
    Place place;
    virtual void print(std::wostream &os) { os << L"Newline ["<<handle()<<"]" << std::endl; }
  };

  class Indent : public Token
  {
  public:
    Place place;
    virtual void print(std::wostream &os) { os << L"Indent ["<<handle()<<"]" << std::endl; }
  };

  class Dedent : public Token
  {
  public:
    Place place;
    virtual void print(std::wostream &os) { os << L"Dedent ["<<handle()<<"]" << std::endl; }
  };
}

namespace std
{
  // For hash and equal functors
  template<>
  struct hash<dragon::Handle>
  {
    std::size_t operator() (const dragon::Handle &h) const
    {
      return h->hash();
    }
  };

  template<>
  struct equal_to<dragon::Handle>
  {
    bool operator () (const dragon::Handle &l, const dragon::Handle &r) const
    {
      return l->equal(r.get().get());
    }
  };
}
