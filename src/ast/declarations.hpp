#pragma once

#include "../token.hpp"

#include "type.hpp"
#include "value.hpp"

#include <unordered_map>

#include <llvm/IR/GlobalVariable.h>

namespace dragon
{
  class IDeclaration
  {
  public:
    // Name of this declaration
    virtual Handle get_name() = 0;

    // Should this declaration be written to manifest?
    virtual bool is_internal() = 0;

    // Parent getters and setters
    virtual Handle get_parent() = 0;
    virtual void set_parent(Handle h) = 0;

    // Desymbolize expressions inside declaration
    virtual void desymbolize() = 0;

    // Translation to LLVM
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod) = 0;
  };

  class ILLVMDeclaration : public IDeclaration
  {
    // Translation to LLVM - DONE
    virtual void llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod) {}
  };

  class IDeclarationContainer
  {
  public:
    // Declaration by name
    virtual Handle by_name(Handle h) = 0;

    // Add declaration
    virtual void add_declaration(Handle h) = 0;
  };
}
