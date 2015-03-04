#pragma once

#include "../node.hpp"
#include "expressions.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace dragon
{
  class IValue : public IExpression
  {
  public:
    virtual void llvm_value(llvm::LLVMContext &ctx, llvm::Module *mod) = 0;
    virtual bool is_constant() { return false; }

    virtual void desymbolize_expression(Handle symbol_table) {}
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
