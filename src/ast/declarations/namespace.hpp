#pragma once

#include "../../token.hpp"
#include "../declarations.hpp"

namespace dragon
{
  class Namespace : public Token, public IDeclaration, public IDeclarationContainer, public ISymbolTable
  {
    Handle _parent;
  public:
    Handle name;
    bool internal = true;
    std::unordered_map<Handle, Handle> declarations;

    Namespace(Handle name) : name(name) {}

    virtual Handle get_name() { return name; }
    virtual bool is_internal() { return internal; }
    virtual Handle get_parent() { return _parent; }
    virtual void set_parent(Handle h) { _parent = h; }
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod)
    {
      for(auto p : declarations)
        p.second.as<IDeclaration>()->llvm_decl(ctx, mod);
    }

    virtual Handle by_name(Handle h) { if(declarations.count(h) > 0) return declarations[h]; else return Handle(); }
    virtual void add_declaration(Handle h)
    {
      auto decl = h.is<IDeclaration>();
      assert(declarations.count(decl->get_name()) == 0);
      declarations[decl->get_name()] = h;
      decl->set_parent(shared_from_this());
    }

    virtual Handle get_parent_table() { return _parent; }
    virtual bool lookup_this_only(Handle identifier, Handle &result)
    {
      auto it = declarations.find(identifier);
      bool ret = it != declarations.end();
      if(ret) result = it->second;
      return ret;
    }

    virtual void print(std::wostream &os) const
    {
      os << "Namespace ["<<handle()<<"] ( name = "<<int(name)<<(internal?", internal":"")<<", decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "] )" << std::endl;
    }
  };
}
