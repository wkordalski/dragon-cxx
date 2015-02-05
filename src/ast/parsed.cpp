#include "../ast.hpp"

namespace dragon
{
  void File::fillin(Handle h)
  {
    //auto cont = h.is<IDeclarationContainer>();
    for(auto hd : declarations)
    {
      if(auto tt = hd.as<NamespaceDecl>()) tt->fillin_decls(h);
    }
  }

  void NamespaceDecl::fillin_decls(Handle h)
  {
    auto cont = h.is<IDeclarationContainer>();
    auto hc = h;
    for(auto hn : name)
    {
      hc = cont->by_name(hn);
      if(!hc.valid())
      {
        hc = Handle::make<Namespace>(hn);
        cont->add_declaration(hc);
      }
      cont = hc.is<IDeclarationContainer>();
    }
    for(auto hd : declarations)
    {
      if(auto tt = hd.as<NamespaceDecl>()) tt->fillin_decls(hc);
    }
  }
}
