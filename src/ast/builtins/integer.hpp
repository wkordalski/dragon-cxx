#pragma once

#include "../declarations.hpp"
#include "../template.hpp"
#include "../type.hpp"
#include "../value.hpp"

#include <llvm/ADT/APSInt.h>
#include <llvm/Support/raw_ostream.h>

namespace dragon
{
  class LLVMIntegralValue;

  class IntegralType : public Node, public IType, public IDeclaration
  {
  protected:
    Handle _parent;
    int _width = 32;
    bool _sign = false;
  public:
    IntegralType(int width = 32, bool sign = false) : _width(width), _sign(sign) {}

    virtual Handle get_name() { return Handle() /* TODO */; }
    virtual bool is_internal() { return false; }
    virtual Handle get_parent() { return _parent; }
    virtual void set_parent(Handle h) { _parent = h; }
    virtual void desymbolize() { /* TODO */ }
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod) { llvm_type(ctx, mod); }

    int get_width() { return _width; }
    bool get_sign() { return _sign; }

    virtual void llvm_type(llvm::LLVMContext &ctx, llvm::Module *mod);

    virtual void print(std::wostream &os) const
    {
      os << "IntegralType ["<<handle()<<"] ( width = \""<<_width<<"\""<<(_sign?", signed" : ", unsigned")<<" )" << std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      return { _parent };
    }
  };

  class LLVMIntegralType : public IntegralType, public ILLVMType
  {
    llvm::IntegerType *_type;
  public:
    LLVMIntegralType(IntegralType *base, llvm::LLVMContext &ctx, llvm::Module *mod)
    {
      _width = base->get_width();
      _sign = base->get_sign();
      _type = llvm::IntegerType::get(ctx, _width);
    }

    virtual llvm::Type * get_llvm_type() { return _type; }

    friend class dragon::LLVMIntegralValue;
  };

  class IntegralValue : public Node, public ICompileTimeValue
  {
  protected:
    Handle type;
  public:
    llvm::APSInt value;

    IntegralValue(std::wstring s);
    IntegralValue(std::wstring s, int width, bool sign = false);

    void print(std::wostream &os) const
    {
      std::string s;
      {
        llvm::raw_string_ostream oos(s);
        oos << value;
      }
      os << "IntegralValue ["<<handle()<<"] ( type = "<<int(type)<<", value = \""<<s.c_str()<< "\", width=\""<<value.getBitWidth()<<"\" )" << std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      return { type };
    }

    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod);
  };

  class LLVMIntegralValue : public Node
  {
    Handle _type;
    llvm::Constant *val;
  public:
    LLVMIntegralValue(llvm::APSInt value, Handle type);


    virtual std::vector<Handle> get_members() const
    {
      return { _type };
    }
  };

  class IntegralTemplate : public Node, public ITemplate, public IDeclaration
  {
    Handle _default;
    Handle _parent;
    Handle _id;
  public:
    IntegralTemplate(Handle id, bool sign = false) : _id(id)
    {
      _default = Handle::make<IntegralType>(32, sign);
    }

    virtual Handle get_name() { return _id; }
    virtual bool is_internal() { return false; }
    virtual Handle get_parent() { return _parent; }
    virtual void set_parent(Handle h) { _parent = h; }
    virtual void desymbolize() { /* TODO */ }
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod) {}

    virtual std::vector<Handle> get_members() const
    {
      return { _default, _parent, _id };
    }

    Handle get_default() { return _default; }
  };
}
