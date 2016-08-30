#include "TypedefVisitor.h"

#include <boost/filesystem.hpp>

#include "utils.h"


TypedefVisitor::TypedefVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName)
{
  results::get().structDecls.clear();
}


bool TypedefVisitor::VisitDecl(clang::Decl* decl)
{

  const clang::RecordDecl* structure = llvm::dyn_cast<clang::RecordDecl>(decl);

   if ( structure == nullptr )
      return true;
   // RecordDecl are class struct and union, we trigger only for struct
   if ( structure->isStruct() ){
      const clang::RecordDecl* structdef = structure->getDefinition();
      // Forward/Partial definition skipped (should not happen)
      if( structdef == nullptr )
         return true;

      results::get().structDecls.insert(structure);//or  structdef ??
   } 

   return true;
}

