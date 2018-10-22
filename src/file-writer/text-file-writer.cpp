#include "text-file-writer.h"

namespace file_signature {

    text_file_writer::text_file_writer( const std::string &file_path )
    {
        tf_stream_.open( file_path, std::ofstream::trunc );
        if ( !tf_stream_ ) {
            throw std::runtime_error{ "Can not open file \"" + file_path + "\"" };
        }
    }

    void text_file_writer::write( const std::string &content )
    {
        tf_stream_.write( content.data( ), content.size( ) );
    }

    void text_file_writer::flush( )
    {
        tf_stream_.flush( );
    }

} // namespace file_signature
