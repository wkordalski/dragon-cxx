#pragma once

#include <cassert>

namespace dragon
{
  class Node;

  // Subnode types declarations goes here
  //...

  // Visitor base
  class Visitor
  {
  public:
    void visit(Node &n) { assert("Unimplemented visitor!" and false); }


    // Subnode types visits goes here
    //...
  };
}
