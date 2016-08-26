#include "utils.h"

#include <clang/AST/Decl.h>
#include <boost/filesystem/convenience.hpp>

void utils::writeBeginFFF(std::ostringstream& out, const std::set<std::string>& includePaths)
{

   out << "/** " << std::endl;
   out << " *\n *\n *\n *\n *\n *" << std::endl;
   out << " */" << std::endl << std::endl << std::endl;


   out << "// Include test and utilities mock headers\n";
   out << "#include \"fff.h\"\n" << std::endl;

   for(  auto include : includePaths ){
      out << "#include \"" << include << "\"\n";
   }
   out << "\nDEFINE_FFF_GLOBALS;\n\n";

   out << "/**" << std::endl;
   out << " *\tMOCKED FUNCTIONS BEGIN" << std::endl;
   out << " */" << std::endl << std::endl;

}

void utils::writeEndFFF(std::ostringstream& out)
{
   out << std::endl;
   out << "/**" << std::endl;
   out << " *\tMOCKED FUNCTIONS END" << std::endl;
   out << " */" << std::endl << std::endl;

}
void utils::writeMockFunctionFFF(const clang::FunctionDecl* funcDecl, std::ostringstream& out)
{
         
   std::string returnType = funcDecl->getReturnType().getAsString();

   std::string isVariadic;

   if ( funcDecl->isVariadic() )
   {
      isVariadic = "_VARARG";
   }

   if ( returnType == "void" )
   {
      out << "FAKE_VOID_FUNC" << isVariadic << "( ";
   }
   else
   {
      out << "FAKE_VALUE_FUNC" << isVariadic << "( " << returnType << ", ";
   }
   

   out << funcDecl->getNameInfo().getName().getAsString();
       
   const int numParms = funcDecl->getNumParams();           

   for ( int i=0; i<numParms; ++i)
   {
      const clang::ParmVarDecl* _currentParam = funcDecl->getParamDecl(i);
      out << ", " << _currentParam->getType().getAsString(); 
   }

   if ( funcDecl->isVariadic() )
   {
      out << ", ...";
   }

   out <<" );";

   out << std::endl;

}


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
