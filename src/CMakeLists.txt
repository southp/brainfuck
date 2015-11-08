
add_library(
    bf_common
    parse.cpp
    utils.cpp
)

target_include_directories( bf_common PUBLIC ${BF_INCLUDE_PATH} )

add_executable(
    runbf
    interpreter.cpp
)

target_link_libraries(
    runbf
    bf_common
)
