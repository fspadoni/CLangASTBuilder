
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
   // clang::Rewriter& _reWriter;
   //std::ostringstream& _out;
   const std::string _fileName;

   std::set<const clang::FunctionDecl*>& _functionDecls;
   std::set<const clang::FunctionDecl*>& _functionToUnitTest;

public:

   //explicit FuncDeclVisitor( clang::Rewriter& reWriter, const std::string& fileNamePath );
   explicit FuncDeclVisitor(clang::ASTContext*                     context,
                            std::set<const clang::FunctionDecl*>&  functionDecls,
                            std::set<const clang::FunctionDecl*>&  functionToUnitTest,
                            const std::string&                     fileName);


   bool VisitDecl(clang::Decl* decl);

   
   bool VisitStmt(clang::Stmt* stmt);
   
};


