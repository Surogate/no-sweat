
#include <iostream>
#include <string>
#include <vector>
#include <experimental\filesystem>
#include <winapifamily.h>

namespace windows {
#include "windows.h"
}

namespace std {
using namespace std::experimental;
}

static std::filesystem::path compiler =
    R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe").";
static std::filesystem::path file =
    R".("D:\Home\ancel\GitHub\no-sweat\Meta-compiler\main.cpp").";
static std::vector<std::filesystem::path> include_directory = {
    R".("C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include").",
    R".("C:\Program Files (x86)\Windows Kits\8.1\Include\um").",
    R".("C:\Program Files (x86)\Windows Kits\10\Include\10.0.10150.0\ucrt").",
    R".("C:\Program Files (x86)\Windows Kits\8.1\Include\shared").",
    R".("C:\Program Files (x86)\Windows Kits\8.1\Include\winrt")."};
static std::filesystem::path output_directory = file.parent_path();
static std::string output_object_file_command = R".(/Fo:).";
static std::string include_directory_command = R".(/I).";
static std::string exception_enable_command = R".(/EHsc).";
static std::string just_compile_command = R".(/c).";

#ifdef _WIN32
namespace windows {
std::string GetLastErrorAsString() {
  // Get the error message, if any.
  auto errorMessageID = windows::GetLastError();
  if (errorMessageID == 0)
    return std::string(); // No error message has been recorded

  windows::LPSTR messageBuffer = nullptr;

  size_t size = windows::FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (windows::LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  // Free the buffer.
  windows::LocalFree(messageBuffer);

  return message;
}

bool try_compile(const std::string &command) {
  std::string quoted_command = command;

  std::cout << "start : " << quoted_command << std::endl;

  windows::STARTUPINFO stpinfo;
  windows::PROCESS_INFORMATION procinfo;
  std::memset(&stpinfo, 0, sizeof(stpinfo));
  std::memset(&procinfo, 0, sizeof(procinfo));
  stpinfo.wShowWindow = TRUE;
  stpinfo.cb = sizeof(stpinfo);
  // std::wcout << compiler.c_str() << std::endl;
  windows::BOOL result = windows::CreateProcess(
      NULL, (char *)quoted_command.c_str(), NULL, NULL, TRUE,
      NORMAL_PRIORITY_CLASS, NULL, NULL, &stpinfo, &procinfo);
  if (result == FALSE) {
    auto error = windows::GetLastError();
    auto error_message = GetLastErrorAsString();
  } else {
    WaitForSingleObject(procinfo.hProcess, INFINITE);
  }
  windows::CloseHandle(procinfo.hProcess);
  windows::CloseHandle(procinfo.hThread);
  return result == TRUE;
}

bool try_compile(const std::filesystem::path &source,
                 std::filesystem::path &output,
                 const std::vector<std::filesystem::path> &includes) {
  output = output_directory / (file.filename().stem().string() + ".obj\"");
  std::string command = compiler.string();
  command += " " + just_compile_command;
  command += " " + exception_enable_command;
  for (auto &include : includes) {
    command += " " + include_directory_command + include.string();
  }
  command += " " + output_object_file_command + " " +
             output.string() + " " + source.string();
  return windows::try_compile(command);
}
}
#endif //!_WIN32

int main(int argc, char **argv) {
  std::filesystem::path output;
  windows::try_compile(file, output, include_directory);

  return 0;
}
