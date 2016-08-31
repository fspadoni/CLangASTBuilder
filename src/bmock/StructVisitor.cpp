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
      

      
      //  check if the type is used in function types
      const std::string typeName = structdef->getNameAsString();// getTypeForDecl()->getCanonicalTypeInternal().getTypePtr();
      if ( results::get().functionTypeNames.find( typeName ) == results::get().functionTypeNames.end() )
         return true;
         
      
      
      //  check if this RecordDecl is already in a TypedefDecl
      for (auto typedefIter : results::get().typedefNameDecls )
      {
         const clang::RecordDecl* type_struct = typedefIter->getUnderlyingType()->getAsStructureType()->getDecl();
         
         if ( structdef != type_struct )
         {
            results::get().structDecls.insert(structure);
         }
      }
      
   }
   //if ( results::get().typedefNameDecls.find() )
   //results::get().structDecls.insert(structure);//or  structdef ??
//      for ( const auto clang::TypedefNameDecl type_def : results::typedefNameDecls ){
//	 if ()
//      }
  

return true;
}

