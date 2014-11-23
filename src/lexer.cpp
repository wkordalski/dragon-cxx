#include "lexer.hpp" 

#include <cassert>

namespace dragon
{
  bool is_newline(wchar_t c)
  {
    return c == L'\n';
  }

  bool is_whitespace(wchar_t c)
  {
    // TODO
    return false;
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

  CommentRemover::CommentRemover(IGenerator<Character> &source)
    : _source(source)
  {
  }

  bool CommentRemover::_next(Character &value)
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
          if(!_wysiwyg) _escape = !_escape;
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
            if(_premline > 0)
            {
              value = curr;
              _premline--;
              return true;
            }
            _multiline--;
            if(_multiline == 0)
            {
              _instring = false;
              _wysiwyg = false;
            }
            value = curr;
            return true;
          }
          else
          {
            _instring = false;
            _wysiwyg = false;
            value = curr;
            return true;
          }
        }
        else if(is_newline(curr.chr) and _multiline == 0)
        {
          throw LexerError(curr,
            "String literal not closed before new line");
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
            Character n1;
            if(_source.peek(n1, 0) and n1.chr == L'#')
            {
              _source.get(n1);
              nestcmt++;
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
        if(curr.chr == L'\'')
        {
          Character n1, n2;
          if(_source.peek(n1, 0) and _source.peek(n2, 1)
            and n1.chr == L'\'' and n2.chr == L'\'')
          {
            // setup multiline literal
            _premline = 2;
            _multiline = 3;
            _instring = true;
            _delim = L'\'';
            value = curr;
            return true;
          }
          else
          {
            _multiline = 0;
            _instring = true;
            _delim = L'\'';
            value = curr;
            return true;
          }
        }
        if(curr.chr == L'\"')
        {
          Character n1, n2;
          if(_source.peek(n1, 0) and _source.peek(n2, 1)
            and n1.chr == L'\"' and n2.chr == L'\"')
          {
            // setup multiline literal
            _premline = 2;
            _multiline = 3;
            _instring = true;
            _delim = L'\"';
            value = curr;
            return true;
          }
          else
          {
            _multiline = 0;
            _instring = true;
            _delim = L'\"';
            value = curr;
            return true;
          }
        }
        if(curr.chr == L'r')
        {
          Character n1;
          if(_source.peek(n1, 0) and
            (n1.chr == L'\'' or n1.chr == L'\"'))
          {
            _wysiwyg = true;
          }
          value = curr;
          return true;
        }
        if(curr.chr == L'#')
        {
          linecmt = true;
          cc = curr.col;
          cr = curr.row;
          continue;
        }
        if(curr.chr == L'/')
        {
          Character n1;
          if(_source.peek(n1, 0))
          {
            if(n1.chr == L'*')
            {
              // Block comment
              _source.get(n1);
              blockcmt = true;
              cc = curr.col;
              cr = curr.row;
              continue;
            }
            if(n1.chr == L'#')
            {
              // Nested comment
              _source.get(n1);
              nestcmt = 1;
              cc = curr.col;
              cr = curr.row;
              continue;
            }
            // Not a comment
            value = curr;
            return true;
          }
          else
          {
            value = curr;
            return true;
          }
        }
        // else - normal letters
        value = curr;
        return true;
      }
    }
    if(_source.empty())
    {
      if(_instring) throw LexerError({L'\0',-1,-1},
        "String literal not closed before end of file");
      if(blockcmt) throw LexerError({L'\0',-1,-1},
        "Block comment not closed before end of file");
      if(nestcmt) throw LexerError({L'\0',-1,-1},
        "Nested comment not closed before end of file");
      return false;
    }
    assert(false);
  }

  bool CommentRemover::_source_empty()
  {
    return _source.empty();
  }
}