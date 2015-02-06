#pragma once

#include "../token.hpp"
#include <llvm/IR/Value.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>

namespace dragon
{
  class LLVMValue
  {
  public:
    llvm::Value lval;
  };

  class IntegralValue : public Token
  {
  protected:
    Handle type;
  public:
    llvm::APInt value;

    IntegralValue(std::wstring s) {}

    void print(std::wostream &os) { os << "IntegralValue ["<<handle()<<"] <todo>" << std::endl; }
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
