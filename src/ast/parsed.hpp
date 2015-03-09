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

  class NamespaceDecl : public Node
  {
  public:
    std::vector<Handle> name;
    std::vector<Handle> declarations;

    NamespaceDecl() {}
    NamespaceDecl(std::vector<Handle> name, std::vector<Handle> decls = {}) : name(name), declarations(decls) {}

    virtual std::vector<Handle> get_members() const
    {
      return gccollect(name, declarations);
    }

    /*virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(name.size() + declarations.size());
      for(auto h : name) r.push_back(h);
      for(auto h : declarations) r.push_back(h);
      return r;
    }*/
  };
}
