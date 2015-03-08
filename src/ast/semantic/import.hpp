#pragma once

#include "../../node.hpp"

namespace dragon
{
  class Import : public Node
  {
  public:
    std::vector<Handle> name;

    Import(std::vector<Handle> name) : name(name) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
    virtual std::size_t hash() const { return hash_sequence<std::hash<Handle>>(name); }
    virtual bool equal(const Node *t) const
    {
      if(auto tt = dynamic_cast<const Import*>(t))
        return std::equal(name.begin(), name.end(), tt->name.begin(), tt->name.end(), [](Handle a, Handle b) { return a % b; });
      else return false;
    }
  };
}
