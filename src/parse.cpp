#include <string>
#include <vector>
#include <stack>

#include "parse.h"
#include "Program.h"
#include "Command.h"

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
