#pragma once

#include "../containers.hpp"
#include "../node.hpp"
#include "../visitor.hpp"
#include <unordered_set>
#include <unordered_map>

namespace dragon
{
  /*
   * Copies module to specified assembly
   */
  class Loader : public Visitor
  {
    Root assembly;
    std::unordered_map<int, int> readdress;
    
    // Copy result
    Root result;
    // TODO
  public:
    void load(Handle module, Handle assembly);
    
    Handle translate(Handle h)
    {
      if(int(h) == 0) return Handle();
      if(readdress.count(int(h)) > 0)
      {
        return Handle(readdress[int(h)]);
      }
      Handle ret(nullptr);
      readdress[int(h)] = int(ret);
      return Handle(nullptr);
    }
    std::vector<Handle> translate(std::vector<Handle> v)
    {
      std::vector<Handle> ret;
      ret.reserve(v.size());
      std::transform(v.begin(), v.end(),
                     std::back_insert_iterator<std::vector<Handle>>(ret),
                     [this](Handle h){ return translate(h); });
      return ret;
    }
    
    Handle copy(Handle h)
    {
      h->accept(*this);
      return result;
    }
    
    HVector copy(HVector v)
    {
      HVector ret;
      ret.reserve(v.size());
      for(Handle h : v)
      {
        h->accept(*this);
        ret.push_back(result);
      }
      return ret;
    }
    
    std::unordered_set<Handle> copy(std::unordered_set<Handle> v)
    {
      std::unordered_set<Handle> ret;
      ret.reserve(v.size());
      for(Handle h : v)
      {
        h->accept(*this);
        ret.insert(result);
      }
      return ret;
    }
    
  public:
    // Source tokens
    virtual void visit(Identifier &n);
    virtual void visit(Literal &n);
    // Semantic nodes
    virtual void visit(Assembly &n);
    virtual void visit(Module &n);
    virtual void visit(ModuleName &n);
    virtual void visit(ModuleFileName &n);
    virtual void visit(sema::Namespace &n);
    virtual void visit(sema::Variable &n);
  };
}
