#include "writer.h"

#include "utils.h"

#include <clang/AST/Decl.h>
#include <boost/filesystem/convenience.hpp>

#include <iostream>
#include <fstream>


void Writer::BeginFFF(std::ostringstream& out, const std::set<std::string>& includePaths)
{

   out << "/** " << std::endl;
   out << " *\n *\n *\n *\n *\n *" << std::endl;
   out << " */" << std::endl << std::endl << std::endl;


   out << "// Include test and utilities mock headers\n";
   out << "#include \"fff.h\"\n" << std::endl;

   for(  auto include : includePaths ){
      out << "#include \"" << include << "\"\n";
   }
   out << "\nDEFINE_FFF_GLOBALS;\n\n";

   out << "/**" << std::endl;
   out << " *\tMOCKED FUNCTIONS BEGIN" << std::endl;
   out << " */" << std::endl << std::endl;

}

void Writer::EndFFF(std::ostringstream& out)
{
   out << std::endl;
   out << "/**" << std::endl;
   out << " *\tMOCKED FUNCTIONS END" << std::endl;
   out << " */" << std::endl << std::endl;

}
void Writer::MockFunctionFFF(const clang::FunctionDecl* funcDecl, std::ostringstream& out)
{
         
   std::string returnType = funcDecl->getReturnType().getAsString();

   std::string isVariadic;

   if ( funcDecl->isVariadic() )
   {
      isVariadic = "_VARARG";
   }

   if ( returnType == "void" )
   {
      out << "FAKE_VOID_FUNC" << isVariadic << "( ";
   }
   else
   {
      out << "FAKE_VALUE_FUNC" << isVariadic << "( " << returnType << ", ";
   }
   

   out << funcDecl->getNameInfo().getName().getAsString();
       
   const int numParms = funcDecl->getNumParams();           

   for ( int i=0; i<numParms; ++i)
   {
      const clang::ParmVarDecl* _currentParam = funcDecl->getParamDecl(i);
      out << ", " << _currentParam->getType().getAsString(); 
   }

   if ( funcDecl->isVariadic() )
   {
      out << ", ...";
   }

   out <<" );";

   out << std::endl;

}



void Writer::CreateMockFile(const std::string& fileName, const clang::SourceManager& sourceMgr)
{
   std::ostringstream out;
   std::set<std::string> includePaths;

   //const clang::SourceManager& sourceMgr = _compiler->getSourceManager();

   // look for paths to include in the mock file
   for ( auto funcDecl : results::get().functionDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
      const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();

      boost::filesystem::path p(declSrcFile);

      includePaths.insert( p.filename().string() );
   }

   Writer::BeginFFF( out, includePaths );

   for ( auto funcDecl : results::get().functionDecls )
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
   std::string outputFileName = fileName + "-mocks.h";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();
  
   std::cout << "file written: " << outputFileName << std::endl;
}




void Writer::CreateUnitTestFile(const std::string& fileName, const clang::SourceManager& sourceMgr)
{
   std::ostringstream out;

   boost::filesystem::path fpathUT(fileName);
   std::string fnameUT = fpathUT.filename().string();

   std::set<std::string> includePaths;

   //const clang::SourceManager& sourceMgr = _compiler->getSourceManager();

   // look for paths to include in the mock file
   for ( auto funcDecl : results::get().functionToUnitTest )
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
   std::string outputFileName = fileName + "-ugtest.c";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();
  
   std::cout << "file written: " << outputFileName << std::endl;

}


void Writer::CreateSerializationFile(const std::string& fileName, const clang::SourceManager& sourceMgr){
   std::ostringstream out;

   boost::filesystem::path fpathUT(fileName);
   std::string fnameUT = fpathUT.filename().string();

   std::set<std::string> includePaths;

   for ( auto typedefDecl : results::get().typedefNameDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = typedefDecl->getSourceRange().getBegin();
      const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();
      const std::string includeFile =  boost::filesystem::path(declSrcFile).filename().string();
      
      if ( !includeFile.empty() )
      {
         includePaths.insert( includeFile );
      }
   }

   for ( auto structDecl : results::get().structDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = structDecl->getSourceRange().getBegin();
      const std::string declSrcFile = sourceMgr.getFilename(declSrcLoc).str();
      const std::string includeFile =  boost::filesystem::path(declSrcFile).filename().string();
      
      if ( !includeFile.empty() )
      {
         includePaths.insert( includeFile );
      }
   }


   for ( auto typedefDecl : results::get().typedefNameDecls )
   {
      
      // get declaration source location
      const clang::SourceLocation declSrcLoc = typedefDecl->getSourceRange().getBegin();
      
      // this way append the row and column to the name string
      const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);

      out << "/**" <<std::endl;
      out << " * name: " << typedefDecl->getNameAsString() << std::endl;
      out << " * file: " << declSrcFile << std::endl;
      out << " */" << std::endl;
      
      const clang::RecordDecl* structDecl = typedefDecl->getUnderlyingType()->getAsStructureType()->getDecl();
      out << "typedef struct " << structDecl->getNameAsString() << "\n{\n";
      
      for ( const auto field : structDecl->fields() ){
         out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
      }
      
      out << "} " << typedefDecl->getNameAsString() << ";\n\n";
   }
   
   
   for ( auto structDecl : results::get().structDecls )
   {
      // get declaration source location
      const clang::SourceLocation declSrcLoc = structDecl->getSourceRange().getBegin();
      
      // this way append the row and column to the name string
      const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);

      out << "/**" <<std::endl;
      out << " * name: " << structDecl->getNameAsString() << std::endl;
      out << " * file: " << declSrcFile << std::endl;
      out << " */" << std::endl;
      
      if ( structDecl->isAnonymousStructOrUnion() )
      {
         out << "typedef struct " << "\n{\n";
         
         for ( const auto field : structDecl->fields() )
         {
            out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
         }
         
         out << "} " << structDecl->getNameAsString() << ";\n\n";
      }
      else
      {
         out << "struct " << structDecl->getNameAsString() << "\n{\n";
          
         for ( const auto field : structDecl->fields() )
         {
            out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
         }
         
         out << "};\n\n";
      }
  
   }
   
   out << "/* @owner \\TODO */\n";
   out << "/**\n";
   out << " * @file  " << fnameUT << "-serializer.h \n";
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
   out << "}\n\n\n";
   
   

   
   
   std::ofstream outputFile;
   std::string outputFileName = fileName + "-serializer.h";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();

   std::cout << "file written: " << outputFileName << std::endl;

}

