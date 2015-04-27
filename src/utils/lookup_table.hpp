#pragma once

#include "../node.hpp"

namespace dragon
{
  class LookupTable : public Node
  {
  public:
    Member parent;
    MVector places;

    LookupTable() = default;
  };
}
