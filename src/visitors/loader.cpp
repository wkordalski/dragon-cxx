#include "loader.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/assembly.hpp"

#include "../queries/comparer.hpp"

namespace dragon {
  void Loader::load ( Handle module, Handle assembly )
  {
    Comparer cmp;
    Handle modname = module.as<Module>()->name;
    for(auto h : assembly.as<Assembly>()->modules)
    {
      if(cmp.compare(h.as<Module>()->name, modname))
      {
        assert(false && "Such module already loaded");
      }
    }
    // So we can now safely copy module...
    this->assembly = assembly;
    module->accept(*this);
    this->assembly = Handle();
  }
}