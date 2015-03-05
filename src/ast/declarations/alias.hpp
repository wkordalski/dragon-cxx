#pragma once

#include "../../node.hpp"

namespace dragon
{
  struct Alias : public Node
  {
    Handle parent;
    Handle name;
    bool internal = true;
    Handle target;

    Alias(Handle name, Handle target) : name(name), target(target) {}
  };
}
