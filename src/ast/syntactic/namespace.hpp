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
			MVector name;
			MVector declarations;

			NamespaceDeclaration() = default;
			NamespaceDeclaration(LVector name, LVector decls = {})
        : name(to_member(name)), declarations(to_member(decls)) {}

			virtual void accept(Visitor &v)
			{
				v.visit(*this);
			}
		};
	
	}
}