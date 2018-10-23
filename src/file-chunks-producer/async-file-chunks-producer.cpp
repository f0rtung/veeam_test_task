#include "async-file-chunks-producer.h"

#include <chrono>

namespace file_signature {

    async_file_chunks_producer::async_file_chunks_producer( file_reader_iface_ptr file_reader,
                                                            std::size_t readers_count,
                                                            std::size_t chunk_size )
        : file_reader_{ std::move( file_reader ) }
        , next_chunk_index_{ 0 }
        , mem_pool_{ readers_count * 3, chunk_size }
    { }

    chunk_t async_file_chunks_producer::chunk( )
    {
        std::unique_lock<std::mutex> lock{ chunks_queue_m_ };
        while( chunks_queue_.empty( ) ) {
            reading_exception_.rethrow_exception_if_exists( );
            lock.unlock( );
            std::this_thread::sleep_for( std::chrono::microseconds( 10 ) );
            lock.lock( );
        }
        chunk_t chunk{ std::move ( chunks_queue_.front( ) ) };
        chunks_queue_.pop( );
        return chunk;
    }

    void async_file_chunks_producer::start( )
    {
        reading_thread_ = std::thread( &async_file_chunks_producer::async_read, this );
    }

    void async_file_chunks_producer::stop( )
    {
        if ( reading_thread_.joinable( ) ) {
            reading_thread_.join( );
        }
    }

    void async_file_chunks_producer::async_read( )
    {
        try{
            while ( !file_reader_->eof( ) ) {
                bytes buf{ mem_pool_.wait_pop( ) };
                const std::size_t read_bytes{ file_reader_->read( buf.data( ), buf.size( ) ) };
                if ( read_bytes < buf.size( ) ) { // last chunk
                    buf.resize( read_bytes );
                }
                std::lock_guard<std::mutex> _{ chunks_queue_m_ };
                chunks_queue_.emplace( &mem_pool_, next_chunk_index_++, std::move( buf ) );
            }
            throw no_more_chunks_exception{ };
        } catch ( ... ) {
            reading_exception_.set_exception( std::current_exception( ) );
        }
    }

} // namespace file_signature
