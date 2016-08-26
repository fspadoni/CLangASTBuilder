#ifndef _CLangASTBuilder_actions_h__
#define _CLangASTBuilder_actions_h__

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

//#include <clang/AST/RecursiveASTVisitor.h>
//#include <clang/AST/ASTConsumer.h>
//#include <clang/AST/ASTContext.h>
//#include <clang/Frontend/FrontendAction.h>
//#include <clang/Frontend/CompilerInstance.h>
//#include <clang/Rewrite/Core/Rewriter.h>

//#include <vector>
//#include <string>


class CallFuncMatchCallback : public clang::ast_matchers::MatchFinder::MatchCallback
{

public:
   void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

};


#endif // _CLangASTBuilder_actions_h__