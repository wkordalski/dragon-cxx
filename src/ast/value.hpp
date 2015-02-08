#pragma once

#include "../token.hpp"
#include "../lexer/lexer.hpp"
#include <llvm/IR/Value.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace dragon
{
  class ICompileTimeValue
  {
    // TODO
    virtual void to_llvm() = 0;
  };

  class IntegralValue : public Token, public ICompileTimeValue
  {
  protected:
    Handle type;
  public:
    llvm::APSInt value;

    IntegralValue(std::wstring s);
    IntegralValue(std::wstring s, int width, bool sign = false);

    void print(std::wostream &os)
    {
      std::string s;
      {
        llvm::raw_string_ostream oos(s);
        oos << value;
      }
      os << "IntegralValue ["<<handle()<<"] ( type = "<<int(type)<<", value = "<<s.c_str()<< ")" << std::endl;
    }

    virtual void to_llvm()
    {
      //auto llv = new LLVMIntegralConstantValue(value, type.to_llvm());
      //replace(llv);
      assert(false && "TODO");
    }
  };

  class IntegralSignedValue
  {
  public:
    Handle type;
    llvm::APSInt value;
  };

  class LLVMIntegralConstantValue
  {
  public:
  };
}
