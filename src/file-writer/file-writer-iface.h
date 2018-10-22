#pragma once

#include <string>
#include <memory>

namespace file_signature {

    class file_writer_iface
    {
    public:
        virtual ~file_writer_iface( ) = default;
        virtual void write( const std::string &content ) = 0;
        virtual void flush( ) = 0;
    };

    using file_writer_iface_ptr = std::unique_ptr<file_writer_iface>;

} // namespace file_signature
