#ifndef _CLangASTBuilder_FuncUTDefVisitor_h__
#define _CLangASTBuilder_FuncUTDefVisitor_h__ 

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



class FuncUTDeclVisitor : public clang::RecursiveASTVisitor<FuncUTDeclVisitor>
{

   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit FuncUTDeclVisitor(clang::ASTContext* context, std::string fileName);

   
   bool VisitDecl(clang::Decl* decl);
   
};



class FuncUTDefConsumer : public clang::ASTConsumer 
{

   FuncUTDefVisitor* _defVisitor;
   FuncUTDeclVisitor* _declVisitor;

public:

   FuncUTDefConsumer(clang::ASTContext*     context,  std::string fileName );


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


class FuncUTDefAction : public clang::ASTFrontendAction
{

public:

   FuncUTDefAction() {};

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;
   
   virtual void EndSourceFileAction() override;

};

#endif // _CLangASTBuilder_FuncUTDefVisitor_h__