cmake_minimum_required( VERSION 3.3 )

project( Brainfuck )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -std=c++11" )

add_subdirectory( src )
