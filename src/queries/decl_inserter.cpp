#include "decl_inserter.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/namespace.hpp"

#include "get_decl_by_name.hpp"
#include "get_decl_id.hpp"

namespace dragon {
	
  void DeclarationToContainerInserter::visit ( Assembly &n )
  {
		assert(!GetDeclarationByName().get(GetDeclarationId().get(decl), n.handle()).valid()
						&& "Declaration suchly named already exists.");
    n.declarations.insert(decl);
		decl = HeapRoot();
  }
	
  void DeclarationToContainerInserter::visit ( sema::Namespace &n )
  {
		assert(!GetDeclarationByName().get(GetDeclarationId().get(decl), n.handle()).valid()
						&& "Declaration suchly named already exists.");
    n.declarations.insert(decl);
		decl = HeapRoot();
  }
}