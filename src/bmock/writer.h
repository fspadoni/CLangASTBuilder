#ifndef _CLangASTBuilder_writer_h__
#define _CLangASTBuilder_writer_h__

#include <clang/Basic/SourceManager.h>

#include <string>
#include <sstream>
#include <set>


namespace clang {
   class FunctionDecl;
}


class Writer 
{

   static void BeginFFF(std::ostringstream&           out,
                        const std::set<std::string>&  includePaths);

   static void EndFFF(std::ostringstream& out);
   
   static void MockFunctionFFF(const clang::FunctionDecl*   funcDecl,
                               std::ostringstream&          out);


public:
   
   static void CreateMockFile(const std::string&            fileName,
                              const clang::SourceManager&   sourceMgr);
   
   static void CreateUnitTestFile(const std::string&           fileName,
                                  const clang::SourceManager&  sourceMgr);
   
   static void CreateSerializationFile(const std::string&            filename,
                                       const clang::SourceManager&   sourceMgr);
   
};


#endif // _CLangASTBuilder_writer_h__

