#pragma once

#include "../token.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

namespace dragon
{
  class IType
  {
  public:
    virtual void llvm_type(llvm::LLVMContext &ctx, llvm::Module *mod) = 0;
    virtual bool is_const() { return false; }
  };

  class ILLVMType : public IType
  {
  public:
    virtual llvm::Type * get_llvm_type() = 0;
    virtual void llvm_type(llvm::LLVMContext &ctx, llvm::Module *mod) {}
  };
}
