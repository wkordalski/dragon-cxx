#pragma once

#include "../../node.hpp"

#include <llvm/IR/GlobalVariable.h>

namespace dragon
{
  struct Variable : public Node
  {
    Handle parent;
    Handle name;
    Handle type;
    Handle value;
    std::vector<Handle> attribs;
    bool internal = true;

    Variable(Handle name, Handle type = Handle(), Handle value = Handle(), std::vector<Handle> attribs = {}, bool internal = true)
      : parent(Handle()), name(name), type(type), value(value), attribs(attribs), internal(internal) {}
  };

  struct LLVMVariable : public Variable
  {
    llvm::GlobalVariable *native;

    LLVMVariable(Variable *base, llvm::GlobalVariable *native) : Variable(*base), native(native)
    {
    }
  };
}
