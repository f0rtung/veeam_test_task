#pragma once

#include "hash.h"

#include <memory>

namespace file_signature {

    class file_hash_consumer_iface
    {

    public:
        virtual ~file_hash_consumer_iface( ) = default;
        virtual void start( ) = 0;
        virtual void stop( ) = 0;
        virtual void add_hash( hash_t &&hash ) = 0;
    };

    using file_hash_consumer_iface_ptr = std::unique_ptr<file_hash_consumer_iface>;

} // namespace file_signature
