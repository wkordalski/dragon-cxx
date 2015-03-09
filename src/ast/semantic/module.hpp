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
 * Represents a module
 */

#pragma once

#include "../../node.hpp"

#include <unordered_set>
#include <vector>

namespace dragon
{
  class Module : public Node
  {
  public:
    std::vector<Handle> name;
    std::unordered_set<Handle> decls;

    Module(std::vector<Handle> name) : name(name) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
    virtual std::size_t hash() const { return hash_sequence<std::hash<Handle>>(name); }
    virtual bool equal(const Node *t) const
    {
      if(auto tt = dynamic_cast<const Module*>(t))
        return std::equal(name.begin(), name.end(), tt->name.begin(), tt->name.end(), [](Handle a, Handle b) { return a % b; });
      else return false;
    }
  };
}
