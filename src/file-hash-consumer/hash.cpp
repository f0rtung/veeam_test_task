#include "hash.h"

namespace file_signature {

    hash_t::hash_t( const std::string &hash, std::size_t idx )
        : hash_{ hash }
        , idx_ { idx }
    { }

    const std::string &hash_t::hash( ) const noexcept
    {
        return hash_;
    }

    std::size_t hash_t::index( ) const noexcept
    {
        return idx_;
    }

} // namespace file_signature
