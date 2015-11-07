#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

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

struct Environment {
    Environment() : data_buffer( new unsigned char ( DATA_BUFFER_SIZE ) ) {}

    constexpr auto DATA_BUFFER_SIZE = 4096;
    unique_ptr< unsigned char [] > data_buffer;
};

auto parse( const std::string& command_str) -> std::vector< Command > {
    auto commands = std::vector< Command > ();

    return commands;
}

auto read_source( std::IOStream& ) -> std::string {
    return "";
}

auto execute_command( Environment& env, const Command cmd ) -> void {
}

int main( int argc, char* argv[] ) {
    return 0;
}
