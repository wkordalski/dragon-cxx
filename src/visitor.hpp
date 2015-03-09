#pragma once

#include <cassert>

namespace dragon
{
  class Node;

  // Subnode types declarations goes here

  //
  // Source tokens
  //
  class Identifier;
  class Operator;
  class Literal;
  class Newline;
  class Indent;
  class Dedent;

  //
  // Syntactic nodes
  //
  class File;
  namespace syntax
  {
    class VariablesDeclaration;
    class SingleVariableDeclaration;
  }

  //
  // Semantic nodes
  //
  class Assembly;
  class Module;

  // Visitor base
  class Visitor
  {
  public:
    virtual void visit(Node &n) { assert("Unimplemented visitor!" and false); }

    // Subnode types visits goes here

    // Source tokens
    virtual void visit(Identifier &n) {}
    virtual void visit(Operator &n) {}
    virtual void visit(Literal &n) {}
    virtual void visit(Newline &n) {}
    virtual void visit(Indent &n) {}
    virtual void visit(Dedent &n) {}
    // Syntactic nodes
    virtual void visit(File &n) {}
    virtual void visit(syntax::VariablesDeclaration &n) {}
    virtual void visit(syntax::SingleVariableDeclaration &n) {}
    // Semantic nodes
    virtual void visit(Assembly &n) {}
    virtual void visit(Module &n) {}
  };
}
