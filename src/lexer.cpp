#include "lexer.hpp"

#include <cassert>
#include <list>

namespace dragon
{
  bool is_newline(wchar_t c)
  {
    return c == L'\n';
  }

  bool is_whitespace(wchar_t c)
  {
    if(c == L' ') return true;
    if(c == L'\t') return true;
    return false;
  }

  bool is_digit(wchar_t c)
  {
    if(c == L'0') return true;
    if(c == L'1') return true;
    if(c == L'2') return true;
    if(c == L'3') return true;
    if(c == L'4') return true;
    if(c == L'5') return true;
    if(c == L'6') return true;
    if(c == L'7') return true;
    if(c == L'8') return true;
    if(c == L'9') return true;
    return false;
  }

  bool is_letter(wchar_t c)
  {
    // [TODO] Write it better
    if(c >= L'A' and c <= L'Z') return true;
    if(c >= L'a' and c <= L'z') return true;
    if(c == L'_') return true;
    return false;
  }

  int char_width(wchar_t c)
  {
    if(c == L' ') return 1;
    if(c == L'\t') return 2;  /* TAB WIDTH */
    return 0;
  }

  SourceReader::SourceReader(std::wistream &source)
    : _stream(source)
  {
  }

  bool SourceReader::_next(Character &value)
  {
    wchar_t chr;
    if(!this->_stream.get(chr)) return false;
    value = {chr, row, col};
    col++;
    if(is_newline(chr))
    {
      col = 0;
      row++;
    }
    return true;
  }

  bool SourceReader::_source_empty()
  {
    if(this->_stream) return false;
    return true;
  }
}
