#include "matchers.h"

#include <clang/Basic/SourceManager.h>

#include "utils.h"


#include <iostream>
#include <sstream>

void CallFuncMatchCallback::run(const clang::ast_matchers::MatchFinder::MatchResult& result)
{

   const clang::SourceManager* _srcMgr = result.SourceManager;

      
   if ( const clang::CallExpr* _callexpr =  result.Nodes.getNodeAs<clang::CallExpr>("callfunc") )
   {
      clang::SourceLocation srcLoc = _callexpr->getSourceRange().getBegin();

      std::string fileName = _srcMgr->getFilename(srcLoc).str();

      const clang::Decl* _funcDecl = _callexpr->getCalleeDecl();
   }

   return;
}
