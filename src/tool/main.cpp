//#include <iostream>


//#define __STDC_LIMIT_MACROS
//#define __STDC_CONSTANT_MACROS

#include "actions.h"

#include <clang/Tooling/Tooling.h>


#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>
#include <clang/Frontend/ASTUnit.h>


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

   //clang::DiagnosticConsumer _diagConsumer();
   //_tool.setDiagnosticConsumer(&_diagConsumer);

   
   //FuncDeclAction _funDeclAction;
   //std::unique_ptr<clang::tooling::FrontendActionFactory> _factory = clang::tooling::newFrontendActionFactory(&_funDeclFactory);
   int result = _tool.run( clang::tooling::newFrontendActionFactory<FuncDeclAction>().get()  );
   //std::vector< std::unique_ptr< clang::ASTUnit > > _ASTs;
   //_tool.buildASTs( _ASTs );

   //clang::CompilerInstance _compiler;
   //clang::DiagnosticOptions _diagOptions;
   //_compiler.createDiagnostics();

   return 0;
}
