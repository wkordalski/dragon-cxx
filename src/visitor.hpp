#pragma once

#include <cassert>

namespace dragon
{
  class Node;

  // Subnode types declarations goes here
  class Identifier;

  // Visitor base
  class Visitor
  {
  public:
    virtual void visit(Node &n) { assert("Unimplemented visitor!" and false); }

    // Subnode types visits goes here
    virtual void visit(Identifier &n) {}
  };
}
