#include "comparer.hpp"
#include "../ast/source.hpp"

namespace dragon
{
  void Comparer::visit ( Identifier &n )
  {
		if(auto tt = arg.is<Identifier>())
		{
			ret = (tt->text == n.text);
			return;
		}
		ret = false;
		return;
  }
}