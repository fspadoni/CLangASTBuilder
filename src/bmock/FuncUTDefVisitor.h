 

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <set>
#include <string>
#include <sstream>

class FuncUTDefVisitor : public clang::RecursiveASTVisitor<FuncUTDefVisitor>
{

   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit FuncUTDefVisitor(clang::ASTContext* context, std::string fileName);

   
   bool VisitDecl(clang::Decl* decl);
   
};






class FuncUTDefConsumer : public clang::ASTConsumer 
{

   FuncUTDefVisitor* _visitor;

public:

   FuncUTDefConsumer(clang::ASTContext*     context,  std::string fileName );


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


class FuncUTDefAction : public clang::ASTFrontendAction
{

public:

   FuncUTDefAction() {};

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

};

