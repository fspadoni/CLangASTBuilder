   if ( func->isThisDeclarationADefinition() )
   {
   
      clang::CXXMethodDecl* method = llvm::dyn_cast<clang::CXXMethodDecl>(func);

      if ( method != nullptr )
      {
         if ( method->isStatic() )
            std::cout << "static ";
         else if (method->isVirtual())
            std::cout << "virtual ";
      }

      std::cout << func->getReturnType().getAsString() << " ";

      if ( method != nullptr )
      {
         const clang::CXXRecordDecl* parent = method->getParent();
         if (parent != nullptr )
         {
            std::cout << parent->getNameAsString() << "::";
         }
      }

      std::cout << func->getNameInfo().getName().getAsString() << "( " ;
          
      const int numParms = func->getNumParams();           
      if ( numParms > 0 )
      {
         int i;
         for ( i=0; i<numParms-1; ++i)
         {
            const clang::ParmVarDecl* _currentParam = func->getParamDecl(i);
            std::cout << _currentParam->getType().getAsString() << " "  <<  _currentParam->getQualifiedNameAsString() << ", "; 
         }

         const clang::ParmVarDecl* _currentParam = func->getParamDecl(numParms-1);
         std::cout << _currentParam->getType().getAsString() << " "  <<  _currentParam->getQualifiedNameAsString();
      }
      else
      {
         std::cout << "void";
      }
      
      std::cout <<" )";

      if ( method != nullptr )
      {
         if ( method->isConst() )
            std::cout << " const";
      }

      std::cout << std::endl;


      // source location
      const clang::SourceLocation srcLoc = func->getSourceRange().getBegin();
      const clang::SourceLocation srcLoc2 = func->getNameInfo().getBeginLoc();

      const clang::SourceManager& srcMgr = _context->getSourceManager();
      const std::string srcFile = srcLoc.printToString(srcMgr);
      //std::cout << srcMgr.getFilename(srcLoc).str() << std::endl;
      std::cout << srcFile << std::endl; 
   }
   else
   {
      // source location
      clang::SourceLocation srcLoc = func->getSourceRange().getBegin();
      clang::SourceLocation srcLoc2 = func->getNameInfo().getBeginLoc();

      const clang::SourceManager& srcMgr = _context->getSourceManager();
      //const std::string srcFile = srcLoc.printToString(srcMgr);
      std::string srcFile = srcMgr.getFilename(srcLoc).str(); // << std::endl;
      //std::cout << srcFile << std::endl;

      // add comment to file
      if ( srcFile.find( _fileName) != std::string::npos )
      {
         std::string functionName( func->getNameInfo().getName().getAsString() );
         std::string returnType(func->getReturnType().getAsString());

         std::stringstream comment;
         comment << "/** \n *\t<function name>: " << functionName << "\n *\n *\t<function description>: \n *\n";

         const int numParms = func->getNumParams();           
         for ( int i=0; i<numParms; ++i)
         {
            const clang::ParmVarDecl* _currentParam = func->getParamDecl(i);
            comment << " *\t@param " << _currentParam->getQualifiedNameAsString() << ":  \n"; 
         }

         if ( returnType != "void" )
         {
            comment << " *\n *\t@return:  \n";  
         }

         comment << " */ \n";


      }

   }
