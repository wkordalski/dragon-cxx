#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
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
    virtual void print(std::wostream &os) const { os << L"Token ["<< h <<"]" << std::endl; }
    virtual bool equal(const Token *t) const { assert(false && "Unimplemented comparison between tokens"); }
    virtual size_t hash() const { assert(false && "Unimplemented hash operation"); }

    // Token replaces this token.
    void replace(Token *);

    friend class Handle;

  protected:
    int handle() const { return h; }
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
    operator bool() const { return valid(); }
    bool operator !() const { return !valid(); }

    explicit operator int() const { return h; }

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
