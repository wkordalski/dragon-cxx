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
		
		void print(Handle h) { h->accept(*this); }

		// Source tokens
    virtual void visit(Identifier &n);
    //virtual void visit(Operator &n);
    //virtual void visit(Literal &n);
    //virtual void visit(Newline &n);
    //virtual void visit(Indent &n);
    //virtual void visit(Dedent &n);
    // Syntactic nodes
    //virtual void visit(File &n);
		//virtual void visit(syntax::NamespaceDeclaration &n);
    //virtual void visit(syntax::UseDeclaration &n);
    //virtual void visit(syntax::UsingNamespaceDeclaration &n);
    //virtual void visit(syntax::VariablesDeclaration &n);
    //virtual void visit(syntax::SingleVariableDeclaration &n);
    // Semantic nodes
    virtual void visit(Assembly &n);
    virtual void visit(Module &n);
    virtual void visit(ModuleName &n);
		virtual void visit(sema::Namespace &n);
    virtual void visit(sema::Variable &n);
    // Utilities nodes
    virtual void visit(LookupTable &n);
  };
}
