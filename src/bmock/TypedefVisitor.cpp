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
   
   const clang::RecordType* type_struct = type_def->getUnderlyingType()->getAsStructureType();
   if( type_struct == nullptr )
      return true;
   
   const clang::RecordDecl* structure = type_struct->getDecl();
   if( structure == nullptr )
      return true;
   
   if( structure->getDefinition() == nullptr )
      return true;

   results::get().typedefNameDecls.insert(type_def);
   
   return true;
}

