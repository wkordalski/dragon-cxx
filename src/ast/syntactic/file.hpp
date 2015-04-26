#pragma once

#include "../../node.hpp"
#include "../../containers.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Handle docstring = Handle();
    HVector declarations;
    Handle filename;

    File() {}
    File(Handle filename, HVector decls, Handle doc = Handle())
      : docstring(doc), declarations(decls), filename(filename) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
