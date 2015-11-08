#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <stack>
#include <array>
#include <map>

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

struct Program {
    Program( std::vector< Command > cmds, std::map< size_t, size_t > loop_scopes ) :
        cmds( cmds ),
        loop_scopes( loop_scopes ) {}

    std::vector< Command > cmds;
    std::map< size_t, size_t > loop_scopes;
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
    auto execute( const Program& prog ) -> int {
        size_t total_cmds = prog.cmds.size();

        for( size_t inst_ptr = 0; inst_ptr < total_cmds; ) {
            inst_ptr = execute_command( prog, inst_ptr );
        }

        return 0;
    }

private:
    auto execute_command( const Program& prog, size_t inst_ptr ) -> size_t {
        auto& cmds = prog.cmds;
        auto& loop_scopes = prog.loop_scopes;

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
                if ( 0 == data_buffer[ data_ptr ] ) {
                    auto loop_end = loop_scopes.find( inst_ptr );

                    assert( loop_end != loop_scopes.end() );

                    return loop_end->second + 1;
                } else {
                    return inst_ptr + 1;
                }
            }

            case Command::LoopEnd: {
                auto loop_beg = loop_scopes.find( inst_ptr );

                assert( loop_beg != loop_scopes.end() );

                return loop_beg->second;
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

auto parse( const std::string& command_str ) -> Program {
    auto commands = std::vector< Command >();
    auto loop_scopes = std::map< size_t, size_t >();
    auto loop_stack  = std::stack< size_t >();

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
                loop_stack.push( commands.size() - 1 );

                break;
            }

            case ']': {
                commands.push_back( Command::LoopEnd );

                auto loop_beg_idx = loop_stack.top();
                auto cur_cmd_idx  = commands.size() - 1;

                loop_scopes[ cur_cmd_idx  ] = loop_beg_idx;
                loop_scopes[ loop_beg_idx ] = cur_cmd_idx;

                loop_stack.pop();

                break;
            }

            default:
                continue;
        }
    }

    return Program( commands, loop_scopes );
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
    auto program    = parse( src_string );

    return env.execute( program );
}
