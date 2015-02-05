#pragma once

#include "../token.hpp"

#include <unordered_map>

namespace dragon
{
  class IDeclaration
  {
    // Name of this declaration
    virtual Handle get_name() = 0;

    // Should this declaration be written to manifest?
    virtual bool is_internal() = 0;
  };

  class IDeclarationContainer
  {
    // TODO
  };

  class Namespace : public Token, public IDeclaration, public IDeclarationContainer
  {
  public:
    Handle name;
    bool internal = true;
    std::unordered_map<Handle, Handle> declarations;

    Namespace(Handle name) : name(name) {}

    virtual Handle get_name() { return name; }
    virtual bool is_internal() { return internal; }
  };

  class Variable : public Token, public IDeclaration
  {
    // TODO
  };
}
