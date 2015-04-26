#include "node.hpp"

#include <iostream>

#include "visitors/gc.hpp"

namespace dragon {
  static std::size_t _mem_used = 0;
  static std::size_t _mem_limit = 0x100000; // 1Mb
  
  void * Node::operator new ( std::size_t size )
  {
    return allocate(size);
  }

  void Node::operator delete ( void *pointer, std::size_t size )
  {
    deallocate(pointer, size);
  }
  
  void * allocate ( std::size_t size )
  {
    //std::cout << "Allocation   ("<<size<<"): " << _mem_used << "/" << _mem_limit << std::endl;
    // Cant run GC here because of inconsistent state!
    _mem_used += size;
    return malloc(size);
  }

  void deallocate ( void *pointer, std::size_t size )
  {
    //std::cerr << "Deallocating (" << size << ") of " << _mem_used << std::endl;
    if(size > _mem_used)
    {
      std::cerr << "Freeing unallocated memory!!!" << std::endl;
    }
    _mem_used -= size;
    return free(pointer);
  }
  
  std::size_t memory_use()
  {
    return _mem_used;
  }
  
  std::size_t memory_limit()
  {
    return _mem_limit;
  }
  
  void memory_limit(std::size_t limit)
  {
    _mem_limit = limit;
  }
}