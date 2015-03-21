#pragma once

#include "../node.hpp"
#include "../utils/collect.hpp"

namespace dragon
{
  class ImportDecls : public Node
  {
  public:
    std::vector<Handle> imports;

    ImportDecls() {}
    ImportDecls(std::vector<Handle> imports) : imports(imports) {}

    virtual std::vector<Handle> get_members() const
    {
      return imports;
    }
  };

  class ImportDecl : public Node
  {
  public:
    std::vector<Handle> module;

    ImportDecl() {}
    ImportDecl(std::vector<Handle> module) : module(module) {}

    virtual std::vector<Handle> get_members() const { return module; }
  };
}
