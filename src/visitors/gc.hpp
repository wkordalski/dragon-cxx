#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

#include <list>
#include <unordered_map>
#include <unordered_set>

namespace dragon
{
  class GC : public Visitor
  {
  private:
    std::unordered_map<int, Node *> &objects;
    std::list<int> &roots;
    std::unordered_set<int> flag;

    // Returns true if next process is needed
    // False if the node was earlier processed
    bool mark(Node &n) { return flag.erase(int(n.handle())) > 0; }

  public:
    GC(std::unordered_map<int, Node *> &objects, std::list<int> &roots) : objects(objects), roots(roots) {}

    // runs clean-up
    void run()
    {
      flag.clear();
      flag.reserve(objects.size());
      for(auto p : objects) flag.insert(p.first);
      for(int h : roots) objects[h]->accept(*this);
      for(int h : flag)
      {
        delete objects[h];
        objects.erase(h);
      }
      flag.clear();
    }

    virtual void visit(Identifier &n);
  };

  extern GC gc;
}
