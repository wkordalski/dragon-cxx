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
 * Prints nodes to some output stream
 */

#include "printer.hpp"

#include "../ast/semantic/assembly.hpp"
#include "../ast/semantic/module.hpp"
#include "../ast/semantic/variable.hpp"

#include "../utils/lookup_table.hpp"

namespace dragon
{
	// Source tokens
  void NodePrinter::visit ( Identifier &n )
  {
    out << "Identifier ["<<n.handle()<<"] ( text = \""<<n.text<<"\" )" << std::endl;
  }
  // Syntactic nodes
	// Semantic nodes
  void NodePrinter::visit ( Assembly &n )
  {
		out << "Assembly ["<<n.handle()<<"] ( modules = [ ";
		for(auto h : n.modules) out << h << " ";
		out << "], declarations = [ ";
		for(auto p : n.declarations) out << "<" << p.first << ", " << p.second << "> ";
		out << "] )" << std::endl;
	}
  void NodePrinter::visit ( Module &n )
  {
    out << "Module ["<<n.handle()<<"] "
				<< "( name = [ ";
		for(auto h : n.name) out << h << " ";
		out << "], dependencies = [ ";
		for(auto h : n.deps) out << h << " ";
		out << "], declarations = [ ";
		for(auto h : n.decls) out << h << " ";
		out << "] )" << std::endl;
  }
  void NodePrinter::visit ( ModuleSpecification &n )
  {
    out << "ModuleSpecification ["<<n.handle()<<"] "
				<< "( name = [ ";
		for(auto h : n.name) out << h << " ";
		out << "] )" << std::endl;
  }
  void NodePrinter::visit ( sema::Variable &n )
  {
    out << "sema::Variable [" << n.handle() << "] "
				<< "( id = " << n.id << ", type = " << n.type << ", value = " << n.value << ", parent = " << n.parent
				<< ", attribs = [ ";
		for(auto h : n.attributes) out << h << " ";
		out <<"] )" << std::endl;
  }
	// Utilities nodes
  void NodePrinter::visit ( LookupTable &n )
  {
    out << "LookupTable [" << n.handle() << "]" << std::endl;
  }

}
