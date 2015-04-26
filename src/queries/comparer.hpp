#pragma once

#include "../node.hpp"
#include "../visitor.hpp"

namespace dragon {
	class Comparer : public Visitor
	{
		HeapRoot arg;
		bool ret;
	public:
		bool compare(Handle g, Handle h)
		{
			arg = h;
			ret = false;
			g->accept(*this);
			return ret;
		}
		
	public:
		virtual void visit( Identifier &n );
	};
}