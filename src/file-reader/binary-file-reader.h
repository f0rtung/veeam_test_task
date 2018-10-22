#pragma once

#include "file-reader-iface.h"

#include <string>
#include <fstream>

namespace file_signature {

    class binary_file_reader
        : public file_reader_iface
    {
    public:
        explicit binary_file_reader( const std::string &file_path );

    public:
        std::size_t read( byte *buf, std::size_t size ) override;
        std::size_t size( ) const noexcept override;
        bool eof( ) const override;

    private:
        std::ifstream bf_stream_;
        std::size_t bf_size_;
    };

} // namespace file_signature
