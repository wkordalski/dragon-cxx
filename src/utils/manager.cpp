#include "../node.hpp"

#include <cassert>
#include <iomanip>
#include <stack>
#include <unordered_map>

namespace dragon
{
  struct ObjectInfo
  {
    enum GCState
    {
      Unknown,
      Known
    };

    GCState color;

    void set_unknown() { color = GCState::Unknown; }
    void set_known() { color = GCState::Known; }
    bool is_known() { return color == GCState::Known; }
  };

  std::unordered_map<int, std::pair<Node *, ObjectInfo>> manager_map;
  std::list<int> manager_roots;
  int manager_counter = 1;

  Node * Handle::operator -> () const { return get(); }
  Node * Handle::operator *  () const { return get(); }

  Handle::Handle() : h(0) { }

  Handle::Handle(Node * p)
  {
    // We want small numbers for debugging purposes
    if(manager_counter > 1024) manager_counter = 1;
    // Find free handle
    while(manager_counter == 0 or manager_map.count(manager_counter) > 0) manager_counter++;
    manager_map[manager_counter] = std::make_pair(p, ObjectInfo());
    p->self = Handle(manager_counter);
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
    assert(handle == 0 or manager_map.count(handle) > 0);
  }

  bool Handle::exists(int handle)
  {
    return manager_map.count(handle) > 0;
  }

  Handle::~Handle() {}

  Handle & Handle::operator = (const Handle &orig)
  {
    this->h = orig.h;
    return *this;
  }

  Node * Handle::get() const
  {
    assert(h != 0);
    return manager_map[h].first;
  }

  const Handle & Handle::set(Node *p) const
  {
    if(valid()) delete get();
    p->self = *this;
    manager_map[h].first = p;
    return *this;
  }

  Handle & Handle::set(Node *p)
  {
    if(valid()) delete get();
    p->self = *this;
    manager_map[h].first = p;
    return *this;
  }

  bool Handle::valid() const
  {
    return (h != 0);
  }

  Handle Handle::operator % (Handle h) const
  {
    if(!valid() or !h.valid()) return Handle();
    if(get()->equal(h.get())) return h;
    else return Handle();
  }

  void Node::replace(Node *t)
  {
    self.set(t);
  }

  void Handle::cleanup()
  {
    std::stack<int> known;
    // Let's start with setting color to Unknown
    for(auto &p : manager_map)
      p.second.second.set_unknown();
    // Color roots with Known
    for(int h : manager_roots)
    {
      manager_map[h].second.set_known();
      known.push(h);
    }
    // Work with graph
    while(!known.empty())
    {
      int h = known.top();
      known.pop();
      auto p = manager_map[h].first;
      auto vc = p->get_members();
      for(auto vch : vc)
      {
        if(!vch) continue;
        auto hh = vch.h;
        if(!manager_map[hh].second.is_known())
        {
          manager_map[hh].second.set_known();
          known.push(hh);
        }
      }
    }
    // Remove unknown objects
    std::vector<std::pair<int, std::pair<Node *, ObjectInfo>>> trash;
    for(auto &p : manager_map)
    {
      if(!p.second.second.is_known())
      {
        trash.push_back(p);
      }
    }
    for(auto &p : trash)
    {
      delete p.second.first;
      manager_map.erase(p.first);
    }
  }

  Root::Root()
  {
    this->h = 0;
    this->entry = manager_roots.end();
  }

  Root::Root(const Handle &h)
  {
    this->h = h.h;
    manager_roots.push_front(h.h);
    this->entry = manager_roots.begin();
  }
  Root::Root(Root &h)
  {
    this->h = h.h;
    manager_roots.push_front(h.h);
    this->entry = manager_roots.begin();
  }
  Root::Root(Root &&h)
  {
    this->h = h.h;
    this->entry = h.entry;
    h.h = 0;
    h.entry = manager_roots.end();
  }

  Root & Root::operator = (const Handle &h)
  {
    if(this->h != 0)
      manager_roots.erase(this->entry);
    this->h = h.h;
    manager_roots.push_front(h.h);
    this->entry = manager_roots.begin();
    return *this;
  }
  Root & Root::operator = (Root &h)
  {
    if(this->h != 0)
      manager_roots.erase(this->entry);
    this->h = h.h;
    manager_roots.push_front(h.h);
    this->entry = manager_roots.begin();
    return *this;
  }
  Root & Root::operator = (Root &&h)
  {
    if(this->h != 0)
      manager_roots.erase(this->entry);
    this->h = h.h;
    this->entry = h.entry;
    h.h = 0;
    h.entry = manager_roots.end();
    return *this;
  }

  Root::~Root()
  {
    if(this->h != 0)
      manager_roots.erase(this->entry);
  }
}

namespace std
{
  std::wostream & operator << (std::wostream &os, dragon::Handle h)
  {
    return os << int(h);
  }
}
