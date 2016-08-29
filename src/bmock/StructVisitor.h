#ifndef _CLangASTBuilder_StructVisitor_h__
#define _CLangASTBuilder_StructVisitor_h__ 

#include <clang/AST/RecursiveASTVisitor.h>

#include <string>

class StructVisitor : public clang::RecursiveASTVisitor<StructVisitor>
{
   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit StructVisitor(clang::ASTContext*    context,
                          std::string           fileName);

   
   bool VisitDecl(clang::Decl* decl);
   
};


#endif // _CLangASTBuilder_StructVisitor_h__

