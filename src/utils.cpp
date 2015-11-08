#include <fstream>
#include <string>

#include "utils.h"

// TODO: could make use of streaming to reduce memory footprint
auto read_source( std::ifstream& src ) -> std::string {
    std::string content;
    std::string buffer;

    while( std::getline( src, buffer ) ) {
        content += buffer;
    }

    return content;
}

