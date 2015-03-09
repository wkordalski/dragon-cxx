#include "importer.hpp"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

namespace dragon
{
  std::vector<Handle> Importer::deserialize()
  {
    std::vector<Handle> lod;
    ar >> lod;
    std::vector<Handle> ret;
    ret.reserve(lod.size());
    for(Handle h : lod)
    {
      if(readdress.count(int(h)) == 0)
      {
        Handle nh(nullptr);
        readdress[int(h)] = int(nh);
      }
      ret.push_back(Handle(readdress[int(h)]));
      required.insert(h);
    }
    // Load nodes and translate its handles
    while(required.size() > 0)
    {
      // read nodes until all required are loaded
    }
    return ret;
  }
}
