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

    RKeyword(std::wstring t, std::function<void(Token*)> f = [](Token*){}) : text(t), converter(f) {}

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

    RIdentifier(std::function<void(Token*)> f = [](Token*){}) : converter(f) {}

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

    ROperator(std::wstring t, std::function<void(Token*)> f = [](Token*){}) : text(t), converter(f) {}

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

  class RLiteral : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    RLiteral(std::function<void(Token*)> f = [](Token*){}) : converter(f) {}

    virtual Handle match(iterator &current, iterator end)
    {
      if(current == end) return Handle();
      Handle cc = *current;
      Token *t = cc.get().get();
      auto *tt = dynamic_cast<Literal *>(t);
      if(tt == nullptr) return Handle();
      // [TODO] Do we need to exclude all keywords here?
      // Then we can use PrefixTree there.
      tt->converter = converter;
      current++;
      return cc;
    }
  };

  class RNewline : public Rule
  {
  public:
    std::function<void(Token*)> converter = [](Token *t){};

    RNewline(std::function<void(Token*)> f = [](Token*){}) : converter(f) {}

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

    RIndent(std::function<void(Token*)> f = [](Token*){}) : converter(f) {}

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

    RDedent(std::function<void(Token*)> f = [](Token*){}) : converter(f) {}

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


  // USE THIS ONLY WITHIN A SEQUENCE
  // AND WITH YOUR BRAIN USAGE
  // IT IS SO UNSTABLE
  class ROptional : public Rule
  {
  public:
    Rule * rule;

    std::function<void(Token*)> converter = [](Token *t){};

    ROptional(Rule *r, std::function<void(Token*)> f = [](Token*){}) : rule(r), converter(f) {}

    virtual Handle match(iterator &current, iterator end)
    {
      Handle t = rule->match(current, end);
      if(not t.valid()) return Handle();
      auto *r = new Optional();
      r->token = t;
      r->converter = converter;
      return Handle(r);
    }
  };

  class RSequence : public Rule
  {
  public:
    std::vector<Rule *> rules;
    std::function<void(Token*)> converter = [](Token *t){};

    RSequence(std::vector<Rule *> r, std::function<void(Token*)> f = [](Token*){}) : rules(r), converter(f) {}

    virtual Handle match(iterator &current, iterator end)
    {
      return partial_match({}, rules.begin(), current, end);
    }

    Handle partial_match(std::vector<Handle> prefix, std::vector<Rule*>::iterator rule, iterator &current, iterator end)
    {
      iterator it = current;
      std::vector<Handle> &tok = prefix;
      for(; rule != rules.end(); rule++)
      {
        // we must match them all
        auto &m = *rule;
        auto ot = dynamic_cast<ROptional*>(m);
        if(ot == nullptr)
        {
          Handle t = m->match(it, end);
          if(not t.valid()) return Handle();
          tok.push_back(t);
        }
        else
        {
          iterator bakap = it;
          Handle t = ot->match(it, end);
          iterator cakap = it;
          if(not t.valid()) continue; // we can simply miss it :D
          // now we should branch
          rule++;
          Handle a = partial_match(tok, rule, bakap, end);
          tok.push_back(t);
          Handle b = partial_match(tok, rule, cakap, end);
          if(a.valid() and b.valid()) return Handle();
          if(a.valid())
          {
            current = bakap;
            return a;
          }
          if(b.valid())
          {
            current = cakap;
            return b;
          }
          return Handle();
        }
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

    RAlternative(std::vector<Rule*> r, std::function<void(Token*)> f = [](Token*){}) : rules(r), converter(f) {}

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

    RRepeat(Rule *r, std::function<void(Token*)> f = [](Token*){}) : rule(r), converter(f) {}

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
