#include <iostream>
#include <sstream>

#include "../src/lexer.hpp"

using namespace std;

int main()
{
  wstring s = L"import asd # import modułu asd\n"
              L"var s = \"mój tekst z /*komentarzem*/\"\n"
              L"var t = 3 /# a /# b #/ c #/ /* a /* b */\n"
              L"\'\'\'\n"
              L"Multiline string\n"
              L"Line two\n"
              L"\'\'\'\n";
  wcout << s;
  wistringstream ss(s);
  dragon::SourceReader sr(ss);
  dragon::CommentRemover cr(sr);
  dragon::Character ch;
  while(cr.get(ch))
  {
    wcout << ch.chr;
  }
  return 0;
} 
