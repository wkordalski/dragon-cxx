#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
  po::options_description opts("Options");
  std::string script = "", config = "";
  opts.add_options() 
    ("help,h", "Print help messages")
    ("script,s", po::value<std::string>(&script), "Build script file")
    ("config,c", po::value<std::string>(&config), "Config file");
 
  po::variables_map vm; 
  
  try
  {
    po::store(po::parse_command_line(argc, argv, opts), vm);
    vm.notify();
    
    if(vm.count("help"))
    {
      std::cout << opts << std::endl;
      return 0;
    }
    
    if(vm.count("script") == 0)
    {
      std::cerr << "[ ERROR ] No script file passed!" << std::endl;
      return 1;
    }
  }
  catch(po::error &e)
  {
    std::cerr << "[ ERROR ] " << e.what() << std::endl;
    return 1;
  }
  
  std::cout << "Running with script file: " << script << std::endl;
  if(config != "")
  {
    std::cout << "Passed config file: " << config << std::endl;
  }
	return 0;
}