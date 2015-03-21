#include "get_decl_by_name.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/namespace.hpp"

#include "comparer.hpp"
#include "get_decl_id.hpp"

namespace dragon
{
  void GetDeclarationByName::visit ( Assembly &n )
  {
		Comparer cmp;
		GetDeclarationId gdi;
    for(Handle h : n.declarations)
		{
			Handle ih = gdi.get(h);
			if(cmp.compare(ih, id))
			{
				if(ret) assert(false && "Ambiguity found here!");
				ret = h;
			}
		}
  }
  
  void GetDeclarationByName::visit ( sema::Namespace &n )
  {
    Comparer cmp;
		GetDeclarationId gdi;
    for(Handle h : n.declarations)
		{
			Handle ih = gdi.get(h);
			if(cmp.compare(ih, id))
			{
				if(ret) assert(false && "Ambiguity found here!");
				ret = h;
			}
		}
  }

}