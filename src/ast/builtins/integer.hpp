#pragma once

#include "../type.hpp"
#include "../value.hpp"

#include <llvm/ADT/APSInt.h>
#include <llvm/Support/raw_ostream.h>

namespace dragon
{
  class LLVMIntegralConstantValue : public Token
  {
    Handle _type;
    llvm::Constant *val;
  public:
    LLVMIntegralConstantValue(llvm::APSInt value, Handle type);
  };

  class IntegralType : public Token, public IType
  {
  protected:
    int _width = 32;
    bool _sign = false;
  public:
    IntegralType(int width = 32, bool sign = false) : _width(width), _sign(sign) {}

    int get_width() { return _width; }
    bool get_sign() { return _sign; }

    virtual void llvm_type(llvm::LLVMContext &ctx, llvm::Module *mod);
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

    friend class dragon::LLVMIntegralConstantValue;
  };



  class IntegralValue : public Token, public ICompileTimeValue
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
      os << "IntegralValue ["<<handle()<<"] ( type = "<<int(type)<<", value = "<<s.c_str()<< " )" << std::endl;
    }

    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod);
  };


}
