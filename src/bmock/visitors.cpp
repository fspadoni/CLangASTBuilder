#include "visitors.h"

#include "utils.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>


//FuncDeclVisitor::FuncDeclVisitor(clang::Rewriter& reWriter, const std::string& fileNamePath )
FuncDeclVisitor::FuncDeclVisitor(clang::ASTContext*                     context,
                                 std::set<const clang::FunctionDecl*>&  functionDecls,
                                 std::set<const clang::FunctionDecl*>&  functionToUnitTest,
                                 const std::string&                     fileName )
:  _context(context), 
   _functionDecls(functionDecls),
   _functionToUnitTest(functionToUnitTest),
   _fileName(fileName)
{
   //rewriter.setSourceMgr(_context->getSourceManager(), _context->getLangOpts());
}


bool FuncDeclVisitor::VisitDecl(clang::Decl* decl)
{
   const clang::FunctionDecl* func = llvm::dyn_cast<clang::FunctionDecl>(decl);

   if (func == nullptr)
   {
      return true;
   }

   if ( !func->doesThisDeclarationHaveABody() ){
      return true;
   }

   const clang::FunctionDecl* funcDef = func->getDefinition();

   if ( funcDef == nullptr)
   {
      return true;
   }

   // get declaration source location
   const clang::SourceManager& srcMgr = _context->getSourceManager();
   const clang::SourceLocation declSrcLoc = func->getSourceRange().getBegin();
   const std::string declSrcFile = srcMgr.getFilename(declSrcLoc).str();
   boost::filesystem::path p1(_fileName);
   boost::filesystem::path p2(declSrcFile);
   std::string res = (declSrcFile.find( _fileName) != std::string::npos) ? " TRUE" : " FALSE";

   std::cout << p1.string() << " " << p2.string() << res << std::endl;
   // check if the funcDecl is in the input argument file
   if ( declSrcFile.find( _fileName) != std::string::npos )
   {
      // this function doesn't need to be mocked but tested
      //std::cout << "this function doesn't need to be mocked but tested" << std::endl;
      _functionToUnitTest.insert(funcDef);
   }

   return true;
}


bool FuncDeclVisitor::VisitStmt(clang::Stmt* stmt)
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
   _functionDecls.insert(funcDecl);

   return true;
}


