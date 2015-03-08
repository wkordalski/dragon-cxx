#pragma once

#include "../../node.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    File() {}
    File(std::vector<Handle> decls, Handle doc = Handle())
      : docstring(doc), declarations(decls) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
