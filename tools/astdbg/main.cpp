#include <iostream>
#include <fstream>

#include "../../src/lexer/scanner.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
  std::vector<dragon::Handle> roots;
  for(int i = 1; i < argc; i++)
  {
    std::wifstream input(argv[i]);
    dragon::Handle rt;
    dragon::Scanner sc(input);
    dragon::Parser pr(sc, rt);
    pr.parse();
    roots.push_back(rt);
  }
  int cmd;
  std::wcout << ">> ";
  while(std::cin >> cmd)
  {
    if(cmd == 0)
    {
      for(dragon::Handle h : roots)
      {
        h->print(std::wcout);
      }
    }
    else
    {
      dragon::Handle(cmd)->print(std::wcout);
    }
    std::wcout << ">> ";
  }
  std::wcout << "\b\b\b\b    \b\b\b\b";
  return 0;
}
