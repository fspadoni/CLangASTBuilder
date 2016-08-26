#ifndef _CLangASTBuilder_writer_h__
#define _CLangASTBuilder_writer_h__

#include <string>
#include <sstream>
#include <set>


namespace clang {
   class FunctionDecl;
}


class Writer 
{

public:


   static void BeginFFF(std::ostringstream& out, const std::set<std::string>& includePaths);

   static void EndFFF(std::ostringstream& out);
   
   static void MockFunctionFFF(const clang::FunctionDecl* funcDecl, std::ostringstream& out);


};


#endif // _CLangASTBuilder_writer_h__