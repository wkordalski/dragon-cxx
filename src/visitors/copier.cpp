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

#include "copier.hpp"

#include "../ast/source.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/namespace.hpp"
#include "../ast/semantic/variable.hpp"

namespace dragon {
  //
  // Source tokens
  //
  void Copier::visit ( Identifier &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Identifier();
    node.set(np);
    auto &m = *np;
    m.text = n.text;
    m.place = n.place;
    result = node;
  }
  
  void Copier::visit ( Literal &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Literal();
    node.set(np);
    auto &m = *np;
    m.text = n.text;
    m.place = n.place;
    result = node;
  }
  
  //
  // Semantic nodes
  //
  void Copier::visit ( Assembly &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Assembly();
    node.set(np);
    result = node;
  }
  
  void Copier::visit ( Module &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new Module();
    node.set(np);
    auto &m = *np;
    // Copy it's contents - w/ assembly handle
    m.assembly = copy(n.assembly);
    m.name = copy(n.name);
    m.deps = copy(n.deps);
    m.decls = copy(n.decls);
    result = node;    
  }
  
  void Copier::visit ( ModuleName &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new ModuleName();
    node.set(np);
    auto &m = *np;
    m.name = copy(n.name);
    result = node;
  }
  
  void Copier::visit ( ModuleFileName &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new ModuleFileName();
    node.set(np);
    auto &m = *np;
    m.files = copy(n.files);
    result = node;
  }
  
  void Copier::visit ( sema::Namespace &n )
  {
    HeapRoot parent = copy(n.parent);
    /* Create new namespace */
    HeapRoot node = translate(n.handle());
    auto *np = new sema::Namespace();
    node.set(np);
    auto &m = *np;
    m.id = copy(m.id);
    m.parent = parent;
    // Do not copy members...
    result = node;
  }

  void Copier::visit ( sema::Variable &n )
  {
    HeapRoot node = translate(n.handle());
    auto *np = new sema::Variable();
    node.set(np);
    auto &m = *np;
    m.id = copy(n.id);
    m.type = copy(n.type);
    m.value = copy(n.value);
    m.attributes = copy(n.attributes);
    // CHECK FOR PARENT - use copy() :-P
    m.parent = copy(n.parent);
    result = node;
  }
}