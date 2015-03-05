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

    virtual void print(std::wostream &os) const
    {
      os << "Namespace ["<<handle()<<"] ( name = "<<int(name)<<(internal?", internal":"")<<", decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "] )" << std::endl;
    }
  };
}
