#pragma once

#include "../token.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace dragon
{
  class IValue
  {
  public:
    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod) = 0;
    virtual bool is_constant() { return false; }
  };

  class ICompileTimeValue : public IValue
  {
    virtual bool is_constant() { return true; }
  };

  class ILLVMValue : public IValue
  {
  public:
    virtual llvm::Value * get_llvm_value() = 0;
    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod) { }
  };
}
