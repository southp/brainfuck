#ifndef __BRAINFUCK__COMMAND__H
#define __BRAINFUCK__COMMAND__H

enum class Command {
    Forward,
    Backward,
    Inc,
    Dec,
    Print,
    Read,
    LoopBegin,
    LoopEnd
};

#endif
