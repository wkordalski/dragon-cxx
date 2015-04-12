#include "memory.hpp"

namespace dragon {
  namespace user {
    Memory Memory::_instance;
    
    Memory &Memory::get()
    {
      return _instance;
    }
    
    void Memory::gc()
    {
      // Run GC and eventually clean some entries within cache
    }
    
    Handle Memory::load ( boost::filesystem::path filename )
    {
      // TODO
      return {};
    }

    void Memory::save ( Handle handle, boost::filesystem::path filename )
    {
      // TODO
    }

  }
}