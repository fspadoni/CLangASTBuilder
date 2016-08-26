
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
   //clang::Rewriter _reWriter;

public:



   //FuncDeclConsumer(clang::Rewriter& reWriter, const std::string& fileNamePath );
   FuncDeclConsumer(clang::ASTContext*                      context,
                    std::set<const clang::FunctionDecl*>&   functionDecls,
                    std::set<const clang::FunctionDecl*>&   functionToUnitTest,
                    const std::string&                      fileNamePath );

 

   virtual ~FuncDeclConsumer();


   virtual void HandleTranslationUnit(clang::ASTContext& ctx) override;


};


class FuncDeclAction : public clang::ASTFrontendAction
{
   clang::CompilerInstance* _compiler;
   
   //clang::Rewriter _reWriter;
   std::ostringstream _out;
   std::string _fileName;

   std::set<const clang::FunctionDecl*> _functionDecls;
//   std::set<clang::FunctionDecl*> _functionToMock;
   std::set<const clang::FunctionDecl*> _functionToUnitTest;
   void CreateMockFile(void);
   void CreateUnitTestFile(void);

public:
   

   FuncDeclAction();

   virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;

   virtual void EndSourceFileAction() override;


};

