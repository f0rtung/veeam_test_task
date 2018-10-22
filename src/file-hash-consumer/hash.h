#pragma once

#include <string>

namespace file_signature {

    class hash_t
    {
    public:
        hash_t( const std::string &hash, std::size_t idx );
        const std::string &hash( ) const noexcept;
        std::size_t index( ) const noexcept;

    private:
        const std::string hash_;
        const std::size_t idx_;
    };

} // namespace file_signature
