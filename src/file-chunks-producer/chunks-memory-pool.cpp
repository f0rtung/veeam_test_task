#include "chunks-memory-pool.h"

namespace file_signature {

    chunks_memory_pool::chunks_memory_pool( std::size_t pool_size, std::size_t chunk_size )
    {
        for ( std::size_t idx = 0; idx < pool_size; ++idx ) {
            pool_.emplace_back( chunk_size );
        }
    }

    void chunks_memory_pool::push( bytes &&memory )
    {
        std::lock_guard<std::mutex> _{ pool_guard_ };
        pool_.push_back( std::move( memory ) );
    }

    bytes chunks_memory_pool::pop( )
    {
        std::lock_guard<std::mutex> _{ pool_guard_ };
        if ( pool_.empty( ) ) {
            throw empty_chunks_memory_pool_exception{ };
        }
        bytes mem{ std::move( pool_.front( ) ) };
        pool_.pop_front( );
        return mem;
    }

} // namespace file_signature
