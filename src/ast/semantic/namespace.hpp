#pragma once

#include "../../node.hpp"
#include "../../visitor.hpp"

#include <unordered_set>

namespace dragon
{
	namespace sema
	{
		class Namespace : public Node
		{
		public:
			Handle id;
			Handle parent;
			std::unordered_set<Handle> declarations;
			
			Namespace() = default;
			Namespace(Handle id, Handle parent) : id(id), parent(parent) {}
			
			virtual void accept(Visitor &v)
			{
				v.visit(*this);
			}
		};
	}
}