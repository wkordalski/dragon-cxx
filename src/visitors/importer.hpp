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
 * Imports the tree from persistent representation
 */

#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>

namespace dragon
{
  class Importer : public Visitor
  {
    typedef void (Importer::*decode_func)(Handle &);

    boost::archive::binary_iarchive ar;
    std::unordered_map<int, int> readdress;
    std::unordered_set<int> required;

    static decode_func decoder[];

  public:
    Importer(std::istream &out) : ar(out) {}

    std::vector<Handle> deserialize();

  protected:
    Handle translate(Handle h)
    {
      if(int(h) == 0) return Handle();
      if(readdress.count(int(h)) == 0)
      {
        Handle nh(nullptr);
        readdress[int(h)] = int(nh);
        required.insert(int(h));
      }
      return Handle(readdress[int(h)]);
    }

  public:
    /*
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
    */
  protected:
    template<class T>
    void read(Handle &h);
  };
}
