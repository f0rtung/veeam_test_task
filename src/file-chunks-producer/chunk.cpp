#include "chunk.h"

namespace file_signature {

    chunk_t::chunk_t( std::size_t index, bytes &&memory )
        : index_{ index }
        , memory_{ std::move( memory ) }
    { }

    bytes &chunk_t::buf( ) noexcept
    {
        return memory_;
    }

    std::size_t chunk_t::index( ) const noexcept
    {
        return index_;
    }

} // namespace file_signature
