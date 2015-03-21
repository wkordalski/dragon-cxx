#include "../../node.hpp"

#include <unordered_set>
#include <unordered_map>

namespace dragon
{
  class Assembly : public Node
  {
  public:
    std::unordered_set<Handle> modules;
    std::unordered_set<Handle> declarations;

    virtual void accept(Visitor &v){ v.visit(*this); }
  };
}
