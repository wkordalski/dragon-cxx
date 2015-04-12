#pragma once

#include <boost/filesystem.hpp>

namespace dragon
{
  namespace user
  {
    // Gets input file and creates some output files
    class Compiler
    {
      // Can be cached in-memory...
      boost::filesystem::path src; // Source files path
      boost::filesystem::path obj; // Object files path
      boost::filesystem::path mod; // Manifests of modules
      
      // Binary output files...
      boost::filesystem::path bin;
      
      
      
      
      
    };
  }
}