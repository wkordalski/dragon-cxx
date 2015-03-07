#pragma once

#include "../node.hpp"

#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace dragon
{
  //int gccollect_count() { return 0; }
  int gccollect_count(const dragon::Handle &);
  int gccollect_count(const std::vector<dragon::Handle> &v);
  int gccollect_count(const std::unordered_set<dragon::Handle> &s);
  int gccollect_count(const std::unordered_map<dragon::Handle, dragon::Handle> &s);

  template<class T, class... Args>
  int gccollect_count(const T& v, Args... args)
  {
    return gccollect_count(args...) + gccollect_count(v);
  }

  ///////////////

  //void gccollect_insert(std::vector<dragon::Handle> &v) {}
  void gccollect_insert(std::vector<dragon::Handle> &v, const dragon::Handle &h);
  void gccollect_insert(std::vector<dragon::Handle> &v, const std::vector<dragon::Handle> &w);
  void gccollect_insert(std::vector<dragon::Handle> &v, const std::unordered_set<dragon::Handle> &w);
  void gccollect_insert(std::vector<dragon::Handle> &v, const std::unordered_map<dragon::Handle, dragon::Handle> &w);
  
  template<class T, class... Args>
  void gccollect_insert(std::vector<dragon::Handle> &v, const T &w, Args... args)
  {
    gccollect_insert(v, w);
    gccollect_insert(v, args...);
  }

  template<class... Args>
  std::vector<Handle> gccollect(Args... args)
  {
    std::vector<Handle> r;
    r.reserve(gccollect_count(args...));
    gccollect_insert(r, args...);
    return r;
  }
}
