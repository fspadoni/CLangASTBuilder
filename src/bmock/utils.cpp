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
  
}

std::string utils::changeFileExtension(const std::string& fileName, const std::string& newExt)
{
   return boost::filesystem::change_extension(fileName, newExt).string();
}


void utils::fillFunctionQualTypes(void)
{
   results::get().functionDeclTypes.clear();
   
   //canonical Types: http://clang.llvm.org/docs/InternalsManual.html#canonical-types
   
   for ( const clang::FunctionDecl* funcDecl : results::get().functionDecls )
   {
      const clang::QualType returnType = funcDecl->getReturnType();
      
      results::get().functionDeclTypes.insert( returnType->getCanonicalTypeInternal().getTypePtrOrNull() );

      
      const int numParms = funcDecl->getNumParams();   
      for ( int i=0; i<numParms; ++i)
      {
         const clang::ParmVarDecl* _currentParam = funcDecl->getParamDecl(i);
         const clang::QualType qualType = _currentParam->getOriginalType();
         
         results::get().functionDeclTypes.insert( qualType->getCanonicalTypeInternal().getTypePtrOrNull() ); 
      }
   }
   
   
   for ( const clang::FunctionDecl* funcDecl : results::get().functionToUnitTest )
   {
      const clang::QualType returnType = funcDecl->getReturnType();
      
      results::get().functionDeclTypes.insert( returnType->getCanonicalTypeInternal().getTypePtrOrNull() );
         
      const int numParms = funcDecl->getNumParams();   
      for ( int i=0; i<numParms; ++i)
      {
         const clang::ParmVarDecl* _currentParam = funcDecl->getParamDecl(i);   
         const clang::QualType qualType = _currentParam->getOriginalType();
         
         results::get().functionDeclTypes.insert( qualType->getCanonicalTypeInternal().getTypePtrOrNull() );
      }
   }
   
}