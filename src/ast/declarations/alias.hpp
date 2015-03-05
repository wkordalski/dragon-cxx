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

    virtual void print(std::wostream &os) const
    {
      os << "Alias ["<<handle()<<"] ( name = "<<int(name)<<(internal?", internal":"")<<", target = "<<int(target)<<" )" << std::endl;
    }
  };
}
