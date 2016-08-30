
#include "Consumer.h"

#include "MockVisitor.h"
#include "FuncUTDefVisitor.h"
#include "StructVisitor.h"
#include "TypedefVisitor.h"

#include <clang/AST/ASTContext.h>


Consumer::Consumer(clang::ASTContext*  context,
                   std::string         fileName )
   : ASTConsumer()
   , _mockVisitor( nullptr )
   , _defVisitor( nullptr )
   , _declVisitor( nullptr )
   , _typedefVisitor( nullptr )
   , _structVisitor( nullptr )
{
   _mockVisitor =  new MockVisitor( context, fileName );
   _defVisitor = new FuncUTDefVisitor( context, fileName );
   _declVisitor = new FuncUTDeclVisitor( context, fileName );
   _typedefVisitor = new TypedefVisitor( context, fileName );
   _structVisitor = new StructVisitor( context, fileName );
}


void Consumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _mockVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
   _defVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
   _declVisitor->TraverseDecl(ctx.getTranslationUnitDecl());   
   _typedefVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
   _structVisitor->TraverseDecl(ctx.getTranslationUnitDecl());
}

