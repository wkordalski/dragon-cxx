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
      {
        if(name.size()!= tt->name.size()) return false;
        else
        {
          for(int i = 0; i < name.size(); i++)
            if(!(name[i] % tt->name[i]))
              return false;
          return true;
        }
      }
      else return false;
    }

    virtual void print(std::wostream &os) const
    {
      os << "Import ["<<handle()<<"] ( module = [ ";
      for(auto h : name) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    virtual std::vector<Handle> get_members() const { return name; }
  };

  struct Assembly : public Node
  {
    std::unordered_set<Handle> imports;
    std::unordered_map<Handle, Handle> declarations;

    virtual void print(std::wostream &os) const
    {
      os << "Assembly ["<<handle()<<"] ( decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "], imports = [ ";
      for(auto h : imports) os << int(h) << " ";
      os << "] )" << std::endl;
    }

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
