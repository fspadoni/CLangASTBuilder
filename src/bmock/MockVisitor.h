#ifndef _CLangASTBuilder_MockVisitor_h__
#define _CLangASTBuilder_MockVisitor_h__ 

#include <clang/AST/RecursiveASTVisitor.h>

#include <string>

class MockVisitor : public clang::RecursiveASTVisitor<MockVisitor>
{

   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit MockVisitor(clang::ASTContext*   context,
                        std::string          fileName);
   
   
   //bool VisitStmt(clang::Stmt* stmt);
   
   bool VisitCallExpr(clang::CallExpr *call);
   
};


#endif // _CLangASTBuilder_MockVisitor_h__

