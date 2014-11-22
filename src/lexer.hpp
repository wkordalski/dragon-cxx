#pragma once

#include "generator.hpp"
#include "token.hpp"

#include <istream>

struct Character
{
  wchar_t chr;
  int row;
  int col;

  Character() : this('\0', -1, -1) {}

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

  virtual const char * what() { return _msg.c_str(); }

  Character reason() { return _place; }
};

bool is_newline(wchar_t c)
{
  return c == L'\n';
}

bool is_whitespace(wchar_t c)
{
  // TODO
  return false;
}

class SourceReader : public IGenerator<Character>
{
  std::wistream &_stream;
  // indexing from 0
  int row = 0;
  int col = 0;

public:
  SourceReader(std::wistream &source);

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
  bool _escape = false;
  wchar_t _delim; // only ' and " supported!
  // Greater delimeters not supported!
  // (exception is multiline string,
  //  where the _delim must go three times)

public:
  CommentRemover(IGenerator<Character> &source);

protected:
  virtual bool _next(Character &value)
  {
    // Is line or block comment mode?
    bool linecmt = false;
    bool blockcmt = false;

    // How many nestable comments to close?
    int nestcmt = 0;

    // true if we need only '/' to close comment
    bool waitcmt = false;

    // Comment row, column - to know where to emit space.
    int cr, cc;

    Character curr;
    while(_source.get(curr))
    {
      if(_instring)
      {
        // TODO - no comments here!
        // But checking delimeters, escape sequences, multiline strings...
        if(curr.chr == L'\\')
        {
          _escape = !_escape;
          if(_multiline > 0) _multiline = 3;
          value = curr;
          return true;
        }
        else if(_escape)
        {
          value = curr;
          if(_multiline > 0) _multiline = 3;
          return true;
        }
        else if(curr.chr == _delim)
        {
          if(_multiline > 0)
          {
            _multiline--;
            if(_multiline == 0) _instring = false;
            value = curr;
            return true;
          }
          else
          {
            _instring = false;
            value = curr;
            return true;
          }
        }
        else if(is_newline(curr.chr) and _multiline == 0)
        {
          throw LexerError(curr,
            "String literal not closed before new line.");
        }
        else
        {
          value = curr;
          if(_multiline > 0) _multiline = 3;
          return true;
        }
      }
      else
      {
        // TODO - code here - so comments also
        // for comment return whitespace (space?) character
        // and string beginnings are here...
        if(nestcmt > 0)
        {
          // mamy komentarz zagnieżdżany ( /# ... #/ )
          // czekamy na #/
          // UWAGA: #/#
          if(curr.chr == L'/')
          {
            if(waitcmt)
            {
              nestcmt--;
              if(nestcmt == 0)
              {
                value = {L' ', cr, cc};
                blockcmt = linecmt = false;
                return true;
              }
              continue;
            }
            else continue;
          }
          else if(curr.chr == L'#')
          {
            waitcmt = true;
            continue;
          }
          else
          {
            waitcmt = false;
            continue;
          }
        }
        if(blockcmt)
        {
          if(curr.chr == L'/')
          {
            if(waitcmt)
            {
              value = {L' ', cr, cc};
              blockcmt = linecmt = false;
              return true;
            }
            else continue;
          }
          else if(curr.chr == L'*')
          {
            waitcmt = true;
            continue;
          }
          else
          {
            waitcmt = false;
            continue;
          }
        }
        if(linecmt)
        {
          if(is_newline(curr.chr))
          {
            linecmt = false;
            value = {L'\n', cr, cc};
            return true;
          }
          else continue;
        }
        // Normal code
        // think about beginnings of strings and comments
        // TODO
      }
    }
    if(_source.empty())
    {
      if(_instring) throw LexerError({L'\0',-1,-1},
        "String not closed before end of file");
      if(blockcmt) throw LexerError({L'\0',-1,-1},
        "Block comment not closed before end of file");
      if(nestcmt) throw LexerError({L'\0',-1,-1},
        "Nested comment not closed before end of file");
      return false;
    } 
  }

  virtual bool _source_empty();
};

class LineProcessor : public IGenerator<Character>
{
  // merges broken lines
};

class Tokenizer : public IGenerator<Token>
{
  // tokenizes code
}