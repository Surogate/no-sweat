#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

#include "boost\program_options\variables_map.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\positional_options.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\cmdline.hpp"
#include "boost\program_options\parsers.hpp"

#include "trim.hpp"
#include "error_status.hpp"
#include "afilesystem.hpp"

struct input
{
   astd::filesystem::path compiler_config;
   astd::filesystem::path project_config;
};

bool verify_input(boost::program_options::variables_map& vm,
    const std::string& key, astd::filesystem::path& value)
{
   auto it_comp = vm.find(key);
   if(it_comp != vm.end())
   {
      value
          = remove_quote(it_comp->second.as<astd::filesystem::path>().string());
      if(astd::filesystem::exists(value))
      {

         return true;
      }
      else
      {
         std::cout << "error" << std::endl;
         std::cout << value << " not found !" << std::endl;
         return false;
      }
   }
   return false;
}

error_status::type parse_input(int argc, char** argv, input& input_struct)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")("compiler",
       boost::program_options::value<astd::filesystem::path>(),
       "compiler configuration file in xml")("project",
       boost::program_options::value<astd::filesystem::path>(),
       "project configuration file in xml");

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
      return error_status::COMMAND_LINE_PARSER_FAILED;
   }

   if(vm.count("help") || !vm.size() || !vm.count("compiler")
       || !vm.count("project"))
   {
      std::cout << desc << std::endl;
      return error_status::COMMAND_LINE_PARAMETER_MISSING;
   }

   astd::filesystem::path file_path;
   bool result = verify_input(vm, "compiler", file_path);
   if(result)
   {
      input_struct.compiler_config = file_path;
   }
   else
      return error_status::COMMAND_LINE_COMPILER_INVALID;
   result = verify_input(vm, "project", file_path);
   if(result)
   {
      input_struct.project_config = file_path;
   }
   else
      return error_status::COMMAND_LINE_PROJECT_INVALID;

   return error_status::STATUS_OK;
}

#endif //! INPUT_HPP
