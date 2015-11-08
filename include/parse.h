#ifndef __BRAINFUCK__COMMON__H
#define __BRAINFUCK__COMMON__H

#include <string>

class Program;

auto parse( const std::string& command_str ) -> Program;

#endif
