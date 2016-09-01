#include "TypedefVisitor.h"
#include "utils.h"


#include <clang/AST/ASTContext.h>

#include <boost/filesystem.hpp>

#include <iostream>


TypedefVisitor::TypedefVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName)
{
  results::get().typedefNameDecls.clear();
}


   
bool TypedefVisitor::VisitDecl(clang::Decl* decl)
{

   const clang::TypedefNameDecl* type_def = llvm::dyn_cast<clang::TypedefNameDecl>(decl);
   if ( type_def == nullptr )
      return true;
   
   //const std::string typeName = type_def->getCanonicalDecl()->getNameAsString();   

   //canonical Types: http://clang.llvm.org/docs/InternalsManual.html#canonical-types
   const clang::QualType qualType = type_def->getCanonicalDecl()->getUnderlyingType();
   const clang::Type* declType = qualType->getCanonicalTypeInternal().getTypePtrOrNull();
   
   if ( declType == nullptr )
      return true;
   
   //if ( declType->isBuiltinType() == true )
     // return true;
   
   // get declaration source location
   const clang::SourceLocation declSrcLoc = type_def->getSourceRange().getBegin();
   const std::string declSrcFile = _context->getSourceManager().getFilename(declSrcLoc).str();
   // TO DO: fix this
   // temporary check
   // WEAK CHECK if declaration is in CommercialCode path
   if ( declSrcFile.find( "CommercialCode") == std::string::npos )
   {
      // this is (probably) a system function
      //std::cout << "this is (probably) a system function" << std::endl;
      return true;
   }
   
   
   if ( results::get().functionDeclTypes.find( declType ) ==  results::get().functionDeclTypes.end() )
      return true;

   results::get().typedefNameDecls.insert(type_def);
   
   return true;
}

