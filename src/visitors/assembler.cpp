#include "assembler.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/use.hpp"
#include "../ast/syntactic/variable.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/variable.hpp"
#include "../ast/semantic/module.hpp"

#include "../utils/lookup_table.hpp"

namespace dragon
{
  Assembler::Assembler()
  {
    assembly = Handle::make<Assembly>();
  }
  
  void Assembler::assemble ( std::vector< Handle > files, Handle module )
  {
		insert_module = module;
		for(auto h : files) h->accept(*this);
  }
  
  Handle Assembler::new_module()
  {
		auto h = Handle::make<Module>();
		assembly.as<Assembly>()->modules.insert(h);
		return h;
  }
  
  void Assembler::visit(File &n)
  {
    // we have to do something
    // scan for using-namespaces
    // and push a lookup table
    lookups.push(Handle::make<LookupTable>());
    containers.push(assembly);
    for(auto h : n.declarations) h->accept(*this);
    lookups.pop();
    containers.pop();
  }

  void Assembler::visit(syntax::UseDeclaration &n)
  {
    for(auto h : n.decls) h->accept(*this);
  }
  void Assembler::visit(syntax::UsingNamespaceDeclaration &n)
  {
    lookups.top().as<LookupTable>()->places.push_back(n.handle());
  }

  void Assembler::visit(syntax::VariablesDeclaration &n)
  {
		attributes.push(n.attrs);
    for(auto h : n.decls) h->accept(*this);
		attributes.pop();
  }

  void Assembler::visit(syntax::SingleVariableDeclaration &n)
  {
    auto h = Handle::make<sema::Variable>();
		auto np = h.as<sema::Variable>();
    // Initialize it!
		np->id = n.id;
		np->type = n.type;
		np->value = n.value;
		np->attributes = attributes.top();
    // set lookup table to lookups.top()
		lookup[h] = lookups.top();
    // set its parent to containers.top()
		np->parent = containers.top();
    // and add it to the container
		if(auto tt = containers.top().is<Assembly>()) tt->declarations[n.id] = np->handle();
		//if(auto tt = containers.top().is<Namespace>()) tt->declarations[n.id] = np->handle();
		// and add to module
		insert_module.as<Module>()->decls.insert(h);
  }

  /*
  void Assembler::visit(syntax::NamespaceDeclaration &n)
  {
    // scan for using-namespaces
    // and push a lookup table
  }
  */

}
