#include "../ast.hpp"

namespace dragon
{
  void File::fillin(Handle h)
  {
    //auto cont = h.is<IDeclarationContainer>();
    for(auto hd : declarations)
    {
      if(auto tt = hd.as<NamespaceDecl>()) tt->fillin_decls(h);
      if(auto tt = hd.as<VariableDecls>()) tt->fillin_decls(h);
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
      if(auto tt = hd.as<VariableDecls>()) tt->fillin_decls(hc);
    }
  }

  void VariableDecls::fillin_decls(Handle h)
  {
    for(auto hd : decls) hd.as<VariableDecl>()->fillin_decls(h, attribs);
  }

  void VariableDecl::fillin_decls(Handle h, std::vector<Handle> attribs)
  {
    auto cont = h.is<IDeclarationContainer>();
    auto hc = cont->by_name(id);
    if(hc) assert(false && "Redeclaration of variable!");
    hc = Handle::make<Variable>(id);
    auto node = hc.is<Variable>();
    node->attribs = attribs;
    node->type = type;
    node->value = value;
    cont->add_declaration(hc);
  }
}
