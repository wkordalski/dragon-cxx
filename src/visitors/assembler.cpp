#include "assembler.hpp"

#include "../node.hpp"
#include "../visitor.hpp"

#include "../ast/syntactic/file.hpp"
#include "../ast/syntactic/use.hpp"
#include "../ast/syntactic/variable.hpp"
#include "../ast/syntactic/namespace.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/namespace.hpp"
#include "../ast/semantic/variable.hpp"

#include "../queries/get_decl_by_name.hpp"
#include "../queries/decl_inserter.hpp"

#include "../utils/lookup_table.hpp"

namespace dragon
{
  Assembler::Assembler()
  {
    assembly = Handle::make<Assembly>();
  }
  
  void Assembler::assemble ( dragon::HVector files, dragon::Handle module )
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
  void Assembler::visit ( syntax::NamespaceDeclaration &n )
  {
		for(auto ih : n.name)
		{
			// Check if such namespace exists
			GetDeclarationByName gdbn;
			Handle h = gdbn.get(ih, containers.top());
			if(!h.valid())
			{
				h = Handle::make<sema::Namespace>(ih, containers.top());
				DeclarationToContainerInserter dtci;
				dtci.insert(h, containers.top());
			}
			lookups.push(Handle::make<LookupTable>());
			containers.push(h);
		}
		
		for(auto h : n.declarations) h->accept(*this);
		
		for(auto ih : n.name)
		{
			lookups.pop();
			containers.pop();
		}
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
		DeclarationToContainerInserter().insert(h, containers.top());
		// and add to module
		insert_module.as<Module>()->decls.insert(h);
  }
}
