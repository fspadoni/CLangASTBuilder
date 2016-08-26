
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <vector>
#include <string>


class FuncDeclVisitor : public clang::RecursiveASTVisitor<FuncDeclVisitor>
{

   //clang::ASTContext *_context;
   clang::Rewriter& _reWriter;
   const std::string _fileNamePath;

public:

   explicit FuncDeclVisitor( clang::Rewriter& reWriter, const std::string& fileNamePath );

   bool VisitDecl(clang::Decl* decl);

   
   bool VisitStmt(clang::Stmt* st);
   

private:


   bool testFunction(const unsigned int& ref, const unsigned int* ptr) const;

   };



class FuncDeclConsumer : public clang::ASTConsumer 
{

   FuncDeclVisitor* _visitor;
   //clang::Rewriter _reWriter;

public:



   FuncDeclConsumer(clang::Rewriter& reWriter, const std::string& fileNamePath );

 

   virtual ~FuncDeclConsumer();


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;


};


class FuncDeclAction : public clang::ASTFrontendAction
{
   clang::Rewriter _reWriter;

public:
   

   FuncDeclAction();

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

   virtual void EndSourceFileAction() override;


};

