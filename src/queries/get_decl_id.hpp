#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon
{
	class GetDeclarationId : public Visitor
	{
		HeapRoot ret;
	public:
		Handle get(Handle declaration)
		{
			ret = HeapRoot();
			declaration->accept(*this);
			assert(ret.valid() && "Unnamed declarations not supported...");
			return ret;
		}
		
	public:
		virtual void visit( sema::Namespace &n );
		virtual void visit( sema::Variable &n );
	};
}