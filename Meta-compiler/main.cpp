
#include <iostream>
#include <string>
#include <vector>
#include <experimental\filesystem>
#include <winapifamily.h>

#include "trim.hpp"

namespace windows
{
#include "windows.h"
}

namespace std
{
using namespace std::experimental;
}

struct project_config
{
   std::vector<std::filesystem::path> input_files
       = {R".("D:\Home\ancel\GitHub\no-sweat\Meta-compiler\main.cpp")."};

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directory
       = {R".("D:\Home\ancel\GitHub\no-sweat\Meta-compiler")."};

   std::vector<std::filesystem::path> libraries_directories;

   std::vector<std::filesystem::path> libraries_names;

   std::filesystem::path output_directory
       = R".("D:\Home\ancel\GitHub\no-sweat\Meta-compiler\").";

   std::filesystem::path executable_name
       = R".("D:\Home\ancel\GitHub\no-sweat\Meta-compiler\meta_comp.exe").";
};

struct compiler_config
{
   std::filesystem::path compiler =
       R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe").";
   std::filesystem::path linker
       = R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\link.exe").";

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directory = {
       R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include").",
       R".("C:\Program Files (x86)\Windows Kits\8.1\Include\um").",
       R".("C:\Program Files (x86)\Windows Kits\10\Include\10.0.10150.0\ucrt").",
       R".("C:\Program Files (x86)\Windows Kits\8.1\Include\shared").",
       R".("C:\Program Files (x86)\Windows Kits\8.1\Include\winrt")."};

   std::vector<std::filesystem::path> libraries_directories = {
       R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib").",
       R".("C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86").",
       R".("C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10150.0\ucrt\x86")."};
   std::vector<std::filesystem::path> libraries_names
       = {"kernel32.lib", "user32.lib", "gdi32.lib", "winspool.lib",
           "comdlg32.lib", "advapi32.lib", "shell32.lib", "ole32.lib",
           "oleaut32.lib", "uuid.lib", "odbc32.lib", "odbccp32.lib"};

   std::string output_object_file_command = R".(/Fo:).";
   std::string include_directory_command = R".(/I).";
   std::string exception_enable_command = R".(/EHsc).";
   std::string just_compile_command = R".(/c).";
   std::string output_executable_command = R".(/OUT:).";
   std::string library_directory_command = R".(/LIBPATH:).";
};

#ifdef _WIN32
namespace windows
{
std::string GetLastErrorAsString()
{
   // Get the error message, if any.
   auto errorMessageID = windows::GetLastError();
   if(errorMessageID == 0)
      return std::string(); // No error message has been recorded

   windows::LPSTR messageBuffer = nullptr;

   size_t size = windows::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
           | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
       (windows::LPSTR)&messageBuffer, 0, NULL);

   std::string message(messageBuffer, size);

   // Free the buffer.
   windows::LocalFree(messageBuffer);

   return message;
}

bool execute_command(const std::string& command)
{
   windows::STARTUPINFO stpinfo;
   windows::PROCESS_INFORMATION procinfo;
   std::memset(&stpinfo, 0, sizeof(stpinfo));
   std::memset(&procinfo, 0, sizeof(procinfo));
   stpinfo.wShowWindow = TRUE;
   stpinfo.cb = sizeof(stpinfo);
   windows::BOOL result
       = windows::CreateProcess(NULL, (char*)command.c_str(), NULL, NULL, TRUE,
           NORMAL_PRIORITY_CLASS, NULL, NULL, &stpinfo, &procinfo);
   if(result == FALSE)
   {
      auto error = windows::GetLastError();
      auto error_message = GetLastErrorAsString();
      std::cout << "error " << error << " : " << error_message << std::endl;
   }
   else
   {
      WaitForSingleObject(procinfo.hProcess, INFINITE);
   }
   windows::CloseHandle(procinfo.hProcess);
   windows::CloseHandle(procinfo.hThread);
   return result == TRUE;
}
}
#endif //!_WIN32

void add_to_command(std::string& command, const std::string& value)
{
   command += " " + value;
}

void add_to_command(
    std::string& command, const std::vector<std::string>& values)
{
   for(auto& value : values)
   {
      add_to_command(command, value);
   }
}

void add_to_command(
    std::string& command, const std::vector<std::filesystem::path>& values)
{
   for(auto& value : values)
   {
      add_to_command(command, value.string());
   }
}

void add_to_command(std::string& command, const std::string& command_name,
    bool space, const std::filesystem::path& to_be_added)
{
   command += " " + command_name;
   if(space)
      command += " ";
   command += to_be_added.string();
}

void add_to_command(std::string& command, const std::string& command_name,
    const std::filesystem::path& to_be_added)
{
   //bool space = trim(command_name).back() == ':';
   bool space = false;
   command += " " + command_name;
   if(space)
      command += " ";
   command += to_be_added.string();
}

void add_to_command(std::string& command, const std::string& command_name,
    const std::vector<std::filesystem::path>& to_be_added)
{
   //bool space = trim(command_name).back() == ':';
   bool space = false;
   for(auto& adding_path : to_be_added)
   {
      add_to_command(command, command_name, space, adding_path);
   }
}

bool try_compile(const compiler_config& comp_conf,
    const project_config& proj_config, const std::filesystem::path source_file,
    std::filesystem::path& output)
{
   auto dir = remove_quote(proj_config.output_directory.string());
   dir += source_file.filename().stem().generic_string() + ".obj";
   output = add_quote(dir);

   std::string command = comp_conf.compiler.string();

   add_to_command(command, comp_conf.just_compile_command);
   add_to_command(command, comp_conf.exception_enable_command);

   add_to_command(command, comp_conf.include_directory_command,
       proj_config.header_directory);
   add_to_command(command, comp_conf.include_directory_command,
       comp_conf.header_directory);

   add_to_command(
       command, comp_conf.output_object_file_command, output.string());
   add_to_command(command, source_file.string());

   std::cout << "compile " << source_file << " to " << output << std::endl;
   output = remove_quote(output.string());
   std::filesystem::remove(output);
   output = add_quote(output.string());
   return windows::execute_command(command) && std::filesystem::exists(remove_quote(output.string()));
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
   std::string command = comp_conf.linker.string();

   add_to_command(command, comp_conf.library_directory_command,
       proj_config.libraries_directories);
   add_to_command(command, comp_conf.library_directory_command,
       comp_conf.libraries_directories);

   add_to_command(command, proj_config.object_files);
   add_to_command(command, comp_conf.object_files);

   add_to_command(command, proj_config.libraries_names);
   add_to_command(command, comp_conf.libraries_names);

   add_to_command(command, comp_conf.output_executable_command,
       proj_config.executable_name);

   std::cout << "link " << proj_config.executable_name << std::endl;
   std::filesystem::path output = remove_quote(proj_config.executable_name.string());
   std::filesystem::remove(output);
   return windows::execute_command(command) && std::filesystem::exists(output);
}

int main(int argc, char** argv)
{
   std::filesystem::path output;
   compiler_config comp_conf;
   project_config proj_conf;
   bool result = try_compile(comp_conf, proj_conf);
   if (result)
   {
      std::cout << "compile " << proj_conf.executable_name << " ok !" << std::endl;
      result = try_link(comp_conf, proj_conf);
      if (result)
      {
         std::cout << "linking " << proj_conf.executable_name << " ok !" << std::endl;
      }
   }
   if (!result)
   {
      std::cout << "compiling " << proj_conf.executable_name << " fail !" << std::endl;
   }
   return 0;
}
