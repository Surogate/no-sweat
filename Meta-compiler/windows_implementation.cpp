#include <string>
#include <iostream>

#include "windows_implementation.hpp"

#ifdef _WIN32
namespace windows
{
#include "windows.h"
#include <winapifamily.h>

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
   stpinfo.wShowWindow = FALSE;
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
      WaitForSingleObject(procinfo.hThread, INFINITE);
      WaitForSingleObject(procinfo.hProcess, INFINITE);
   }
   windows::CloseHandle(procinfo.hProcess);
   windows::CloseHandle(procinfo.hThread);
   return result == TRUE;
}
}
#endif //!_WIN32