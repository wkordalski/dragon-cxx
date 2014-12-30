#include "lexer.hpp"

#include <cassert>
#include <list>

namespace dragon
{
  int eat_indentation(IGenerator<Character> &source, Character &first)
  {
    if(source.empty())
    {
      first = Character();
      return 0;
    }
    source.peek(first);
    Character n1;
    int counter = 0;
    while(source.peek(n1))
    {
      if(is_whitespace(n1.chr))
      {
        source.get(n1);
        counter += char_width(n1.chr);
      } else
      if(is_newline(n1.chr))
      {
        source.get(n1);
        counter = 0;
        if(not source.peek(first))
          first = Character();
      }
      else
      {
        break;
      }
    }
    return counter;
  }

  Tokenizer::Tokenizer(IGenerator<Character> &source)
    : _source(source)
  {
    _indent.push(0);

    auto add_operator = [this](std::wstring op) { _opers.insert(op.begin(), op.end()); };
    add_operator(L"=");
    add_operator(L"+");
    add_operator(L"-");
    add_operator(L"*");
    add_operator(L"/");
    add_operator(L"%");
    add_operator(L"+=");
    add_operator(L"-=");
    add_operator(L"*=");
    add_operator(L"/=");
    add_operator(L"%=");

    add_operator(L"(");
    add_operator(L"[");
    add_operator(L"{");
    add_operator(L"}");
    add_operator(L"]");
    add_operator(L")");
  }

