#pragma once

#include "file-chunks-producer-iface.h"
#include "../file-reader/binary-file-reader.h"

#include <mutex>

namespace file_signature {

    class simple_file_chunks_producer
        : public file_chunks_producer_iface
    {
    public:
        simple_file_chunks_producer( file_reader_iface_ptr file_reader, std::size_t chunk_size );

    public:
        chunk_t chunk( ) override;
        void start( ) override;
        void stop( ) override;

    private:
        std::mutex reader_guard_;
        file_reader_iface_ptr file_reader_;
        std::size_t next_chunk_index_;
        std::size_t chunk_size_;
    };

} // namespace file_signature
