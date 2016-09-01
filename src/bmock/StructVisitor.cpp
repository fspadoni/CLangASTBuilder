#include "StructVisitor.h"

#include <boost/filesystem.hpp>

#include "utils.h"


StructVisitor::StructVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName)
{
  results::get().structDecls.clear();
}


bool StructVisitor::VisitDecl(clang::Decl* decl)
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
         
   
      const clang::Type* declType = structdef->getTypeForDecl();
      if ( results::get().functionDeclTypes.find( declType ) ==  results::get().functionDeclTypes.end() )
         return true;
   
      //  check if this RecordDecl is already in a TypedefDecl
      for (auto typedefIter : results::get().typedefNameDecls )
      {
         //canonical Types: http://clang.llvm.org/docs/InternalsManual.html#canonical-types
         const clang::QualType typedefQualType = typedefIter->getCanonicalDecl()->getUnderlyingType();
         const clang::Type* typedefDeclType = typedefQualType->getCanonicalTypeInternal().getTypePtrOrNull();
         
         if ( declType != typedefDeclType )
         {
            results::get().structDecls.insert(structure);
         }
      }
      
   }  

   return true;
}

