#ifndef COMMAND_SEEKER_HPP
#define COMMAND_SEEKER_HPP

#include <string>
#include <vector>

#include <experimental\filesystem>

namespace std
{
using namespace std::experimental;
}

struct compiler_config;

struct command_seeker
{
   command_seeker(const std::string& input_value);

   command_seeker(const std::vector<std::string>& input_values);

   command_seeker(const compiler_config& conf, const std::string& command_name);

   command_seeker(const compiler_config& conf,
       const std::vector<std::string>& command_names);

   inline const std::vector<std::string>& get() const { return values; }

   std::vector<std::string> values;
};

void add_to_command(std::string& command, const command_seeker& value);
void add_to_command(
    std::string& command, const std::vector<std::filesystem::path>& values);

void add_to_command(std::string& command, const command_seeker& command_name,
    const std::filesystem::path& to_be_added);

void add_to_command(std::string& command, const command_seeker& command_name,
    const std::vector<std::filesystem::path>& to_be_added);

#endif //! COMMAND_SEEKER_HPP
