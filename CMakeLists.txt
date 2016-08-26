cmake_minimum_required(VERSION 3.0.2)

project(clangTest)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
message(STATUS "output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

#set( CMAKE_C_COMPILER clang )
#set( CMAKE_CXX_COMPILER clang++ )
#set( CMAKE_C_COMPILER_ID  clang )
#set( CMAKE_CXX_COMPILER_ID  clang++ )

message(STATUS "C   compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID}")

# generates a json file containing the compiler calls for all translation units
#set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ) 

subdirs(src/parser)
subdirs(src/tool)
subdirs(src/bmock)

