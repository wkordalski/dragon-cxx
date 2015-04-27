#pragma once

#include "../../containers.hpp"
#include "../../node.hpp"
#include "../../visitor.hpp"

namespace dragon
{
  namespace syntax
  {
    class VariablesDeclaration : public Node
    {
    public:
      MVector attrs;
      MVector decls;
      Member docstring;

      VariablesDeclaration() = default;

      VariablesDeclaration(LVector attrs, LVector decls, Local docstring)
        : attrs(to_member(attrs)), decls(to_member(decls)), docstring(docstring) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class SingleVariableDeclaration : public Node
    {
    public:
      Member id;
      Member type;
      Member value;

      SingleVariableDeclaration() = default;

      SingleVariableDeclaration(Local id, Local type, Local value)
        : id(id), type(type), value(value) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };
  }
}
