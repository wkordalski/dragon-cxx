#pragma once

#include "token.hpp"
#include "ast/decls.hpp"

namespace dragon
{
  class Assembly : public Token, public IDeclarationContainer
  {
    std::unordered_map<Handle, Handle> declarations;
  public:

    virtual Handle by_name(Handle h) { if(declarations.count(h) > 0) return declarations[h]; else return Handle(); }
    virtual void add_declaration(Handle h)
    {
      auto decl = h.is<IDeclaration>();
      assert(declarations.count(decl->get_name()) == 0);
      declarations[decl->get_name()] = h;
    }

    virtual void print(std::wostream &os)
    {
      os << "Assembly ["<<handle()<<"] ( decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "])" << std::endl;
    }
  };

  class CompiledAssembly : public Token
  {
    // TODO: everything
  };

  Handle files_to_assembly(std::vector<Handle> files);
  Handle compile_assembly(Handle assembly);
}


#include "ast/parsed.hpp"
