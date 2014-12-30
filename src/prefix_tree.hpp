#pragma once

#include <map>

namespace dragon
{
  template<class T>
  class PrefixTree
  {
  protected:
    bool _ending;   // if here can the entry end
    std::map<T, PrefixTree<T>*> _follow;

  public:
    template<class _ForwardIterator>
    void insert(_ForwardIterator begin, _ForwardIterator end)
    {
      if(begin == end) _ending = true;
      else
      {
        T c = *begin;
        begin++;
        auto it = _follow.find(c);
        if(it == _follow.end())
        {
          auto pt = new PrefixTree<T>();
          pt->insert(begin, end);
          _follow[c] = pt;
        }
        else it->second->insert(begin, end);
      }
    }

    template<class _ForwardIterator>
    bool exists(_ForwardIterator begin, _ForwardIterator end) const
    {
      if(begin == end) return _ending;
      else
      {
        T c = *begin;
        begin++;
        auto it = _follow.find(c);
        if(it == _follow.end()) return false;
        else return it->second->exists(begin, end);
      }
    }

    template<class _ForwardIterator>
    bool prefix(_ForwardIterator begin, _ForwardIterator end) const
    {
      if(begin == end) return not _empty();
      else
      {
        T c = *begin;
        begin++;
        auto it = _follow.find(c);
        if(it == _follow.end()) return false;
        else return it->second->prefix(begin, end);
      }
    }

    template<class _ForwardIterator>
    void remove(_ForwardIterator begin, _ForwardIterator end)
    {
      if(begin == end) _ending = false;
      else
      {
        T c = *begin;
        begin++;
        auto it = _follow.find(c);
        if(it != _follow.end())
        {
          it->second->remove(begin, end);
          if(it->second->_empty())
          {
            delete it->second;
            _follow.erase(it);
          }
        }
      }
    }

  protected:
    bool _empty() const
    {
      if(_ending or _follow.size() > 0) return false;
      else return true;
    }
  };
}
