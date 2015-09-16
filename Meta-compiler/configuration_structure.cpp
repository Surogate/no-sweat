
#include <iostream>
#include <map>
#include <functional>

#include "configuration_structure.hpp"
#include "pugixml.hpp"

compiler_config parse_compiler_config::parse(
    const std::filesystem::path& comp_file_path)
{
   pugi::xml_document doc;
   doc.load_file(comp_file_path.c_str());
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }

   return result;
}

void parse_compiler_config::xml_parse(const pugi::xml_node& doc)
{
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void parse_compiler_config::compiler_parse(const pugi::xml_node& doc)
{
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void parse_compiler_config::name_parse(const pugi::xml_node& doc)
{
   result.name = doc.child_value();
}

void parse_compiler_config::compiler_executable_parse(const pugi::xml_node& doc)
{
   result.compiler_executable = doc.child_value();
}

void parse_compiler_config::linker_executable_parse(const pugi::xml_node& doc)
{
   result.linker_executable = doc.child_value();
}

void parse_compiler_config::include_directories_parse(const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      result.header_directory.push_back(dir.child_value());
   }
}

void parse_compiler_config::library_directories_parse(const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      result.libraries_directories.push_back(dir.child_value());
   }
}

void parse_compiler_config::libraries_parse(const pugi::xml_node& doc)
{
   for(auto& lib : doc)
   {
      result.libraries_names.push_back(lib.child_value());
   }
}

void parse_compiler_config::command_dictionnary_parse(const pugi::xml_node& doc)
{
   for(auto& item : doc)
   {
      auto key_found = item.find_child([](const pugi::xml_node& n)
          {
             return std::string(n.name()) == "key";
          });
      auto value_found = item.find_child([](const pugi::xml_node& n)
          {
             return std::string(n.name()) == "value";
          });
      result.commands[key_found.child_value()] = value_found.child_value();
   }
}
