#pragma once

#include "../common/type-defs.h"
#include "chunks-memory-pool.h"

namespace file_signature {

    class chunk_t
    {
    public:
        chunk_t( chunks_memory_pool *memory_pool, std::size_t index, bytes &&memory );
        ~chunk_t( );
        bytes &buf( ) noexcept;
        std::size_t index( ) const noexcept;

    private:
        chunks_memory_pool *memory_pool_;
        std::size_t index_;
        bytes memory_;
    };

} // namespace file_signature
