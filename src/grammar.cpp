#include "parser.hpp"

namespace dragon
{
  void Parser::init_grammar()
  {
    auto kwd = [](std::wstring t){ return new RKeyword(t);};
    auto id = [](){return new RIdentifier();};
    auto op = [](std::wstring t){ return new ROperator(t);};
    auto alt = [](std::vector<Rule*> r = {}){return new RAlternative(r);};
    auto rep = [](Rule *r) { return new RRepeat(r);};
    auto seq = [](std::vector<Rule*> r){ return new RSequence(r);};

    auto ind = [](){ return new RIndent(); };
    auto ded = [](){ return new RDedent(); };
    auto nl = [](){ return new RNewline(); };

    auto declaration = alt();
    auto program = rep(declaration);

    auto block = [&](Rule *r)
    {
      static std::map<Rule *, Rule *> memo;
      if(memo.count(r) == 0) memo[r] = seq({ind(),rep(r),ded()});
      return memo[r];
    };

    auto binop = [&](Rule *r, Rule *s)
    {
      return seq({rep(seq({r,s})), r});
    };

    auto nm_smt = seq({ kwd(L"namespace"), binop(id(), op(L".")), nl(), block(declaration)});
    declaration->add(nm_smt);
    declaration->add( seq({id(), nl()}) );
    auto imp_smt = seq({ kwd(L"import"), binop(id(), op(L".")), nl() });
    declaration->add(imp_smt);
    // DECLARATIONS
    //auto module_name = seq( { rep( seq({id(), op(L".")}) ), id() } );
    //auto import_smt = seq({ kwd(L"import"), module_name});
    //declaration->add(import_smt);

    //auto func_smt = seq({new RKeyword(L"def"), new RIdentifier(), new ROperator(L"("), new ROperator(L")" /* TODO */})

    this->program = program;
    grammar["program"] = program;
    grammar["declaration"] = declaration;
  }
}

/*
Match * init_rules()
{
  auto declaration = new RAlternative();
  auto program = new RRepeat(declaration);
  auto module_name = new RRepeat(new RIdentifier(), new ROperator(L"."));
  auto import_smt = new RSequence(new RKeyword(L"import"), module_name);
  declaration->add(import_smt);
  return program;
}
*/
