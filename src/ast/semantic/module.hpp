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

#include "../../containers.hpp"
#include "../../node.hpp"
#include "../../visitor.hpp"

#include <unordered_set>
#include <vector>

namespace dragon
{
  class Module : public Node
  {
  public:
    Handle name;
    Handle assembly;
		std::unordered_set<Handle> deps;
    std::unordered_set<Handle> decls;

		Module() = default;
    Module(Handle name) : name(name) {}

    virtual void accept(Visitor &v) { v.visit(*this); }
  };
  
  /*
   * Node for specifying name of module
   */
  class ModuleName : public Node
  {
  public:
    HVector name;
    
    virtual void accept(Visitor &v) { v.visit(*this); }
  };
  
  /*
   * Node for specyfying name of module as a set of compiled files
   */
  class ModuleFileName : public Node
  {
  public:
    HVector files;
    
    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
