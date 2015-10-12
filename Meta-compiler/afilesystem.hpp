#ifndef AFILESYSTEM_HPP
#define AFILESYSTEM_HPP

#if _MSC_VER > 1700
#include <experimental/filesystem>

namespace astd
{
   namespace filesystem
   {
      typedef std::experimental::filesystem::path path;
      using std::experimental::filesystem::exists;
      using std::experimental::filesystem::remove;
   }
}

#else

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

namespace astd
{
   namespace filesystem
   {
      typedef boost::filesystem::path path;
      using boost::filesystem::exists;
      using boost::filesystem::remove;
   }
}

#endif

#endif //!AFILESYSTEM_HPP
