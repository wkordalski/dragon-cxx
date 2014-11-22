#pragma once

template<typename T>
class IGenerator
{
  std::vector<T> _forward;
public:
  bool empty()
  {
    return _forward.empty() and source_empty()
  };

  bool peek(Character &value, int position)
  {
    while(_forward.size() <= position)
    {
      Character curr;
      if(next(curr)) _forward.push_back(curr);
      else return false;
    }
    value = _forward[position];
    return true;
  }

  bool get(Character &value)
  {
    if(_forward.size() > 0)
    {
      value = _forward[0];
      _forward.pop_front();
      return true;
    }
    Character curr;
    if(next(curr)) curr;
    else return false;
    return true;
  }

protected:
  virtual bool _source_empty() = 0;
  virtual bool _next(T &value) = 0;
};
