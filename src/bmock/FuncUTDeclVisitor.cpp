
#include "FuncUTDeclVisitor.h"

#include "utils.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <set>


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


FuncUTDeclConsumer::FuncUTDeclConsumer(clang::ASTContext*  context,
                           std::string         fileName )
   : ASTConsumer()
   , _visitor( nullptr )
{
   _visitor =  new FuncUTDeclVisitor( context, fileName );
}



void FuncUTDeclConsumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _visitor->TraverseDecl(ctx.getTranslationUnitDecl());
}



clang::ASTConsumer* FuncUTDeclAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
{
   return new FuncUTDeclConsumer(  &compiler.getASTContext(), inFile.str() );
} 


void FuncUTDeclAction::EndSourceFileAction(){
   //Writer::CreateUnitTestFile(  getCurrentFile().str(), getCompilerInstance().getSourceManager() );
}