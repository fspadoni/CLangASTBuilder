#include "visitors.h"

#include "utils.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>


FuncDeclVisitor::FuncDeclVisitor(clang::ASTContext*  context,
                                 const std::string&  fileName )
:  _context(context), 
   _fileName(fileName)
{
   //rewriter.setSourceMgr(_context->getSourceManager(), _context->getLangOpts());
}


bool FuncDeclVisitor::VisitDecl(clang::Decl* decl)
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
   } else {
      for ( auto func_i : results::functionToUnitTest ){
         //func_i.
      }
      //std::set<clang::FunctionDecl*>::const_iterator iter = results::functionToUnitTest.find(declSrcFile)
   }

   return true;
}


bool FuncDeclVisitor::VisitStmt(clang::Stmt* stmt)
{

   // check if the statement is a function call
   const clang::CallExpr* funcCall = llvm::dyn_cast<clang::CallExpr>(stmt);
   if ( funcCall == nullptr )
      return true;

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
      return true;


   // TO DO: fix this
   // temporary check
   // WEAK CHECK if declaration is in CommercialCode path
   if ( declSrcFile.find( "CommercialCode") == std::string::npos )
      return true;
   
   // mock this function
   //std::cout << "accepted" << std::endl;
   results::functionDecls.insert(funcDecl);

   return true;
}


