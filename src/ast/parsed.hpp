#pragma once

#include "../node.hpp"
#include "../utils/collect.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    File() {}
    File(std::vector<Handle> decls, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(declarations.size() + 1);
      r.push_back(docstring);
      for(auto h : declarations) r.push_back(h);
      return r;
    }
  };

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

  class VariableDecls : public Node
  {
  public:
    std::vector<Handle> attribs;
    std::vector<Handle> decls;
    Handle docstring;

    VariableDecls() {}
    VariableDecls(std::vector<Handle> attribs, std::vector<Handle> decls, Handle doc = Handle())
      : attribs(attribs), decls(decls), docstring(doc) {}

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(attribs.size() + decls.size() + 1);
      for(auto h : attribs) r.push_back(h);
      for(auto h : decls) r.push_back(h);
      r.push_back(docstring);
      return r;
    }
  };

  class VariableDecl : public Node
  {
  public:
    Handle id;
    Handle type;
    Handle value;

    VariableDecl() {}
    VariableDecl(Handle id, Handle type, Handle value) : id(id), type(type), value(value) {}

    virtual std::vector<Handle> get_members() const
    {
      return {id, type, value};
    }
  };
}
