#pragma once

#include "../token.hpp"

namespace dragon
{
  class File : public Token
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

    void fillin(Handle h);
  };

  class ImportDecls : public Token
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
  };

  class ImportDecl : public Token
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
  };

  class NamespaceDecl : public Token
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

    void fillin_decls(Handle h);
  };

  class VariableDecls : public Token
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

    void fillin_decls(Handle h);
  };

  class VariableDecl : public Token
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

    void fillin_decls(Handle h, std::vector<Handle> attribs);
  };
}
