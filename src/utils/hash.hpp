#pragma once

#include <algorithm>

namespace dragon
{
  static inline std::size_t hash_combine(std::size_t seed, std::size_t hash)
  {
    return seed ^ (hash + 0x9e3779b9 + (seed<<6) + (seed>>2));
  }

  template<typename Hash, typename Arg>
  std::size_t hash_args(const Arg &arg)
  {
    return Hash()(arg);
  }

  template<typename Hash, typename... Hashes, typename Arg, typename... Args>
  std::size_t hash_args(const Arg &arg, Args... args)
  {
    return hash_combine(hash_args<Hashes...>(args...), Hash()(arg));
  }

  template<typename Hash, typename T>
  std::size_t hash_sequence(const T &arg)
  {
    return std::accumulate(arg.begin(), arg.end(), 7, hash_combine);
  }
}
