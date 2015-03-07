#include "collect.hpp"

namespace dragon
{
  //int gccollect_count() { return 0; }

  int gccollect_count(const dragon::Handle &)
  {
    return 1;
  }

  int gccollect_count(const std::vector<dragon::Handle> &v)
  {
    return v.size();
  }

  int gccollect_count(const std::unordered_set<dragon::Handle> &s)
  {
    return s.size();
  }

  int gccollect_count(const std::unordered_map<dragon::Handle, dragon::Handle> &s)
  {
    return s.size() * 2;
  }

  ///////////////

  //void gccollect_insert(std::vector<dragon::Handle> &v) {}

  void gccollect_insert(std::vector<dragon::Handle> &v, const dragon::Handle &h)
  {
    v.push_back(h);
  }

  void gccollect_insert(std::vector<dragon::Handle> &v, const std::vector<dragon::Handle> &w)
  {
    for(auto h : w) v.push_back(h);
  }

  void gccollect_insert(std::vector<dragon::Handle> &v, const std::unordered_set<dragon::Handle> &w)
  {
    for(auto h : w) v.push_back(h);
  }

  void gccollect_insert(std::vector<dragon::Handle> &v, const std::unordered_map<dragon::Handle, dragon::Handle> &w)
  {
    for(auto p : w)
    {
      v.push_back(p.first);
      v.push_back(p.second);
    }
  }
}
