#pragma once

#include "../common/type-defs.h"

#include <list>
#include <mutex>

namespace file_signature {

    class empty_chunks_memory_pool_exception
        : public std::exception
    {
    public:
        const char* what( ) const noexcept
        {
            return "Empty chunks memory pool";
        }
    };

    class chunks_memory_pool
    {
    public:
        chunks_memory_pool( std::size_t pool_size, std::size_t chunk_size );
        void push( bytes &&memory );
        bytes pop( );

    private:
        std::list<bytes> pool_;
        mutable std::mutex pool_guard_;
    };

} // namespace file_signature
