
#include "loader.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/variable.hpp"
#include "../ast/semantic/namespace.hpp"
#include "../ast/source.hpp"

#include "../queries/comparer.hpp"
#include "../queries/get_decl_by_name.hpp"

namespace dragon {
  void Loader::load ( Handle module, Handle assembly )
  {
    Comparer cmp;
    Handle modname = module.as<Module>()->name;
    for(auto h : assembly.as<Assembly>()->modules)
    {
      if(cmp.compare(h.as<Module>()->name, modname))
      {
        assert(false && "Such module is already loaded");
      }
    }
    // So we can now safely copy module...
    this->assembly = assembly;
    module->accept(*this);
    this->assembly.as<Assembly>()->modules.insert(result);
    this->assembly = Handle();
  }
  
  //
  // Source tokens
  //
  void Loader::visit ( Identifier &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Identifier();
    node.set(np);
    auto &m = *np;
    m.text = n.text;
    m.place = n.place;
    result = node;
  }
  
  void Loader::visit ( Literal &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Literal();
    node.set(np);
    auto &m = *np;
    m.text = n.text;
    m.place = n.place;
    result = node;
  }
  
  //
  // Semantic nodes
  //
  void Loader::visit ( Assembly &n )
  {
    result = assembly;
  }
  
  void Loader::visit ( Module &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Module();
    node.set(np);
    auto &m = *np;
    // Copy it's contents - w/o assembly handle
    m.name = copy(n.name);
    m.deps = copy(n.deps);
    m.decls = copy(n.decls);
    result = node;    
  }
  
  void Loader::visit ( ModuleName &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new ModuleName();
    node.set(np);
    auto &m = *np;
    m.name = copy(n.name);
    result = node;
  }
  
  void Loader::visit ( ModuleFileName &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new ModuleFileName();
    node.set(np);
    auto &m = *np;
    m.files = copy(n.files);
    result = node;
  }
  
  void Loader::visit ( sema::Namespace &n )
  {
    // Should optionally copy Namespace if no such exists...
    // firstly check if parent exists...
    HeapRoot parent = copy(n.parent);
    // check if such namespace exists
    GetDeclarationByName gdbn;
    Handle mem = gdbn.get(n.id, parent);
    if(mem)
    {
      /* Return existing namespace */
      result = mem;
    }
    else
    {
      /* Create new namespace */
      HeapRoot node = translate(n.handle());
      auto *np = new sema::Namespace();
      node.set(np);
      auto &m = *np;
      m.id = copy(m.id);
      m.parent = parent;
      // Do not copy members...
      result = node;
    }
  }

  void Loader::visit ( sema::Variable &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new sema::Variable();
    node.set(np);
    auto &m = *np;
    m.id = copy(n.id);
    m.type = copy(n.type);
    m.value = copy(n.value);
    m.attributes = copy(n.attributes);
    // CHECK FOR PARENT - use copy() :-P
    m.parent = copy(n.parent);
    
    result = node;
  }
}