#include "../token.hpp"
#include "decls.hpp"

#include <unordered_set>

namespace dragon
{
  class Import : public Token
  {
  public:
    std::vector<Handle> name;

    Import(std::vector<Handle> name) : name(name) {}

    virtual std::size_t hash() const { return hash_sequence<std::hash<Handle>>(name); }
    virtual bool equal(const Token *t) const
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
      os << "Import ["<<handle()<<"] ( module = [";
      for(auto h : name) os << int(h) << " ";
      os << "] )" << std::endl;
    }
  };

  class Assembly : public Token, public IDeclarationContainer
  {
    std::unordered_set<Handle> imports;
    std::unordered_map<Handle, Handle> declarations;
  public:

    virtual Handle by_name(Handle h) { if(declarations.count(h) > 0) return declarations[h]; else return Handle(); }
    virtual void add_declaration(Handle h)
    {
      auto decl = h.is<IDeclaration>();
      assert(declarations.count(decl->get_name()) == 0);
      declarations[decl->get_name()] = h;
    }

    virtual void print(std::wostream &os) const
    {
      os << "Assembly ["<<handle()<<"] ( decls = [ ";
      for(auto p : declarations) os << int(p.second) << " ";
      os << "], imports = [ ";
      for(auto h : imports) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    friend class ImportDecl;
  };

  class CompiledAssembly : public Token
  {
    // TODO: everything
  };

  Handle files_to_assembly(std::vector<Handle> files);
  Handle compile_assembly(Handle assembly);
  void init_builtins(Handle assembly);
}
