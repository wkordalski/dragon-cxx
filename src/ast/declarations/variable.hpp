#pragma once

#include "../../node.hpp"
#include "../declarations.hpp"
#include "../expressions.hpp"
#include "../type.hpp"
#include "../value.hpp"

namespace dragon
{
  class Variable : public Node, public IDeclaration, public IValue
  {
  protected:
    Handle _parent;
    Handle _name;
    Handle _type;
    Handle _value;
    std::vector<Handle> _attribs;
    bool _internal = true;

  public:
    Variable(Handle name, Handle type = Handle(), Handle value = Handle(), std::vector<Handle> attribs = {}, bool internal = true)
      : _parent(Handle()), _name(name), _type(type), _value(value), _attribs(attribs), _internal(internal) {}

    virtual Handle get_name() { return _name; }
    virtual bool is_internal() { return _internal; }
    virtual Handle get_parent() { return _parent; }
    virtual void set_parent(Handle h) { _parent = h; }
    virtual void desymbolize()
    {
      if(_type) _type.as<IExpression>()->desymbolize_expression(_parent);
      if(_value) _value.as<IExpression>()->desymbolize_expression(_parent);
    }
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod);

    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod)
    {
      assert(false and "TODO");
    }

    Handle get_type() { return _type; }
    std::vector<Handle> get_attribs() { return _attribs; }
    Handle get_value() { return _value; }

    virtual void print(std::wostream &os) const
    {
      os << "Variable ["<<handle()<<"] ( name = "<<int(_name)<<(_internal?", internal":"")<<", type = "<<int(_type)<<", value = "<<int(_value);
      if(!_attribs.empty())
      {
        os << ", attribs = [ ";
        for(auto p : _attribs) os << int(p) << " ";
        os << "]";
      }
      os << " )" << std::endl;
    }
  };

  class LLVMVariable : public Variable
  {
    llvm::GlobalVariable *_llvm;
  public:

    LLVMVariable(Variable *base, llvm::LLVMContext &ctx, llvm::Module *mod) : Variable(*base)
    {
      assert(_type.is<IType>());
      _type.as<IType>()->llvm_type(ctx, mod);
      llvm::Type *ty = _type.as<ILLVMType>()->get_llvm_type();
      bool isconst = _type.as<ILLVMType>()->is_const();
      llvm::Constant *cv = nullptr;
      if(_value)
      {
        assert(_value.is<IValue>());
        _value.as<IValue>()->llvm_value(ctx, mod);
        assert(_value.as<ILLVMValue>()->is_constant());
        cv = dynamic_cast<llvm::Constant*>(_value.as<ILLVMValue>()->get_llvm_value());
      }
      _llvm = new llvm::GlobalVariable(ty, isconst, llvm::GlobalValue::LinkageTypes::ExternalLinkage /* TODO */, cv, "" /* TODO: name generation */,
                                       llvm::GlobalValue::ThreadLocalMode::NotThreadLocal, 0 /* address space */, false /* TODO: what is this? */);
    }

    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod) {}
    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod) { assert(false and "TODO");}
  };
}
