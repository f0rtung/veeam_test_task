#include "hash-functions.h"

#include <boost/crc.hpp>

namespace file_signature {

    std::string crc_32( const bytes &buf )
    {
        boost::crc_32_type result;
        result.process_bytes( buf.data( ), buf.size( ) );
        return std::to_string( result.checksum( ) );
    }

} // namespace file_signature
