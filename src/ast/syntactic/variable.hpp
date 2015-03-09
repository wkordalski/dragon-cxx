#pragma once

#include "../../node.hpp"

namespace dragon
{
  namespace syntax
  {
    class VariablesDeclaration : public Node
    {
    public:
      std::vector<Handle> attrs;
      std::vector<Handle> decls;
      Handle docstring;

      VariablesDeclaration(std::vector<Handle> attrs, std::vector<Handle> decls, Handle docstring)
        : attrs(attrs), decls(decls), docstring(docstring) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class SingleVariableDeclaration : public Node
    {
    public:
      Handle id;
      Handle type;
      Handle value;

      SingleVariableDeclaration(Handle id, Handle type, Handle value)
        : id(id), type(type), value(value) {}

      virtual void accept(Visitor &v) { v.visit(*this); }
    };
  }
}
