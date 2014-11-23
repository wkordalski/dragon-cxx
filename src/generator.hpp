#pragma once

#include <deque>

namespace dragon
{
  template<typename T>
  class IGenerator
  {
    std::deque<T> _forward;
  public:
    bool empty()
    {
      return _forward.empty() and _source_empty();
    };

    bool peek(T &value, int position)
    {
      while(_forward.size() <= position)
      {
        T curr;
        if(_next(curr)) _forward.push_back(curr);
        else return false;
      }
      value = _forward[position];
      return true;
    }

    bool get(T &value)
    {
      if(_forward.size() > 0)
      {
        value = _forward[0];
        _forward.pop_front();
        return true;
      }
      T curr;
      if(_next(curr)) value = curr;
      else return false;
      return true;
    }

  protected:
    virtual bool _source_empty() = 0;
    virtual bool _next(T &value) = 0;
  };
}