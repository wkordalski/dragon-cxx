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
    void accept(const Handle &h) { if(h) h->accept(*this); }
    void accept(const std::vector<Handle> &v)
    { for(auto h : v) if(h) h->accept(*this); }

    void accept(const std::unordered_set<Handle> &v)
    { for(auto h : v) if(h) h->accept(*this); }

    void accept(const std::unordered_map<Handle, Handle> &v)
    {
      for(auto p : v)
      {
        if(p.first) p.first->accept(*this);
        if(p.second) p.second->accept(*this);
      }
    }

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
      //std::cerr << "Removed " << flag.size() << " objects..." << std::endl;
      flag.clear();
    }

    // Source tokens
    virtual void visit(Identifier &n);
    virtual void visit(Operator &n);
    virtual void visit(Literal &n);
    virtual void visit(Newline &n);
    virtual void visit(Indent &n);
    virtual void visit(Dedent &n);
    // Syntactic nodes
    virtual void visit(File &n);
    virtual void visit(syntax::VariablesDeclaration &n);
    virtual void visit(syntax::SingleVariableDeclaration &n);
    // Semantic nodes
    virtual void visit(Assembly &n);
    virtual void visit(Module &n);
  };

  extern GC gc;
}
