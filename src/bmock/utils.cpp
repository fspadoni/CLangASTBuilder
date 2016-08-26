#include "utils.h"

#include <clang/AST/Decl.h>
#include <boost/filesystem/convenience.hpp>


std::string utils::removeFileExtension( const std::string& fileNamePath)
{
   size_t lastdot = fileNamePath.find_last_of(".");
   
   if (lastdot == std::string::npos )
      return fileNamePath;
   else
      return fileNamePath.substr(0, lastdot);

  // return boost::filesystem::path(fileNamePath).stem().string();
  
}

std::string utils::changeFileExtension(const std::string& fileName, const std::string& newExt)
{

   return boost::filesystem::change_extension(fileName, newExt).string();
}
