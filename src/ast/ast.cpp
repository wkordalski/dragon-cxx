#include "../ast.hpp"

namespace dragon
{
  Handle create_declaration_tree(std::vector<Handle> files)
  {
    auto ass = Handle::make<Assembly>();
    for(auto f : files)
    {
      if(auto ff = f.as<File>()) ff->fillin(ass);
      else assert(false && "Why are you putting not-file handle as a file handle?");
    }
    return ass;
  }

  Handle files_to_assembly(std::vector<Handle> files)
  {
    auto h = create_declaration_tree(files);
    return h;
  }
}
