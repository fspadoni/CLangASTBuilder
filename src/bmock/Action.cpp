
#include "Action.h"

#include "Consumer.h"
#include "writer.h"

#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>

clang::ASTConsumer* Action::CreateASTConsumer(clang::CompilerInstance& compiler,
                                              llvm::StringRef inFile)
{
   return new Consumer(  &compiler.getASTContext(), inFile.str() );
}


void Action::EndSourceFileAction(){
   Writer::CreateMockFile(  getCurrentFile().str(), getCompilerInstance().getSourceManager() );
   Writer::CreateUnitTestFile(  getCurrentFile().str(), getCompilerInstance().getSourceManager() );
   Writer::CreateSerializationFile(  getCurrentFile().str(), getCompilerInstance().getSourceManager() );
}

