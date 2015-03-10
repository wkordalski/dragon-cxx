#include "importer.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/variable.hpp"

#include "../ast/semantic/module.hpp"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>


namespace dragon
{
  template<>
  void Importer::read<Identifier>(Handle &h)
  {
    auto np = new Identifier();
    auto &n = *np;
    ar >> n.text;
    h.set(np);
  }

  template<>
  void Importer::read<File>(Handle &h)
  {
    auto np = new File();
    auto &n = *np;
    Handle docstring;
    std::vector<Handle> declarations;
    ar >>  docstring >> declarations;
    n.docstring = translate(docstring);
    std::transform(declarations.begin(), declarations.end(),
                   std::back_insert_iterator<std::vector<Handle>>(n.declarations),
                   [this](Handle h){return translate(h);});
    h.set(np);
  }

  template<>
  void Importer::read<syntax::VariablesDeclaration>(Handle &h)
  {
    auto np = new syntax::VariablesDeclaration();
    auto &n = *np;
    Handle docstring;
    std::vector<Handle> decls;
    std::vector<Handle> attrs;
    ar >> attrs >> decls >> docstring;
    n.docstring = translate(docstring);
    std::transform(decls.begin(), decls.end(),
                   std::back_insert_iterator<std::vector<Handle>>(n.decls),
                   [this](Handle h){return translate(h);});
   std::transform(attrs.begin(), attrs.end(),
                  std::back_insert_iterator<std::vector<Handle>>(n.attrs),
                  [this](Handle h){return translate(h);});
    h.set(np);
  }

  template<>
  void Importer::read<syntax::SingleVariableDeclaration>(Handle &h)
  {
    auto np = new syntax::SingleVariableDeclaration();
    Handle id, type, value;
    auto &n = *np;
    ar >> id >> type >> value;
    n.id = translate(id);
    n.type = translate(type);
    n.value = translate(value);
    h.set(np);
  }

  Importer::decode_func Importer::decoder[] =
  {
    nullptr,
    &Importer::read<Identifier>,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Importer::read<File>,
    &Importer::read<syntax::VariablesDeclaration>,
    &Importer::read<syntax::SingleVariableDeclaration>,
    nullptr,
    nullptr,
    nullptr,
    nullptr
  };


  std::vector<Handle> Importer::deserialize()
  {
    std::vector<Handle> lod;
    ar >> lod;
    std::vector<Handle> ret;
    ret.reserve(lod.size());
    for(Handle h : lod)
    {
      ret.push_back(translate(h));
      required.insert(int(h));
    }
    // Load nodes and translate its handles
    while(required.size() > 0)
    {
      Handle nh;
      int tid;
      ar >> nh >> tid;
      Handle h = translate(nh);
      // we have right handle
      // now call to fill it correctly
      assert(tid*sizeof(decode_func) < sizeof(decoder));
      auto f = decoder[tid];
      assert(f && "Should be a proper function - not nullptr");
      if(f) (this->*f)(h);
      required.erase(int(nh));
    }
    return ret;
  }
}
