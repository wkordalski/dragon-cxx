#pragma once

#include "../../node.hpp"
#include "../declarations.hpp"

namespace dragon
{
  class Alias : public Node, public IDeclaration
  {
    Handle _parent;
  public:
    Handle _name;
    bool _internal = true;
    Handle _target;

    Alias(Handle name, Handle target) : _name(name), _target(target) {}

    virtual Handle get_name() { return _name; }
    virtual bool is_internal() { return _internal; }
    virtual Handle get_parent() { return _parent; }
    virtual void set_parent(Handle h) { _parent = h; }
    virtual void desymbolize()
    {
      _target.as<IDeclaration>()->desymbolize();
    }

    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod)
    {
      _target.as<IDeclaration>()->llvm_decl(ctx, mod);
    }

    virtual void print(std::wostream &os) const
    {
      os << "Alias ["<<handle()<<"] ( name = "<<int(_name)<<(_internal?", internal":"")<<", target = "<<int(_target)<<" )" << std::endl;
    }

    Handle get_target() { return _target; }
  };
}
