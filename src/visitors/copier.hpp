/*
 * Copyright (c) 2015 Wojciech Kordalski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Copies nodes into a new instances (should be the same as doing export and import)
 */

#pragma once

#include "../containers.hpp"
#include "../node.hpp"
#include "../visitor.hpp"

#include <unordered_map>
#include <unordered_set>

namespace dragon
{
  class Copier : public Visitor
  {
    std::unordered_map<int, int> readdress;
    
    // Copy result
    Root result;
  public:
    Handle translate(Handle h)
    {
      if(int(h) == 0) return Handle();
      if(readdress.count(int(h)) > 0)
      {
        return Handle(readdress[int(h)]);
      }
      Handle ret(nullptr);
      readdress[int(h)] = int(ret);
      return ret;
    }
    HVector translate(HVector v)
    {
      HVector ret;
      ret.reserve(v.size());
      std::transform(v.begin(), v.end(),
                     std::back_inserter(ret),
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
    
    HVector copy(std::vector<Root> v)
    {
      HVector ret;
      ret.reserve(v.size());
      for(Root h : v)
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