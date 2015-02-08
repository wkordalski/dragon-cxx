#include "value.hpp"

namespace dragon
{
  char digit_utf_to_ascii(wchar_t c)
  {
    if(c == L'0') return '0';
    if(c == L'1') return '1';
    if(c == L'2') return '2';
    if(c == L'3') return '3';
    if(c == L'4') return '4';
    if(c == L'5') return '5';
    if(c == L'6') return '6';
    if(c == L'7') return '7';
    if(c == L'8') return '8';
    if(c == L'9') return '9';
    if(c == L'a') return 'a';
    if(c == L'b') return 'b';
    if(c == L'c') return 'c';
    if(c == L'd') return 'd';
    if(c == L'e') return 'e';
    if(c == L'f') return 'f';
    if(c == L'A') return 'a';
    if(c == L'B') return 'b';
    if(c == L'C') return 'c';
    if(c == L'D') return 'd';
    if(c == L'E') return 'e';
    if(c == L'F') return 'f';
    assert(false and "Wrong character");
  }

  IntegralValue::IntegralValue(std::wstring s)
  {
    assert(s.size() > 0);
    wchar_t base = '\0';
    if(s.size() > 2 and s[0] == L'0' and !is_digit(s[1]))
    {
      base = s[1];
      s.erase(0, 2);
    }
    std::string t = "";
    std::transform(s.begin(), s.end(), std::back_inserter(t), digit_utf_to_ascii);
    unsigned char radix = 10;
    switch(base)
    {
      case L'\0': radix = 10; break;
      case L'x':  radix = 16; break;
      case L'b':  radix = 2;  break;
      case L'o':  radix = 8;  break;
      default: assert(false && "Compilation error - unsupported radix character");
    }
    auto srt = llvm::StringRef(t);
    int width = llvm::APInt::getBitsNeeded(srt, radix);
    if(width & 7) width = (width & ~7) + 8;
    if(width > 64 and width & 63) width = (width & ~63) + 64;
    value = llvm::APSInt(llvm::APInt(width, srt, radix), false);
  }

  IntegralValue::IntegralValue(std::wstring s, int width, bool sign)
  {
    assert(s.size() > 0);
    wchar_t base = '\0';
    if(s.size() > 2 and s[0] == L'0' and !is_digit(s[1]))
    {
      base = s[1];
      s.erase(0, 2);
    }
    std::string t = "";
    std::transform(s.begin(), s.end(), std::back_inserter(t), digit_utf_to_ascii);
    unsigned char radix = 10;
    switch(base)
    {
      case L'\0': radix = 10; break;
      case L'x':  radix = 16; break;
      case L'b':  radix = 2;  break;
      case L'o':  radix = 8;  break;
      default: assert(false && "Compilation error - unsupported radix character");
    }
    auto srt = llvm::StringRef(t);
    value = llvm::APSInt(llvm::APInt(width, srt, radix), !sign);
  }

}
