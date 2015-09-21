
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <experimental\filesystem>

#include "trim.hpp"
#include "configuration_structure.hpp"
#include "command_seeker.h"
#include "windows_implementation.hpp"
#include "input.hpp"
#include "error_status.hpp"

namespace std
{
using namespace std::experimental;
}

bool try_compile(std::string command, const configuration_file& project,
    const std::filesystem::path source_file, std::filesystem::path& output)
{
   auto dir = remove_quote(project.output_directory.string());
   dir += source_file.filename().stem().generic_string() + ".obj";
   std::filesystem::remove(output);
   output = add_quote(dir);

   add_to_command(command, {project, "OUTPUT_OBJECT_FILE"}, output.string());
   add_to_command(command, source_file.string());

   std::cout << "compile " << source_file << " to " << output << std::endl;

   return windows::execute_command(command)
       && std::filesystem::exists(remove_quote(output.string()));
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
      std::filesystem::path output;
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
   std::filesystem::path output
       = remove_quote(project.executable_name.string());
   std::filesystem::remove(output);
   return windows::execute_command(command) && std::filesystem::exists(output);
}

int main(int argc, char** argv)
{
   input command_input;

   error_status::type parse_result = parse_input(argc, argv, command_input);
   if(parse_result != error_status::STATUS_OK)
      return parse_result;

   configuration_parser project_parser;
   project_parser.parse(command_input.compiler_config);
   if(project_parser.error)
   {
      std::cout << "error parsing " << command_input.compiler_config
                << std::endl;
      return error_status::COMPILER_FILE_PARSING_FAILED;
   }
   project_parser.parse(command_input.project_config);
   if(project_parser.error)
   {
      std::cout << "error parsing " << command_input.project_config;
      return error_status::PROJECT_FILE_PARSING_FAILED;
   }
   auto configurations = project_parser.results;

   auto project_to_compile = configurations.front() + configurations.back();
   project_to_compile = project_to_compile + project_to_compile.configuration.front();

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
