#pragma once

#include "../visitor.hpp"

#include <iostream>

#include "../ast/source.hpp"

namespace dragon
{
  class NodePrinter : public Visitor
  {
  public:
    std::wostream &out = std::wcout;

    virtual void visit(Identifier &n)
    {
      out << "Identifier ["<<n.handle()<<"] ( text = \""<<n.text<<"\" )" << std::endl;
    }
  };
}
