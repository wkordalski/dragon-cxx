#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "utils/hash.hpp"

#include "location.hh"

namespace dragon
{
  class Handle;
  class Node;
  class Visitor;

  class Handle
  {
  public:
    Handle();
    Handle(Node *);
    Handle(const Handle &);
    virtual ~Handle();

    Handle & operator = (const Handle &);

    Node * operator -> () const;
    Node * operator *  () const;

    Node * get() const;
    const Handle & set(Node *) const;
    Handle & set(Node *);

    bool valid() const;
    operator bool() const { return valid(); }
    bool operator !() const { return !valid(); }

    Handle operator % (Handle h) const;

    template<class T>
    T * is()
    {
      return dynamic_cast<T*>(this->get());
    }

    template<class T>
    T * as()
    {
      if(auto r = is<T>()) return r;
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
    explicit operator int() const { return h; }


  protected:
    int h;

    friend class Node;
    friend class Root;
  };

  // Represents a token
  class Node
  {
  protected:
    Handle self;    // handle for itself
  public:
    virtual ~Node() {};
    virtual void accept(Visitor &v) { assert("Unimplemented acceptor!" and false); }
    virtual bool equal(const Node *t) const { assert(false && "Unimplemented comparison between nodes"); }
    virtual size_t hash() const { assert(false && "Unimplemented hash operation"); }

    // Token replaces this token.
    void replace(Node *);

    friend class Handle;

  public:
    Handle handle() const { return self; }
  };


  // A root object
  class Root : public Handle
  {
    std::list<int>::iterator entry;
  public:
    Root();
    Root(const Handle &h);
    Root(Root &h);
    Root(Root &&h);

    Root & operator = (const Handle &h);
    Root & operator = (Root &h);
    Root & operator = (Root &&h);

    virtual ~Root();
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
      return l->equal(r.get());
    }
  };


  std::wostream & operator << (std::wostream &os, dragon::Handle h);
}
