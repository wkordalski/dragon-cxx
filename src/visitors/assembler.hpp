#pragma once

#include "../containers.hpp"
#include "../node.hpp"
#include "../visitor.hpp"

#include <stack>
#include <unordered_map>

namespace dragon
{
  class Assembler : public Visitor
  {
    Object assembly;
		Object insert_module;
    std::unordered_map<Handle, Handle> lookup;
    std::stack<Handle> lookups;
    std::stack<Handle> containers;
		std::stack<MVector> attributes;
    // TODO: Attributes

  public:
    Assembler();

		Local new_module();
    void assemble( dragon::LVector files, dragon::Local module );

    Local get_assembly() { return assembly; }
    auto get_lookup_table() { return lookup; }
    // returns a table that extends declarations with information about using-namespace derectives
    // needed by desymbolization algorithm

  public:
    // Syntactic nodes
    virtual void visit(File &n);
		virtual void visit(syntax::NamespaceDeclaration &n);
    virtual void visit(syntax::UseDeclaration &n);
    virtual void visit(syntax::UsingNamespaceDeclaration &n);
    virtual void visit(syntax::VariablesDeclaration &n);
    virtual void visit(syntax::SingleVariableDeclaration &n);
  };
}
