#pragma once

#include "token.hpp"

namespace dragon
{
  class Program : public Token
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    Program(Handle h) : declarations({h}) {}
    Program(std::vector<Handle> decls = {}, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
  };

  class NamespaceDecl : public Token
  {
  public:
    std::vector<Handle> name;
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    virtual void levelup() { /* TODO */ }
  };

  class VariableDecl : public Token
  {
  public:
    /* TODO */

    virtual void levelup() { /* TODO */ }
  };
}
