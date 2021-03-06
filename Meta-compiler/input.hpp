#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

#include "boost/program_options/variables_map.hpp"
#include "boost/program_options/options_description.hpp"
#include "boost/program_options/positional_options.hpp"
#include "boost/program_options/options_description.hpp"
#include "boost/program_options/cmdline.hpp"
#include "boost/program_options/parsers.hpp"

#include "trim.hpp"
#include "error_status.hpp"
#include "afilesystem.hpp"

struct input
{
   std::vector<astd::filesystem::path> project_config;
   std::vector<std::string> configuration_to_execute;
};

bool verify_input(const boost::program_options::variables_map& vm,
    const std::string& key, std::vector<astd::filesystem::path>& value)
{
   auto it_comp = vm.find(key);
   if(it_comp != vm.end())
   {
      value = it_comp->second.as<std::vector<astd::filesystem::path>>();
      bool result = true;
      for(auto& path : value)
      {
         path = remove_quote(path.string());
         result = result && astd::filesystem::exists(path);
         if(!result)
         {
            std::cout << "error!" << std::endl;
            std::cout << path << " not found !" << std::endl;
         }
      }
      return result;
   }
   std::cout << "error!" << std::endl;
   std::cout << "no configuration file !" << std::endl;
   return false;
}

bool verify_input(const boost::program_options::variables_map& vm,
    const std::string& key, std::vector<std::string>& value)
{
   auto it = vm.find(key);
   if(it != vm.end())
   {
      value = it->second.as<std::remove_reference<decltype(value)>::type>();
      return true;
   }
   return false;
}

error_status::type parse_input(int argc, char** argv, input& input_struct)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")("project",
       boost::program_options::value<std::vector<astd::filesystem::path>>(),
       "project configuration file in xml")("run",
       boost::program_options::value<std::vector<std::string>>(),
       "configuration name to run");

   boost::program_options::positional_options_description positional_option;
   positional_option.add("run", -1);

   boost::program_options::variables_map vm;
   try
   {
      boost::program_options::store(
          boost::program_options::command_line_parser(argc, argv)
              .options(desc)
              .positional(positional_option)
              .style(boost::program_options::command_line_style::unix_style
                  | boost::program_options::command_line_style::
                        allow_long_disguise)
              .allow_unregistered()
              .run(),
          vm);
      boost::program_options::notify(vm);
   }
   catch(boost::program_options::error e)
   {
      std::cout << desc << std::endl;
      return error_status::COMMAND_LINE_PARSER_FAILED;
   }

   if(vm.count("help") || !vm.size() || !vm.count("project"))
   {
      std::cout << desc << std::endl;
      return error_status::COMMAND_LINE_PARAMETER_MISSING;
   }

   if(!verify_input(vm, "project", input_struct.project_config))
      return error_status::COMMAND_LINE_PROJECT_INVALID;
   if(!verify_input(vm, "run", input_struct.configuration_to_execute))
      return error_status::COMMAND_LINE_NO_CONFIGURATION;
   return error_status::STATUS_OK;
}

#endif //! INPUT_HPP
