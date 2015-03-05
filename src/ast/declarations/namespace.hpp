#pragma once

#include "../../node.hpp"

namespace dragon
{
  struct Namespace : public Node
  {
    Handle parent;
    Handle name;
    bool internal = true;
    std::unordered_map<Handle, Handle> declarations;

    Namespace(Handle name) : name(name) {}
  };
}
