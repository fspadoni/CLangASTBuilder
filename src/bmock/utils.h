
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

   /** 
    *	<function name>: removeFileExtension
    *
    *	<function description>: 
    *
    *	@param fileName:  
    *
    *	@return:  
    */ 
   static std::string removeFileExtension(const std::string& fileName);


   /** 
    *	<function name>: changeFileExtension
    *
    *	<function description>: 
    *
    *	@param fileName:  
    *	@param newExtension:  
    *
    *	@return:  
    */ 
   static std::string changeFileExtension(const std::string& fileName, const std::string& newExtension);

};
