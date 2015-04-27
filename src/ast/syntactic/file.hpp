#pragma once

#include "../../node.hpp"
#include "../../containers.hpp"

namespace dragon
{
  class File : public Node
  {
  public:
    Member docstring = Handle();
    MVector declarations;
    Member filename;

    File() {}
    File(Local filename, LVector decls, Local doc = Handle())
      : docstring(doc), declarations(to_member(decls)), filename(filename) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
