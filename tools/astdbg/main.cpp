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

namespace drg = dragon;

int main(int argc, char *argv[])
{
  {
    std::vector<drg::Root> fileroots;
    std::vector<char*> files;
    for(int i = 1; i < argc; i++)
    {
      std::wifstream input(argv[i]);
      drg::Handle rt;
      drg::FileScanner sc(input, argv[i]);
      drg::Parser pr(sc, rt, drg::Handle::make<drg::Literal>(L"somefile"));
      std::wcout << "Parsing:   " << argv[i] << std::endl;
      if(pr.parse() == 0)
      {
        fileroots.push_back(rt);
        files.push_back(argv[i]);
      }
    }
    drg::Root ass;
    // Assembling working with namespaces
    if(true)
		{
			dragon::Assembler amb;
			std::vector<drg::Handle> filehandles;
			std::transform(fileroots.begin(), fileroots.end(), std::inserter(filehandles, filehandles.begin()),
				[](drg::Root &h){ return drg::Handle(h); }
			);
			drg::Root mod = amb.new_module();
			amb.assemble(filehandles, mod);
			ass = amb.get_assembly();
			
			drg::Root as2;
			
			{
				std::ofstream ofs("export.dex");
				drg::Exporter dex(ofs);
				dex.serialize(std::vector<drg::Handle>{drg::Handle(mod)});
				ofs.close();
				drg::gc.run();
			}
			{
				std::ifstream ifs("export.dex");
				drg::Importer dim(ifs);
				auto v = dim.deserialize();
				ifs.close();
				assert(v.size() == 1);
				as2 = v[0];
			}
			
			std::wcout << "Imported module: " << as2 << std::endl;
		}
		int cmd;
    std::wcout << ">> ";
		drg::NodePrinter p;
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
      else if(drg::Handle::exists(cmd))
      {
        p.print(drg::Handle(cmd));
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
    drg::gc.run();
    drg::Handle::cleanup();
		std::wcout << "-- exitted --" << std::endl;
    return 0;
  }
	return 0;
}
