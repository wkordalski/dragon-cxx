#include "containers.hpp"

namespace dragon {
  MVector to_member(LVector v)
  {
    MVector ret;
    ret.reserve(v.size());
    std::transform(v.begin(), v.end(), std::back_inserter(ret), [](StackRoot &r){return Handle(r);});
    return ret;
  }
}