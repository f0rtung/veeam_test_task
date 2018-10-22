#pragma once

#include "file-chunks-producer-iface.h"
#include "../file-reader/binary-file-reader.h"
#include "chunks-memory-pool.h"

#include <mutex>

namespace file_signature {

    class simple_file_chunks_producer
        : public file_chunks_producer_iface
    {
    public:
        simple_file_chunks_producer( file_reader_iface_ptr file_reader,
                                     std::size_t readers_count,
                                     std::size_t chunk_size );

    public:
        chunk_t chunk( ) override;

    private:
        file_reader_iface_ptr file_reader_;
        std::size_t next_chunk_index_;
        std::mutex reader_guard_;
        chunks_memory_pool mem_pool_;
    };

} // namespace file_signature
