#include "utils.h"

#include <clang-c/Index.h>
#include <clang-c/Platform.h>

#include <stdio.h>



/*
std::vector<CXCursor> utils::visitAST(CXCursor cursor)
{
 
   std::vector<CXCursor> children;

   clang_visitChildren(cursor, std::bind(utils::visitCursor, _3), &children );

   return children
}
*/

CXChildVisitResult  utils::cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData data )
{
   CXCursorKind kind = clang_getCursorKind(cursor);
   CXString name = clang_getCursorSpelling(cursor);
   CXString unifiedSymbolRes = clang_getCursorUSR(cursor);
   CXModule moduleName = clang_Cursor_getModule(cursor);

   //CXString mangledSymbol = clang_Cursor_getMangling(cursor);
   CXType type = clang_getCursorType(cursor);

   if (kind == CXCursor_FunctionDecl || kind == CXCursor_CXXMethod)
   {
      if ( clang_isCursorDefinition(cursor))
      {
         printf( "function definition: \n"); 
      }
      else
      {
         return CXChildVisit_Recurse;
         printf( "function  declaration: \n"); 
      }

      
      printf("%s %s( ", utils::getTypeSpelling( clang_getCursorResultType(cursor)).c_str(), utils::getCursorSpelling(cursor).c_str() );

      //printf("method: %s kindName: %s \n", utils::getCursorSpelling(cursor).c_str(), utils::getCursorKindName(kind).c_str() );
      //printf("kind name: %s \n", utils::getTypeSpelling(type).c_str());

      int numArgTypes = clang_getNumArgTypes(type);
      //printf("numArgTypes: %d", numArgTypes);
      
      if (numArgTypes != 0)
      {
         // visit method children
         int numParams = numArgTypes;
         clang_visitChildren(cursor, *functionDeclVisitor, &numParams);

         //printf("number of params: %i \n", numParams);

      }
      else
      {
         printf("void");
      }
      
      printf( " );\n" );

      // print function location
      CXSourceLocation location = clang_getCursorLocation(cursor);
      CXString fileName;
      CXFile file;
      unsigned int line, column, offset;
      clang_getFileLocation(location ,&file, &line, &column, &offset);
      printf("function location : %s (line %i, col %i)\n", utils::getFileName(file).c_str(), line, column );
      printf( "Unified Symbol Resource: %s \n", utils::getCString(unifiedSymbolRes).c_str());
      printf( "Module name: %s \n", utils::getCString( clang_Module_getName(moduleName) ).c_str());
      printf("-----------------------------------------\n");      
 
   }

   return CXChildVisit_Recurse;
}


CXChildVisitResult utils::functionDeclVisitor(CXCursor cursor, CXCursor parent, CXClientData data)
{

   CXCursorKind kind = clang_getCursorKind(cursor);
   CXType type = clang_getCursorType(cursor);

   int numArgTypes = clang_getNumArgTypes(type);
   //CXType funcType = clang_getArgType(type);

   if (kind == CXCursor_ParmDecl)
   {
      CXString name = clang_getCursorSpelling(cursor);
      CXString typeString = clang_getTypeSpelling(type);
      const char* typeCstr = clang_getCString(typeString);
      printf(" %s %s", utils::getTypeSpelling(type).c_str(),  utils::getCursorSpelling(cursor).c_str() );
      //if (typeCstr != "\0");
      //   printf("type: %s", typeCstr);

      int *numParams = (int*)data;
      if ( --(*numParams) > 0)
         printf(",");
   }

   return CXChildVisit_Continue;
}


void utils::printDiagnostics(CXTranslationUnit translationUnit)
{
   int numDiagnostics = clang_getNumDiagnostics(translationUnit);
   printf("Number of diagnostics %i \n", numDiagnostics );

   for (int i=0; i<numDiagnostics; ++i)
   {
      CXDiagnostic diagnostic = clang_getDiagnostic(translationUnit, i);
      CXString error = clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions());
      fprintf(stderr, "%s\n", clang_getCString(error));
      clang_disposeString(error);
   }

}

void utils::printCursorTokens(CXTranslationUnit translationUnit, CXCursor currentCursor)
{
   CXToken* tokens;
   unsigned int numTokens;
   
   CXSourceRange sourceRange = clang_getCursorExtent(currentCursor);

   clang_tokenize(translationUnit, sourceRange, &tokens, &numTokens);

   for (int i=0; i<numTokens; ++i)
   {
      printTokenInfo(translationUnit, tokens[i]);

   }

   clang_disposeTokens(translationUnit, tokens, numTokens);
}


void utils::printTokenInfo(CXTranslationUnit translationUnit, CXToken currentToken)
{
   CXString tokenString = clang_getTokenSpelling(translationUnit, currentToken);
   CXTokenKind kind = clang_getTokenKind(currentToken);

   switch(kind)
   {
      case CXToken_Comment:
         printf("Token : %s \t| COMMENT \n", clang_getCString(tokenString));
         break;

      case CXToken_Identifier:
         printf("Token : %s \t| IDENTIFIER \n", clang_getCString(tokenString));
         break;
      
      case CXToken_Keyword:
         printf("Token : %s \t| KEYWORD \n", clang_getCString(tokenString));
         break;

      case CXToken_Punctuation:
         printf("Token : %s \t| PUNCTUATION \n", clang_getCString(tokenString));
         break;

      default:
         break;
   }

}

std::string utils::getCString(CXString cxstring)
{
   std::string result = clang_getCString(cxstring);
   clang_disposeString(cxstring);
   return result;
}

std::string utils::getCursorKindName(CXCursorKind cursorKind)
{
   CXString kindName = clang_getCursorKindSpelling( cursorKind );
   std::string result = clang_getCString(kindName);
   clang_disposeString(kindName);
   return result;
}

std::string utils::getCursorSpelling( CXCursor cursor)
{
   CXString cursorSpelling = clang_getCursorSpelling(cursor);
   std::string result = clang_getCString(cursorSpelling);
   clang_disposeString(cursorSpelling);
   return result;
}

std::string utils::getTypeSpelling(CXType type)
{
   CXString typeSpelling = clang_getTypeSpelling(type);
   std::string result = clang_getCString(typeSpelling);
   clang_disposeString(typeSpelling);
   return result;
}

std::string utils::getFileName(CXFile file)
{
   CXString fileString = clang_getFileName(file);
   std::string result = clang_getCString(fileString);
   clang_disposeString(fileString);
   return result;
}

unsigned int utils::fillArguments( char**& args, CXCompileCommand command )
{
   unsigned int numArgs = clang_CompileCommand_getNumArgs(command);

   args = new char*[numArgs];

   for (int i=0; i<numArgs; ++i)
   {
      CXString argument = clang_CompileCommand_getArg(command, i);
      std::string argString = clang_getCString(argument);
      args[i] = new char[argString.size()+1];

      std::fill(args[i], args[i] + argString.size() + 1, 0);
      std::copy(argString.begin(), argString.end(), args[i]);

      clang_disposeString(argument);
   }

   return numArgs;
}

void utils::printArguments( unsigned int numArgs, char**& args )
{
    for (int i=0; i<numArgs; ++i)
    {
       printf("%s\n", args[i]);
    }

}

void utils::clearArguments( unsigned int numArgs, char**& args )
{
    for (int i=0; i<numArgs; ++i)
    {
       delete[] args[i];
    }

    delete [] args;
}

