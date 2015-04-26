#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon
{
	class GetDeclarationByName : public Visitor
	{
		HeapRoot id;
		HeapRoot ret;
	public:
		Handle get(Handle id, Handle container)
		{
			this->id = id;
			this->ret = HeapRoot();
			container->accept(*this);
			return ret;
		}
		
	public:
		virtual void visit( Assembly &n );
		virtual void visit( sema::Namespace &n );
	};
}