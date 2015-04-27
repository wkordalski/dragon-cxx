#pragma once

#include "node.hpp"

#include <unordered_set>
#include <vector>

namespace dragon {
  
  typedef StackRoot Local;
  typedef Handle Member;  
  typedef HeapRoot Object;
  
  
  typedef std::pair<Local, Local> LLPair;
  typedef std::pair<Member, Member> MMPair;
  
  typedef std::vector<StackRoot, NodeAllocator<StackRoot>> LVector;
  typedef std::vector<Handle, NodeAllocator<Handle>> MVector;
  typedef std::vector<HeapRoot, NodeAllocator<HeapRoot>> OVector;

  typedef std::vector<std::pair<Member, Member>,
                      NodeAllocator<std::pair<Member, Member>>> MMVector;
  
  
  typedef std::unordered_set<StackRoot, NodeAllocator<StackRoot>> LSet;
  typedef std::unordered_set<Handle, NodeAllocator<Handle>> MSet;
  typedef std::unordered_set<HeapRoot, NodeAllocator<HeapRoot>> OSet;
  
  // Deprecated
  typedef std::vector<Handle, NodeAllocator<Handle>> MVector;
  
  MVector to_member(LVector v);
  
  template<class T, class... Args>
  Local make_node(Args... args)
  {
    return Local(Handle(new T(args...)));
  }
}