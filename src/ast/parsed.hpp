#pragma once

#include "../containers.hpp"
#include "../node.hpp"

namespace dragon
{
  class ImportDecls : public Node
  {
  public:
    HVector imports;

    ImportDecls() {}
    ImportDecls(HVector imports) : imports(imports) {}
  };

  class ImportDecl : public Node
  {
  public:
    HVector module;

    ImportDecl() {}
    ImportDecl(HVector module) : module(module) {}
  };
}
