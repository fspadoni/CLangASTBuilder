//#include <iostream>


//#define __STDC_LIMIT_MACROS
//#define __STDC_CONSTANT_MACROS

#include "actions.h"
#include "matchers.h"

#include <clang/Tooling/Tooling.h>


#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>
#include <clang/Frontend/ASTUnit.h>

#include <clang/AST/Expr.h>

//#include <boost/filesystem.hpp>
//#include <boost/filesystem/path.hpp>
//#include <boost/program_options/parsers.hpp>
//#include <boost/program_options/options_description.hpp>
//#include <boost/program_options/variables_map.hpp>

#include <vector>
#include <memory>

/*
TEST (clang::tooling::runToolOnCode, CanSyntaxCheckCode )
{
   EXPECT_TRUE(runToolOnCode( new clang::SyntaxOnlyAction, "class X {};") );
}
*/

static llvm::cl::OptionCategory testToolCategory("test tool options");

static llvm::cl::extrahelp commonHelp( clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp additionalHelp("\n .... some more help");



int main(int argc, const char *argv[])
{

   clang::tooling::CommonOptionsParser _optionParser(argc, argv,testToolCategory );
 
   clang::tooling::ClangTool _tool(_optionParser.getCompilations(), _optionParser.getSourcePathList() );

   int result = _tool.run( clang::tooling::newFrontendActionFactory<FuncDeclAction>().get() );


   //CallFuncMatchCallback _callFuncCallback;
   //clang::ast_matchers::MatchFinder _matchFinder;
   //clang::ast_matchers::StatementMatcher _callFunc = clang::CallExpr(); //callee(), &_callFuncCallback );

   return 0;
}
