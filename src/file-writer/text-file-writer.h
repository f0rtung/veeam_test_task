#pragma once

#include "file-writer-iface.h"

#include <fstream>

namespace file_signature {

    class text_file_writer
        : public file_writer_iface
    {
    public:
        text_file_writer( const std::string &file_path );

        void write( const std::string &content ) override;
        void flush( ) override;

    private:
        std::ofstream tf_stream_;
    };

    using text_file_writer_ptr = std::unique_ptr<text_file_writer>;

} // namespace file_signature
