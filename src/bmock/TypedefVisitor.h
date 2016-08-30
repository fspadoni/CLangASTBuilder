#ifndef _CLangASTBuilder_TypedefVisitor_h__
#define _CLangASTBuilder_TypedefVisitor_h__ 

#include <clang/AST/RecursiveASTVisitor.h>

#include <string>

class TypedefVisitor : public clang::RecursiveASTVisitor<TypedefVisitor>
{
   clang::ASTContext *_context;
   std::string _fileName;

public:

   explicit TypedefVisitor(clang::ASTContext*    context,
                          std::string           fileName);

   
   bool VisitDecl(clang::Decl* decl);
   
};


#endif // _CLangASTBuilder_TypedefVisitor_h__

