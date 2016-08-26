#include "actions.h"

#include "visitors.h"
#include "utils.h"
#include "writer.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>




FuncDeclConsumer::FuncDeclConsumer(clang::ASTContext*   context,
                                   const std::string&   fileName )
   : ASTConsumer()
   , _visitor( nullptr )
{
   _visitor =  new FuncDeclVisitor( context, fileName );
}

FuncDeclConsumer::~FuncDeclConsumer()
{
}


void FuncDeclConsumer::HandleTranslationUnit(clang::ASTContext& ctx) 
{
   _visitor->TraverseDecl(ctx.getTranslationUnitDecl());
}

FuncDeclAction::FuncDeclAction()
   : _compiler(nullptr)
{
}

clang::ASTConsumer* FuncDeclAction::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile)
{
 
   _compiler = &compiler;

   //_reWriter.setSourceMgr( compiler.getSourceManager(), compiler.getLangOpts() );

   _fileName = utils::removeFileExtension( inFile.str() );
   const std::string headerFileName = utils::changeFileExtension(inFile.str(), "h");

   return new FuncDeclConsumer(  &compiler.getASTContext(),  _fileName );
}

void FuncDeclAction::EndSourceFileAction(){
   CreateMockFile();
   CreateUnitTestFile();
}


void FuncDeclAction::CreateMockFile()
{
   std::ostringstream out;
   std::set<std::string> includePaths;

   const clang::SourceManager& sourceMgr = _compiler->getSourceManager();

   // look for paths to include in the mock file
   for ( auto funcDecl : results::functionDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
      const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();

      boost::filesystem::path p(declSrcFile);

      includePaths.insert( p.filename().string() );
   }

   Writer::BeginFFF( out, includePaths );

   for ( auto funcDecl : results::functionDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
      
      //const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();
      
      // this way append the row and column to the name string
      const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);

      out << "/**" <<std::endl;
      out << " * name: " << funcDecl->getNameInfo().getName().getAsString() << std::endl;
      out << " * file: " << declSrcFile << std::endl;
      out << " */" << std::endl;
     
      Writer::MockFunctionFFF(funcDecl, out);
      
      out << std::endl << std::endl; 
   }

   std::ofstream outputFile;
   std::string outputFileName = _fileName + "-mocks.h";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();
  
   std::cout << "file written: " << outputFileName << std::endl;
}




void FuncDeclAction::CreateUnitTestFile()
{
   std::ostringstream out;

   boost::filesystem::path fpathUT(_fileName);
   std::string fnameUT = fpathUT.filename().string();

   std::set<std::string> includePaths;

   const clang::SourceManager& sourceMgr = _compiler->getSourceManager();

   // look for paths to include in the mock file
   for ( auto funcDecl : results::functionToUnitTest )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
      const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();

      boost::filesystem::path p(declSrcFile);

      includePaths.insert( p.filename().string() );
   }

   out << "/* @owner \\TODO */\n";
   out << "/**\n";
   out << " * @file  " << fnameUT << "-ugtest.c \n";
   out << " * @brief \\TODO.\n";
   out << " *\n";
   out << " * @copyright Copyright of this program is the property of AMADEUS, without\n";
   out << " * whose written permission reproduction in whole or in part is prohibited.\n";
   out << " *\n";
   out << " */\n\n";

   out << "extern \"C\"{\n";
   for ( auto include : includePaths ){
      out << "#include \"" << include <<  "\"\n";
   }
   out << "#include \"" << fnameUT << "-mocks.h\"\n";
   out << "}\n\n";
   out << "#include <gtest/gtest.h>\n\n";

   out << "class Test_" << fnameUT << " : public ::testing::Test\n";
   out << "{\n";
   out << "protected:\n";
   out << "   void SetUp()\n{\n}\n";
   out << "   void TearDown()\n{\n}\n";
   out << "};\n";


   
   out << "int main(int argc, char *argv[])\n{\n";
   out << "   ::testing::InitGoogleTest(&argc, argv);\n";
   out << "   return RUN_ALL_TESTS();\n}\n";


   std::ofstream outputFile;
   std::string outputFileName = _fileName + "-ugtest.c";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();
  
   std::cout << "file written: " << outputFileName << std::endl;

}

