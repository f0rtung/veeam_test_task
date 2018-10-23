#pragma once

#include "../common/type-defs.h"

namespace file_signature {

    class chunk_t
    {
    public:
        chunk_t( ) = default;
        chunk_t( std::size_t index, bytes &&memory );
        chunk_t( chunk_t & ) = delete;
        chunk_t &operator=( chunk_t & ) = delete;
        chunk_t( chunk_t && ) = default;
        chunk_t &operator=( chunk_t && ) = default;
        bytes &buf( ) noexcept;
        std::size_t index( ) const noexcept;

    private:
        std::size_t index_;
        bytes memory_;
    };

} // namespace file_signature
