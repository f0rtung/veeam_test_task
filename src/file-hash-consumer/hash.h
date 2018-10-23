#pragma once

#include <string>

namespace file_signature {

    class hash_t
    {
    public:
        hash_t( const std::string &hash, std::size_t idx );
        hash_t( hash_t & ) = delete;
        hash_t &operator=( hash_t & ) = delete;
        hash_t( hash_t && ) = default;
        hash_t &operator=( hash_t && ) = default;
        const std::string &hash( ) const noexcept;
        std::size_t index( ) const noexcept;

    private:
        const std::string hash_;
        const std::size_t idx_;
    };

} // namespace file_signature
