
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

namespace std
{
using namespace std::experimental;
}

bool try_compile(std::string command, const compiler_config& comp_conf,
    const project_config& proj_config, const std::filesystem::path source_file,
    std::filesystem::path& output)
{
   auto dir = remove_quote(proj_config.output_directory.string());
   dir += source_file.filename().stem().generic_string() + ".obj";
   std::filesystem::remove(output);
   output = add_quote(dir);

   add_to_command(command, {comp_conf, "OUTPUT_OBJECT_FILE"}, output.string());
   add_to_command(command, source_file.string());

   std::cout << "compile " << source_file << " to " << output << std::endl;

   return windows::execute_command(command)
       && std::filesystem::exists(remove_quote(output.string()));
}

std::string prepare_command(const compiler_config& comp_conf,
   const project_config& proj_config)
{
   std::string command = comp_conf.compiler_executable.string();

   add_to_command(
      command, { comp_conf, std::vector<std::string>{"JUST_COMPILE",
      "EXCEPTION_ENABLE", "DYNAMIC_COMPILATION"} });

   add_to_command(
      command, { comp_conf, "INCLUDE_DIRECTORY" }, proj_config.header_directory);
   add_to_command(
      command, { comp_conf, "INCLUDE_DIRECTORY" }, comp_conf.header_directory);
   return command;
}

bool try_compile(const compiler_config& comp_conf, project_config& proj_config)
{
   bool result = true;
   auto it = proj_config.input_files.begin();
   auto ite = proj_config.input_files.end();
   std::string command = prepare_command(comp_conf, proj_config);
   while(it != ite && result)
   {
      std::filesystem::path output;
      result = result && try_compile(command, comp_conf, proj_config, *it, output);
      if(result)
      {
         proj_config.object_files.emplace_back(output);
      }
      ++it;
   }
   return result;
}

bool try_link(const compiler_config& comp_conf, project_config& proj_config)
{
   std::string command = add_quote(comp_conf.linker_executable.string());

   add_to_command(command,
       {comp_conf, std::vector<std::string>{"DATA_EXECUTION_PREVENTION",
                       "RANDOMIZED_BASE_ADDRESS"}});

   add_to_command(command, {comp_conf, "LIBRARY_DIRECTORY"},
       proj_config.libraries_directories);
   add_to_command(command, {comp_conf, "LIBRARY_DIRECTORY"},
       comp_conf.libraries_directories);

   add_to_command(command, proj_config.object_files);
   add_to_command(command, comp_conf.object_files);

   add_to_command(command, proj_config.libraries_names);
   add_to_command(command, comp_conf.libraries_names);

   add_to_command(
       command, {comp_conf, "OUTPUT_EXECUTABLE"}, proj_config.executable_name);

   std::cout << "link " << proj_config.executable_name << std::endl;
   std::filesystem::path output
       = remove_quote(proj_config.executable_name.string());
   std::filesystem::remove(output);
   return windows::execute_command(command) && std::filesystem::exists(output);
}

int main(int argc, char** argv)
{
   compiler_config comp_conf;
   project_config proj_conf;
   input command_input;

   int parse_result = parse_input(argc, argv, command_input);
   if (parse_result != 0)
      return parse_result;

   parse_compiler_config parser;
   comp_conf = parser.parse(command_input.compiler_config);

   bool compile_result = try_compile(comp_conf, proj_conf);
   if(compile_result)
   {
      std::cout << "compile " << proj_conf.executable_name << " ok !"
                << std::endl;
      compile_result = try_link(comp_conf, proj_conf);
      if(compile_result)
      {
         std::cout << "linking " << proj_conf.executable_name << " ok !"
                   << std::endl;
      }
   }
   if(!compile_result)
   {
      std::cout << "compiling " << proj_conf.executable_name << " fail !"
                << std::endl;
   }
   return 0;
}
