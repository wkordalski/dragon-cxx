#include <iomanip>
#include <iostream>
#include <fstream>

#include "../../src/lexer/scanner.hpp"
#include "parser.hpp"

#include "../../src/ast/value.hpp"

int main(int argc, char *argv[])
{
  std::vector<dragon::Handle> fileroots;
  std::vector<char*> files;
  for(int i = 1; i < argc; i++)
  {
    std::wifstream input(argv[i]);
    dragon::Handle rt;
    dragon::FileScanner sc(input, argv[i]);
    dragon::Parser pr(sc, rt);
    std::wcout << "Parsing:   " << argv[i] << std::endl;
    if(pr.parse() == 0)
    {
      fileroots.push_back(rt);
      files.push_back(argv[i]);
    }
  }
  dragon::Handle ass;
  if(!fileroots.empty())
  {
    std::wcout << "Linking..." << std::endl;
    ass = dragon::files_to_assembly(fileroots);
  }
  if(ass or false /* check if we turned on/off desymbolization */)
  {
    dragon::desymbolize_expressions(ass);
  }
  int cmd;
  std::wcout << ">> ";
  while(std::cin >> cmd)
  {
    if(cmd == 0)
    {
      if(fileroots.empty())
      {
        std::wcout << "No file roots" << std::endl;
      }
      else
      {
        std::wcout << "File roots: " << std::endl;
        for(int i = 0; i < fileroots.size(); i++)
          std::wcout<< std::setw(7)<<std::right<< int(fileroots[i]) << "    " << files[i] <<std::endl;
      }
      if(ass) std::wcout << "Assembly:  " << int(ass) << std::endl;
    }
    else if(dragon::Handle::exists(cmd))
    {
      dragon::Handle(cmd)->print(std::wcout);
    }
    else
    {
      std::wcout << "-- invalid handle" << std::endl;
    }
    std::wcout << ">> ";
  }
  std::wcout << "\b\b\b\b    \b\b\b\b";
  dragon::Handle::cleanup();
  return 0;
}
