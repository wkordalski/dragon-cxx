#pragma once

#include "../node.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    File() {}
    File(std::vector<Handle> decls, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
    virtual void print(std::wostream &os) const
    {
      os << "File [" << handle() << "] ( doc = " << int(docstring) << ", decls = [ ";
      for(auto h : declarations) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(declarations.size() + 1);
      r.push_back(docstring);
      for(auto h : declarations) r.push_back(h);
      return r;
    }

    void fillin(Handle h);
  };

  class ImportDecls : public Node
  {
  public:
    std::vector<Handle> imports;

    ImportDecls() {}
    ImportDecls(std::vector<Handle> imports) : imports(imports) {}

    virtual void print(std::wostream &os) const
    {
      os << "ImportDecls ["<<handle()<<"] ( imports = [ ";
      for(auto h : imports) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void add_imports(Handle h);

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

    virtual void print(std::wostream &os) const
    {
      os << "ImportDecl ["<<handle()<<"] ( name = [ ";
      for(auto h : module) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void add_import(Handle h);

    virtual std::vector<Handle> get_members() const { return module; }
  };

  class NamespaceDecl : public Node
  {
  public:
    std::vector<Handle> name;
    std::vector<Handle> declarations;

    NamespaceDecl() {}
    NamespaceDecl(std::vector<Handle> name, std::vector<Handle> decls = {}) : name(name), declarations(decls) {}

    virtual void print(std::wostream &os) const
    {
      os << "NamespaceDecl [" << handle() << "] ( name = [ ";
      for(auto h : name) os << int(h) << " ";
      os << "], decls = [ ";
      for(auto h : declarations) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(name.size() + declarations.size());
      for(auto h : name) r.push_back(h);
      for(auto h : declarations) r.push_back(h);
      return r;
    }

    void fillin_decls(Handle h);
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

    virtual void print(std::wostream &os) const
    {
      os << "VariableDecls ["<<handle()<<"] ( attribs = [ ";
      for(auto h : attribs) os << int(h) << " ";
      os << "], decls = [ ";
      for(auto h : decls) os << int(h) << " ";
      os << "])" << std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(attribs.size() + decls.size() + 1);
      for(auto h : attribs) r.push_back(h);
      for(auto h : decls) r.push_back(h);
      r.push_back(docstring);
      return r;
    }

    void fillin_decls(Handle h);
  };

  class VariableDecl : public Node
  {
  public:
    Handle id;
    Handle type;
    Handle value;

    VariableDecl() {}
    VariableDecl(Handle id, Handle type, Handle value) : id(id), type(type), value(value) {}

    virtual void print(std::wostream &os) const
    {
      os << "VariableDecl ["<<handle()<<"] ( id = " << int(id) << ", type = "<< int(type) << ", value = "<<int(value)<<")" <<std::endl;
    }

    virtual std::vector<Handle> get_members() const
    {
      return {id, type, value};
    }

    void fillin_decls(Handle h, std::vector<Handle> attribs);
  };
}
