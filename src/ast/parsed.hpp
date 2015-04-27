#pragma once

#include "../containers.hpp"
#include "../node.hpp"

namespace dragon
{
  class ImportDecls : public Node
  {
  public:
    MVector imports;

    ImportDecls() {}
    ImportDecls(LVector imports) : imports(to_member(imports)) {}
  };

  class ImportDecl : public Node
  {
  public:
    MVector module;

    ImportDecl() {}
    ImportDecl(LVector module) : module(to_member(module)) {}
  };
}
