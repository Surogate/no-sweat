
#include "command_seeker.h"
#include "configuration_structure.hpp"
#include "trim.hpp"

#include <iostream>

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

command_seeker::command_seeker(
    const configuration_file& conf, const std::vector<std::string>& command_names)
{
   auto ite = conf.option_dictionnary.end();
   for(auto& name : command_names)
   {
      auto it = conf.option_dictionnary.find(name);
      if(it != ite)
      {
         values.push_back(it->second);
      }
      else {
         std::cout << "option " << name << " not found in option_dictionnary" << std::endl;
      }
   }
}

void add_to_command(std::string& command, const command_seeker& value)
{
   for(auto& com : value.get())
      command += " " + com;
}

void add_to_command(
    std::string& command, const std::vector<std::filesystem::path>& values)
{
   for(auto& value : values)
   {
      add_to_command(command, add_quote(value.string()));
   }
}

void add_to_command(std::string& command, const command_seeker& command_name,
    const std::filesystem::path& to_be_added)
{
   add_to_command(command, command_name);
   command += add_quote(to_be_added.string());
}

void add_to_command(std::string& command, const command_seeker& command_name,
    const std::vector<std::filesystem::path>& to_be_added)
{
   for(auto& adding_path : to_be_added)
   {
      add_to_command(command, command_name, adding_path);
   }
}