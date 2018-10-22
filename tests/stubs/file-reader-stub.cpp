#include "file-reader-stub.h"

file_reader_stub::file_reader_stub( const std::string &content )
    : content_{ content }
    , read_pos_{ content_.cbegin( ) }
{ }

std::size_t file_reader_stub::read( byte *buf, std::size_t size )
{
    const std::size_t available{ static_cast<std::size_t>( content_.cend( ) - read_pos_ ) };
    if ( size > available ) {
        std::copy( read_pos_, content_.cend( ), buf );
        read_pos_ = content_.cend( );
        return available;
    } else {
        std::copy( read_pos_, read_pos_ + size, buf );
        read_pos_ += size;
        return size;
    }
}

std::size_t file_reader_stub::size( ) const noexcept
{
    return content_.size( );
}

bool file_reader_stub::eof( ) const
{
    return read_pos_ == content_.cend( );
}
