#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon {
	class DeclarationToContainerInserter : public Visitor
	{
		Root decl;
	public:
		void insert(Handle declaration, Handle container)
		{
			assert(declaration.valid());
			decl = declaration;
			container->accept(*this);
			assert(!decl.valid());
		}
		
	public:
		virtual void visit( Assembly &n );
		virtual void visit( sema::Namespace &n );
	};
}