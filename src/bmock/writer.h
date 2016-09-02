#ifndef _CLangASTBuilder_writer_h__
#define _CLangASTBuilder_writer_h__

#include <clang/Basic/SourceManager.h>

#include <string>
#include <sstream>
#include <set>


namespace clang {
   class FunctionDecl;
}

namespace Plustache {
   class Context;
}

class Writer 
{   
   
   static std::shared_ptr<const Plustache::Context> CreateMockContext(  const std::set<std::string>            &includePaths,
                                             const std::set<const clang::FunctionDecl*>   &funcDecls,
                                             const std::string                      &fileName,
                                             const clang::SourceManager             &sourceMgr);
   

   static void MockFunctionFFF(const clang::FunctionDecl* const    funcDecl,
                               std::ostringstream&           out,
                               const clang::SourceManager&   sourceMgr);
   
   static void WriteTemplate(   std::shared_ptr<const Plustache::Context>      context,
                                const std::string  &fileName);
   
public:
   
   static void CreateMockFile(const std::string&            fileName,
                              const clang::SourceManager&   sourceMgr);
   
   static void CreateUnitTestFile(const std::string&           fileName,
                                  const clang::SourceManager&  sourceMgr);
   
   static void CreateSerializationFile(const std::string&            filename,
                                       const clang::SourceManager&   sourceMgr);
  
   
   
   
   
};


#endif // _CLangASTBuilder_writer_h__

