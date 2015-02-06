#pragma once

#include "../token.hpp"

#include <unordered_map>

namespace dragon
{
  class IDeclaration
  {
  public:
    // Name of this declaration
    virtual Handle get_name() = 0;

    // Should this declaration be written to manifest?
    virtual bool is_internal() = 0;
  };

  class IDeclarationContainer
  {
  public:
    // Declaration by name
    virtual Handle by_name(Handle h) = 0;

    // Add declaration
    virtual void add_declaration(Handle h) = 0;
  };

  class Namespace : public Token, public IDeclaration, public IDeclarationContainer
  {
  public:
    Handle name;
    bool internal = true;
    std::unordered_map<Handle, Handle> declarations;

    Namespace(Handle name) : name(name) {}

    virtual Handle get_name() { return name; }
    virtual bool is_internal() { return internal; }
    virtual Handle by_name(Handle h) { if(declarations.count(h) > 0) return declarations[h]; else return Handle(); }
    virtual void add_declaration(Handle h)
    {
      auto decl = h.is<IDeclaration>();
      assert(declarations.count(decl->get_name()) == 0);
      declarations[decl->get_name()] = h;
    }

    virtual void print(std::wostream &os)
    {
      os << "Namespace ["<<handle()<<"] ( name = "<<int(name)<<(internal?", internal":"")<<", decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "])" << std::endl;
    }
  };

  class Variable : public Token, public IDeclaration
  {
  public:
    Handle name;
    bool internal = true;
    std::vector<Handle> attribs;
    Handle type;
    Handle value;

    Variable(Handle name) : name(name) {}

    virtual Handle get_name() { return name; }
    virtual bool is_internal() { return internal; }

    virtual void print(std::wostream &os)
    {
      os << "Variable ["<<handle()<<"] ( name = "<<int(name)<<(internal?", internal":"")<<", type = "<<int(type)<<", value = "<<int(value);
      if(!attribs.empty())
      {
        os << ", attribs = [ ";
        for(auto p : attribs) os << int(p) << " ";
        os << "]";
      }
      os << ")" << std::endl;
    }
  };
}
