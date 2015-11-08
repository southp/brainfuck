#ifndef __BRAINFUCK__PROGRAM__H
#define __BRAINFUCK__PROGRAM__H

#include <vector>
#include <map>

enum class Command;

struct Program {
    Program( std::vector< Command > cmds, std::map< size_t, size_t > loop_scopes ) :
        cmds( cmds ),
        loop_scopes( loop_scopes ) {}

    std::vector< Command > cmds;
    std::map< size_t, size_t > loop_scopes;
};

#endif
