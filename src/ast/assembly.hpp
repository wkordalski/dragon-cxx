#include "../node.hpp"

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

    virtual std::vector<Handle> get_members() const { return name; }
  };

  struct Assembly : public Node
  {
    std::unordered_set<Handle> imports;
    std::unordered_map<Handle, Handle> declarations;

    virtual std::vector<Handle> get_members() const
    {
      std::vector<Handle> r;
      r.reserve(imports.size() + declarations.size() * 2);
      for(auto h : imports) r.push_back(h);
      for(auto p : declarations)
      {
        r.push_back(p.first);
        r.push_back(p.second);
      }
      return r;
    }
  };
}
