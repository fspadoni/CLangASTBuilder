
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <set>
#include <string>
#include <sstream>

class MockVisitor : public clang::RecursiveASTVisitor<MockVisitor>
{

   clang::ASTContext *_context;
   std::string _fileName;

public:


   explicit MockVisitor(clang::ASTContext*                     context, std::string fileName  );



   
   bool VisitStmt(clang::Stmt* stmt);
   
};






class MockConsumer : public clang::ASTConsumer 
{

   MockVisitor* _visitor;

public:

   MockConsumer(clang::ASTContext*     context,  std::string fileName );


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


class MockAction : public clang::ASTFrontendAction
{
   //clang::CompilerInstance* _compiler;
   
   //std::ostringstream _out;
   //std::string _fileName;

   //void CreateMockFile(void);
   //void CreateUnitTestFile(void);

public:

   MockAction() {};

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

};

