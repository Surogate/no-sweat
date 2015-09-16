#ifndef INPUT_HPP
#define INPUT_HPP

#include <experimental\filesystem>
#include <iostream>

#include "boost\program_options\variables_map.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\positional_options.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\cmdline.hpp"
#include "boost\program_options\parsers.hpp"

#include "trim.hpp"

namespace std
{
using namespace std::experimental;
}

struct input
{
   std::filesystem::path compiler_config;
};

int parse_input(int argc, char** argv, input& input_struct)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")("compiler",
       boost::program_options::value<std::filesystem::path>(),
       "compiler configuration file in xml");

   boost::program_options::variables_map vm;
   try
   {
      boost::program_options::store(
          boost::program_options::command_line_parser(argc, argv)
              .options(desc)
              .style(boost::program_options::command_line_style::unix_style
                  | boost::program_options::command_line_style::
                        allow_long_disguise)
              .allow_unregistered()
              .run(),
          vm);
   }
   catch(boost::program_options::error e)
   {
      std::cout << desc << std::endl;
      return 1;
   }

   if(vm.count("help") || !vm.size() || !vm.count("compiler"))
   {
      std::cout << desc << std::endl;
      return 2;
   }

   auto it_comp = vm.find("compiler");
   if(it_comp != vm.end())
   {
      std::filesystem::path compiler_path
          = remove_quote(it_comp->second.as<std::filesystem::path>().string());
      if(std::filesystem::exists(compiler_path))
      {
         input_struct.compiler_config = compiler_path;
      }
      else
      {
         std::cout << "error" << std::endl;
         std::cout << compiler_path << " not found !" << std::endl;
         return 3;
      }
   }
   return 0;
}

#endif //! INPUT_HPP
