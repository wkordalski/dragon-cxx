#include "get_decl_id.hpp"
#include "../ast/semantic/variable.hpp"
#include "../ast/semantic/namespace.hpp"

namespace dragon
{
  void GetDeclarationId::visit ( sema::Namespace &n )
  {
    ret = n.id;
  }

  void GetDeclarationId::visit ( sema::Variable &n )
  {
    ret = n.id;
  }
}