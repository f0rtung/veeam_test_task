#pragma once

#include "file-chunks-producer-iface.h"
#include "../file-reader/binary-file-reader.h"
#include "../common/exception_ptr_wrapper.h"

#include <atomic>
#include <mutex>
#include <vector>
#include <thread>

namespace file_signature {

    class dump_lock_free_file_chunks_producer
        : public file_chunks_producer_iface
    {
    public:
        dump_lock_free_file_chunks_producer( file_reader_iface_ptr file_reader,
                                             std::size_t chunks_count,
                                             std::size_t chunk_size );

    public:
        chunk_t chunk( ) override;
        void start( ) override;
        void stop( ) override;

    private:
        void async_read( );

    private:
        file_reader_iface_ptr file_reader_;
        std::size_t chunk_size_;
        std::atomic_size_t next_chunk_index_;
        std::atomic_size_t next_read_chunk_index_;
        std::atomic_size_t max_chunks_count_;
        std::vector<chunk_t> chunks_;
        std::thread reading_thread_;
        exception_ptr_wrapper reading_exception_;
        std::atomic_bool finish_;
    };

} // namespace file_signature
