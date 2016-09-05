
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "writer.h"
#include "utils.h"

#include <boost/filesystem/convenience.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/PrettyPrinter.h>
#include <plustache/plustache_types.hpp>
#include <plustache/template.hpp>

using std::cout;
using std::string;
using std::ifstream;
using std::stringstream;
using PlustacheTypes::ObjectType;
using PlustacheTypes::CollectionType;
using Plustache::Context;
using Plustache::template_t;



std::shared_ptr<const Plustache::Context> Writer::CreateMockContext(const std::set<std::string>                  &includePaths,
                                                                    const std::set<const clang::FunctionDecl*>   &funcDecls,
                                                                    const std::string                            &fileName,
                                                                    const clang::SourceManager                   &sourceMgr){
   
   std::shared_ptr<Plustache::Context> context = std::make_shared<Plustache::Context>();
   
   //Context              *c = new Context();
   ObjectType            Include;
   ObjectType            Mock;
   std::ostringstream    out;
   
   for(auto iter : includePaths){
      Include["include"] = iter;
      context->add("includes", Include);      
   }
   

   for ( auto iter : funcDecls ) {
      Writer::MockFunctionFFF( iter, out, sourceMgr);
      Mock["definition"] = out.str();
      context->add("mocks", Mock);
      out.str("");
   }
   
   context->add("filename", fileName);
   context->add("newline", "\n");
   
   return context;
}



void Writer::WriteTemplate(std::shared_ptr<const Plustache::Context>      context,
                                 const std::string& templateFileName,
                                 const std::string                      &outFileName){
   template_t     t;
   stringstream   buffer;
   string         result;
   
   boost::filesystem::path full_path( boost::filesystem::current_path() );
   
   std::cout << full_path.string();
   
   ifstream template_file(templateFileName);
   
   if (template_file.fail() )
   {
      std::cout << "template file for mock functions not found";
      return;
   }
   
   buffer << template_file.rdbuf();
   string         template_buff(buffer.str());
   
   result = t.render(template_buff, *context);
  
   //if ( context.get("includes") )
   //delete context.get("includes");
   //delete context.get("mocks");
   
   std::ofstream outputFile;   
   std::string outputFileName = outFileName;   
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << result;
   outputFile.close();
  
   std::cout << "file written: " << outputFileName << std::endl;
}



void Writer::MockFunctionFFF(const clang::FunctionDecl   *funcDecl,
                             std::ostringstream          &out,
                             const clang::SourceManager  &sourceMgr){
   
   // get declaration source location
   const clang::SourceLocation declSrcLoc = funcDecl->getSourceRange().getBegin();
   // this way append the row and column to the name string
   const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);

   out << "/**" <<std::endl;
   out << " * name: " << funcDecl->getNameInfo().getName().getAsString() << std::endl;
   out << " * file: " << declSrcFile << std::endl;
   out << " */" << std::endl;   
   
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

   std::shared_ptr<const Plustache::Context> context = CreateMockContext(includePaths, results::get().functionDecls, fileName, sourceMgr);

   Writer::WriteTemplate(context, std::string("../../mock.template"), fileName + "-mocks.h");
   
}



std::shared_ptr<const Plustache::Context> Writer::CreateUnitTestContext(const std::set<std::string>                  &includePaths,
                                                                    const std::set<const clang::FunctionDecl*>   &functionToUnitTest,
                                                                    const std::string                            &fileName,
                                                                    const clang::SourceManager                   &sourceMgr){
   
   std::shared_ptr<Plustache::Context> context = std::make_shared<Plustache::Context>();
   
   //Context              *c = new Context();
   ObjectType            Include;
   ObjectType            FunctionToUnitTest;
   
   for(auto iter : includePaths){
      Include["include"] = iter;
      context->add("includes", Include);      
   }
   // add mock file in includes list
   Include["include"] = boost::filesystem::path(fileName).filename().string() + "-mocks.h";
   context->add("includes", Include); 

   for ( auto iter : functionToUnitTest ) {
      FunctionToUnitTest["functionName"] = iter->getNameAsString();
      context->add("functionToUnitTest", FunctionToUnitTest);
   }
   
   // create a C++ class name from the fileName
   std::string TestFilename = boost::filesystem::path(fileName).filename().string();
   TestFilename = utils::removeDashes(TestFilename);
   TestFilename = utils::removeFileExtension(TestFilename);
   
   context->add("testFilename", TestFilename);   
   context->add("filename", fileName);
   context->add("newline", "\n");
   
   return context;
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

      includePaths.insert( utils::changeFileExtension(p.filename().string(), "h") );
   }

   std::shared_ptr<const Plustache::Context> context = CreateUnitTestContext(includePaths, results::get().functionToUnitTest, fileName, sourceMgr);
   
   Writer::WriteTemplate(context, std::string("../../UT.template"), fileName + "-ugtest.c" );
   
