#include "../../node.hpp"

#include <unordered_set>
#include <unordered_map>

namespace dragon
{
  class Assembly : public Node
  {
  public:
    std::unordered_set<Handle> imports;
    std::unordered_map<Handle, Handle> declarations;

    virtual void accept(Visitor &v){ v.visit(*this); }
  };
}
