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
      HVector attrs;
      HVector decls;
      Handle docstring;

      VariablesDeclaration() = default;

      VariablesDeclaration(HVector attrs, HVector decls, Handle docstring)
        : attrs(attrs), decls(decls), docstring(docstring) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class SingleVariableDeclaration : public Node
    {
    public:
      Handle id;
      Handle type;
      Handle value;

      SingleVariableDeclaration() = default;

      SingleVariableDeclaration(Handle id, Handle type, Handle value)
        : id(id), type(type), value(value) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };
  }
}
