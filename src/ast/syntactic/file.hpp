#pragma once

#include "../../node.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;
    Handle filename;

    File() {}
    File(Handle filename, std::vector<Handle> decls, Handle doc = Handle())
      : docstring(doc), declarations(decls), filename(filename) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
