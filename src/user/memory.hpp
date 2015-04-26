/*
 * Copyright (c) 2015 Wojciech Kordalski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Manages memory
 */
#pragma once

#include "../containers.hpp"
#include "../node.hpp"

#include <queue>
#include <unordered_map>

#include <boost/filesystem.hpp>

namespace dragon
{
  namespace user
  {
    class Memory
    {
      // Singleton
    private:
      static Memory _instance;
    private:
      Memory() = default;
      
      // Bytes we can allocate (for cache)
      std::size_t _mem_amount;
      
      // Some queue for storing write-to-file requests
      std::queue<int> write_queue;
      
      // Cache
      int cache_counter = 0;
      std::unordered_map<int, std::vector<Root>> cache;
      
      // File assignation
      std::unordered_map<std::wstring, int> files;
      
      // File hard/symlinking
      std::unordered_map<std::wstring, boost::filesystem::path> symlinks;
      
      // Memory management
    public:
      static Memory & get();
      
      void gc();
      
      void set_available_memory(std::size_t amount)
      {
        _mem_amount = amount;
      }
      unsigned long long int get_available_memory()
      {
        return _mem_amount;
      }
      
      // Cache management
    public:
      /*
       * Manages saving/loading manifests and AST trees from files.
       * Memory manager can cache such handles and return cached handle
       * when load is called without loading it from file system.
       */
      HVector load(boost::filesystem::path filename);
      void save(HVector handle, std::vector<boost::filesystem::path> filename);
    };
  }
}