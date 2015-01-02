#pragma once

#include "token.hpp"

#include <cassert>

namespace dragon
{
  class Rule
  {
  public:
    typedef std::vector<Handle>::iterator iterator;

    virtual ~Rule() {}

    // tries to match tokens beginning from current
    // end points to the end of token vector
    // if match was successful, returns token and moves current
    // otherwise returns null and do not change current iterator
    virtual Handle match(iterator &current, iterator end) = 0;
  };

  class RKeyword : public Rule
  {
  public:
    std::wstring text;
    std::function<void(Token*)> converter = [](Token *t){};

    RKeyword(std::wstring t) : text(t) {}

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Identifier *>(t);
      if(tt == nullptr) return Handle();
      if(tt->text != text) return Handle();
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RIdentifier : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Identifier *>(t);
      if(tt == nullptr) return Handle();
      // [TODO] Do we need to exclude all keywords here?
      // Then we can use PrefixTree there.
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class ROperator : public Rule
  {
  public:
    std::wstring text;
    std::function<void(Token*)> converter = [](Token *t){};

    ROperator(std::wstring t) : text(t) {}

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Operator *>(t);
      if(tt == nullptr) return Handle();
      if(tt->text != text) return Handle();
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RNewline : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Newline *>(t);
      if(tt == nullptr) return Handle();
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RIndent : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Indent *>(t);
      if(tt == nullptr) return Handle();
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RDedent : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Dedent *>(t);
      if(tt == nullptr) return Handle();
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RSequence : public Rule
  {
  public:
    std::vector<Rule *> rules;
    std::function<void(Token*)> converter = [](Token *t){};

    RSequence(std::vector<Rule *> r) : rules(r) {}

    virtual Handle match(iterator &current, iterator end)
    {
      iterator it = current;
      std::vector<Handle> tok;
      for(auto &m : rules)
      {
        // we must match them all
        Handle t = m->match(it, end);
        if(not t.valid()) return Handle();
        tok.push_back(t);
      }
      auto *r = new Sequence();
      r->tokens = tok;
      r->converter = converter;
      current = it;
      return Handle(r);
    }
  };

  class RAlternative : public Rule
  {
  public:
    std::vector<Rule *> rules;
    std::function<void(Token*)> converter = [](Token *t){};

    RAlternative(std::vector<Rule*> r) : rules(r) {}

    void add(Rule *r)
    {
      rules.push_back(r);
    }

    virtual Handle match(iterator &current, iterator end)
    {
      Handle tok;
      iterator ft = current;
      for(auto &m : rules)
      {
        // we must match one of them
        iterator it = current;
        Handle t = m->match(it, end);
        if(not t.valid()) continue;
        if(tok.valid()) return Handle();
        tok = t;
        ft = it;
      }
      if(not tok.valid()) return Handle();
      auto *r = new Alternative();
      r->token = tok;
      r->converter = converter;
      current = ft;
      return Handle(r);
    }
  };

  class RRepeat : public Rule
  {
  public:
    Rule * rule;
    int min = 0;
    int max = -1;    // max = -1 => infinity

    std::function<void(Token*)> converter = [](Token *t){};

    RRepeat(Rule *r) : rule(r) {}

    virtual Handle match(iterator &current, iterator end)
    {
      iterator it = current;
      std::vector<Handle> tok;
      for(int i = 0; max == -1 or i < max; i++)
      {
        // we must match some first of them
        Handle t = rule->match(it, end);
        if(not t.valid()) break;
        tok.push_back(t);
      }
      if(tok.size() < min) return Handle();
      auto *r = new Repeat();
      r->tokens = tok;
      r->converter = converter;
      current = it;
      return Handle(r);
    }
  };
}
