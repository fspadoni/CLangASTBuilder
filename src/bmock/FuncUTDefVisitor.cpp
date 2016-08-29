
#include "FuncUTDefVisitor.h"

#include "utils.h"
#include "writer.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>


FuncUTDefVisitor::FuncUTDefVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName)
{

  results::get().functionToUnitTest.clear();
  results::get().includesForUT.clear();
}



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
         results::get().functionToUnitTest.insert(func);
   } 

   return true;
}


FuncUTDeclVisitor::FuncUTDeclVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName) {}



bool FuncUTDeclVisitor::VisitDecl(clang::Decl* decl)
{

  const clang::FunctionDecl* func = llvm::dyn_cast<clang::FunctionDecl>(decl);

   if (func == nullptr)
      return true;

   if ( !func->hasBody() ){
      // get declaration source location
      const clang::SourceManager& srcMgr = _context->getSourceManager();
      const clang::SourceLocation declSrcLoc = func->getSourceRange().getBegin();
      const std::string declSrcFile = srcMgr.getFilename(declSrcLoc).str();
      // check if the funcDecl is in the input argument file
    
      for ( auto func_i : results::get().functionToUnitTest ){
         if( func_i->getNameInfo().getName().getAsString() == 
	     func->getNameInfo().getName().getAsString() ){
	    boost::filesystem::path p(declSrcFile);
	    results::get().includesForUT.insert(p.filename().string());
	    break;
	 }
      }
   
   } 

   return true;
}



FuncUTDefConsumer::FuncUTDefConsumer(clang::ASTContext*  context,
                           std::string         fileName )
   : ASTConsumer()
   , _defVisitor( nullptr )
   , _declVisitor( nullptr )
{
   _defVisitor = new FuncUTDefVisitor( context, fileName );
   _declVisitor = new FuncUTDeclVisitor( context, fileName );
}



void FuncUTDefConsumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _defVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
   _declVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
}



clang::ASTConsumer* FuncUTDefAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
{
   return new FuncUTDefConsumer(  &compiler.getASTContext(), inFile.str() );
}

void FuncUTDefAction::EndSourceFileAction(){
   Writer::CreateUnitTestFile(  getCurrentFile().str(), getCompilerInstance().getSourceManager() );
}
