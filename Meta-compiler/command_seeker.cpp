#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>

#include "configuration_structure.hpp"
#include "command_seeker.h"
#include "trim.hpp"

command_seeker::command_seeker(const std::string& input_value)
{
   values.push_back(input_value);
}

command_seeker::command_seeker(const std::vector<std::string>& input_values)
    : values(input_values)
{
}

command_seeker::command_seeker(
    const configuration_file& conf, const std::string& command_name)
{
   auto it = conf.option_dictionnary.find(command_name);
   if(it != conf.option_dictionnary.end())
   {
      values.push_back(it->second);
   }
}

command_seeker::command_seeker(const configuration_file& conf,
    const std::vector<std::string>& command_names)
{
   auto ite = conf.option_dictionnary.end();
   for(auto& name : command_names)
   {
      auto it = conf.option_dictionnary.find(name);
      if(it != ite)
      {
         values.push_back(it->second);
      }
      else
      {
         std::cout << "option " << name << " not found in option_dictionnary"
                   << std::endl;
      }
   }
}

void add_to_command(std::string& command, const std::string& value)
{
   command += " " + value;
}

void add_to_command(
    std::string& command, const std::vector<std::string>& values)
{
   for(auto& com : values)
   {
      add_to_command(command, com);
   }
}

void add_to_command(std::string& command, const command_seeker& value)
{
   add_to_command(command, value.get());
}

void add_to_command(std::string& command, const astd::filesystem::path& value)
{
   add_to_command(command, add_quote(value.string()));
}

void add_to_command(
    std::string& command, const std::vector<astd::filesystem::path>& values)
{
   for(auto& value : values)
   {
      add_to_command(command, add_quote(value.string()));
   }
}

void add_to_command(std::string& command, const command_seeker& command_name,
    const astd::filesystem::path& to_be_added)
{
   add_to_command(command, command_name);
   command += add_quote(to_be_added.string());
}

void add_to_command(std::string& command, const command_seeker& command_name,
    const std::vector<astd::filesystem::path>& to_be_added)
{
   for(auto& adding_path : to_be_added)
   {
      add_to_command(command, command_name, adding_path);
   }
}

std::string get_value_of(
    const std::string& name, const configuration_file& conf)
{
   typedef std::function<std::string(const configuration_file&)> func_ptr;
   static std::map<std::string, func_ptr> static_config
       = {{"name",
              [](const configuration_file& conf)
              {
                 std::string result;
                 add_to_command(result, conf.name);
                 return result;
              }},
           {"input_files",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.input_files);
                  return result;
               }},
           {"object_files",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.object_files);
                  return result;
               }},
           {"header_directories",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.header_directories);
                  return result;
               }},
           {"libraries_directories",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.libraries_directories);
                  return result;
               }},
           {"libraries_names",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.libraries_names);
                  return result;
               }},
           {"output_directory",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.output_directory);
                  return result;
               }},
           {"executable_name",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.executable_name);
                  return result;
               }},
           {"compiler_executable",
               [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.compiler_executable);
                  return result;
               }},
           {"linker_executable", [](const configuration_file& conf)
               {
                  std::string result;
                  add_to_command(result, conf.linker_executable);
                  return result;
               }}};

   auto it = static_config.find(name);
   if(it != static_config.end())
   {
      return it->second(conf);
   }
   else
   {
      auto it_option = conf.option_dictionnary.find(name);
      if(it_option != conf.option_dictionnary.end())
      {
         return it_option->second;
      }
      else
      {
         std::cerr << name << " is not found" << std::endl;
      }
   }
}