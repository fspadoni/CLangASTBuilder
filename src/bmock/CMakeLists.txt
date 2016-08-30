cmake_minimum_required(VERSION 3.0.2)

project(buildMocks)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
message(STATUS "output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

SET(CMAKE_CXX_COMPILER "g++-4.8")
SET(CMAKE_C_COMPILER "gcc-4.8") 
SET(CMAKE_CXX_COMPILER_VERSION "4.8")
SET(CMAKE_C_COMPILER_VERSION "4.8")
SET(CMAKE_CXX_FLAGS "-fmax-errors=10")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")


#set( CMAKE_C_COMPILER clang )
#set( CMAKE_CXX_COMPILER clang++ )
#set( CMAKE_C_COMPILER_ID  clang )
#set( CMAKE_CXX_COMPILER_ID  clang++ )

message(STATUS "C   compiler: ${CMAKE_C_COMPILER_ID}  version: ${CMAKE_C_COMPILER_VERSION}" )
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}" )

# generates a json file containing the compiler calls for all translation units
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )




#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)



set ( HEADER_FILES
      MockVisitor.h
      FuncUTDefVisitor.h
      StructVisitor.h
      TypedefVisitor.h
      writer.h
      Consumer.h
      Action.h
      utils.h
      )

set ( SOURCE_FILES
      main.cpp
      MockVisitor.cpp
      FuncUTDefVisitor.cpp
      StructVisitor.cpp
      TypedefVisitor.cpp
      writer.cpp
      Consumer.cpp
      Action.cpp
      utils.cpp
      )
    
#http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project

# LLVM package
find_package ( LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM_LIBRARY_DIR ${LLVM_LIBRARY_DIRS}")
message(STATUS "LIBCLANG_LIBRARY_NAME: ${LIBCLANG_LIBRARY_NAME}")

llvm_map_components_to_libnames(LLVM_LIB_LIBRARY  core support option irreader )

message(STATUS "LLVM_LIBCLANG_LIBRARY: ${LLVM_LIBCLANG_LIBRARY}")


#message( STATUS "llvm install prefix: ${LLVM_INSTALL_PREFIX}" )
#message( STATUS "llvm include dir: ${LLVM_INCLUDE_DIRS}" )



# add LLVM includes
include_directories( /.${LLVM_INCLUDE_DIRS} )
add_definitions(${LLVM_DEFINITIONS})
#include_directories( "/./usr/include" )


#find_library(LibClang_LIBRARY NAMES clangTooling clangParse  PATH_SUFFIXES llvm)
#message(STATUS "LibClang_LIBRARY: ${LibClang_LIBRARY}")

find_package(Clang REQUIRED  )
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${Clang_DEFINITIONS})
message(STATUS "CLANG_LIBS:  ${CLANG_LIBS}") 

find_package(Boost REQUIRED 
    COMPONENTS 
    system
    filesystem
    program_options
    thread
)

# add boost includes
include_directories( /.${Boost_INCLUDE_DIRS})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES )
foreach( dir ${dirs})
  message (STATUS "include dir: ${dir}" )
endforeach()




add_executable( ${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )



# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core irreader)

link_directories("/./usr/lib64")


# statically link with LibClang
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LLVM_LIB_LIBRARY} ${CLANG_LIBS} )

#add_custom_command( TARGET ${PROJECT_NAME} 
#		    POST_BUILD 
#		    COMMAND ${CMAKE_COMMAND} -E 
#		    copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}  ~/workspaces/CommercialCode/Currency/build_Linux-XSeries_Release
#		    COMMENT "Copy buildMocks to ~/workspaces/CommercialCode/Currency/build_Linux-XSeries_Release"
#		    )