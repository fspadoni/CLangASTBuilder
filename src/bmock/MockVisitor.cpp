#include "MockVisitor.h"

#include "utils.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>


MockVisitor::MockVisitor(clang::ASTContext* context, std::string fileName):  _context(context), _fileName(fileName) {}



bool MockVisitor::VisitStmt(clang::Stmt* stmt)
{

   // check if the statement is a function call
   const clang::CallExpr* funcCall = llvm::dyn_cast<clang::CallExpr>(stmt);
   if ( funcCall == nullptr )
   {
      // this statement is not a function call
      return true;
   }

   const clang::FunctionDecl* funcDecl = funcCall->getDirectCallee();

   //std::cout << funcDecl->getNameInfo().getName().getAsString() << "\t";

   // source location
   const clang::SourceLocation srcLoc = stmt->getSourceRange().getBegin();

   const clang::SourceManager& srcMgr = _context->getSourceManager();
   const std::string srcFile = srcMgr.getFilename(srcLoc).str();

   // get declaration source location
   const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
   const std::string declSrcFile = srcMgr.getFilename(declSrcLoc).str();
      
   // check if the statement is in the input argument file
   if ( declSrcFile.find( _fileName) != std::string::npos )
   {
      // this function doesn't need to be mocked but tested
      //std::cout << "this function doesn't need to be mocked but tested" << std::endl;
      //_functionToUnitTest.insert(funcDecl);
      return true;
   }


   // TO DO: fix this
   // temporary check
   // WEAK CHECK if declaration is in CommercialCode path
   if ( declSrcFile.find( "CommercialCode") == std::string::npos )
   {
      // this is (probably) a system function
      //std::cout << "this is (probably) a system function" << std::endl;
      return true;
   }
   
   // mock this function
   //std::cout << "accepted" << std::endl;
   result::functionDecls.insert(funcDecl);

   return true;
}


MockConsumer::MockConsumer(clang::ASTContext*  context,
                           std::string         fileName )
   : ASTConsumer()
   , _visitor( nullptr )
{
   _visitor =  new MockVisitor( context, fileName );
}



void MockConsumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _visitor->TraverseDecl(ctx.getTranslationUnitDecl());
}



clang::ASTConsumer* MockAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
{
   return new MockConsumer(  &compiler.getASTContext(), inFile.str() );
}