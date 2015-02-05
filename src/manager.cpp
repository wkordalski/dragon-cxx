#include "token.hpp"

#include <cassert>
#include <iomanip>
#include <unordered_map>

namespace dragon
{
  std::unordered_map<int, std::pair<int, Handle::pointer>> manager_map;
  int manager_counter = 1;

  Handle::pointer Handle::operator -> () const { return get(); }
  Handle::pointer Handle::operator *  () const { return get(); }

  Handle::Handle() : h(0) { }

  Handle::Handle(Token *t) : Handle(Handle::pointer(t)) {}

  Handle::Handle(Handle::pointer p)
  {

    while(manager_counter == 0 or manager_map.count(manager_counter) > 0) manager_counter++;
    manager_map[manager_counter] = std::make_pair(1, p);
    p->h = manager_counter;
    this->h = manager_counter;
    manager_counter++;
  }

  Handle::Handle(const Handle &orig)
  {
    this->h = orig.h;
    if(h != 0) manager_map[h].first++;
  }

  // ONLY DEBUGGING PURPOSES
  Handle::Handle(int handle)
  {
    this->h = handle;
    if(h == 0) return;
    assert(manager_map.count(handle) > 0 && "Invalid handle passed");
    manager_map[h].first++;
  }

  bool Handle::exists(int handle)
  {
    return manager_map.count(handle) > 0;
  }

  Handle::~Handle()
  {
    if(h == 0) return;
    if(manager_map.count(h) == 0) return;
    manager_map[h].first--;
    if(manager_map[h].first == 0)
    {
      manager_map.erase(h);
    }
  }

  Handle & Handle::operator = (const Handle &orig)
  {
    if(this->h == orig.h) return *this;

    if(h != 0)
    {
      manager_map[h].first--;
      if(manager_map[h].first == 0)
      {
        manager_map.erase(h);
      }
    }

    this->h = orig.h;
    if(h != 0) manager_map[h].first++;
    return *this;
  }

  Handle::pointer Handle::get() const
  {
    assert(h != 0);
    return manager_map[h].second;
  }

  const Handle & Handle::set(Handle::pointer p) const
  {
    assert(h != 0);
    p->h = h;
    manager_map[h].second = p;
    return *this;
  }

  Handle & Handle::set(Handle::pointer p)
  {
    assert(h != 0);
    p->h = h;
    manager_map[h].second = p;
    return *this;
  }

  bool Handle::valid() const
  {
    return (h != 0);
  }

  void Token::replace(Token *t)
  {
    t->h = h;
    h = 0;
    assert(t->h != 0);
    manager_map[t->h].second = Handle::pointer(t);
  }

  void Handle::cleanup()
  {

    std::vector<int> hs;
    for(std::pair<int,std::pair<int,Handle::pointer>> p : manager_map)
    {
      hs.push_back(p.first);
    }
    if(hs.size() > 0)
    {
      std::wcerr << L"LEAKS DETECTED           \t [  running clean-up  ]" << std::endl;
      for(int h : hs)
      {
        std::wcerr << L"Freeing managed object: 0x" << std::internal << std::hex << std::setw(4) << std::setfill(L'0') << h;
        if(manager_map.count(h))
        {
          std::wcerr << L"\t [ " << std::internal << std::hex << std::setw(18) << std::setfill(L'0') << manager_map[h].second.get() << L" ]" << std::endl;
          manager_map.erase(h);
        }
        else
        {
          std::wcerr << L"\t [    dead pointer    ]" << std::endl;
        }
      }
      std::wcerr << L"Clear container..." << std::endl;
    }
    manager_map.clear();
  }
}
