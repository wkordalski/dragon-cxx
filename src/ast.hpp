#pragma once

#include "token.hpp"

namespace dragon
{
  class Program : public Token
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    Program() {}
    Program(std::vector<Handle> decls, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
  };

  class NamespaceDecl : public Token
  {
  public:
    std::vector<Handle *> name;
    std::vector<Handle *> declarations;

    virtual void levelup() { /* TODO */ }
  };
}