  bool Tokenizer::_next(token &value)
  {
    // TODO
    //
    // Tokeny:
    //  indent/dedent - gdy newline = True
    //  literal - gdy zaczyna się cyfrą lub jest identyfikator ~ apostrof lub cudzysłów
    //  operator - gdy zaczyna się znakiem operatora
    //  identifier - inaczej
  start:
    if(_newline)
    {
      // Measure current indentation
      // If newline encountred, start measurement once again
      _newline = false;
      Character n1;
      int counter = eat_indentation(_source, n1);
      // generate indentation notes if needed
      int &nc = _indent.top();
      if(counter > nc)
      {
        _indent.push(counter);
        auto ind = new Indent();
        ind->place = { n1.row, n1.col, 0 };
        value = ind;
        return true;
      } else
      if(counter < nc)
      {
        auto ded = new Dedent();
        ded->place = { n1.row, n1.col, 0 };
        value = ded;
        _indent.pop();
        while(counter < _indent.top())
        {
          _indent.pop();
          auto ddd = new Dedent();
          ddd->place = { n1.row, n1.col, 0 };
          _queue(ddd);
        }
        if(counter > _indent.top()) throw LexerError(n1, "Wrong indentation is here");
        return true;
      }
      // else do sth else
    }

    Character cc;
    if(not _source.peek(cc, 0))
    {
      if(not _paren.empty()) throw LexerError(Character(), "Unclosed brackets at the end of file");
      if(_indent.empty() or _indent.top() == 0)
      {
        return false;
      }
      else
      {
        value = new Dedent();
        _indent.pop();
        while((!_indent.empty()) and _indent.top() > 0)
        {
          _indent.pop();
          _queue(new Dedent());
        }
        return true;
      }
    }
    wchar_t &c = cc.chr;

    if(c == L'\\')
    {
      _source.get(cc);
      // skip whitespaces to newline
      Character n1;
      while(_source.get(n1))
      {
        if(is_whitespace(n1.chr))
        {} else
        if(is_newline(n1.chr))
        {
          break;
        }
        else throw LexerError(Character(), "No newline after merge-line character");
      }
      if(_source.empty()) throw LexerError(Character(), "End-of-line encountred in a wrong place");
      Character nli;
      int counter = eat_indentation(_source, nli);
      if(counter <= _indent.top()) throw LexerError(nli, "Wrong indentation is here");
      // read some next token
      goto start;
    }
    if(is_letter(c) or c == '\'' or c == '\"')
    {
      std::vector<Character> tok;
      Character n1, n2;
      bool liter = false;
      bool wysiwyg = false;
      wchar_t delim = L'\0';
      bool multiline = false;
      bool escape = false;
      int premline = 0;      // premultiline characters
      if(is_letter(c)) goto read_id;
      if(c == '\'' or c == '\"') goto read_li;
    read_id:
      // TODO: read id
      while(_source.peek(n1))
      {
        if(is_letter(n1.chr) or is_digit(n1.chr))
        {
          _source.get(n1);
          tok.push_back(n1);
        } else
        if(n1.chr == '\'' or n1.chr == '\"')
          goto read_li;
        else
          goto read_dy;
      }
      goto read_dy;

    read_li:
      // TODO: read string literal
      liter = true;
      wysiwyg = (tok.size()>0 and tok[tok.size()-1].chr == L'r');
      escape = false;
      _source.get(n1);
      tok.push_back(n1);
      delim = n1.chr;
      if(_source.empty()) throw LexerError(Character(), "Unclosed string literal");
      _source.peek(n1);
      if(n1.chr == delim and _source.peek(n2, 1) and n2.chr == delim)
      {
        multiline = true;
        premline = 2; // two characters to skip
      }
      while(_source.peek(n1))
      {
        if(premline > 0)
        {
          _source.get(n1);
          tok.push_back(n1);
          premline--;
          continue;
        }
        if(escape)
        {
          _source.get(n1);
          tok.push_back(n1);
          escape = false;
          continue;
        }
        if(n1.chr == L'\\')
        {
          _source.get(n1);
          tok.push_back(n1);
          if(not wysiwyg) escape = true;
          continue;
        }
        if(n1.chr == delim)
        {
          if(multiline)
          {
            if(_source.peek(n1, 1) and _source.peek(n2, 2) and n1.chr == delim and n2.chr == delim)
            {
              _source.get(n1);
              tok.push_back(n1);
              _source.get(n1);
              tok.push_back(n1);
              _source.get(n1);
              tok.push_back(n1);
              _source.peek(n1);
              if(is_letter(n1.chr) or is_digit(n1.chr)) goto read_id;
              if(n1.chr == L'\'' or n1.chr == L'\"') goto read_li;
              goto read_dy;
            }
            else
            {
              _source.get(n1);
              tok.push_back(n1);
              continue;
            }
          }
          else
          {
            _source.get(n1);
            tok.push_back(n1);
            _source.peek(n1);
            if(is_letter(n1.chr) or is_digit(n1.chr)) goto read_id;
            if(n1.chr == L'\'' or n1.chr == L'\"') goto read_li;
            goto read_dy;
          }
        }
        if(not multiline and is_newline(n1.chr))
        {
          assert(false && "Comment remover should handle this invalid string literals");
        }
        _source.get(n1);
        tok.push_back(n1);
        continue;
      }
      throw LexerError(Character(), "Unclosed literal before end-of-file");

    read_dy:
      if(liter)
      {
        auto li = new Literal();
        li->text = L"";
        li->text.reserve(tok.size());
        for(Character n2 : tok)
          li->text += n2.chr;
        li->place = { tok[0].row, tok[0].col, li->text.size() };
        value = li;
      }
      else
      {
        auto id = new Identifier();
        id->text = L"";
        id->text.reserve(tok.size());
        for(Character n2 : tok)
          id->text += n2.chr;
        id->place = {tok[0].row, tok[0].col, id->text.size() };
        value = id;
      }
      return true;
    }
    if(is_digit(c))
    {
      // return literal (number)
      // eat while it is digit, letter or [.'"]
      std::vector<Character> tok;
      Character n1;
      while(_source.peek(n1))
      {
        if(is_letter(n1.chr) or is_digit(n1.chr)
        || n1.chr == L'.' or n1.chr == L'\'' or n1.chr == L'\"')
        {
          tok.push_back(n1);
          _source.get(n1);
        }
        else break;
      }
      auto li = new Literal();
      li->text = L"";
      li->text.reserve(tok.size());
      for(Character n2 : tok)
        li->text += n2.chr;
      li->place = { tok[0].row, tok[0].col, li->text.size() };
      value = li;
      return true;
    }
    if(is_operator(c))
    {
      // return matching operator
      Character tok = cc;
      std::vector<wchar_t> opr;
      Character n1;
      while(_source.peek(n1, opr.size()))
      {
        opr.push_back(n1.chr);
        if(_opers.prefix(opr.begin(), opr.end()))
          continue;
        else
          break;
      }
      opr.pop_back();
      while(opr.size() > 0)
      {
        if(_opers.exists(opr.begin(), opr.end()))
        {
          auto op = new Operator();
          op->text = std::wstring(opr.begin(), opr.end());
          op->place = { tok.row, tok.col, opr.size() };
          value = op;
          for(int i = 0; i < opr.size(); i++) _source.get(n1);
          if(op->text == L"(") _paren.push(L")"); else
          if(op->text == L"[") _paren.push(L"]"); else
          if(op->text == L"{") _paren.push(L"}"); else
          if(op->text == L")" or op->text == L"]" or op->text == L"}")
          {
            if(_paren.empty()) throw LexerError(tok, "Unmatched brace is here");
            if(op->text == _paren.top()) _paren.pop();
            else throw LexerError(tok, "Unmatched brace is here");
          }
          return true;
        }
        else
        {
          opr.pop_back();
        }
      }
      throw LexerError(cc, "Unknown operator");
    }
    if(is_newline(c))
    {
      if(_paren.empty())
      {
        _newline = true;
        auto nlt = new Newline();
        nlt->place = { cc.row, cc.col, 1 };
        _source.get(cc);
        value = nlt;
        return true;
      }
      else
      {
        _source.get(cc);
        Character nli;
        int counter = eat_indentation(_source, nli);
        if(_source.empty()) throw LexerError(Character(), "Unclosed brackets and the end-of-file");
        if(counter <= _indent.top()) throw LexerError(nli, "Wrong indentation is here");
        goto start;
      }
    }
    if(is_whitespace(c))
    {
      // ommit them and go to the begenning
      Character n1;
      while(_source.peek(n1))
      {
        if(is_whitespace(n1.chr))
          _source.get(n1);
        else
          break;
      }
      goto start;
    }
    throw LexerError(cc, "Unknown character type");
  }

  bool Tokenizer::_source_empty()
  {
    return _source.empty();
  }

  bool Tokenizer::is_operator(wchar_t c)
  {
    std::list<wchar_t> L = {c};
    return _opers.prefix(L.begin(), L.end());
  }
}
