#include "memory.hpp"

#include "../visitors/copier.hpp"
#include "../visitors/gc.hpp"
#include "../visitors/importer.hpp"
#include "../visitors/exporter.hpp"

#include <cstdlib>

#include <fstream>

namespace dragon {
  namespace user {
    Memory Memory::_instance;
    
    Memory &Memory::get()
    {
      return _instance;
    }
    
    void Memory::gc()
    {
      dragon::gc.run();
    }
    
    HVector Memory::load ( boost::filesystem::path filename )
    {
      auto it = this->files.find(filename.wstring());
      if(it != files.end())
      {
        // Load from disk
        auto jt = this->symlinks.find(filename.wstring());
        std::ifstream ifs;
        if(jt != symlinks.end())
        {
          ifs.open(jt->second.c_str());
        }
        else
        {
          ifs.open(filename.c_str());
        }
        Importer importer(ifs);
        HVector ret = importer.deserialize();
        std::vector<Root> rrt;
        rrt.reserve(ret.size());
        for(Handle h : ret) rrt.push_back(h);
        files[filename.wstring()] = cache_counter;
        cache[cache_counter] = rrt;
        cache_counter++;
        Copier copier;
        return copier.copy(ret);
      }
      else
      {
        Copier copier;
        return copier.copy(cache[it->second]);
      }
    }

    void Memory::save ( HVector handle, std::vector<boost::filesystem::path> filename )
    {
      // 1) Make copy
      Copier copier;
      HVector ret = copier.copy(handle);
      std::vector<Root> rrt;
      rrt.reserve(ret.size());
      for(Handle h : ret) rrt.push_back(h);
      // 2) Save it to cache
      for(boost::filesystem::path p : filename)
      {
        symlinks[p.wstring()] = filename[0];
        if(files.count(p.wstring()) > 0) files.erase(p.wstring());
      }
      files[filename[0].wstring()] = cache_counter;
      cache[cache_counter] = rrt;
      cache_counter++;
      // 3) Save the file to disk
      for(boost::filesystem::path p : filename)
      {
        std::ofstream ofs(p.c_str());
        Exporter exporter(ofs);
        exporter.serialize(handle);
        ofs.close();
      }
    }
  }
}