#pragma once

#include "../utils/generator.hpp"
#include "../utils/prefix_tree.hpp"
#include "../token.hpp"

#include <istream>
#include <stack>

namespace dragon
{
  struct Character
  {
    wchar_t chr;
    unsigned long row;
    unsigned long col;

    Character() : Character(L'\0', -1, -1) {}

    Character(wchar_t chr, int row, int col)
      : chr(chr), row(row), col(col)
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

  bool is_letter(wchar_t c);

  bool is_digit(wchar_t c);
  bool is_hex_digit(wchar_t c);

  int char_width(wchar_t c);

  class SourceReader : public IGenerator<Character>
  {
    std::wistream &_stream;
    // indexing from 0
    int row = 0;
    int col = 0;
    bool nomore = false;

  public:
    SourceReader(std::wistream &source);

    // returns characters with specified column/row
    // adds a newline at the end-of-file
    // it must be there for grammar simplification

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

  class Tokenizer : public IGenerator<Token*>
  {
    typedef Token * token;
    IGenerator<Character> &_source;
    PrefixTree<wchar_t> _opers;

    std::stack<std::wstring> _paren;  // closing braces
    std::stack<int> _indent;          // indentation blocks
    bool _newline = true;             // new line encountred - count indentation

  public:
    Tokenizer(IGenerator<Character> &source);

  protected:
    virtual bool _next(token &value);
    virtual bool _source_empty();

    bool is_operator(wchar_t c);
  };

  std::vector<Handle> tokenize(std::wistream &source);
}
