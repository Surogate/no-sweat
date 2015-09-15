#ifndef WINDOWS_IMPLEMENTATION_HPP
#define WINDOWS_IMPLEMENTATION_HPP

#include <string>

#ifdef _WIN32
namespace windows
{
std::string GetLastErrorAsString();

bool execute_command(const std::string& command);
}
#endif //!_WIN32

#endif //! WINDOWS_IMPLEMENTATION_HPP
