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
   std::string name;

   std::vector<std::filesystem::path> input_files /*= {
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\main.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\windows_implementation.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\command_seeker.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\configuration_structure.cpp)."}*/;

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directories
       /*= {R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler).",
           R".(D:\Home\ancel\GitHub\pugixml\src).",
           R".(D:\Home\ancel\GitHub\boost)."}*/;

   std::vector<std::filesystem::path> libraries_directories /*= {
       R".(D:\Home\ancel\GitHub\boost\stage\lib).",
       R".(D:\Home\ancel\GitHub\pugixml\scripts\Release)."}*/;

   std::vector<std::filesystem::path> libraries_names
       /* = {"libboost_program_options-vc140-mt-1_59.lib", "pugixml.lib"}*/;

   std::filesystem::path output_directory
       /*= R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\)."*/;

   std::filesystem::path executable_name
       /*= R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\meta_comp.exe)."*/;
};

struct compiler_config
{
   std::string name;
   std::filesystem::path compiler_executable;
   std::filesystem::path linker_executable;

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directories;

   std::vector<std::filesystem::path> libraries_directories;
   std::vector<std::filesystem::path> libraries_names;

   std::map<std::string, std::string> commands;
};

struct parse_projet_config
{
   project_config parse(const std::filesystem::path& proj_file_path);
   void xml_parser(const pugi::xml_node& doc);
   void project_parser(const pugi::xml_node& doc);
   void name_parser(const pugi::xml_node& doc);
   void input_files_parser(const pugi::xml_node& doc);
   void object_files_parser(const pugi::xml_node& doc);
   void header_directories_parser(const pugi::xml_node& doc);
   void libraries_directories_parser(const pugi::xml_node& doc);
   void libraries_names_parser(const pugi::xml_node& doc);
   void output_directory_parser(const pugi::xml_node& doc);
   void executable_name_parser(const pugi::xml_node& doc);

   project_config result;
   bool error = false;
   std::map<std::string, std::function<void(const pugi::xml_node&)>>
       state_machine = {{"xml", std::bind(&parse_projet_config::xml_parser,
                                    this, std::placeholders::_1)},
           {"xml", std::bind(&parse_projet_config::xml_parser, this,
                       std::placeholders::_1)},
           {"project", std::bind(&parse_projet_config::project_parser, this,
                           std::placeholders::_1)},
           {"name", std::bind(&parse_projet_config::name_parser, this,
                        std::placeholders::_1)},
           {"input_files", std::bind(&parse_projet_config::input_files_parser,
                               this, std::placeholders::_1)},
           {"object_files", std::bind(&parse_projet_config::object_files_parser,
                                this, std::placeholders::_1)},
           {"header_directories",
               std::bind(&parse_projet_config::header_directories_parser, this,
                   std::placeholders::_1)},
           {"libraries_directories",
               std::bind(&parse_projet_config::libraries_directories_parser,
                   this, std::placeholders::_1)},
           {"libraries_names",
               std::bind(&parse_projet_config::libraries_names_parser, this,
                   std::placeholders::_1)},
           {"output_directory",
               std::bind(&parse_projet_config::output_directory_parser, this,
                   std::placeholders::_1)},
           {"executable_name",
               std::bind(&parse_projet_config::executable_name_parser, this,
                   std::placeholders::_1)}};
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

   bool error = false;
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
