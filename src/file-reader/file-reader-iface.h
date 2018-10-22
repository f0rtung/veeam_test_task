#pragma once

#include "../common/type-defs.h"

#include <memory>

namespace file_signature {

    class file_reader_iface
    {
    public:
        virtual ~file_reader_iface( ) = default;
        virtual std::size_t read( byte *buf, std::size_t size ) = 0;
        virtual std::size_t size( ) const noexcept = 0;
        virtual bool eof( ) const = 0;

    };

    using file_reader_iface_ptr = std::unique_ptr<file_reader_iface>;

} // namespace file_signature
