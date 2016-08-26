
#include "FuncUTDefVisitor.h"

#include "utils.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>


FuncUTDefVisitor::FuncUTDefVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName) {}



bool FuncUTDefVisitor::VisitDecl(clang::Decl* decl)
{

  const clang::FunctionDecl* func = llvm::dyn_cast<clang::FunctionDecl>(decl);

   if (func == nullptr)
      return true;

   if ( func->hasBody() ){
      // get declaration source location
      const clang::SourceManager& srcMgr = _context->getSourceManager();
      const clang::SourceLocation declSrcLoc = func->getSourceRange().getBegin();
      const std::string declSrcFile = srcMgr.getFilename(declSrcLoc).str();
      // check if the funcDecl is in the input argument file
      if ( declSrcFile.find( _fileName) != std::string::npos )
         results::functionToUnitTest.insert(func);
   } 

   return true;
}


FuncUTDefConsumer::FuncUTDefConsumer(clang::ASTContext*  context,
                           std::string         fileName )
   : ASTConsumer()
   , _visitor( nullptr )
{
   _visitor =  new FuncUTDefVisitor( context, fileName );
}



void FuncUTDefConsumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _visitor->TraverseDecl(ctx.getTranslationUnitDecl());
}



clang::ASTConsumer* FuncUTDefAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
{
   return new FuncUTDefConsumer(  &compiler.getASTContext(), inFile.str() );
}