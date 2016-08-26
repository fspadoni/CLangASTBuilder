
#include <string>
#include <sstream>
#include <set>


namespace clang {
   class FunctionDecl;
}

class utils 
{

public:


   static void writeBeginFFF(std::ostringstream& out, const std::set<std::string>& includePaths);

   static void writeEndFFF(std::ostringstream& out);
   
   static void writeMockFunctionFFF(const clang::FunctionDecl* funcDecl, std::ostringstream& out);


};
