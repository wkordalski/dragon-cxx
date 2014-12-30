#include "../src/lexer.hpp"

#include <sstream>

#include <gtest/gtest.h>
/*
TEST(TokenizerTest, SimpleSomething)
{
  std::wstring code =
  L"var x = 3\n"
  L"var z = x \n"
  L"var y = (x + z)\n"
  L"var s = cr\"str\\\"asd'qwe\\\'\'rty asd\n"
  L"var u = f(1+\n"
  L"          2+\n"
  L"          3)\n"
  L"var S = \\\n"
  L" '''\n"
  L" A nice docstring\n"
  L" or some other multiline string\n"
  L" '''\n";
  std::wistringstream ss(code);
  dragon::SourceReader sr(ss);
  dragon::Tokenizer tk(sr);
  std::vector<dragon::Token *> tok;
  dragon::Token *t;
  while(tk.get(t))
  {
    tok.push_back(t);
    t->print(std::wcout);
  }
  std::wcout << std::endl;
}
*/