//    out << "/* @owner \\TODO */\n";
//    out << "/**\n";
//    out << " * @file  " << fnameUT << "-ugtest.c \n";
//    out << " * @brief \\TODO.\n";
//    out << " *\n";
//    out << " * @copyright Copyright of this program is the property of AMADEUS, without\n";
//    out << " * whose written permission reproduction in whole or in part is prohibited.\n";
//    out << " *\n";
//    out << " */\n\n";
// 
//    out << "extern \"C\"{\n";
//    for ( auto include : includePaths ){
//       out << "#include \"" << include <<  "\"\n";
//    }
//    out << "#include \"" << fnameUT << "-mocks.h\"\n";
//    out << "}\n\n";
//    out << "#include <gtest/gtest.h>\n\n";
// 
//    out << "class Test_" << fnameUT << " : public ::testing::Test\n";
//    out << "{\n";
//    out << "protected:\n";
//    out << "   void SetUp()\n{\n}\n";
//    out << "   void TearDown()\n{\n}\n";
//    out << "};\n";
// 
// 
//    
//    out << "int main(int argc, char *argv[])\n{\n";
//    out << "   ::testing::InitGoogleTest(&argc, argv);\n";
//    out << "   return RUN_ALL_TESTS();\n}\n";


//    std::ofstream outputFile;
//    std::string outputFileName = fileName + "-ugtest.c";
//    outputFile.open( outputFileName, std::fstream::out );
//    outputFile << out.str();
//    outputFile.close();
  
//    std::cout << "file written: " << outputFileName << std::endl;

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
   
   
   
   for ( auto typedefDecl : results::get().typedefNameDecls )
   {
      const clang::QualType typedefQualType = typedefDecl->getUnderlyingType(); // ->getCanonicalTypeInternal();
      
       // get declaration source location
      const clang::SourceLocation declSrcLoc = typedefDecl->getSourceRange().getBegin();
      
      // this way append the row and column to the name string
      const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);

      out << "/**" <<std::endl;
      out << " * name: " << typedefDecl->getNameAsString() << std::endl;
      out << " * file: " << declSrcFile << std::endl;
      out << " */" << std::endl;
         
     
      if( const clang::RecordType* structType = typedefQualType->getAsStructureType() ){

         const clang::RecordDecl* structDecl = structType->getDecl();
         out << "typedef struct " << structDecl->getNameAsString() << "\n{\n";
      
         for ( const auto field : structDecl->fields() ){
            out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
         }
      
         out << "} " << typedefDecl->getNameAsString() << ";\n\n";
      }
      
      else if ( const clang::EnumType* enumType = typedefQualType->getAs<clang::EnumType>() ) {
   
         const clang::EnumDecl* enumDecl = enumType->getDecl();
         
         out << "typedef enum " << enumDecl->getNameAsString()  << " {\n";
         
         for ( auto iter : enumDecl->enumerators() )
         {
            out << "   " << iter->getNameAsString() << "\t = " << iter->getInitVal().toString(10) << ",\n";
         }
         
         const std::string qualTypeName = typedefQualType.getAsString();
         
         const clang::QualType canonicalQualType = typedefQualType->getCanonicalTypeInternal();
         const std::string canonicalTypeName = canonicalQualType.getAsString();
         
         out << "} " << canonicalTypeName << ";\n\n";         
      }
      
      else if ( const clang::TypedefType* typedefType = typedefQualType->getAs<clang::TypedefType>() ) {
         
         const clang::TypedefNameDecl* typedefDecl = typedefType->getDecl();
         
         const clang::QualType canonicalQualType = typedefType->getCanonicalTypeInternal();
         const std::string canonicalTypeName = canonicalQualType.getAsString();
         
         out << "typedef " << canonicalTypeName << " " << typedefDecl->getNameAsString()  << "\n\n";
         
      }

      else if ( const clang::BuiltinType* typedefType = typedefQualType->getAs<clang::BuiltinType>() ) {
         
         //const clang::BuiltinDecl* typedefDecl = typedefType->getDecl();
         
         //const clang::QualType canonicalQualType = typedefType->getCanonicalTypeInternal();
         //const std::string canonicalTypeName = canonicalQualType.getAsString();
         
         //const clang::QualType canonicalQualType = typedefDecl->getCanonicalDecl()->getUnderlyingType();
         out << "typedef " << typedefQualType.getAsString() << " " << typedefDecl->getNameAsString()  << ";\n\n";
         
         
         //out << "typedef " << typedefType->getName( clang::PrintingPolicy(clang::LangOptions()) ).str() << " " << typedefDecl->getNameAsString()  << ";\n";
         
      }
      
   }
   

//   for ( auto structDecl : results::get().istructDecls )
//   {
//      // get declaration source location
//      const clang::SourceLocation declSrcLoc = structDecl->getSourceRange().getBegin();
//      
//      // this way append the row and column to the name string
//      const std::string declSrcFile = declSrcLoc.printToString(sourceMgr);
//
//      out << "/**" <<std::endl;
//      out << " * name: " << structDecl->getNameAsString() << std::endl;
//      out << " * file: " << declSrcFile << std::endl;
//      out << " */" << std::endl;
//      
//      if ( structDecl->isAnonymousStructOrUnion() )
//      {
//         out << "typedef struct " << "\n{\n";
//         
//         for ( const auto field : structDecl->fields() )
//         {
//            out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
//         }
//         
//         out << "} " << structDecl->getNameAsString() << ";\n\n";
//      }
//      else
//      {
//         out << "struct " << structDecl->getNameAsString() << "\n{\n";
//          
//         for ( const auto field : structDecl->fields() )
//         {
//            out << "   " << field->getType().getAsString() << "\t" << field->getNameAsString() << "\n";
//         }
//         
//         out << "};\n\n";
//      }
//  
//   } 

   
   
   std::ofstream outputFile;
   std::string outputFileName = fileName + "-serializer.h";
   outputFile.open( outputFileName, std::fstream::out );
   outputFile << out.str();
   outputFile.close();

   std::cout << "file written: " << outputFileName << std::endl;

}

