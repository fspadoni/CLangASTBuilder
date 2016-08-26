 

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <set>
#include <string>
#include <sstream>

class FuncUTDeclVisitor : public clang::RecursiveASTVisitor<FuncUTDeclVisitor>
{

   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit FuncUTDeclVisitor(clang::ASTContext* context, std::string fileName);

   
   bool VisitDecl(clang::Decl* decl);
   
};






class FuncUTDeclConsumer : public clang::ASTConsumer 
{

   FuncUTDeclVisitor* _visitor;

public:

   FuncUTDeclConsumer(clang::ASTContext*     context,  std::string fileName );


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


class FuncUTDeclAction : public clang::ASTFrontendAction
{

public:

   FuncUTDeclAction() {};

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

};

 
