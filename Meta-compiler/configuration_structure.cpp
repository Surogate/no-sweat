
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>

#include "configuration_structure.hpp"
#include "pugixml.hpp"

configuration_file operator+(
    const configuration_file& lvalue, const configuration_file& rvalue)
{
   configuration_file result;
   push_back(result.name, lvalue.name);
   push_back(result.name, rvalue.name);
   push_back(result.configuration, lvalue.configuration);
   push_back(result.configuration, rvalue.configuration);

   result.compilation_command
       = lvalue.compilation_command + " " + rvalue.compilation_command;
   result.linking_command
       = lvalue.linking_command + " " + rvalue.linking_command;

   push_back(result.input_files, lvalue.input_files);
   push_back(result.input_files, rvalue.input_files);
   push_back(result.object_files, lvalue.object_files);
   push_back(result.object_files, rvalue.object_files);
   push_back(result.header_directories, lvalue.header_directories);
   push_back(result.header_directories, rvalue.header_directories);
   push_back(result.libraries_directories, lvalue.libraries_directories);
   push_back(result.libraries_directories, rvalue.libraries_directories);
   push_back(result.libraries_names, lvalue.libraries_names);
   push_back(result.libraries_names, rvalue.libraries_names);

   if(lvalue.output_directory.empty())
   {
      result.output_directory = rvalue.output_directory;
   }
   else
   {
      result.output_directory = lvalue.output_directory;
   }

   if(lvalue.executable_name.empty())
   {
      result.executable_name = rvalue.executable_name;
   }
   else
   {
      result.executable_name = lvalue.executable_name;
   }

   if(lvalue.compiler_executable.empty())
   {
      result.compiler_executable = rvalue.compiler_executable;
   }
   else
   {
      result.compiler_executable = lvalue.compiler_executable;
   }

   if(lvalue.linker_executable.empty())
   {
      result.linker_executable = rvalue.linker_executable;
   }
   else
   {
      result.linker_executable = lvalue.linker_executable;
   }

   push_back(result.option_dictionnary, lvalue.option_dictionnary);
   push_back(result.option_dictionnary, rvalue.option_dictionnary);
   return result;
}

std::vector<configuration_file> configuration_parser::parse(
    const std::filesystem::path& proj_file_path)
{
   pugi::xml_document doc;
   auto parse_result = doc.load_file(proj_file_path.c_str());
   error = parse_result.status;
   if(error == pugi::status_ok)
   {
      for(auto& node : doc)
      {
         state_machine[node.name()](node);
      }
   }
   return results;
}

void configuration_parser::xml_parser(const pugi::xml_node& doc)
{
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void configuration_parser::compiler_parser(const pugi::xml_node& doc)
{
   results.push_back(configuration_file());
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void configuration_parser::project_parser(const pugi::xml_node& doc)
{
   results.push_back(configuration_file());
   for(auto& node : doc)
   {
      state_machine[node.name()](node);
   }
}

void configuration_parser::name_parser(const pugi::xml_node& doc)
{
   results.back().name.push_back(doc.child_value());
}

void configuration_parser::compilation_command_parser(const pugi::xml_node& doc)
{
   results.back().compilation_command = doc.child_value();
}

void configuration_parser::linking_command_parser(const pugi::xml_node& doc)
{
   results.back().linking_command = doc.child_value();
}

void configuration_parser::sub_configuration_parser(const pugi::xml_node& doc)
{
   configuration_parser parser;
   parser.project_parser(doc);
   error = parser.error;
   results.back().configuration.insert(results.back().configuration.end(), parser.results.begin(), parser.results.end());
}

void configuration_parser::input_files_parser(const pugi::xml_node& doc)
{
   for(auto& file : doc)
   {
      results.back().input_files.push_back(file.child_value());
   }
}

void configuration_parser::object_files_parser(const pugi::xml_node& doc)
{
   for(auto& file : doc)
   {
      results.back().object_files.push_back(file.child_value());
   }
}

void configuration_parser::header_directories_parser(const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      results.back().header_directories.push_back(dir.child_value());
   }
}

void configuration_parser::libraries_directories_parser(
    const pugi::xml_node& doc)
{
   for(auto& dir : doc)
   {
      results.back().libraries_directories.push_back(dir.child_value());
   }
}

void configuration_parser::libraries_names_parser(const pugi::xml_node& doc)
{
   for(auto& name : doc)
   {
      results.back().libraries_names.push_back(name.child_value());
   }
}

void configuration_parser::output_directory_parser(const pugi::xml_node& doc)
{
   results.back().output_directory = doc.child_value();
}

void configuration_parser::executable_name_parser(const pugi::xml_node& doc)
{
   results.back().executable_name = doc.child_value();
}

void configuration_parser::compiler_executable_parser(const pugi::xml_node& doc)
{
   results.back().compiler_executable = doc.child_value();
}

void configuration_parser::linker_executable_parser(const pugi::xml_node& doc)
{
   results.back().linker_executable = doc.child_value();
}

void configuration_parser::option_dictionnary_parser(const pugi::xml_node& doc)
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
      std::string key_str = key_found.child_value();
      std::transform(
          key_str.begin(), key_str.end(), key_str.begin(), ::toupper);
      results.back().option_dictionnary[std::move(key_str)]
          = value_found.child_value();
   }
}
