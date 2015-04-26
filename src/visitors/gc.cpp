#include "gc.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/use.hpp"
#include "../ast/syntactic/variable.hpp"
#include "../ast/syntactic/namespace.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/variable.hpp"
#include "../ast/semantic/namespace.hpp"

#include "../utils/lookup_table.hpp"


namespace dragon
{
  // Source tokens
  void GC::visit(Identifier &n)
  {
    mark(n);
  }
  void GC::visit(Operator &n)
  {
    mark(n);
  }
  void GC::visit(Literal &n)
  {
    mark(n);
  }
  void GC::visit(Newline &n)
  {
    mark(n);
  }
  void GC::visit(Indent &n)
  {
    mark(n);
  }
  void GC::visit(Dedent &n)
  {
    mark(n);
  }
  // Syntactic nodes
  void GC::visit(File &n)
  {
    if(mark(n))
    {
      accept(n.docstring);
      accept(n.declarations);
      accept(n.filename);
    }
  }
  void GC::visit ( syntax::NamespaceDeclaration &n )
  {
    if(mark(n))
		{
			accept(n.name);
			accept(n.declarations);
		}
  }

  void GC::visit(syntax::UseDeclaration &n)
  {
    if(mark(n))
    {
      accept(n.decls);
    }
  }
  void GC::visit(syntax::UsingNamespaceDeclaration &n)
  {
    if(mark(n))
    {
      accept(n.name);
    }
  }
  void GC::visit(syntax::VariablesDeclaration &n)
  {
    if(mark(n))
    {
      accept(n.attrs);
      accept(n.decls);
      accept(n.docstring);
    }
  }
  void GC::visit(syntax::SingleVariableDeclaration &n)
  {
    if(mark(n))
    {
      accept(n.id);
      accept(n.type);
      accept(n.value);
    }
  }
  // Semantic nodes
  void GC::visit(Assembly &n)
  {
    if(mark(n))
    {
      accept(n.modules);
      accept(n.declarations);
    }
  }
  void GC::visit(Module &n)
  {
    if(mark(n))
    {
      accept(n.name);
      accept(n.assembly);
			accept(n.deps);
			accept(n.decls);
    }
  }
  void GC::visit ( ModuleName &n )
  {
    if(mark(n))
		{
			accept(n.name);
		}
  }
  void GC::visit ( sema::Namespace &n )
  {
    if(mark(n))
		{
			accept(n.id);
			accept(n.parent);
			accept(n.declarations);
		}
  }
  void GC::visit(sema::Variable &n)
  {
    if(mark(n))
		{
			accept(n.id);
			accept(n.type);
			accept(n.value);
			accept(n.attributes);
			accept(n.parent);
		}
  }
  // Utilities nodes
  void GC::visit(LookupTable &n)
  {
    if(mark(n))
    {
      accept(n.parent);
      accept(n.places);
    }
  }
}
