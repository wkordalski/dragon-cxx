#include "../token.hpp"
#include "parsed.hpp"
#include "value.hpp"

namespace dragon
{
  bool is_digit(wchar_t c);
  bool is_hex_digit(wchar_t c);

  void Literal::parse_literal()
  {
    assert(text.size() > 0 && "Empty literal token is forbidden");
    if(is_digit(text[0]))
    {
      // It is digit
      // it has raw unsigned integral literal
      // and postfix
      // postfix starts with _ or letter w/o A-F and a-f
      auto str = text.begin();
      auto end = text.end();
      auto pfx = std::find_if_not(str, end, [](wchar_t c){ return is_hex_digit(c) or c == L'.' or c == L'\'';});
      std::wstring lit = L"", opr = L"";
      std::copy_if(str, pfx, std::back_inserter(lit), [](wchar_t c){ return c != L'\''; } );
      std::copy(pfx, end, std::back_inserter(opr));
      if(pfx == end)
      {
        replace(new IntegralValue(lit));
      }
      else
      {
        auto hi = Handle::make<IntegralValue>(lit);
        auto pi = Handle::make<Identifier>(opr);
        replace(new PostfixLiteralOperator(hi, pi));
      }
    }
    else
    {
      // It is string literal / character literal
    }
  }
}
