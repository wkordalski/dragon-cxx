#pragma once

#include "../../containers.hpp"
#include "../../node.hpp"
#include "../../visitor.hpp"

namespace dragon
{
	namespace syntax
	{
		class NamespaceDeclaration : public Node
		{
		public:
			HVector name;
			HVector declarations;

			NamespaceDeclaration() = default;
			NamespaceDeclaration(HVector name, HVector decls = {}) : name(name), declarations(decls) {}

			virtual void accept(Visitor &v)
			{
				v.visit(*this);
			}
		};
	
	}
}