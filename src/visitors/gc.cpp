#include "gc.hpp"

#include "../ast/source.hpp"

namespace dragon
{
  void GC::visit(Identifier &n) { mark(n); }
}
