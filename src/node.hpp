#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "visitor.hpp"

#include "utils/hash.hpp"

#include "location.hh"

#include <boost/serialization/serialization.hpp>

namespace dragon
{
  class Handle;
  class Node;

  class Handle
  {
    typedef int id;
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

    // SERIALIZATION
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
      ar & h;
    }

  protected:
    id h;

    friend class Node;
    friend class Root;
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

  // Represents a token
  class Node
  {
  protected:
    Handle self;    // handle for itself
  public:
    virtual ~Node() {};
    virtual void accept(Visitor &v) { assert("Unimplemented acceptor!" and false); }
    
    // Token replaces this token.
    void replace(Node *);
    
    void * operator new (std::size_t size);
    void operator delete(void *pointer, std::size_t size);

    friend class Handle;

  public:
    Handle & handle() { return self; }
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
  
  void * allocate(std::size_t size);
  void deallocate(void *pointer, std::size_t size);
  std::size_t memory_use();
  std::size_t memory_limit();
  void memory_limit(std::size_t limit);
  
  template<class T>
  class NodeAllocator : public std::allocator<T>
  {
  public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    T * allocate(size_type count)
    {
      std::size_t size = count * sizeof(T);
      return reinterpret_cast<T*>(dragon::allocate(size));
    }
    void deallocate(T *pointer, size_type count)
    {
      std::size_t size = count * sizeof(T);
      dragon::deallocate(pointer, size);
    }
    
    template<class U>
    bool operator == (const U& rhs)
    {
      return false;
    }
    
    bool operator == (const T& rhs)
    {
      return true;
    }
    
    template<class U>
    bool operator != (const U& rhs)
    {
      return !(*this == rhs);
    }
    
    template<class U>
    struct rebind
    {
      typedef NodeAllocator<U> other;
    };
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
      return hash<int>()(int(h));
    }
  };

  template<>
  struct equal_to<dragon::Handle>
  {
    bool operator () (const dragon::Handle &l, const dragon::Handle &r) const
    {
      return int(l) == int(r);
    }
  };


  std::wostream & operator << (std::wostream &os, dragon::Handle h);
}
