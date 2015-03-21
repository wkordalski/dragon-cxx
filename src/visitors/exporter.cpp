#include "exporter.hpp"

#include "../ast/source.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/use.hpp"
#include "../ast/syntactic/variable.hpp"
#include "../ast/syntactic/namespace.hpp"

#include "../ast/semantic/module.hpp"
#include "../ast/semantic/variable.hpp"
#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/namespace.hpp"

#include "../utils/lookup_table.hpp"

#include <iostream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

// NEXT FREE TID: 19

namespace dragon
{
  void Exporter::serialize(std::vector<Handle> v)
  {
    ar << v;
    save(v);
  }

  void Exporter::serialize(std::vector<Root> v)
  {
    std::vector<Handle> mitm;
    std::transform(v.begin(), v.end(), std::back_insert_iterator<std::vector<Handle>>(mitm), [](Root r){return Handle(r);});
    serialize(mitm);
  }

  // Source tokens
  void Exporter::visit(Identifier &n)
  { int tid = 1; ar << n.handle() << tid << n.text; }
  void Exporter::visit(Operator &n)
  { int tid = 2; ar << n.handle() << tid << n.text; }
  void Exporter::visit(Literal &n)
  { int tid = 3; ar << n.handle() << tid << n.text; }
  void Exporter::visit(Newline &n)
  { int tid = 4; ar << n.handle() << tid; }
  void Exporter::visit(Indent &n)
  { int tid = 5; ar << n.handle() << tid; }
  void Exporter::visit(Dedent &n)
  { int tid = 6; ar << n.handle() << tid; }
  // Syntactic nodes
  void Exporter::visit(File &n)
  {
    int tid = 7;
    ar << n.handle() << tid
       << n.docstring
       << n.declarations;

    save(n.docstring);
    save(n.declarations);
  }
  void Exporter::visit ( syntax::NamespaceDeclaration &n )
  {
    int tid = 17;
		ar << n.handle() << tid
			 << n.name
			 << n.declarations;
			 
		save(n.name);
		save(n.declarations);
  }
  void Exporter::visit(syntax::UseDeclaration &n)
  {
    int tid = 11;
    ar << n.handle() << tid
       << n.decls;

    save(n.decls);
  }
  void Exporter::visit(syntax::UsingNamespaceDeclaration &n)
  {
    int tid = 12;
    ar << n.handle() << tid
       << n.name;

    save(n.name);
  }
  void Exporter::visit(syntax::VariablesDeclaration &n)
  {
    int tid = 8;
    ar << n.handle() << tid
       << n.attrs
       << n.decls
       << n.docstring;

    save(n.attrs);
    save(n.decls);
    save(n.docstring);
  }
  void Exporter::visit(syntax::SingleVariableDeclaration &n)
  {
    int tid = 9;
    ar << n.handle() << tid
       << n.id
       << n.type
       << n.value;

    save(n.id);
    save(n.type);
    save(n.value);
  }
  // Semantic nodes
  void Exporter::visit(Assembly &n)
  {
    // REMEMBER! Exporting whole assembly/namespace shouldn't export all nodes!
    // Why?
    // We export a module.
    // So we export some definitions within the module.
    // Then we save some it's bodies, etc.
    // And parent containers (namespaces, and so on)
    // And their containers
    // And we also export Assembly
    // Then we shouldn't export all the other modules.
		int tid = 16;
		ar << n.handle() << tid;
  }
  void Exporter::visit(Module &n)
  {
    int tid = 10;
    std::vector<Handle> decls;
    std::transform(n.decls.begin(), n.decls.end(), std::back_insert_iterator<std::vector<Handle>>(decls), [](Handle h){return h;});
    ar << n.handle() << tid
       << n.name
       << decls;

    save(n.name);
    save(n.decls);
  }
  void Exporter::visit ( ModuleSpecification &n )
  {
    int tid = 15;
		ar << n.handle() << tid
			 << n.name;
		
		save(n.name);
  }
  void Exporter::visit ( sema::Namespace &n )
  {
    int tid = 18;
		ar << n.handle() << tid
			 << n.parent
			 << n.id;
			 
		save(n.id);
		save(n.parent);
  }
  void Exporter::visit(sema::Variable &n)
  {
    int tid = 14;
		ar << n.handle() << tid
			 << n.parent
			 << n.id
			 << n.type
			 << n.value
			 << n.attributes;
			 
		save(n.parent);
		save(n.id);
		save(n.type);
		save(n.value);
		save(n.attributes);
  }
  // Utilities nodes
  void Exporter::visit(LookupTable &n)
  {
    int tid = 13;
    ar << n.handle() << tid
       << n.parent
       << n.places;

    save(n.parent);
    save(n.places);
  }
}
