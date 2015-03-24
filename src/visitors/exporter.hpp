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
 * Exports the tree into persistent representation
 */

#pragma once

#include "../visitor.hpp"
#include "../node.hpp"

#include <unordered_set>

#include <boost/archive/binary_oarchive.hpp>

namespace dragon
{
  class Exporter : public Visitor
  {
    boost::archive::binary_oarchive ar;
    std::unordered_set<int> flag;
  public:
    Exporter(std::ostream &out) : ar(out) {}

    void serialize(std::vector<Handle> v);
    void serialize(std::vector<Root> v);

  protected:
    void save(Handle h)
    {
      if(!h) return;
      if(flag.count(int(h)) > 0) return;
      flag.insert(int(h));
      h->accept(*this);
    }

    void save(std::vector<Handle> &v)
    {
      for(Handle h : v) save(h);
    }


    void save(std::unordered_set<Handle> &v)
    {
      for(Handle h : v) save(h);
    }

  public:

    // Source tokens
    virtual void visit(Identifier &n);
    virtual void visit(Operator &n);
    virtual void visit(Literal &n);
    virtual void visit(Newline &n);
    virtual void visit(Indent &n);
    virtual void visit(Dedent &n);
    // Syntactic nodes
    virtual void visit(File &n);
		virtual void visit(syntax::NamespaceDeclaration &n);
    virtual void visit(syntax::UseDeclaration &n);
    virtual void visit(syntax::UsingNamespaceDeclaration &n);
    virtual void visit(syntax::VariablesDeclaration &n);
    virtual void visit(syntax::SingleVariableDeclaration &n);
    // Semantic nodes
    virtual void visit(Assembly &n);
    virtual void visit(Module &n);
		virtual void visit(ModuleName &n);
		virtual void visit(sema::Namespace &n);
		virtual void visit(sema::Variable &n);
    // Utilities nodes
    virtual void visit(LookupTable &n);
  };
}
