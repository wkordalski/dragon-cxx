#pragma once

#include "node.hpp"

#include <vector>

//, NodeAllocator<Handle>

namespace dragon {
  typedef std::vector<Handle, NodeAllocator<Handle>> HVector;
}