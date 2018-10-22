#include "chunk.h"

namespace file_signature {

    chunk_t::chunk_t( chunks_memory_pool *memory_pool, std::size_t index, bytes &&memory )
        : memory_pool_{ memory_pool }
        , index_{ index }
        , memory_{ std::move( memory ) }
    { }

    chunk_t::~chunk_t( )
    {
        if ( memory_pool_ ) {
            memory_pool_->push( std::move( memory_ ) );
        }
    }

    bytes &chunk_t::buf( ) noexcept
    {
        return memory_;
    }

    std::size_t chunk_t::index( ) const noexcept
    {
        return index_;
    }

} // namespace file_signature
