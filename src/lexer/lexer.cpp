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

  bool is_hex_digit(wchar_t c)
  {
    if(is_digit(c)) return true;
    if(c == L'a') return true;
    if(c == L'b') return true;
    if(c == L'c') return true;
    if(c == L'd') return true;
    if(c == L'e') return true;
    if(c == L'f') return true;
    if(c == L'A') return true;
    if(c == L'B') return true;
    if(c == L'C') return true;
    if(c == L'D') return true;
    if(c == L'E') return true;
    if(c == L'F') return true;
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
    if(nomore) return false;
    if(!this->_stream.get(chr))
    {
      chr = L'\n';
      nomore = true;
    }
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

  std::vector<Handle> tokenize(std::wistream &source)
  {
    std::vector<Handle> r;

    SourceReader sr(source);
    CommentRemover cr(sr);
    Tokenizer tk(cr);

    Node *t;
    while(tk.get(t))
    {
      r.push_back(Handle(t));
    }
    return r;
  }
}
