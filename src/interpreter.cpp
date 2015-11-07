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
    auto execute_command( std::vector< Command > cmds, size_t inst_ptr, std::stack< int >& loop_stack ) -> size_t {
        assert( data_ptr >= 0 && data_ptr < DATA_BUFFER_SIZE );
        assert( inst_ptr >= 0 && inst_ptr < cmds.size() );

        auto cmd = cmds[ inst_ptr ];

        switch( cmd ) {
            case Command::Forward: {
                ++data_ptr;
                return inst_ptr + 1;
            }

            case Command::Backward: {
                --data_ptr;
                return inst_ptr + 1;
            }

            case Command::Inc: {
                ++data_buffer[ data_ptr ];
                return inst_ptr + 1;
            }

            case Command::Dec: {
                --data_buffer[ data_ptr ];
                return inst_ptr + 1;
            }

            case Command::Print: {
                print( data_buffer[ data_ptr ] );
                return inst_ptr + 1;
            }

            case Command::Read: {
                read( data_buffer[ data_ptr ] );
                return inst_ptr + 1;
            }

            case Command::LoopBegin: {
                loop_stack.push( inst_ptr );
                return inst_ptr + 1;
            }

            case Command::LoopEnd: {
                if ( 0 == data_buffer[ data_ptr ] ) {
                    loop_stack.pop();
                    return inst_ptr + 1;

                } else {
                    int loop_beg = loop_stack.top();
                    return loop_beg + 1;
                }
            }

            default: {
                std::cerr << "Unknown command is found: " << static_cast< int> ( cmd ) << std::endl;
                std::abort();
                return 0; // to suppress error.
            }
        } // switch
    } // execute_command

    auto print ( unsigned char c ) -> void {
#ifdef DEBUG_LOG
        std::cout << "print: " << static_cast< int >( c ) << std::endl;
#endif

        out_device << c;
    }

    auto read ( unsigned char& c ) -> void {
        in_device >> c;

#ifdef DEBUG_LOG
        std::cout << "readed: " << static_cast< int >( c ) << std::endl;
#endif
    }

    static constexpr auto DATA_BUFFER_SIZE = 4096;

    std::array< unsigned char, DATA_BUFFER_SIZE > data_buffer;
    int data_ptr;

    std::istream& in_device;
    std::ostream& out_device;
};

auto parse( const std::string& command_str) -> std::vector< Command > {
    auto commands = std::vector< Command >();

    for( auto c : command_str ) {
        switch( c ) {
            case '>': {
                commands.push_back( Command::Forward );
                break;
            }

            case '<': {
                commands.push_back( Command::Backward );
                break;
            }

            case '+': {
                commands.push_back( Command::Inc );
                break;
            }

            case '-': {
                commands.push_back( Command::Dec );
                break;
            }

            case '.': {
                commands.push_back( Command::Print );
                break;
            }

            case ',': {
                commands.push_back( Command::Read );
                break;
            }

            case '[': {
                commands.push_back( Command::LoopBegin );
                break;
            }

            case ']': {
                commands.push_back( Command::LoopEnd );
                break;
            }

            default:
                continue;
        }
    }

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
