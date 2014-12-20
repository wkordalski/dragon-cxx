#pragma once

#include "generator.hpp"
#include "token.hpp"

#include <istream>

namespace dragon
{
  struct Character
  {
    wchar_t chr;
    int row;
    int col;
    int data; // For indentation

    Character() : Character('\0', -1, -1, 0) {}

    Character(wchar_t chr, int row, int col)
      : chr(chr), row(row), col(col), data(-1)
    {
    }

    Character(int indentation, int row, int col)
      : chr('\0'), row(row), col(col), data(indentation)
    {
    }
  };

  class LexerError : public std::exception
  {
    Character _place;
    std::string _msg;

  public:
    LexerError(Character c, std::string msg)
    {
      _place = c;
      _msg = msg;
    }

    virtual const char * what() const noexcept(true) { return _msg.c_str(); }

    Character reason() { return _place; }
  };

  bool is_newline(wchar_t c);

  bool is_whitespace(wchar_t c);

  int calculate_indent(wchar_t c)
  {
    if(c == L' ') return 1;
    if(c == L'\t') return 2;
  }

  int calculate_indent(std::wstring &s)
  {
    int R = 0;
    for(auto c : s)
      R += calculate_indent(c);
    return R;
  }

  class SourceReader : public IGenerator<Character>
  {
    std::wistream &_stream;
    // indexing from 0
    int row = 0;
    int col = 0;

  public:
    SourceReader(std::wistream &source);

    // returns characters with specified column/row

  protected:
    virtual bool _next(Character &value);
    virtual bool _source_empty();
  };

  class CommentRemover : public IGenerator<Character>
  {
    IGenerator<Character> &_source;

    // GLOBAL STATE: in-string or not
    // In-string literal support
    bool _instring = false;
    int _multiline = 0;
    int _premline = 0;
    bool _escape = false;
    bool _wysiwyg = false;
    wchar_t _delim; // only ' and " supported!
    // Greater delimeters not supported!
    // (exception is multiline string,
    //  where the _delim must go three times)

  public:
    CommentRemover(IGenerator<Character> &source);

    // removes comments from source code
    // one-line strings must be closed before end-of-line

  protected:
    virtual bool _next(Character &value);

    virtual bool _source_empty();
  };

  class Tokenizer : public IGenerator<Token>
  {
    IGenerator<Character> &_source;

    // GLOBAL STATE: in-string or not
    // In-string literal support
    bool _instring = false;
    int _multiline = 0;
    int _premline = 0;
    bool _escape = false;
    bool _wysiwyg = false;
    wchar_t _delim; // only ' and " supported!

    std::stack<wchar_t> _closing;
    int _indent = 0;
    // if true, whitespaces increment this
    bool _newline = true;
    bool _mergeline = false;

  public:
    Tokenizer(IGenerator<Character> &source);

  protected:
    virtual bool _next(Token &value);
    virtual bool _source_empty();
  };
}
