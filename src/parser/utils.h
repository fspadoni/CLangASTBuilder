#include <clang-c/Index.h>
#include <clang-c/CXCompilationDatabase.h>

#include <vector>
#include <string>


class utils 
{

public:

   //static std::vector<CXCursor> visitAST(CXCursor cursor);

   static CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData data );

   static CXChildVisitResult functionDeclVisitor(CXCursor cursor, CXCursor parent, CXClientData data);

   
   static void printDiagnostics(CXTranslationUnit translationUnit);

   static void printCursorTokens(CXTranslationUnit translationUnit, CXCursor currentCursor);

   static void printTokenInfo(CXTranslationUnit translationUnit, CXToken currentToken);

   static std::string getCString(CXString cxstring); 
   
   static std::string getCursorKindName(CXCursorKind cursorKind);

   static std::string getCursorSpelling( CXCursor cursor);

   static std::string getTypeSpelling(CXType type);

   static std::string getFileName(CXFile file);

   static unsigned int fillArguments( char**& args, CXCompileCommand command );

   static void printArguments( unsigned int numArgs, char**& args );
   
   static void clearArguments(unsigned int numArgs, char**& args  );

};
