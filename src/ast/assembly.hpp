#include "../node.hpp"
#include "../utils/collect.hpp"

#include <unordered_set>
#include <unordered_map>

namespace dragon
{
  struct Import : public Node
  {
    std::vector<Handle> name;

    Import(std::vector<Handle> name) : name(name) {}

    virtual std::size_t hash() const { return hash_sequence<std::hash<Handle>>(name); }
    virtual bool equal(const Node *t) const
    {
      if(auto tt = dynamic_cast<const Import*>(t))
        return std::equal(name.begin(), name.end(), tt->name.begin(), tt->name.end(), [](Handle a, Handle b) { return a % b; });
      else return false;
    }
  };

  struct Assembly : public Node
  {
    std::unordered_set<Handle> imports;
    std::unordered_map<Handle, Handle> declarations;
  };
}
