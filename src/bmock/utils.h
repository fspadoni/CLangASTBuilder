#ifndef _CLangASTBuilder_utils_h__
#define _CLangASTBuilder_utils_h__

#include <string>
#include <set>


namespace clang {
   class FunctionDecl;
}


namespace results {
   std::set<const clang::FunctionDecl*>  functionDecls;
   std::set<const clang::FunctionDecl*>  functionToUnitTest;
   std::set<std::string>           includesForUT;
}



class utils 
{

public:
   

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


#endif // _CLangASTBuilder_utils_h__