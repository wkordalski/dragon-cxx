#pragma once

#include "token.hpp"
#include "ast/decls.hpp"

namespace dragon
{
  class Assembly : public Token, public IDeclarationContainer
  {
  public:
    /* Name is identifier or template something */
    Handle member(Handle name);
  };

  class CompiledAssembly : public Token
  {
    // TODO: everything
  };

  Handle files_to_assembly(std::vector<Handle> files);
  Handle compile_assembly(Handle assembly);
}


#include "ast/parsed.hpp"
