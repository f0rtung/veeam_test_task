#pragma once

#include "file-chunks-producer-iface.h"
#include "../file-reader/binary-file-reader.h"
#include "chunks-memory-pool.h"
#include "../common/exception_ptr_wrapper.h"

#include <mutex>
#include <queue>
#include <thread>

namespace file_signature {

    class async_file_chunks_producer
        : public file_chunks_producer_iface
    {
    public:
        async_file_chunks_producer( file_reader_iface_ptr file_reader,
                                     std::size_t readers_count,
                                     std::size_t chunk_size );

    public:
        chunk_t chunk( ) override;
        void start( ) override;
        void stop( ) override;

    private:
        void async_read( );

    private:
        file_reader_iface_ptr file_reader_;
        std::size_t next_chunk_index_;
        std::mutex reader_guard_;
        chunks_memory_pool mem_pool_;
        exception_ptr_wrapper reading_exception_;
        std::thread reading_thread_;

        std::mutex chunks_queue_m_;
        std::queue<chunk_t> chunks_queue_;
    };

} // namespace file_signature
