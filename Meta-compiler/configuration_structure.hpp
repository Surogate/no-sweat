#ifndef CONFIGURATION_STRUCTURE_HPP
#define CONFIGURATION_STRUCTURE_HPP

#include <vector>
#include <string>
#include <experimental\filesystem>
#include <map>

namespace std
{
using namespace std::experimental;
}

struct project_config
{
   std::vector<std::filesystem::path> input_files = {
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\main.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\windows_implementation.cpp).",
       R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\command_seeker.cpp)."};

   std::vector<std::filesystem::path> object_files;

   std::vector<std::filesystem::path> header_directory
       = {R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler).",
           R".(D:\Home\ancel\GitHub\pugixml\src).",
           R".(D:\Home\ancel\GitHub\boost)."};

   std::vector<std::filesystem::path> libraries_directories = {
       R".(D:\Home\ancel\GitHub\boost\stage\lib).",
       R".(D:\Home\ancel\GitHub\pugixml\scripts\Debug)."};

   std::vector<std::filesystem::path> libraries_names
       = {"libboost_program_options-vc140-mt-1_59.lib"};

   std::filesystem::path output_directory
       = R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\).";

   std::filesystem::path executable_name
       = R".(D:\Home\ancel\GitHub\no-sweat\Meta-compiler\meta_comp.exe).";
};

struct compiler_config
{
   std::string name = "VS14";
   std::filesystem::path compiler_executable =
       R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe").";
   std::filesystem::path linker_executable
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
       /*= {"kernel32.lib", "user32.lib", "gdi32.lib", "winspool.lib",
      "comdlg32.lib", "advapi32.lib", "shell32.lib", "ole32.lib",
      "oleaut32.lib", "uuid.lib", "odbc32.lib", "odbccp32.lib"}*/;

   std::map<std::string, std::string> commands = {
       {"OUTPUT_OBJECT_FILE", R".(/Fo:)."}, {"INCLUDE_DIRECTORY", R".(/I)."},
       {"EXCEPTION_ENABLE", R".(/EHsc)."}, {"JUST_COMPILE", R".(/c)."},
       {"OUTPUT_EXECUTABLE", R".(/OUT:)."},
       {"LIBRARY_DIRECTORY", R".(/LIBPATH:)."},
       {"DATA_EXECUTION_PREVENTION", R".(/NXCOMPAT)."},
       {"RANDOMIZED_BASE_ADDRESS", R".(/DYNAMICBASE)."},
       {"DYNAMIC_COMPILATION", R".(/MD)."}, {"STATIC_COMPILATION", R".(/MT)."}};
};

#endif //! CONFIGURATION_STRUCTURE_HPP
