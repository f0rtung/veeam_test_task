#include "file-writer-stub.h"

#include <stdexcept>

file_writer_stub::file_writer_stub( bool can_write )
    : can_write_{ can_write }
{

}

void file_writer_stub::write( const std::string &content )
{
    if ( !can_write_ ) {
        throw std::runtime_error{ "Can not write" };
    }
    buffer_.append( content );
}

void file_writer_stub::flush( )
{
    content_.append( buffer_ );
    buffer_.clear( );
}


const std::string &file_writer_stub::content( ) const noexcept
{
    return content_;
}
