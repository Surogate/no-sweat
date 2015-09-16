#ifndef CONFIGURATION_STRUCTURE_HPP
#define CONFIGURATION_STRUCTURE_HPP

#include <vector>
#include <string>
#include <experimental\filesystem>
#include <map>
#include <functional>
#include "pugixml.hpp"

namespace std
{
using namespace std::experimental;
}

struct project_config
{
   std::vector<std::filesystem::path> input_files = {
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\main.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\windows_implementation.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\command_seeker.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\configuration_structure.cpp)."};

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directory
       = {R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler).",
           R".(D:\Home\ancel\GitHub\pugixml\src).",
           R".(D:\Home\ancel\GitHub\boost)."};

   std::vector<std::filesystem::path> libraries_directories = {
       R".(D:\Home\ancel\GitHub\boost\stage\lib).",
       R".(D:\Home\ancel\GitHub\pugixml\scripts\Release)."};

   std::vector<std::filesystem::path> libraries_names
       = {"libboost_program_options-vc140-mt-1_59.lib", "pugixml.lib"};

   std::filesystem::path output_directory
       = R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\).";

   std::filesystem::path executable_name
       = R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\meta_comp.exe).";
};

struct compiler_config
{
   std::string name = "VS14";
   std::filesystem::path compiler_executable;
   std::filesystem::path linker_executable;

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directory;

   std::vector<std::filesystem::path> libraries_directories;
   std::vector<std::filesystem::path> libraries_names;

   std::map<std::string, std::string> commands;
};

struct parse_compiler_config
{
   compiler_config parse(const std::filesystem::path& comp_file_path);
   void xml_parse(const pugi::xml_node& doc);

   void compiler_parse(const pugi::xml_node& doc);

   void name_parse(const pugi::xml_node& doc);

   void compiler_executable_parse(const pugi::xml_node& doc);

   void linker_executable_parse(const pugi::xml_node& doc);

   void include_directories_parse(const pugi::xml_node& doc);

   void library_directories_parse(const pugi::xml_node& doc);

   void libraries_parse(const pugi::xml_node& doc);

   void command_dictionnary_parse(const pugi::xml_node& doc);

   std::map<std::string, std::function<void(const pugi::xml_node&)>>
       state_machine = {{"xml", std::bind(&parse_compiler_config::xml_parse,
                                    this, std::placeholders::_1)},
           {"compiler", std::bind(&parse_compiler_config::compiler_parse, this,
                            std::placeholders::_1)},
           {"name", std::bind(&parse_compiler_config::name_parse, this,
                        std::placeholders::_1)},
           {"compiler_executable",
               std::bind(&parse_compiler_config::compiler_executable_parse,
                   this, std::placeholders::_1)},
           {"linker_executable",
               std::bind(&parse_compiler_config::linker_executable_parse, this,
                   std::placeholders::_1)},
           {"include_directories",
               std::bind(&parse_compiler_config::include_directories_parse,
                   this, std::placeholders::_1)},
           {"library_directories",
               std::bind(&parse_compiler_config::library_directories_parse,
                   this, std::placeholders::_1)},
           {"libraries", std::bind(&parse_compiler_config::libraries_parse,
                             this, std::placeholders::_1)},
           {"command_dictionnary",
               std::bind(&parse_compiler_config::command_dictionnary_parse,
                   this, std::placeholders::_1)}};

   compiler_config result;
};

#endif //! CONFIGURATION_STRUCTURE_HPP
