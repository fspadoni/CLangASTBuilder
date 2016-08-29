//#include <iostream>


//#define __STDC_LIMIT_MACROS
//#define __STDC_CONSTANT_MACROS

#include "Action.h"

#include <clang/Tooling/Tooling.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>
#include <clang/Frontend/ASTUnit.h>

#include <clang/AST/Expr.h>


static llvm::cl::OptionCategory testToolCategory("test tool options");

static llvm::cl::extrahelp commonHelp( clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp additionalHelp("\n .... some more help");



int main(int argc, const char *argv[])
{

   clang::tooling::CommonOptionsParser _optionParser(argc, argv,testToolCategory );
 
   clang::tooling::ClangTool _tool(_optionParser.getCompilations(), _optionParser.getSourcePathList() );

   int result = _tool.run( clang::tooling::newFrontendActionFactory<Action>().get() );


   return 0;
}
