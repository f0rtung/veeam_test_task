#include "utils.h"

#include <algorithm>

std::string random_string( std::size_t length )
{
    const auto randchar = [ ]( ) -> char
    {
        static const std::string chars(
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
        );
        static const size_t max_index{ chars.size( ) - 1 };
        return chars[ rand( ) % max_index ];
    };
    std::string str( length, 0 );
    std::generate_n( str.begin( ), length, randchar );
    return str;
}
