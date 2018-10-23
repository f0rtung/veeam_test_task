#include "simple-file-chunks-producer.h"

namespace file_signature {

    simple_file_chunks_producer::simple_file_chunks_producer( file_reader_iface_ptr file_reader,
                                                              std::size_t readers_count,
                                                              std::size_t chunk_size )
        : file_reader_{ std::move( file_reader ) }
        , next_chunk_index_{ 0 }
        , mem_pool_{ readers_count, chunk_size }
    { }

    chunk_t simple_file_chunks_producer::chunk( )
    {
        bytes buf{ mem_pool_.pop( ) };
        std::unique_lock<std::mutex> lock{ reader_guard_ };
        if ( file_reader_->eof( ) ) {
            throw no_more_chunks_exception{ };
        }
        const std::size_t read_bytes{ file_reader_->read( buf.data( ), buf.size( ) ) };
        chunk_t chunk{ &mem_pool_, next_chunk_index_++, std::move( buf ) };
        lock.unlock( );
        if ( read_bytes < chunk.buf( ).size( ) ) { // last chunk
            chunk.buf( ).resize( read_bytes );
        }
        return chunk;
    }

    void simple_file_chunks_producer::start( )
    {

    }

    void simple_file_chunks_producer::stop( )
    {

    }

} // namespace file_signature
