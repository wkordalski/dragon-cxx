#pragma once

#include "rules.hpp"

#include <map>
#include <string>
#include <vector>

namespace dragon
{
  class Parser
  {
    // TODO
    std::map<std::string, Rule *> grammar;
    Rule * program;

    void init_grammar();

  public:
    Parser()
    {
      init_grammar();
    }

    Handle parse(std::vector<Handle> tokens)
    {
      auto b = tokens.begin();
      auto e = tokens.end();
      auto h = program->match(b, e);
      if(b != e) return Handle();
      else return h;
    }
  };
}
