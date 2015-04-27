#include "importer.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/use.hpp"
#include "../ast/syntactic/variable.hpp"
#include "../ast/syntactic/namespace.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/namespace.hpp"
#include "../ast/semantic/variable.hpp"

#include "../queries/decl_inserter.hpp"

#include "../utils/lookup_table.hpp"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>


namespace dragon
{
  template<>
  void Importer::read<Identifier>(Local &h)
  {
    auto np = new Identifier();
    auto &n = *np;
    ar >> n.text;
    h.set(np);
  }
  
  template<>
  void Importer::read<Literal>(Local &h)
  {
    auto np = new Literal();
    auto &n = *np;
    ar >> n.text;
    h.set(np);
  }

  template<>
  void Importer::read<File>(Local &h)
  {
    auto np = new File();
    auto &n = *np;
    Handle docstring;
    Handle filename;
    MVector declarations;
    ar >>  docstring >> declarations >> filename;
    n.docstring = translate(docstring);
    n.declarations = translate(declarations);
    n.filename = translate(filename);
    h.set(np);
  }

  template<>
  void Importer::read<syntax::NamespaceDeclaration>(Local &h)
	{
		auto np = new syntax::NamespaceDeclaration();
		auto &n = *np;
		MVector name, decls;
		ar >> name >> decls;
		n.name = translate(name);
		n.declarations = translate(decls);
		h.set(np);
	}
  
  template<>
  void Importer::read<syntax::UseDeclaration>(Local &h)
  {
    auto np = new syntax::UseDeclaration();
    auto &n = *np;
    MVector decls;
    ar >> decls;
    std::transform(decls.begin(), decls.end(),
                   std::back_inserter(n.decls),
                   [this](Handle h){return translate(h);});
    h.set(np);
  }

  template<>
  void Importer::read<syntax::UsingNamespaceDeclaration>(Local &h)
  {
    auto np = new syntax::UsingNamespaceDeclaration();
    auto &n = *np;
    MVector name;
    ar >> name;
    std::transform(name.begin(), name.end(),
                   std::back_inserter(n.name),
                   [this](Handle h){return translate(h);});
    h.set(np);
  }

  template<>
  void Importer::read<syntax::VariablesDeclaration>(Local &h)
  {
    auto np = new syntax::VariablesDeclaration();
    auto &n = *np;
    Handle docstring;
    MVector decls;
    MVector attrs;
    ar >> attrs >> decls >> docstring;
    n.docstring = translate(docstring);
    std::transform(decls.begin(), decls.end(),
                   std::back_inserter(n.decls),
                   [this](Handle h){return translate(h);});
    std::transform(attrs.begin(), attrs.end(),
                  std::back_inserter(n.attrs),
                  [this](Handle h){return translate(h);});
    h.set(np);
  }

  template<>
  void Importer::read<syntax::SingleVariableDeclaration>(Local &h)
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
  
  template<>
  void Importer::read<Assembly>(Local &h)
	{
		auto np = new Assembly();
		h.set(np);
	}
  
  template<>
  void Importer::read<Module>(Local &h)
	{
		auto np = new Module();
		auto &n = *np;
    Handle name;
    Handle assm;
		MVector decls;
    MVector deps;
		ar >> name >> assm >> deps >> decls;
		n.name = translate(name);
    n.assembly = translate(assm);
		std::transform(decls.begin(), decls.end(),
                  std::inserter(n.decls, n.decls.begin()),
                  [this](Handle h){return translate(h);});
    std::transform(deps.begin(), deps.end(),
                  std::inserter(n.deps, n.deps.begin()),
                  [this](Handle h){return translate(h);});
		h.set(np);
	}
  
  template<>
  void Importer::read<ModuleName>(Local &h)
	{
		auto np = new ModuleName();
		auto &n = *np;
		MVector name;
		ar >> name;
		n.name = translate(name);
		h.set(np);
	}
  
  template<>
  void Importer::read<sema::Namespace>(Local &h)
	{
		auto np = new sema::Namespace();
		auto &n = *np;
		Handle id, parent;
		ar >> parent >> id;
		n.parent = translate(parent);
		n.id = translate(id);
		h.set(np);
		defer(parent, [np]()
		{
			DeclarationToContainerInserter().insert(np->handle(), np->parent);
		});
	}
  
  template<>
  void Importer::read<sema::Variable>(Local &h)
	{
		auto np = new sema::Variable();
		auto &n = *np;
		Handle id, type, value, parent;
		MVector attrs;
		ar >> parent >> id >> type >> value >> attrs;
		n.parent = translate(parent);
		n.id = translate(id);
		n.type = translate(type);
		n.value = translate(value);
		std::transform(attrs.begin(), attrs.end(),
                  std::back_inserter(n.attributes),
                  [this](Handle h){return translate(h);});
		h.set(np);
		// set parent!
		defer(parent, [np]()
		{
			DeclarationToContainerInserter().insert(np->handle(), np->parent);
		});
	}

  template<>
  void Importer::read<LookupTable>(Local &h)
  {
    auto np = new LookupTable();
    auto &n = *np;
    Handle parent;
    MVector places;
    ar >> parent >> places;
    n.parent = translate(parent);
    std::transform(places.begin(), places.end(),
                  std::back_inserter(n.places),
                  [this](Handle h){return translate(h);});
    h.set(np);
  }

  Importer::decode_func Importer::decoder[] =
  {
    nullptr,																							// 0
    &Importer::read<Identifier>,
    nullptr,
    &Importer::read<Literal>,
    nullptr,
    nullptr,																							// 5
    nullptr,
    &Importer::read<File>,
    &Importer::read<syntax::VariablesDeclaration>,
    &Importer::read<syntax::SingleVariableDeclaration>,
    &Importer::read<Module>,															// 10
    &Importer::read<syntax::UseDeclaration>,
    &Importer::read<syntax::UsingNamespaceDeclaration>,
    &Importer::read<LookupTable>,
    &Importer::read<sema::Variable>,
    &Importer::read<ModuleName>,									// 15
    &Importer::read<Assembly>,
		&Importer::read<syntax::NamespaceDeclaration>,
		&Importer::read<sema::Namespace>
  };


  MVector Importer::deserialize()
  {
    MVector lod;
    ar >> lod;
    MVector ret;
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
			if(deferred.count(int(nh)))
			{
				for(auto g : deferred[int(nh)]) g();
				deferred.erase(int(nh));
			}
    }
    return ret;
  }
}
