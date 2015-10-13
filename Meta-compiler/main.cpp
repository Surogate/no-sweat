
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "trim.hpp"
#include "configuration_structure.hpp"
#include "command_seeker.h"
#include "windows_implementation.hpp"
#include "input.hpp"
#include "error_status.hpp"
#include "afilesystem.hpp"

bool try_compile(std::string command, const configuration_file& project,
    const astd::filesystem::path source_file, astd::filesystem::path& output)
{
   auto dir = remove_quote(project.output_directory.string());
   dir += source_file.filename().stem().generic_string() + ".obj";
   astd::filesystem::remove(output);
   output = add_quote(dir);

   add_to_command(command, {project, "OUTPUT_OBJECT_FILE"}, output.string());
   add_to_command(command, source_file.string());

   std::cout << "compile " << source_file << " to " << output << std::endl;

   return windows::execute_command(command)
       && astd::filesystem::exists(remove_quote(output.string()));
}

std::string prepare_command(const configuration_file& project)
{
   std::string command = project.compiler_executable.string();

   add_to_command(
       command, {project, std::vector<std::string>{"JUST_COMPILE",
                              "EXCEPTION_ENABLE", "DYNAMIC_STD_LIB"}});

   add_to_command(
       command, {project, "INCLUDE_DIRECTORY"}, project.header_directories);
   return command;
}

bool try_compile(configuration_file& project)
{
   bool result = true;
   auto it = project.input_files.begin();
   auto ite = project.input_files.end();
   std::string command = prepare_command(project);
   while(it != ite && result)
   {
      astd::filesystem::path output;
      result = result && try_compile(command, project, *it, output);
      if(result)
      {
         project.object_files.emplace_back(output);
      }
      ++it;
   }
   return result;
}

bool try_link(const configuration_file& project)
{
   std::string command = add_quote(project.linker_executable.string());

   add_to_command(
       command, {project, std::vector<std::string>{"DATA_EXECUTION_PREVENTION",
                              "RANDOMIZED_BASE_ADDRESS"}});

   add_to_command(
       command, {project, "LIBRARY_DIRECTORY"}, project.libraries_directories);
   add_to_command(command, project.object_files);
   add_to_command(command, project.libraries_names);

   add_to_command(
       command, {project, "OUTPUT_EXECUTABLE"}, project.executable_name);

   std::cout << "link " << project.executable_name << std::endl;
   astd::filesystem::path output
       = remove_quote(project.executable_name.string());
   astd::filesystem::remove(output);
   return windows::execute_command(command) && astd::filesystem::exists(output);
}

bool is_composed(
    const std::string& value, std::string& lvalue, std::string& rvalue)
{
   auto pos = value.find_first_of("::");
   if(pos != std::string::npos)
   {
      lvalue = value.substr(0, pos);
      rvalue = value.substr(pos + sizeof("::") - 1); // we are already on the first letter
      return true;
   }
   return false;
}

bool get_project(const std::vector<configuration_file>& input, const std::string& name, configuration_file& output)
{
   std::string lvalue, rvalue;
   if (!is_composed(name, lvalue, rvalue))
   {
      auto it = std::find_if(input.begin(), input.end(),
         [&](const configuration_file& value)
      {
         return value.name.front() == name;
      });
      if (it != input.end())
      {
         output = *it;
         return true;
      }         
      else
      {
         std::cout << "warning! " << name << " unknown !" << std::endl;
      }
   }
   else
   {
      auto it = std::find_if(input.begin(), input.end(),
         [&](const configuration_file& value)
      {
         return value.name.front() == lvalue;
      });
      if (it != input.end())
      {
         if(get_project(it->configuration, rvalue, output))
         {
            return true;
         }
         else
            std::cout << "warning! " << rvalue << " unknown in " << lvalue << std::endl;
      }
      else
         std::cout << "warning! " << lvalue << " unknown !" << std::endl;
   }
   return false;
}

configuration_file assemble_project_to_compile(
    const std::vector<configuration_file>& input,
    std::vector<std::string>& configuration_to_execute)
{
   configuration_file result;
   if(!configuration_to_execute.size())
   {
      std::cout << "warning, no configuration executed" << std::endl;
   }
   else
   {
      std::cout << "execute:" << std::endl;
      for (auto& str : configuration_to_execute)
      {
         std::cout << str << std::endl;
         configuration_file fetch;
         if (get_project(input, str, fetch))
         {
            result = result + fetch;
         }
      }
   }
   return result;
}

int main(int argc, char** argv)
{
   input command_input;

   error_status::type parse_result = parse_input(argc, argv, command_input);
   if(parse_result != error_status::STATUS_OK)
      return parse_result;

   configuration_parser project_parser;
   bool parser_error = false;
   for (auto& project_path : command_input.project_config)
   {
      project_parser.parse(project_path);
      if (project_parser.error)
      {
         std::cout << "error parsing " << project_path
            << std::endl;
         parser_error = true;
      }
   }
   if (parser_error)
      return error_status::FILE_PARSING_FAILED;

   auto configurations = project_parser.results;

   auto project_to_compile = assemble_project_to_compile(
       configurations, command_input.configuration_to_execute);

   bool compile_result = try_compile(project_to_compile);
   if(compile_result)
   {
      std::cout << "compile " << project_to_compile.executable_name << " ok !"
                << std::endl;
      compile_result = try_link(project_to_compile);
      if(compile_result)
      {
         std::cout << "linking " << project_to_compile.executable_name
                   << " ok !" << std::endl;
      }
      else
      {
         return error_status::LINKING_FAILED;
      }
   }
   else
   {
      std::cout << "compiling " << project_to_compile.executable_name
                << " fail !" << std::endl;
      return error_status::COMPILATION_FAILED;
   }
   return error_status::STATUS_OK;
}
