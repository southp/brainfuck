project( Brainfuck )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -std=c++11" )

set( BF_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include" )

add_subdirectory( src )
