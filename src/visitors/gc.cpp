#include "gc.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/import.hpp"

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
  // Semantic nodes
  void GC::visit(Assembly &n)
  {
    if(mark(n))
    {
      accept(n.imports);
      accept(n.declarations);
    }
  }
  void GC::visit(Import &n)
  {
    if(mark(n))
    {
      accept(n.name);
    }
  }
}
