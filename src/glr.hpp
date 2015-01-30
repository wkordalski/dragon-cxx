#pragma once

namespace dragon
{
  ////////////////////////////////////////////////////////////////////////////
  // Efficient stack with constant duplication
  ////////////////////////////////////////////////////////////////////////////

  template<class T>
  class COWStack : std::enable_shared_from_this<COWStack<T>>
  {
    typedef std::shared_ptr<COWStack<T>> ptr;

  public:
    static ptr make();

  protected:
    COWStack() {}

  public:
    virtual ~COWStack() {}

    ptr push(T value);
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual ptr pop() = 0;
    virtual T & top() = 0;
  };

  template<class T>
  class COWStackRoot : COWStack<T>
  {
  protected:
    COWStackRoot() {}

    friend class COWStack<T>;

  public:
    virtual ~COWStackRoot() {}

    virtual bool empty() { return true;   }
    virtual int size()   { return 0;      }
    virtual ptr pop()    { assert(false); }
    virtual T & top()    { assert(false); }
  };

  template<class T>
  class COWStackElement : COWStack<T>
  {
    T _val;
    ptr _par;

  protected:
    COWStackElement(T value, ptr parent)
      : _val(value), _par(parent) {}

    friend class COWStack<T>;

  public:
    virtual ~COWStackElement() {}

    virtual bool empty() { return false;           }
    virtual int size()   { return par->size() + 1; }
    virtual ptr pop()    { return _par;            }
    virtual T & top()    { return _val;            }
  };

  template<class T>
  COWStack<T>::ptr COWStack<T>::make()
  {
    return ptr(new COWStackRoot());
  }

  template<class T>
  COWStack<T>::ptr COWStack<T>::push(T value)
  {
    return ptr(new COWStackElement(value, shared_from_this()));
  }

  ////////////////////////////////////////////////////////////////////////////
  // Parser type
  ////////////////////////////////////////////////////////////////////////////

  class Parser
  {
    typedef Handle semantic;
    typedef int location;
    typedef COWStack<yyGLRStackItem>::ptr GLRStack;
    typedef std::pair<GLRStack, bool> GLRSetElement;
    typedef std::vector<GLRSetElement> GLRSet;
    // TODO
  public:
    Parser (dragon::Scanner &scanner_yyarg);
    virtual ~Parser ();

    virtual int parse ();

  protected:
    void shift(GLRStack &stack, int state, size_t posn, semantic &val, location &loc);

  protected:
  };
}
