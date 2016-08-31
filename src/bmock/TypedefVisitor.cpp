#include "TypedefVisitor.h"

#include <boost/filesystem.hpp>

#include "utils.h"

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
   
   const clang::Type* type = type_def->getCanonicalDecl()->getTypeForDecl();//  ->getCanonicalDecl()->getUnderlyingType()->getAsStructureType();
   const std::string typeName = type_def->getCanonicalDecl()->getNameAsString();
   //if( type_struct == nullptr )
   //   return true;
   
   //const clang::RecordDecl* structure = type_struct->getDecl();
   //if( structure == nullptr )
   //   return true;
   
   //const clang::RecordDecl* structureDecl = structure->getDefinition();
   //if( structureDecl == nullptr )
   //   return true;

   //clang::QualType canonicalQualType = type_def->getUnderlyingType();
   //if (canonicalQualType.isNull())
   //   return true;
   
   //const clang::TypedefType* type_defType = type_def->getCanonicalDecl()->getUnderlyingType()->getAs<const clang::TypedefType>();
   //const clang::Type* canonicalType = type_def->getCanonicalDecl()->getUnderlyingType()->getAs<clang::TypedefType>();
   //if ( canonicalType == nullptr )
   //   return true;
   
   if ( results::get().functionTypeNames.find( typeName ) ==  results::get().functionTypeNames.end() ){
      //std::cout << type->get ->getUnderlyingType().getAsString() << " Not Found\n";
      std::cout << type_def->getNameAsString() << " Not Found\n";
      return true;
   }
   
   
   /*
   const clang::Type* type = canonicalQualType.getCanonicalType().getTypePtr();
   if ( results::get().functionTypes.find( type ) ==  results::get().functionTypes.end() )
      return true;
   */
   
   
   //type = structureDecl->getTypeForDecl();
   //if ( results::get().functionTypes.find( type ) !=  results::get().functionTypes.end() )
   //   results::get().typedefNameDecls.insert(type_def);
   
   std::cout << type_def->getNameAsString() << " Found\n";
   results::get().typedefNameDecls.insert(type_def);
   
   
   return true;
}

