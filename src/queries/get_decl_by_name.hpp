#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon
{
	class GetDeclarationByName : public Visitor
	{
		Root id;
		Root ret;
	public:
		Handle get(Handle id, Handle container)
		{
			this->id = id;
			this->ret = Root();
			container->accept(*this);
			return ret;
		}
		
	public:
		virtual void visit( Assembly &n );
		virtual void visit( sema::Namespace &n );
	};
}