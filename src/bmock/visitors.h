#ifndef _CLangASTBuilder_visitors_h__
#define _CLangASTBuilder_visitors_h__ 

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <set>
#include <string>
#include <sstream>

class FuncDeclVisitor : public clang::RecursiveASTVisitor<FuncDeclVisitor>
{
   clang::ASTContext *_context;
   const std::string _fileName;

public:

   explicit FuncDeclVisitor(clang::ASTContext*                     context,
                            const std::string&                     fileName);

   bool VisitDecl(clang::Decl* decl);
   
   bool VisitStmt(clang::Stmt* stmt);
};


#endif // _CLangASTBuilder_visitors_h__