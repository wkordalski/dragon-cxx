#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "../../src/lexer/scanner.hpp"
#include "parser.hpp"

#include "../../src/visitors/assembler.hpp"
#include "../../src/visitors/exporter.hpp"
#include "../../src/visitors/gc.hpp"
#include "../../src/visitors/importer.hpp"
#include "../../src/visitors/printer.hpp"

int main(int argc, char *argv[])
{
  {
    std::vector<dragon::Root> fileroots;
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
    dragon::Root ass;
    // Assembling working with namespaces
    if(true)
		{
			dragon::Assembler amb;
			std::vector<dragon::Handle> filehandles;
			std::transform(fileroots.begin(), fileroots.end(), std::inserter(filehandles, filehandles.begin()),
				[](dragon::Root &h){ return dragon::Handle(h); }
			);
			dragon::Root mod = amb.new_module();
			amb.assemble(filehandles, mod);
			ass = amb.get_assembly();
			
			dragon::Root as2;
			
			{
				std::ofstream ofs("export.dex");
				dragon::Exporter dex(ofs);
				dex.serialize(std::vector<dragon::Handle>{dragon::Handle(mod)});
				ofs.close();
				dragon::gc.run();
			}
			{
				std::ifstream ifs("export.dex");
				dragon::Importer dim(ifs);
				auto v = dim.deserialize();
				ifs.close();
				assert(v.size() == 1);
				as2 = v[0];
			}
			
			std::wcout << "Imported module: " << as2 << std::endl;
		}
		int cmd;
    std::wcout << ">> ";
		dragon::NodePrinter p;
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
        p.print(dragon::Handle(cmd));
      }
      else
      {
        std::wcout << "-- invalid handle" << std::endl;
      }
      std::wcout << ">> ";
    }
    
	}
	/*
	{
    std::ofstream ofs("export.dex");
    dragon::Exporter dex(ofs);
    dex.serialize(fileroots);
    ofs.close();
    dragon::gc.run();
  }
  {
    std::ifstream ifs("export.dex");
    dragon::Importer dim(ifs);
    auto v = dim.deserialize();
    ifs.close();
    std::cout << v.size() << std::endl;
  }
  */
  {
		std::wcout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << std::flush;
    dragon::gc.run();
    dragon::Handle::cleanup();
		std::wcout << "-- exitted --" << std::endl;
    return 0;
  }
	return 0;
}
