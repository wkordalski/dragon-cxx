#pragma once

#include "../../node.hpp"
#include "../../visitor.hpp"

namespace dragon
{
	namespace syntax
	{
		class NamespaceDeclaration : public Node
		{
		public:
			std::vector<Handle> name;
			std::vector<Handle> declarations;

			NamespaceDeclaration() = default;
			NamespaceDeclaration(std::vector<Handle> name, std::vector<Handle> decls = {}) : name(name), declarations(decls) {}

			virtual void accept(Visitor &v)
			{
				v.visit(*this);
			}
		};
	
	}
}