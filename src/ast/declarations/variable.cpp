#include "variable.hpp"

namespace dragon
{
  void Variable::llvm_decl(llvm::LLVMContext &ctx, llvm::Module *mod)
  {
    replace(new LLVMVariable(this, ctx, mod));
  }
}
