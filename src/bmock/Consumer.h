#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <clang/AST/ASTConsumer.h>
#include <string>

class FuncUTDefVisitor;
class FuncUTDeclVisitor;
class StructVisitor;
class TypedefVisitor;
class MockVisitor;

class Consumer : public clang::ASTConsumer
{

   FuncUTDefVisitor*    _defVisitor;
   FuncUTDeclVisitor*   _declVisitor;
   StructVisitor*       _structVisitor;
   MockVisitor*         _mockVisitor;
   TypedefVisitor*	_typedefVisitor;

public:

   Consumer(clang::ASTContext*   context,
            std::string          fileName);

   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


#endif /* !__CONSUMER_H__ */

