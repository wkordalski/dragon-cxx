#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon
{
  /*
   * Copies module to specified assembly
   */
  class Loader : public Visitor
  {
    Root assembly;
  public:
    void load(Handle module, Handle assembly);
  };
}