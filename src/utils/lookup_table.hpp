#pragma once

#include "../node.hpp"

namespace dragon
{
  class LookupTable : public Node
  {
  public:
    Handle parent;
    HVector places;

    LookupTable() = default;
  };
}
