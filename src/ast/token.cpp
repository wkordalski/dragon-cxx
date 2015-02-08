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
      int rsi = 0;        // radix support index
      bool rss = false;   // radix support switch
      auto pfx = std::find_if_not(str, end,
      [&rsi, &rss](wchar_t c)
      {
        if(c == L'0' and rsi == 0) rss = true;
        else if(rss and rsi == 1 and (c == L'x' or c == L'o' or c == L'b')) return true;
        rsi++;
        return is_hex_digit(c) or c == L'.' or c == L'\'';
      }
      );
      std::wstring lit = L"", opr = L"";
      std::copy_if(str, pfx, std::back_inserter(lit), [](wchar_t c){ return c != L'\'' and c != L'\"'; } );
      std::copy(pfx, end, std::back_inserter(opr));
      if(std::find(str, pfx, L'.') == pfx)
      {
        if(pfx == end)
        {
          // Default width of integer - I should also add type too (TODO)
          replace(new IntegralValue(lit, 64, true));
        }
        else
        {
          // Postfix operator decides how wide should be the integer
          // I should also add type too (for PostfixLiteralOperator) (TODO)
          auto hi = Handle::make<IntegralValue>(lit);
          auto pi = Handle::make<Identifier>(opr);
          replace(new PostfixLiteralOperator(hi, pi));
        }
      }
      else
      {
        // Decimal literal todo
      }
    }
    else
    {
      // It is string literal / character literal
    }
  }
}
