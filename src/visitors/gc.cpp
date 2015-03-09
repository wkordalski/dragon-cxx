#include "gc.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/variable.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"

namespace dragon
{
  // Source tokens
  void GC::visit(Identifier &n) { mark(n); }
  void GC::visit(Operator &n) { mark(n); }
  void GC::visit(Literal &n) { mark(n); }
  void GC::visit(Newline &n) { mark(n); }
  void GC::visit(Indent &n) { mark(n); }
  void GC::visit(Dedent &n) { mark(n); }
  // Syntactic nodes
  void GC::visit(File &n)
  {
    if(mark(n))
    {
      accept(n.docstring);
      accept(n.declarations);
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
    }
  }
}
