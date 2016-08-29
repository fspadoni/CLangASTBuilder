#ifndef _CLangASTBuilder_utils_h__
#define _CLangASTBuilder_utils_h__

#include <string>
#include <set>

namespace clang {
   class FunctionDecl;
   class RecordDecl;
}


class results {
  
   results(void) {};
   ~results(void) {};
   
public:
   static results& get(void) {
      static results instance;
      return instance;
   };
   
   std::set<const clang::FunctionDecl*>  functionDecls;
   std::set<const clang::FunctionDecl*>  functionToUnitTest;
   std::set<const clang::RecordDecl*>    structDecls;
   std::set<std::string>                 includesForUT;
};


class utils 
{

public:
   
   static std::string removeFileExtension(const std::string& fileName);

   static std::string changeFileExtension(const std::string& fileName, const std::string& newExtension);

};


#endif // _CLangASTBuilder_utils_h__

