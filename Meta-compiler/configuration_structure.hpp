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

struct configuration_file
{
   std::vector<std::string> name;

   std::vector<configuration_file> configuration;

   std::string compilation_command;
   std::string linking_command;

   std::vector<std::filesystem::path> input_files;
   std::vector<std::filesystem::path> object_files;
   std::vector<std::filesystem::path> header_directories;
   std::vector<std::filesystem::path> libraries_directories;
   std::vector<std::filesystem::path> libraries_names;
   std::filesystem::path output_directory;
   std::filesystem::path executable_name;

   std::filesystem::path compiler_executable;
   std::filesystem::path linker_executable;
   std::map<std::string, std::string> option_dictionnary;
};

template <typename T>
void push_back(std::vector<T>& in, const std::vector<T>& value)
{
   in.insert(in.end(), value.begin(), value.end());
}

template <typename T, typename Y>
void push_back(std::map<T, Y>& in, const std::map<T, Y>& value)
{
   in.insert(value.begin(), value.end());
}

configuration_file operator+(
    const configuration_file& lvalue, const configuration_file& rvalue);

struct configuration_parser
{
   std::vector<configuration_file> parse(
       const std::filesystem::path& proj_file_path);
   void xml_parser(const pugi::xml_node& doc);
   void compiler_parser(const pugi::xml_node& doc);
   void project_parser(const pugi::xml_node& doc);
   void name_parser(const pugi::xml_node& doc);
   void compilation_command_parser(const pugi::xml_node& doc);
   void linking_command_parser(const pugi::xml_node& doc);
   void sub_configuration_parser(const pugi::xml_node& doc);
   void input_files_parser(const pugi::xml_node& doc);
   void object_files_parser(const pugi::xml_node& doc);
   void header_directories_parser(const pugi::xml_node& doc);
   void libraries_directories_parser(const pugi::xml_node& doc);
   void libraries_names_parser(const pugi::xml_node& doc);
   void output_directory_parser(const pugi::xml_node& doc);
   void executable_name_parser(const pugi::xml_node& doc);
   void compiler_executable_parser(const pugi::xml_node& doc);
   void linker_executable_parser(const pugi::xml_node& doc);
   void option_dictionnary_parser(const pugi::xml_node& doc);

   std::vector<configuration_file> results;
   pugi::xml_parse_status error = pugi::status_ok;
   std::map<std::string, std::function<void(const pugi::xml_node&)>>
       state_machine = {{"xml", std::bind(&configuration_parser::xml_parser,
                                    this, std::placeholders::_1)},
           {"compiler", std::bind(&configuration_parser::project_parser, this,
                            std::placeholders::_1)},
           {"project", std::bind(&configuration_parser::project_parser, this,
                           std::placeholders::_1)},
           {"compiler_executable",
               std::bind(&configuration_parser::compiler_executable_parser,
                   this, std::placeholders::_1)},
           {"linker_executable",
               std::bind(&configuration_parser::linker_executable_parser, this,
                   std::placeholders::_1)},
           {"name", std::bind(&configuration_parser::name_parser, this,
                        std::placeholders::_1)},
           {"configuration",
               std::bind(&configuration_parser::sub_configuration_parser, this,
                   std::placeholders::_1)},
           {"input_files", std::bind(&configuration_parser::input_files_parser,
                               this, std::placeholders::_1)},
           {"object_files",
               std::bind(&configuration_parser::object_files_parser, this,
                   std::placeholders::_1)},
           {"header_directories",
               std::bind(&configuration_parser::header_directories_parser, this,
                   std::placeholders::_1)},
           {"libraries_directories",
               std::bind(&configuration_parser::libraries_directories_parser,
                   this, std::placeholders::_1)},
           {"libraries_names",
               std::bind(&configuration_parser::libraries_names_parser, this,
                   std::placeholders::_1)},
           {"output_directory",
               std::bind(&configuration_parser::output_directory_parser, this,
                   std::placeholders::_1)},
           {"executable_name",
               std::bind(&configuration_parser::executable_name_parser, this,
                   std::placeholders::_1)},
           {"option_dictionnary",
               std::bind(&configuration_parser::option_dictionnary_parser, this,
                   std::placeholders::_1)},
           {"compilation_command",
               std::bind(&configuration_parser::compilation_command_parser,
                   this, std::placeholders::_1)},
           {"linking_command",
               std::bind(&configuration_parser::linking_command_parser, this,
                   std::placeholders::_1)}};
};

#endif //! CONFIGURATION_STRUCTURE_HPP
