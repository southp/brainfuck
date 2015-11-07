#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <stack>
#include <array>

#include <cassert>
#include <cstdlib>

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

class Environment {
public:
    Environment( std::istream& in = std::cin, std::ostream& out = std::cout ) :
        data_ptr   ( 0 ),
        in_device  ( in ),
        out_device ( out ) {

        memset( data_buffer.data(), 0, DATA_BUFFER_SIZE );
    }

    // Well, there could be richer status code.
    auto execute( const std::vector< Command >& cmds ) -> int {
        auto loop_stack = std::stack< int >();
        size_t total_cmds = cmds.size();

        for( size_t inst_ptr = 0; inst_ptr < total_cmds; ) {
            inst_ptr = execute_command( cmds, inst_ptr, loop_stack );
        }

        return 0;
    }

private:
    auto execute_command( std::vector< Command> cmds, size_t inst_ptr, std::stack< int >& loop_stack ) -> int {
        assert( data_ptr >= 0 && data_ptr < DATA_BUFFER_SIZE );
        assert( inst_ptr >= 0 && inst_ptr < cmds.size() );

        auto cmd = cmds[ inst_ptr ];

        switch( cmd ) {
        case Command::Forward: {
            ++data_ptr;
            break;
        }

        case Command::Backward: {
            --data_ptr;
            break;
        }

        case Command::Inc: {
            ++data_buffer[ data_ptr ];
            break;
        }

        case Command::Dec: {
            --data_buffer[ data_ptr ];
            break;
        }

        case Command::Print: {
            print( data_buffer[ data_ptr ] );
            break;
        }

        case Command::Read: {
            read( data_buffer[ data_ptr ] );
            break;
        }

        case Command::LoopBegin: {
            loop_stack.push( inst_ptr );
            break;
        }

        case Command::LoopEnd: {
            int loop_beg = loop_stack.top();
            loop_stack.pop();

            return loop_beg + 1;
        }

        default:
            break;
        }

        return inst_ptr + 1;
    }

    auto print ( unsigned char c ) -> void {
        out_device << c;
    }

    auto read ( unsigned char& c ) -> void {
        in_device >> c;
    }

    static constexpr auto DATA_BUFFER_SIZE = 4096;

    std::array< unsigned char, DATA_BUFFER_SIZE > data_buffer;
    int data_ptr;

    std::istream& in_device;
    std::ostream& out_device;
};

auto parse( const std::string& command_str) -> std::vector< Command > {
    auto commands = std::vector< Command > ();

    return commands;
}

// TODO: could make use of streaming to reduce memory footprint
auto read_source( std::ifstream& src ) -> std::string {
    std::string content;
    std::string buffer;

    while( std::getline( src, buffer ) ) {
        content += buffer;
    }

    return content;
}

int main( int argc, char* argv[] ) {
    if ( argc < 2 ) {
        std::cerr << "Usage: " << argv[ 0 ] << ' ' << "your-brainfuck-source-filename" << std::endl;
        return -1;
    }

    const auto filename = argv[ 1 ];
    std::ifstream src( filename );

    if ( ! src ) {
        std::cerr << "Failed to read the file: " << filename << std::endl;
        return -1;
    }

    auto env        = Environment();
    auto src_string = read_source( src );
    auto cmds       = parse( src_string );

    return env.execute( cmds );
}
