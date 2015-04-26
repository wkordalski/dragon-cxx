#include "../node.hpp"
#include "../visitors/gc.hpp"

#include <cassert>
#include <iomanip>
#include <stack>
#include <unordered_map>

namespace dragon
{
  std::unordered_map<int, Node *> object_heap;
  std::list<int> manager_heap_roots;
  std::vector<int> manager_stack_roots;
  int manager_counter = 1;
  GC gc(object_heap, manager_heap_roots, manager_stack_roots);

  Node * Handle::operator -> () const { return get(); }
  Node * Handle::operator *  () const { return get(); }

  Handle::Handle() : h(0) { }

  Handle::Handle(Node * p)
  {
    // We want small numbers for debugging purposes
    if(manager_counter > 1024) manager_counter = 1;
    // Find free handle
    while(manager_counter == 0 or object_heap.count(manager_counter) > 0) manager_counter++;
    object_heap[manager_counter] = p;
    if(p != nullptr) p->self = Handle(manager_counter);
    this->h = manager_counter;
    manager_counter++;
  }

  Handle::Handle(const Handle &orig)
  {
    this->h = orig.h;
  }

  // ONLY DEBUGGING PURPOSES
  Handle::Handle(int handle)
  {
    this->h = handle;
    assert(handle == 0 or object_heap.count(handle) > 0);
  }

  bool Handle::exists(int handle)
  {
    return object_heap.count(handle) > 0;
  }

  Handle::~Handle() {}

  Handle & Handle::operator = (const Handle &orig)
  {
    this->h = orig.h;
    return *this;
  }

  Node * Handle::get() const
  {
    assert(h != 0 and object_heap.count(h) > 0);
    return object_heap[h];
  }

  const Handle & Handle::set(Node *p) const
  {
    if(valid() && get()) delete get();
    p->self = *this;
    object_heap[h] = p;
    return *this;
  }

  Handle & Handle::set(Node *p)
  {
    if(valid() && get()) delete get();
    p->self = *this;
    object_heap[h] = p;
    return *this;
  }

  bool Handle::valid() const
  {
    if(h != 0) assert(object_heap.count(h) > 0);
    return (h != 0);
  }
  
  void Node::replace(Node *t)
  {
    self.set(t);
  }

  void Handle::cleanup()
  {
    if(object_heap.size() > 0) std::cerr << "Removing " << object_heap.size() << " objects..." << std::endl;
    for(auto p : object_heap)
    {
      delete p.second;
    }
    object_heap.clear();
  }

  HeapRoot::HeapRoot()
  {
    this->h = 0;
    this->entry = manager_heap_roots.end();
  }

  HeapRoot::HeapRoot(const Handle &h)
  {
    this->h = h.h;
    manager_heap_roots.push_front(h.h);
    this->entry = manager_heap_roots.begin();
  }
  HeapRoot::HeapRoot(const HeapRoot &h)
  {
    this->h = h.h;
    manager_heap_roots.push_front(h.h);
    this->entry = manager_heap_roots.begin();
  }
  HeapRoot::HeapRoot(HeapRoot &&h)
  {
    this->h = h.h;
    this->entry = h.entry;
    h.h = 0;
    h.entry = manager_heap_roots.end();
  }

  HeapRoot & HeapRoot::operator = (const Handle &h)
  {
    if(this->h != 0)
      manager_heap_roots.erase(this->entry);
    this->h = h.h;
    manager_heap_roots.push_front(h.h);
    this->entry = manager_heap_roots.begin();
    return *this;
  }
  HeapRoot & HeapRoot::operator = (HeapRoot &h)
  {
    if(this->h != 0)
      manager_heap_roots.erase(this->entry);
    this->h = h.h;
    manager_heap_roots.push_front(h.h);
    this->entry = manager_heap_roots.begin();
    return *this;
  }
  HeapRoot & HeapRoot::operator = (HeapRoot &&h)
  {
    if(this->h != 0)
      manager_heap_roots.erase(this->entry);
    this->h = h.h;
    this->entry = h.entry;
    h.h = 0;
    h.entry = manager_heap_roots.end();
    return *this;
  }

  HeapRoot::~HeapRoot()
  {
    if(this->h != 0)
      manager_heap_roots.erase(this->entry);
  }
  
  
  StackRoot::StackRoot()
  {
    this->h = 0;
    this->entry = manager_stack_roots.size() - 1;
  }

  StackRoot::StackRoot(const Handle &h)
  {
    this->h = h.h;
    manager_stack_roots.push_back(h.h);
    this->entry = manager_stack_roots.size() - 1;
  }
  StackRoot::StackRoot(const StackRoot &h)
  {
    this->h = h.h;
    manager_stack_roots.push_back(h.h);
    this->entry = manager_stack_roots.size() - 1;
  }
  StackRoot::StackRoot(StackRoot &&h)
  {
    this->h = h.h;
    this->entry = h.entry;
    h.h = 0;
    h.entry = 0;
  }

  StackRoot & StackRoot::operator = (const Handle &h)
  {
    manager_stack_roots[this->entry] = h.h;
    return *this;
  }
  StackRoot & StackRoot::operator = (StackRoot &h)
  {
    manager_stack_roots[this->entry] = h.h;
    return *this;
  }
  StackRoot & StackRoot::operator = (StackRoot &&h)
  {
    manager_stack_roots[this->entry] = h.h;
    return *this;
  }

  StackRoot::~StackRoot()
  {
    const Handle::id empty = std::numeric_limits<Handle::id>::max();
    manager_stack_roots[this->entry] = empty;
    while(manager_stack_roots.size() > 0 && manager_stack_roots[manager_stack_roots.size() - 1] == empty)
      manager_stack_roots.pop_back();
  }
}

namespace std
{
  std::wostream & operator << (std::wostream &os, dragon::Handle h)
  {
    return os << int(h);
  }
}
