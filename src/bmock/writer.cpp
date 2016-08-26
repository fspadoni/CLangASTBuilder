#include "writer.h"

#include <clang/AST/Decl.h>
#include <boost/filesystem/convenience.hpp>

void writer::writeBeginFFF(std::ostringstream& out, const std::set<std::string>& includePaths)
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

void writer::writeEndFFF(std::ostringstream& out)
{
   out << std::endl;
   out << "/**" << std::endl;
   out << " *\tMOCKED FUNCTIONS END" << std::endl;
   out << " */" << std::endl << std::endl;

}
void writer::writeMockFunctionFFF(const clang::FunctionDecl* funcDecl, std::ostringstream& out)
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

