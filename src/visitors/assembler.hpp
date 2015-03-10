#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon
{
  class Assembler : public Visitor
  {
    Handle assembly;
    std::unordered_map<Handle, std::vector<std::vector<Handle>>> lookup;

  public:
    Assembler() = default;

    void assemble(std::vector<Handle> files);

    Handle get_assembly() { return assembly; }
    auto get_lookup_table() { return lookup; }
    // returns a table that extends definitions with information about using-namespace derectives
    // needed by desymbolization algorithm
  };
}
