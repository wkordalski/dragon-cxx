#include "parser.hpp"



namespace dragon
{
  namespace impl
  {
    RKeyword * kwd(std::wstring t, std::function<void(Token*)> f = [](Token*){})
    {
      return new RKeyword(t, f);
    }

    RIdentifier * id(std::function<void(Token*)> f = [](Token*){})
    {
      return new RIdentifier(f);
    }

    ROperator * op(std::wstring t, std::function<void(Token*)> f = [](Token*){})
    {
      return new ROperator(t, f);
    }

    RLiteral * lit(std::function<void(Token*)> f = [](Token*){})
    {
      return new RLiteral(f);
    }

    RAlternative * alt(std::vector<Rule*> r = {}, std::function<void(Token*)> f = [](Token*){})
    {
      return new RAlternative(r, f);
    }

    RRepeat * rep(Rule *r, std::function<void(Token*)> f = [](Token*){})
    {
      return new RRepeat(r, f);
    }

    RSequence * seq(std::vector<Rule*> r, std::function<void(Token*)> f = [](Token*){})
    {
      return new RSequence(r, f);
    }

    ROptional * opt(Rule * r, std::function<void(Token*)> f = [](Token*){})
    {
      return new ROptional(r, f);
    }

    RIndent * ind(std::function<void(Token*)> f = [](Token*){})
    {
      return new RIndent(f);
    }

    RDedent * ded(std::function<void(Token*)> f = [](Token*){})
    {
      return new RDedent(f);
    }

    RNewline * nl(std::function<void(Token*)> f = [](Token*){})
    {
      return new RNewline(f);
    }

    // block token for indented blocks
    RSequence * block(Rule *r, std::function<void(Token*)> f = [](Token*){})
    {
      static std::map<Rule *, RSequence *> memo;
      if(memo.count(r) == 0) memo[r] = seq({ind(),rep(r),ded()});
      return memo[r];
    }

    // Binary operator
    RSequence * binop(Rule *r, Rule *s, std::function<void(Token*)> f = [](Token*){})
    {
      return seq({rep(seq({r,s})), r});
    }
  }

  void Parser::init_grammar()
  {
    using namespace impl;

    auto declaration = alt();
    auto program = rep(declaration);
    auto docstring = lit();

    // ATRIBUTTES
    {
      // TODO
    }
    // TEMPLATES
    {
      // TODO
    }
    // DECLARATIONS
    {
      // IMPORT
      auto import_smt = seq({ kwd(L"import"), binop(id(), op(L".")), nl() });
      declaration->add(import_smt);
      // ALIAS
      auto alias_smt = seq({ kwd(L"use"), id(), op(L"="), binop(id(), op(L".")), nl() });
      declaration->add(alias_smt);
      // USE
      auto use_smt = seq({ kwd(L"use"), binop(id(), op(L".")), nl() });
      declaration->add(use_smt);
      // NAMESPACE
      auto namespace_smt = seq({ kwd(L"namespace"), binop(id(), op(L".")), nl(), block(declaration) });
      declaration->add(namespace_smt);
      // VARIABLE
      auto typespec = seq({ op(L":")/*, type*/ });
      auto initspec = seq({ op(L"=")/*, value */ });
      auto variable_smt = seq({ kwd(L"var"), id(), opt(typespec), opt(initspec), alt({nl(), seq({nl(),ind(),docstring,nl(),ded()})}) });
      declaration->add(variable_smt);
      // TODO
      // FUNCTION
      // TODO
      // PROPERTIES
      // TODO
      // ENUM
      // TODO
      // CLASS
      // TODO
      // INTERFACE
      // TODO
      // CONCEPT
      // TODO
    }

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
