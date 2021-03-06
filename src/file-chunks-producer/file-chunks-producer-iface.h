#pragma once

#include "chunk.h"

#include <memory>

namespace file_signature {

    class no_more_chunks_exception
        : public std::exception
    {
    public:
        const char* what( ) const noexcept
        {
            return "No more chunks";
        }
    };

    class file_chunks_producer_iface
    {
    public:
        virtual ~file_chunks_producer_iface( ) = default;
        virtual chunk_t chunk( ) = 0;
        virtual void start( ) = 0;
        virtual void stop( ) = 0;
    };

    using file_chunks_producer_iface_ptr = std::unique_ptr<file_chunks_producer_iface>;

} // namespace file_signature
