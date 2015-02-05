#include "../src/lexer/lexer.hpp"

#include <sstream>

#include <gtest/gtest.h>
/*
TEST(TokenizerTest, SimpleSomething)
{
  std::wstring code =
  L"var x = 3 # wartosc tutaj\n"
  L"var z = x \n"
  L"var y = /# asd /# bvc #/ qwe #/ (x + z)\n"
  L"var s = cr\"str\\\"asd'qwe\\\'\'rty asd\n"
  L"var u = / * troche kodu * / f(1+\n"
  L"          2+\n"
  L"          3)\n"
  L"var S = a/ *b* /c\\\n"
  L" '''\n"
  L" A nice docstring\n"
  L" or some other multiline string\n"
  L" '''\n"
  L"\n"
  L"\n";
  std::wistringstream ss(code);
  dragon::SourceReader sr(ss);
  dragon::CommentRemover cr(sr);
  dragon::Tokenizer tk(cr);
  std::vector<dragon::Token *> tok;
  dragon::Token *t;
  //dragon::Character c;
  while(tk.get(t))
  {
    tok.push_back(t);
    t->print(std::wcout);
    //std::wcout << c.chr;
  }
  std::wcout << std::endl;
}
*/
