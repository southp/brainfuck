add_executable(
    runbf
    interpreter.cpp
    parse.cpp
    utils.cpp
)

target_include_directories( runbf PUBLIC ${BF_INCLUDE_PATH} )

