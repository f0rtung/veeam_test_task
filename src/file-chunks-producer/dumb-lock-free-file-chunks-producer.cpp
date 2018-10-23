#include "dumb-lock-free-file-chunks-producer.h"

#include <chrono>
#include <iostream>

namespace file_signature {

    dump_lock_free_file_chunks_producer::dump_lock_free_file_chunks_producer( file_reader_iface_ptr file_reader,
                                                                              std::size_t chunks_count,
                                                                              std::size_t chunk_size )
        : file_reader_{ std::move( file_reader ) }
        , chunk_size_{ chunk_size }
        , next_chunk_index_{ 0 }
        , next_read_chunk_index_{ 0 }
        , max_chunks_count_{ chunks_count - 1 }
        , chunks_( chunks_count )
        , finish_{ false }
    { }

    chunk_t dump_lock_free_file_chunks_producer::chunk( )
    {
        while ( true ) {
            std::size_t idx = next_chunk_index_++;
            if ( idx < next_read_chunk_index_ ) {
                return std::move( chunks_[next_chunk_index_++] );
            }
            reading_exception_.rethrow_exception_if_exists( );
        }
    }

    void dump_lock_free_file_chunks_producer::start( )
    {
        reading_thread_ = std::thread( &dump_lock_free_file_chunks_producer::async_read, this );
    }

    void dump_lock_free_file_chunks_producer::stop( )
    {
        if ( reading_thread_.joinable( ) ) {
            reading_thread_.join( );
        }
    }

    void dump_lock_free_file_chunks_producer::async_read( )
    {
        try{
            while ( !file_reader_->eof( ) ) {
                bytes buf( chunk_size_ );
                const std::size_t read_bytes{ file_reader_->read( buf.data( ), buf.size( ) ) };
                if ( read_bytes < buf.size( ) ) { // last chunk
                    buf.resize( read_bytes );
                }
                chunk_t chunk { next_read_chunk_index_, std::move( buf ) };
                chunks_[next_read_chunk_index_] = std::move( chunk );
                next_read_chunk_index_++;
            }
            throw no_more_chunks_exception{ };
        } catch ( ... ) {
            reading_exception_.set_exception( std::current_exception( ) );
        }
    }

} // namespace file_signature
