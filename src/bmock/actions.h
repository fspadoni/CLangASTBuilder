
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <set>
#include <string>
#include <sstream>


class FuncDeclVisitor;


class FuncDeclConsumer : public clang::ASTConsumer 
{

   FuncDeclVisitor* _visitor;

public:

   FuncDeclConsumer(clang::ASTContext*     context,
                    const std::string&     fileNamePath );

   virtual ~FuncDeclConsumer();

   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;

};


class FuncDeclAction : public clang::ASTFrontendAction
{
   clang::CompilerInstance* _compiler;
   
   std::ostringstream _out;
   std::string _fileName;

   void CreateMockFile(void);
   void CreateUnitTestFile(void);

public:

   FuncDeclAction();

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

   virtual void EndSourceFileAction() override;

};

