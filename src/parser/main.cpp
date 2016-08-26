#include <iostream>


//#define __STDC_LIMIT_MACROS
//#define __STDC_CONSTANT_MACROS

#include "utils.h"

#include <clang/Tooling/Tooling.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <clang-c/Index.h>
#include <clang-c/CXCompilationDatabase.h>

/*
TEST (clang::tooling::runToolOnCode, CanSyntaxCheckCode )
{
   EXPECT_TRUE(runToolOnCode( new clang::SyntaxOnlyAction, "class X {};") );
}
*/

int main(int argc, char *argv[])
{

   std::cout << "clang parser test!\n";
   
   if (argc < 2 )
   {
      printf("no files to process \n");
      return 0;
   }

   std::vector<boost::filesystem::path> _files;

   CXCompilationDatabase _compilationDatabase;
   CXCompilationDatabase_Error _compilationDatabaseError;

   for (int i=1;i<argc; ++i)
   {
      boost::filesystem::path _path = boost::filesystem::path( argv[i] );

      if ( !boost::filesystem::exists(_path) )
      {
         continue;
      }

      if ( boost::filesystem::is_directory(_path) )
      {
         boost::filesystem::directory_iterator iter_end;
      
         //try to load compilation database
         _compilationDatabase = clang_CompilationDatabase_fromDirectory( _path.string().c_str(), &_compilationDatabaseError );
         //CXCompileCommands compileCommands = clang_CompilationDatabase_getCompilaeCommands(compilationDatabase );
         

         for (boost::filesystem::directory_iterator iter(_path); iter != iter_end; ++iter )
         {

            if (boost::filesystem::is_directory(*iter))
            {
               printf("skip subdirectory %s \n", iter->path().string().c_str() );
               continue;
            }

            if (boost::filesystem::is_regular_file(*iter) )
            {
               std::string ext = iter->path().extension().string();
               
               if ( ext == ".c" || ext ==".cpp" ) //|| ext == ".cpp" ) 
               {
                  _files.push_back(*iter);
               
                  printf("process file: %s  \n", iter->path().string().c_str() );
               }
               else
               {
                  printf("skip file %s  \n",  iter->path().string().c_str() );
               }
            }
         }
       }
       else if (boost::filesystem::is_regular_file(_path) )
       {

          //_compilationDatabase = clang_CompilationDatabase_fromDirectory( ".", &_compilationDatabaseError );
 
          std::string ext = _path.extension().string();
            
          if ( ext == ".c"  || ext ==".cpp" ) //|| ext == ".cpp" ) 
          {
               _files.push_back(_path);
            
               printf("process file: %s  \n", _path.string().c_str() );
            }
            else
            {
               printf("skip file %s  \n",  _path.string().c_str() );
            }
       }     
   }

      /*
   boost::program_options::options_description options_desc("desc");
   option_desc.add_options();
   boost::program_options:command_line_parser parser = boost::program_options::parse_command_line(argc, argv, options_desc);
   boost::program_options::variables_map var_map;
   boost::program_options::store(  parser, var_map );
   boost::program_options::notify(var_map);
   */

   
   if (_files.size() == 0)
   {
      printf("no files to process");
      //return 0;
   }

   if (  _compilationDatabaseError  )
   {

      _compilationDatabase = clang_CompilationDatabase_fromDirectory( ".", &_compilationDatabaseError );
      if (  !_compilationDatabaseError  )
      {
         printf("a compilation database found\n");
      }
   }

   std::map<boost::filesystem::path, CXTranslationUnit> _pathUnitMap;

   // clang
   int _excludeDeclsFromPCH = 1;
   int _displayDiagnostic = 1;
   CXIndex  _index = clang_createIndex( _excludeDeclsFromPCH, _displayDiagnostic);

   std::vector<const char*> _arguments;

   //arguments.push_back("-I" SYSYTEM_INCLUDE_DIRECTORY );
   _arguments.push_back("-std=c++11");
   _arguments.push_back("-D__STDC_CONSTANT_MACROS");
   _arguments.push_back("-D__STDC_LIMIT_MACROS");

   CXCompileCommands _compileCommands; 
   CXCompileCommand _command;
   unsigned int _numArgs = 0;
   char** _args;

   for ( auto file : _files )
   {
 
       printf("creating translation unit for file: %s \n", file.string().c_str() );

      CXTranslationUnit _translationUnit;

      if (  !_compilationDatabaseError  )
      {
         printf("-clang_CompilationDatabase_getCompileCommands\n");
         _compileCommands = clang_CompilationDatabase_getCompileCommands(_compilationDatabase, file.string().c_str());
         printf("-clang_CompileCommands_getCommand\n");
         _command = clang_CompileCommands_getCommand(_compileCommands, 0);
         printf("-utils::fillArguments\n");
         _numArgs = utils::fillArguments(_args, _command);
     
         utils::printArguments(_numArgs, _args);

         _translationUnit = clang_parseTranslationUnit( _index, 0, _args, _numArgs, nullptr,  0, 0  );

          utils::clearArguments(_numArgs, _args);
      }
      else
      {
          _translationUnit = clang_createTranslationUnitFromSourceFile(
             _index, file.string().c_str(),  _arguments.size(), _arguments.data(), 0, nullptr  );

      }

       utils::printDiagnostics(_translationUnit);
         


      CXCursor _rootCursor = clang_getTranslationUnitCursor(_translationUnit);

      //std::vector<CXCursor> _children;

      //utils::printCursorTokens(_translationUnit, _rootCursor);

      unsigned int res = clang_visitChildren(_rootCursor, *utils::cursorVisitor, 0);

      _pathUnitMap[file.string().c_str()] = _translationUnit;

   }

   //try to load compile database files
   if (_files.size() == 0)
   {

   }

   for (auto map : _pathUnitMap)
   {
      clang_disposeTranslationUnit(map.second);
   }

   if (!_compilationDatabaseError)
   {
      clang_CompilationDatabase_dispose(  _compilationDatabase );
   }

   clang_disposeIndex(_index);

   return 0;
}
