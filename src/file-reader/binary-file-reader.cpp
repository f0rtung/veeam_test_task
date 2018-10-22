#include "binary-file-reader.h"

namespace file_signature {

    binary_file_reader::binary_file_reader( const std::string &file_path )
    {
        const auto open_mod{ std::ifstream::in |
                             std::ifstream::binary |
                             std::ifstream::ate };
        bf_stream_.open( file_path, open_mod );
        if ( !bf_stream_ ) {
            throw std::runtime_error{ "Can not open file \"" + file_path + "\"" };
        }
        bf_size_ = bf_stream_.tellg( );
        bf_stream_.seekg( 0 );
    }

    std::size_t binary_file_reader::read( byte *buf, std::size_t size )
    {
        bf_stream_.read( buf, size );
        return bf_stream_.gcount( );
    }

    std::size_t binary_file_reader::size( ) const noexcept
    {
        return bf_size_;
    }

    bool binary_file_reader::eof( ) const
    {
        return bf_stream_.eof( );
    }

} // namespace file_signature
