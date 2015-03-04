#include "../node.hpp"

#include "ast.hpp"
#include "builtins/integer.hpp"

#include "declarations/alias.hpp"
#include "source.hpp"

namespace dragon
{
  // adds some built-in external types, functions and variables
  void init_builtins(Handle assembly)
  {
    auto ass = assembly.as<Assembly>();

    //
    // INTEGRAL TYPES
    //
    auto itp = Handle::make<IntegralTemplate>(Handle::make<Identifier>(L"Int"));
    ass->add_declaration(itp);
    auto idt = Handle::make<Alias>(Handle::make<Identifier>(L"int"), itp.as<IntegralTemplate>()->get_default());
    ass->add_declaration(idt);
    //ass->add_declaration(itp.as<IntegralTemplate>()->get_default());
  }
}
