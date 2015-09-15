
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <experimental\filesystem>

#include "pugixml.hpp"
#include "boost\program_options\variables_map.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\positional_options.hpp"
#include "boost\program_options\options_description.hpp"
#include "boost\program_options\cmdline.hpp"
#include "boost\program_options\parsers.hpp"

#include "trim.hpp"
#include "configuration_structure.hpp"
#include "command_seeker.h"
#include "windows_implementation.hpp"

namespace std
{
using namespace std::experimental;
}

bool try_compile(const compiler_config& comp_conf,
    const project_config& proj_config, const std::filesystem::path source_file,
    std::filesystem::path& output)
{
   auto dir = remove_quote(proj_config.output_directory.string());
   dir += source_file.filename().stem().generic_string() + ".obj";
   output = add_quote(dir);

   std::string command = comp_conf.compiler_executable.string();

   add_to_command(
       command, {comp_conf, std::vector<std::string>{"JUST_COMPILE",
                                "EXCEPTION_ENABLE", "DYNAMIC_COMPILATION"}});

   add_to_command(
       command, {comp_conf, "INCLUDE_DIRECTORY"}, proj_config.header_directory);
   add_to_command(
       command, {comp_conf, "INCLUDE_DIRECTORY"}, comp_conf.header_directory);

   add_to_command(command, {comp_conf, "OUTPUT_OBJECT_FILE"}, output.string());
   add_to_command(command, source_file.string());

   std::cout << "compile " << source_file << " to " << output << std::endl;
   output = remove_quote(output.string());
   std::filesystem::remove(output);
   output = add_quote(output.string());
   return windows::execute_command(command)
       && std::filesystem::exists(remove_quote(output.string()));
}

bool try_compile(const compiler_config& comp_conf, project_config& proj_config)
{
   bool result = true;
   auto it = proj_config.input_files.begin();
   auto ite = proj_config.input_files.end();
   while(it != ite && result)
   {
      std::filesystem::path output;
      result = result && try_compile(comp_conf, proj_config, *it, output);
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

compiler_config parse_compiler_config(
    const std::filesystem::path& comp_file_path)
{
   return compiler_config();
}

int main(int argc, char** argv)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")("compiler",
       boost::program_options::value<std::filesystem::path>(),
       "compiler configuration file in xml");

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
      return 0;
   }

   if(vm.count("help") || !vm.size() || !vm.count("compiler"))
   {
      std::cout << desc << std::endl;
      return 0;
   }

   compiler_config comp_conf;
   project_config proj_conf;

   auto it_comp = vm.find("compiler");
   if(it_comp != vm.end())
   {
      std::filesystem::path compiler_path
          = remove_quote(it_comp->second.as<std::filesystem::path>().string());
      if(std::filesystem::exists(compiler_path))
         comp_conf = parse_compiler_config(compiler_path);
      else
      {
         std::cout << "error" << std::endl;
         std::cout << compiler_path << " not found !" << std::endl;
         return 1;
      }
   }

   bool result = try_compile(comp_conf, proj_conf);
   if(result)
   {
      std::cout << "compile " << proj_conf.executable_name << " ok !"
                << std::endl;
      result = try_link(comp_conf, proj_conf);
      if(result)
      {
         std::cout << "linking " << proj_conf.executable_name << " ok !"
                   << std::endl;
      }
   }
   if(!result)
   {
      std::cout << "compiling " << proj_conf.executable_name << " fail !"
                << std::endl;
   }
   return 0;
}
