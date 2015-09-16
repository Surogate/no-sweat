
#include <iostream>
#include <map>
#include <functional>

#include "configuration_structure.hpp"
#include "pugixml.hpp"

compiler_config parse_compiler_config::parse(
    const std::filesystem::path& comp_file_path)
{
   pugi::xml_document doc;
   auto parse_result = doc.load_file(comp_file_path.c_str());
   error = parse_result.status != pugi::status_ok;

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
      result.header_directories.push_back(dir.child_value());
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

project_config parse_projet_config::parse(
    const std::filesystem::path& proj_file_path)
{
   pugi::xml_document doc;
   auto parse_result = doc.load_file(proj_file_path.c_str());
   error = parse_result.status != pugi::status_ok;
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
   return result;
}

void parse_projet_config::xml_parser(const pugi::xml_node& doc)
{
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void parse_projet_config::project_parser(const pugi::xml_node& doc)
{
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void parse_projet_config::name_parser(const pugi::xml_node& doc)
{
   result.name = doc.child_value();
}

void parse_projet_config::input_files_parser(const pugi::xml_node& doc)
{
   for(auto& file : doc)
   {
      result.input_files.push_back(file.child_value());
   }
}

void parse_projet_config::object_files_parser(const pugi::xml_node& doc)
{
   for(auto& file : doc)
   {
      result.object_files.push_back(file.child_value());
   }
}

void parse_projet_config::header_directories_parser(const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      result.header_directories.push_back(dir.child_value());
   }
}

void parse_projet_config::libraries_directories_parser(
    const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      result.libraries_directories.push_back(dir.child_value());
   }
}

void parse_projet_config::libraries_names_parser(const pugi::xml_node& doc)
{
   for(auto& name : doc)
   {
      result.libraries_names.push_back(name.child_value());
   }
}

void parse_projet_config::output_directory_parser(const pugi::xml_node& doc)
{
   result.output_directory = doc.child_value();
}

void parse_projet_config::executable_name_parser(const pugi::xml_node& doc)
{
   result.executable_name = doc.child_value();
}